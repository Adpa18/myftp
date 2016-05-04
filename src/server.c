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
#include <server.h>
#include <common.h>
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

SOCKET      init_connection(unsigned int port)
{
    SOCKET      sock;
    SOCKADDR_IN s_in;
    int         enable;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return (-1);
    }
    s_in.sin_family = AF_INET;
    s_in.sin_port = htons(port);
    s_in.sin_addr.s_addr = htonl(INADDR_ANY);
    enable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
        perror("setsockopt");
    if (bind(sock, (SOCKADDR *)&s_in, sizeof(s_in)) == -1)
    {
        perror("bind");
        close(sock);
        return (-1);
    }
    if (listen(sock, MAX_CLIENTS) == -1)
    {
        perror("listen");
        close(sock);
        return (-1);
    }
    return (sock);
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

void    server(unsigned int port)
{
    SOCKET  sock;
    Manager manager;
    fd_set  rdfs;

    if ((sock = init_connection(port)) == -1)
        return;
    manager.size = 0;
    manager.max_fd = sock;
    manager.cwd = getcwd(NULL, 0);
    while (!killed)
    {
        if (!init_select(&rdfs, sock, &manager))
            break;
        if (FD_ISSET(STDIN_FILENO, &rdfs))
            break;
        else if (FD_ISSET(sock, &rdfs))
            new_client(sock, &rdfs, &manager);
        else
            listen_clients(&rdfs, &manager);
    }
    for (int i = 0; i < manager.size; i++)
        close(manager.clients[i].sock);
    free(manager.cwd);
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
