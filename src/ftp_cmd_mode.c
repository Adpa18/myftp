/*
** ftp_cmd_mode.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed May 04 14:31:04 2016 Adrien WERY
** Last update	Wed May 04 14:31:05 2016 Adrien WERY
*/

#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <common.h>
#include "server.h"
#include "cmdlist.h"

char    *ftp_pasv(const char *cmd_line, Client *client)
{
    IN_ADDR addr;
    char    *pasv;
    int     ports[2];
    char    buff[BUFF_SIZE];

    (void)cmd_line;
    srand(time(NULL));
    ports[0] = 128 + (rand() % 128);
    ports[1] = rand() % 0xff;
    addr.s_addr = client->addr;
    pasv = replace(inet_ntoa(addr), '.', ',');
    sprintf(buff, "%s,%d,%d", pasv, ports[0], ports[1]);
    if (client->sock_data != -1)
        close(client->sock_data);
    client->sock_data = init_connection(256 * ports[0] + ports[1]);
    printf("PASV port = %d\n", 256 * ports[0] + ports[1]);
    client->mode = DATA_PASV;
    return (concat("227 Entering Passive Mode (", buff, ")"CRLF));
}

char    *run_pasv(int sock_data, cmd_func func, const char *cmd)
{
    pid_t       pid;
    int         csock;
    SOCKADDR_IN csin;
    socklen_t   sinsize;
    struct timeval to;

    sinsize = sizeof(csin);
    to.tv_sec = 30;
    to.tv_usec = 0;
    setsockopt (sock_data, SOL_SOCKET, SO_RCVTIMEO, (char *)&to, sizeof(to));
    if ((csock = accept(sock_data, (SOCKADDR *)&csin, &sinsize)) == -1)
    {
        perror("accept");
        return (strdup("500 Accept Failed"CRLF));
    }
    if ((pid = fork()) == -1)
    {
        perror("fork");
        close(csock);
        return (strdup("500 Fork Failed"CRLF));
    }
    else if (pid == 0)
    {
        close(sock_data);
        func(csock, cmd);
        close(csock);
        exit(0);
    }
    wait(NULL);
    close(csock);
    return (NULL);
}