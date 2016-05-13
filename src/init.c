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
#include <stdlib.h>
#include "cmdlist.h"
#include "server.h"

SOCKET      init_listen(SOCKET sock)
{
    if (listen(sock, MAX_CLIENTS) == -1)
    {
        perror("listen");
        close(sock);
        return (-1);
    }
    return (sock);
}

SOCKET      init_connection(in_addr_t ip, unsigned int port)
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
    s_in.sin_addr.s_addr = ip;
    enable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
        perror("setsockopt");
    if (bind(sock, (SOCKADDR *)&s_in, sizeof(s_in)) == -1)
    {
        perror("bind");
        close(sock);
        return (-1);
    }
    return (init_listen(sock));
}

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
