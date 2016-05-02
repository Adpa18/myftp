//
// Created by wery_a on 26/04/16.
//

#include "socket.h"
#include <unistd.h>
#include <fcntl.h>

bool    write_socket(SOCKET sock, const char *buffer)
{
    if (!buffer)
        return (false);
    write(sock, buffer, strlen(buffer));
    return (true);
}

int     read_socket(SOCKET sock, char *buffer)
{
    int     n;

    n = 0;
    if ((n = read(sock, buffer, BUFF_SIZE - 1)) == -1)
    {
        perror("read");
        n = 0;
    }
    buffer[n] = 0;
    return (n);
}

void	cat(int in, int out)
{
    char    buffer[BUFF_SIZE];
    int     len;

//    while (read_socket(in, buffer) > 0)
//        write_socket(out, buffer);
    while ((len =  read(in, buffer, BUFF_SIZE)) > 0)
    {
        printf("len = %d\n", len);
        write(out, buffer, len);
    }
//    cat error still read on sock
}

bool    send_file(const char *filename, int sock)
{
    int	fd;

    if ((fd = open(filename, O_RDONLY)) == -1)
    {
        perror(filename);
        return (false);
    }
    cat(fd, sock);
    close(fd);
    return (true);
}

int     recv_file(const char *filename, int sock)
{
    int	fd;

    printf("%s\n", filename);
    if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
    {
        perror(filename);
        return (false);
    }
    cat(sock, fd);
    close(fd);
    return (true);
}