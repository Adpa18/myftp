/*
** client.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Sun Apr 10 16:54:20 2016 Adrien WERY
** Last update	Wed Apr 20 22:47:29 2016 Adrien WERY
*/

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "cmd.h"
#include "array.h"
#include "client.h"

char    **shell()
{
    char    **array;
    char    *cmd;

    write(1, SHELL_PS1, strlen(SHELL_PS1));
    cmd = getLine(0);
    array = split(cmd, " ");
    free(cmd);
    return (array);
}

void    client(char *host, unsigned int port)
{
    char    **cmds;
    char    *ret;

    (void)host;
    (void)port;
    while ((cmds = shell()))
    {
        if ((ret = exec(cmds)))
        {
            write(1, ret, strlen(ret));
            free(ret);
        }
        free_array(cmds);
    }
}

int     main(int ac, char **av)
{
    unsigned int    port;

    if (ac < 3 || !av[1] || (port = atoi(av[2])) == 0)
    {
        write(1, USAGE, strlen(USAGE));
        return (1);
    }
    client(av[1], port);
    return (0);
}
