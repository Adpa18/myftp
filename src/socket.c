/*
** socket.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed May 04 14:31:42 2016 Adrien WERY
** Last update	Wed May 04 14:31:43 2016 Adrien WERY
*/

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "socket.h"

extern bool    killed;

bool    write_socket(SOCKET sock, const char *buffer)
{
    if (!buffer)
        return (false);
    write(sock, buffer, strlen(buffer));
    return (true);
}

char    *read_socket(SOCKET sock)
{
    char    *str;
    char    *tmp;
    char    buffer[BUFF_SIZE];
    int     n;

    str = NULL;
    while (!killed)
    {
        if ((n = read(sock, buffer, BUFF_SIZE - 1)) == -1)
        {
            perror("read");
            return (NULL);
        }
        if (n == 0)
            break;
        buffer[n] = 0;
        tmp = str;
        str = concat(str, buffer, "\r\n\0");
        free(tmp);
        if (buffer[n - 1] == '\n' && buffer[n - 2] == '\r')
            break;
    }
    return (str);
}

void	cat(int in, int out)
{
    char    buffer[BUFF_SIZE];
    int     len;

    while ((len = read(in, buffer, BUFF_SIZE)) > 0)
        write(out, buffer, len);
    if (len == -1)
        perror("read");
}

void    send_file(int sock, const char *filename)
{
    int	fd;

    if ((fd = open(filename, O_RDONLY)) == -1)
    {
        perror(filename);
    }
    else
    {
        cat(fd, sock);
        close(fd);
    }
}

void    recv_file(int sock, const char *filename)
{
    int	fd;

    if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
    {
        perror(filename);
    }
    else
    {
        cat(sock, fd);
        close(fd);
    }
}
