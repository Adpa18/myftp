//
// Created by wery_a on 02/05/16.
//

#include <unistd.h>
#include <stdlib.h>
#include <server.h>
#include <common.h>
#include "server.h"
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
    manager->clients[manager->size].server_sock = sock;
    manager->clients[manager->size].sock = csock;
    manager->clients[manager->size].status = NONE;
    manager->clients[manager->size].root = manager->cwd;
    manager->clients[manager->size].addr = (int)csin.sin_addr.s_addr;
    manager->clients[manager->size].mode = DATA_NO;
    manager->clients[manager->size].use_mode = DATA_NO;
    manager->clients[manager->size].sock_pasv = -1;
    ++manager->size;
    write_socket(csock, WELCOME);
}

void    remove_client(Manager *manager, int to_remove)
{
    free(manager->clients[to_remove].root);
    close(manager->clients[to_remove].sock);
    memmove(manager->clients + to_remove, manager->clients + to_remove + 1,
            (manager->size - to_remove - 1) * sizeof(Client));
    --manager->size;
}

COMMAND get_cmd(const char *cmd_line)
{
    for (unsigned  int i = 0; i < sizeof(cmdlist_str) / sizeof(char *); ++i)
    {
        if (strncasecmp(cmd_line, cmdlist_str[i], strlen(cmdlist_str[i])) == 0)
            return (i);
    }
    return (NO_CMD);
}

char    *response(const char *cmd_line, Client *client)
{
    COMMAND cmd;

    if (cmd_line == NULL || cmd_line[0] == 0 || cmd_line[0] == '\n')
        return (NULL);
    if ((cmd = get_cmd(cmd_line))  == NO_CMD)
        return (strdup(UNKNOW_CMD));
    if (client->status == LOGGED_IN || cmd == USER || cmd == PASS || cmd == QUIT)
        return (cmdlist_func[cmd](cmd_line, client));
    else
        return (strdup(LOGIN_FIRST));
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
        else
        {
            if ((ret = response(buffer, &(manager->clients[i]))))
            {
                if (!strncmp(ret, "221", 3))
                    remove_client(manager, i);
                write_socket(manager->clients[i].sock, ret);
                free(ret);
                manager->clients[i].use_mode = DATA_NO;
            }
        }
        break;
    }
}