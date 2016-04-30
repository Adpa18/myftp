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

# define USAGE          "Usage : ./server port\n"
# define MAX_CLIENTS    42
# define CRLF	        "\r\n"
# define EOT_CLIENT     "Client %d is disconnected !\n"

typedef struct
{
    SOCKET  sock;
}           Client;

typedef struct
{
    Client  clients[MAX_CLIENTS];
    int     size;
    int     max_fd;
}           Manager;

#endif /* !SERVER_H */
