/*
** manage_client.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed May 04 14:31:23 2016 Adrien WERY
** Last update	Thu May 12 12:25:23 2016 Adrien WERY
*/

#include <unistd.h>
#include <stdlib.h>
#include "array.h"
#include "common.h"
#include "server.h"
#include "cmd.h"

bool    new_client(SOCKET sock, fd_set *rdfs, Manager *manager)
{
    SOCKADDR_IN csin;
    socklen_t   sinsize;
    int         csock;

    sinsize = sizeof(csin);
    if ((csock = accept(sock, (SOCKADDR *)&csin, &sinsize)) == -1)
    {
        return (perror("accept"), false);
    }
    if (manager->size >= MAX_CLIENTS)
    {
        write_socket(csock, ERROR_MAX);
        return (close(csock), false);
    }
    FD_SET(csock, rdfs);
    manager->max_fd = csock > manager->max_fd ? csock : manager->max_fd;
    manager->clients[manager->size].sock = csock;
    manager->clients[manager->size].sock_data = -1;
    manager->clients[manager->size].status = NONE;
    manager->clients[manager->size].root = manager->cwd;
    manager->clients[manager->size].cwd = strdup(manager->cwd);
    manager->clients[manager->size].addr = (int)csin.sin_addr.s_addr;
    manager->clients[manager->size++].mode = DATA_NO;
    write_socket(csock, WELCOME);
    return (true);
}

void    remove_client(Manager *manager, int to_remove)
{
    free(manager->clients[to_remove].cwd);
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

char    *response(char *cmd_line, Client *client)
{
    COMMAND cmd;
    int     i;

    if (cmd_line == NULL || cmd_line[0] == 0 || cmd_line[0] == '\n')
        return (NULL);
    i = -1;
    while (cmd_line[++i])
    {
        if (cmd_line[i] == '\n' || cmd_line[i] == '\r')
        {
            cmd_line[i] = 0;
            break;
        }
    }
    if ((cmd = get_cmd(cmd_line))  == NO_CMD)
        return (strdup(UNKNOW_CMD));
    printf("Client %d ==> %s\n", client->sock, cmd_line);
    chdir(client->cwd);
    if (client->status == LOGGED_IN || cmd == USER
        || cmd == PASS || cmd == QUIT)
        return (cmdlist_func[cmd](cmd_line, client));
    else
        return (strdup(LOGIN_FIRST));
}

void    listen_clients(fd_set *rdfs, Manager *manager)
{
    char    **array;
    char    *buffer;
    char    *ret;

    for (int i = 0; i < manager->size; i++)
    {
        if (!FD_ISSET(manager->clients[i].sock, rdfs))
            continue;
        if ((buffer = read_socket(manager->clients[i].sock)) == NULL)
        {
            remove_client(manager, i);
            printf(EOT_CLIENT, i);
        }
        else
        {
            array = split(buffer, "\r\n");
            for (int j = 0; j < array_len((const char **)array); ++j)
            {
                if ((ret = response(array[j], &(manager->clients[i]))))
                {
                    if (!strncmp(ret, "221", 3))
                        remove_client(manager, i);
                    write_socket(manager->clients[i].sock, ret);
                    free(ret);
                }
            }
        }
        free(buffer);
        free_array(array);
        break;
    }
}
