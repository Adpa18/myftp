/*
** server.h for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed May 04 14:30:29 2016 Adrien WERY
** Last update	Wed May 04 14:30:30 2016 Adrien WERY
*/

#ifndef SERVER_H
# define SERVER_H

# include "socket.h"

# define USAGE          "Usage : ./server port path\n"
# define MAX_CLIENTS    42
# define EOT_CLIENT     "Client %d is disconnected !\n"
# define KILL_SIGINT    "Server was kiled by SIGINT\n"

typedef struct
{
    Client  clients[MAX_CLIENTS];
    int     size;
    int     max_fd;
    char    *cwd;
}           Manager;

SOCKET  init_connection(in_addr_t  ip, unsigned int port);
void    listen_clients(fd_set *rdfs, Manager *manager);
bool    new_client(SOCKET sock, fd_set *rdfs, Manager *manager);
void    server(unsigned int port);

#endif /* !SERVER_H */
