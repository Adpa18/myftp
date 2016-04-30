/*
** server.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Sun Apr 10 16:46:45 2016 Adrien WERY
** Last update	Sat Apr 30 18:22:06 2016 Adrien WERY
*/

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <server.h>
#include "server.h"
#include "cmd.h"

SOCKET      init_connection(unsigned int port)
{
    SOCKET  sock;
    SOCKADDR_IN s_in;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return (-1);
    }
    s_in.sin_addr.s_addr = htonl(INADDR_ANY);
    s_in.sin_port = htons(port);
    s_in.sin_family = AF_INET;
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

bool    new_client(SOCKET sock, fd_set *rdfs, Manager *manager)
{
    SOCKADDR_IN csin;
    socklen_t   sinsize;
    int         csock;

    sinsize = sizeof(csin);
    if ((csock = accept(sock, (SOCKADDR *)&csin, &sinsize)) == -1)
    {
        perror("accept");
        return (false);
    }
    FD_SET(csock, rdfs);
    manager->max_fd = csock > manager->max_fd ? csock : manager->max_fd;
    manager->clients[manager->size].sock = csock;
    ++manager->size;
    return (true);
}

void    remove_client(Manager *manager, int to_remove)
{
    close(manager->clients[to_remove].sock);
    memmove(manager->clients + to_remove, manager->clients + to_remove + 1,
            (manager->size - to_remove - 1) * sizeof(Client));
    --manager->size;
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
        return (false);
    }
    return (true);
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
        else if ((ret = exec(buffer)))
        {
            write_socket(manager->clients[i].sock, ret);
            free(ret);
        }
        else
             write_socket(manager->clients[i].sock, UNKNOWN_CMD);
//        break;
    }
}

bool    server_end(Manager *manager, int sock)
{
    for (int i = 0; i < manager->size; i++)
    {
        close(manager->clients[i].sock);
    }
    close(sock);
    return (true);
}

bool    server(unsigned int port)
{
    SOCKET  sock;
    Manager manager;
    fd_set  rdfs;

    if ((sock = init_connection(port)) == -1)
        return (false);
    manager.size = 0;
    manager.max_fd = sock;
    while (42)
    {
        if (!init_select(&rdfs, sock, &manager))
        {
            close(sock);
            return (false);
        }
        if (FD_ISSET(STDIN_FILENO, &rdfs))
            break;
        else if (FD_ISSET(sock, &rdfs))
            new_client(sock, &rdfs, &manager);
        else
            listen_clients(&rdfs, &manager);
    }
    return (server_end(&manager, sock));
}

int     main(int ac, char **av)
{
    unsigned int    port;

    if (ac < 2 || (port = atoi(av[1])) == 0)
    {
        write(1, USAGE, strlen(USAGE));
        return (1);
    }
    if (!server(port))
        return (1);
    return (0);
}
