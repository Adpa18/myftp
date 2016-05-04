/*
** ftp_cmd_mode.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed May 04 14:31:04 2016 Adrien WERY
** Last update	Wed May 04 16:19:52 2016 Adrien WERY
*/

#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <common.h>
#include "array.h"
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
    client->sock_data = init_connection(INADDR_ANY, 256 * ports[0] + ports[1]);
    printf("PASV port = %d\n", 256 * ports[0] + ports[1]);
    client->mode = DATA_PASV;
    return (concat("227 Entering Passive Mode (", buff, ")"CRLF));
}

char    *ftp_port(const char *cmd_line, Client *client)
{
    char    **array;
    char    **data_ip;

    if ((array = split(cmd_line, " ")) == NULL || !array[1])
    {
        if (array)
            free_array(array);
        return (strdup(ERROR_CMD));
    }
    if ((data_ip = split(array[1], ",")) == NULL || !data_ip[0] || !data_ip[1]
        || !data_ip[2] || !data_ip[3] || !data_ip[4] || !data_ip[5])
    {
        if (data_ip)
            free_array(data_ip);
        return (strdup(ERROR_CMD));
    }
    sprintf(client->ip_client, "%s.%s.%s.%s", data_ip[0], data_ip[1], data_ip[2], data_ip[3]);
    client->port_client = 256 * atoi(data_ip[4]) + atoi(data_ip[5]);
    if (client->sock_data != -1)
        close(client->sock_data);
    if ((client->sock_data = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        client->sock_data = -1;
    }
    client->mode = DATA_PORT;
    free_array(array);
    free_array(data_ip);
    return (strdup(PORT_OK));
}

char    *run_port(Client *client, cmd_func func, const char *cmd)
{
    pid_t       pid;
    SOCKADDR_IN s_in;
    struct timeval to;

    to.tv_sec = 30;
    to.tv_usec = 0;
    s_in.sin_family = AF_INET;
    s_in.sin_port = htons(client->port_client);
    s_in.sin_addr.s_addr = inet_addr(client->ip_client);
    setsockopt(client->sock_data, SOL_SOCKET, SO_RCVTIMEO, (char *)&to, sizeof(to));
    if (connect(client->sock_data, (SOCKADDR *)&s_in, sizeof(s_in)) == -1)
    {
        perror("conect");
        return (strdup("500 Connect Failed"CRLF));
    }
    if ((pid = fork()) == -1)
    {
        perror("fork");
        return (strdup("500 Fork Failed"CRLF));
    }
    else if (pid == 0)
    {
        func(client->sock_data, cmd);
        exit(0);
    }
    close(client->sock_data);
    wait(NULL);
    return (NULL);
}

char    *run_pasv(Client *client, cmd_func func, const char *cmd)
{
    pid_t       pid;
    int         csock;
    SOCKADDR_IN csin;
    socklen_t   sinsize;
    struct timeval to;

    sinsize = sizeof(csin);
    to.tv_sec = 30;
    to.tv_usec = 0;
    setsockopt (client->sock_data, SOL_SOCKET, SO_RCVTIMEO, (char *)&to, sizeof(to));
    if ((csock = accept(client->sock_data, (SOCKADDR *)&csin, &sinsize)) == -1)
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
        close(client->sock_data);
        func(csock, cmd);
        close(csock);
        exit(0);
    }
    wait(NULL);
    close(csock);
    return (NULL);
}
