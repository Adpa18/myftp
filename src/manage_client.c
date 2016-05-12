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

void    aws(Manager *manager, int i, char *buffer)
{
    char    **array;
    char    *ret;

    array = split(buffer, "\r\n");
    for (int j = 0; j < array_len((const char **)array); ++j)
    {
        if ((ret = response(array[j], &(manager->clients[i]))))
        {
            write_socket(manager->clients[i].sock, ret);
            free(ret);
            if (!strncmp(ret, "221", 3))
            {
                remove_client(manager, i);
                break;
            }
        }
    }
    free_array(array);
}

void    listen_clients(fd_set *rdfs, Manager *manager)
{
    char    *buffer;

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
            aws(manager, i, buffer);
        }
        free(buffer);
        break;
    }
}
