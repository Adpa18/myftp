/*
** server.h for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Sun Apr 10 16:43:36 2016 Adrien WERY
** Last update	Tue Apr 19 22:24:19 2016 Adrien WERY
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
    const char  *cwd;
}           Manager;

SOCKET  init_connection(unsigned int port);
void    listen_clients(fd_set *rdfs, Manager *manager);
void    new_client(SOCKET sock, fd_set *rdfs, Manager *manager);

#endif /* !SERVER_H */
