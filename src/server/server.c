/*
** server.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Sun Apr 10 16:46:45 2016 Adrien WERY
** Last update	Tue Apr 19 21:23:54 2016 Adrien WERY
*/

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "server.h"

int     main(int ac, char **av)
{
    unsigned int    port;

    if (ac < 2 || (port = atoi(av[1])) == 0)
    {
        write(1, USAGE, strlen(USAGE));
        return (1);
    }
    return (0);
}
