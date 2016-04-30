/*
** client.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Sun Apr 10 16:54:20 2016 Adrien WERY
** Last update	Sat Apr 30 18:20:50 2016 Adrien WERY
*/

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "cmd.h"
#include "client.h"

bool        local_cmd(char *cmd)
{
    char    *ret;

    if (cmd[0] != 'l' || (ret = exec(&cmd[1])) == NULL)
        return (false);
    write(1, ret, strlen(ret));
    free(ret);
    return (true);
}

SOCKET      init_connection(const char *ip_address, unsigned int port)
{
    SOCKET  sock;
    SOCKADDR_IN s_in;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return (-1);
    }
    s_in.sin_addr.s_addr = inet_addr(ip_address);
    s_in.sin_port = htons(port);
    s_in.sin_family = AF_INET;
    if (connect(sock, (SOCKADDR *) &s_in, sizeof(SOCKADDR)) == -1)
    {
        perror("connect");
        close(sock);
        return (-1);
    }
    return (sock);
}

bool    init_select(fd_set *rdfs, int sock, int first)
{
    if (first == 0)
        write(1, SHELL_PS1, strlen(SHELL_PS1));
    FD_ZERO(rdfs);
    FD_SET(STDIN_FILENO, rdfs);
    FD_SET(sock, rdfs);
    if (select(sock + 1, rdfs, NULL, NULL, NULL) == -1)
    {
        perror("select");
        close(sock);
        return (false);
    }
    return (true);
}

void    shell(int sock)
{
    char    *cmd;

    cmd = getLine(0);
    if (!strcmp(cmd, "quit"))
    {
        close(sock);
        exit(0);
    }
    if (!local_cmd(cmd))
        write_socket(sock, cmd);
    free(cmd);
}

bool    client(char *host, unsigned int port)
{
    SOCKET  sock;
    fd_set  rdfs;
    char    buffer[BUFF_SIZE];

    buffer[0] = 0;
    if ((sock = init_connection(host, port)) == -1)
        return (false);
    while (42)
    {
        if (!init_select(&rdfs, sock, buffer[0]))
            return (false);
        if (FD_ISSET(STDIN_FILENO, &rdfs))
            shell(sock);
        else if (FD_ISSET(sock, &rdfs))
        {
            if (read_socket(sock, buffer) == 0)
            {
                write(1, EOT_SERVER, strlen(EOT_SERVER));
                break;
            }
            write(1, buffer, strlen(buffer));
            buffer[0] = 0;
        }
    }
    return (true);
}

int     main(int ac, char **av)
{
    unsigned int    port;

    if (ac < 3 || !av[1] || (port = atoi(av[2])) == 0)
    {
        write(1, USAGE, strlen(USAGE));
        return (1);
    }
    client(av[1], port);
    return (0);
}
