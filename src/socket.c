//
// Created by wery_a on 26/04/16.
//

#include "socket.h"

bool    write_socket(SOCKET sock, const char *buffer)
{
    if (send(sock, buffer, strlen(buffer), 0) < 0)
        return (false);
    return (true);
}

int     read_socket(SOCKET sock, char *buffer)
{
    int n = 0;

    if ((n = recv(sock, buffer, BUFF_SIZE - 1, 0)) < 0)
        n = 0;
    buffer[n] = 0;
    return (n);
}
