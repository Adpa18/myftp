//
// Created by wery_a on 02/05/16.
//

#include <unistd.h>
#include <stdlib.h>
#include "server.h"
#include "array.h"
#include "cmd.h"

void    new_client(SOCKET sock, fd_set *rdfs, Manager *manager)
{
    SOCKADDR_IN csin;
    socklen_t   sinsize;
    int         csock;

    sinsize = sizeof(csin);
    if ((csock = accept(sock, (SOCKADDR *)&csin, &sinsize)) == -1)
    {
        perror("accept");
        return;
    }
    FD_SET(csock, rdfs);
    manager->max_fd = csock > manager->max_fd ? csock : manager->max_fd;
    manager->clients[manager->size].sock = csock;
    ++manager->size;
}

void    remove_client(Manager *manager, int to_remove)
{
    close(manager->clients[to_remove].sock);
    memmove(manager->clients + to_remove, manager->clients + to_remove + 1,
            (manager->size - to_remove - 1) * sizeof(Client));
    --manager->size;
}

bool    fileManage(const char *cmd, int sock)
{
    char    **cmds;
    bool    ret;

    ret = true;
    if (!(cmds = split(cmd, " ")))
        return (false);
    if (cmds[0] && cmds[1])
    {
        if (!strcmp(cmds[0], "put"))
            recv_file(cmds[1], sock);
        else if (!strcmp(cmds[0], "get"))
            send_file(cmds[1], sock);
        else if (!strcmp(cmds[0], "mput"))
            for (int i = 1; cmds[i]; ++i)
                recv_file(cmds[i], sock);
        else if (!strcmp(cmds[0], "mget"))
            for (int i = 1; cmds[i]; ++i)
                send_file(cmds[i], sock);
        else
            ret = false;
    }
    else
        ret = false;
    free_array(cmds);
    return (ret);
}

void    listen_clients(fd_set *rdfs, Manager *manager)
{
    char    buffer[BUFF_SIZE];
    char    *ret;

    for (int i = 0; i < manager->size; i++)
    {
        if (!FD_ISSET(manager->clients[i].sock, rdfs))
            continue;
        if (read_socket(manager->clients[i].sock, buffer) == 0)
        {
            remove_client(manager, i);
            printf(EOT_CLIENT, i);
        }
        else if (fileManage(buffer, manager->clients[i].sock))
        {
            write_socket(manager->clients[i].sock, buffer);
            write_socket(manager->clients[i].sock, " TRANSFERT SUCCESS");
        }
        else if ((ret = exec(buffer)))
        {
            write_socket(manager->clients[i].sock, ret);
            free(ret);
        }
        else
            write_socket(manager->clients[i].sock, UNKNOWN_CMD);
    }
}