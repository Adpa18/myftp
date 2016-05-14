/*
** server.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Sun Apr 10 16:46:45 2016 Adrien WERY
** Last update	Wed May 04 14:31:27 2016 Adrien WERY
*/

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "server.h"

bool    killed = false;

void        kill_sig(int sig)
{
    if (sig == SIGINT)
    {
        write(1, KILL_SIGINT, strlen(KILL_SIGINT));
        killed = true;
    }
}

bool    init_select(fd_set *rdfs, int sock, Manager *manager)
{
    FD_ZERO(rdfs);
    FD_SET(STDIN_FILENO, rdfs);
    FD_SET(sock, rdfs);
    for (int i = 0; i < manager->size; ++i)
    {
        FD_SET(manager->clients[i].sock, rdfs);
    }
    if (select(manager->max_fd + 1, rdfs, NULL, NULL, NULL) == -1)
    {
        perror("select");
        close(sock);
        return (false);
    }
    return (true);
}

void    clean_manager(Manager *manager)
{
    for (int i = 0; i < manager->size; i++)
    {
        free(manager->clients[i].cwd);
        close(manager->clients[i].sock);
    }
    free(manager->cwd);
}

void    server(unsigned int port)
{
    SOCKET  sock;
    Manager manager;
    fd_set  rdfs;

    if ((sock = init_connection(INADDR_ANY, port)) == -1)
        return;
    manager.size = 0;
    manager.max_fd = sock;
    manager.cwd = getcwd(NULL, 0);
    while (!killed)
    {
        if (!init_select(&rdfs, sock, &manager))
            break;
        if (FD_ISSET(sock, &rdfs))
            new_client(sock, &rdfs, &manager);
        else
            listen_clients(&rdfs, &manager);
    }
    clean_manager(&manager);
    close(sock);
}

int     main(int ac, char **av)
{
    unsigned int    port;

    if (ac < 3 || (port = atoi(av[1])) == 0)
    {
        write(1, USAGE, strlen(USAGE));
        return (1);
    }
    if (chdir(av[2]) == -1)
    {
        perror("path");
        return (1);
    }
    signal(SIGINT, &kill_sig);
    server(port);
    return (0);
}
