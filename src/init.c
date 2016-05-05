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
