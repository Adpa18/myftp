/*
** ftp_cmd_mode.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed May 04 14:31:04 2016 Adrien WERY
** Last update	Thu May 12 12:23:49 2016 Adrien WERY
*/

#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <common.h>
#include "common.h"
#include "array.h"
#include "server.h"
#include "cmdlist.h"

char    *ftp_pasv(const char *cmd_line, Client *client)
{
    IN_ADDR addr;
    char    *pasv;
    int     p[2];
    char    buff[BUFF_SIZE];

    (void)cmd_line;
    srand(time(NULL));
    p[0] = 128 + (rand() % 128);
    p[1] = rand() % 0xff;
    addr.s_addr = client->addr;
    pasv = replace(inet_ntoa(addr), '.', ',');
    sprintf(buff, "%s,%d,%d", pasv, p[0], p[1]);
    if (client->sock_data != -1)
        close(client->sock_data);
    client->sock_data = init_connection(INADDR_ANY, 256 * p[0] + p[1]);
    client->mode = DATA_PASV;
    return (concat("227 Entering Passive Mode (", buff, ")"CRLF));
}

char    *ftp_port(const char *cmd_line, Client *cl)
{
    char    **array;
    char    **d_ip;

    if ((array = split(cmd_line, " ")) == NULL || !array[1])
        return (free_array(array), strdup(ERROR_CMD));
    if ((d_ip = split(array[1], ",")) == NULL || !d_ip[0] || !d_ip[1]
        || !d_ip[2] || !d_ip[3] || !d_ip[4] || !d_ip[5])
        return (free_array(d_ip), strdup(ERROR_CMD));
    sprintf(cl->ip_client, "%s.%s.%s.%s",
            (d_ip[0][0] == '(') ? &d_ip[0][1] : d_ip[0], d_ip[1],
            d_ip[2], replace(d_ip[3], ')', 0));
    cl->port_client = 256 * atoi(d_ip[4]) + atoi(d_ip[5]);
    if (cl->sock_data != -1)
        close(cl->sock_data);
    if ((cl->sock_data = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        cl->sock_data = -1;
    }
    cl->mode = DATA_PORT;
    free_array(array);
    free_array(d_ip);
    return (strdup(PORT_OK));
}

char    *run_port(Client *cl, cmd_func func, const char *cmd)
{
    pid_t       pid;
    SOCKADDR_IN s_in;
    struct timeval t;

    t.tv_sec = 30;
    t.tv_usec = 0;
    s_in.sin_family = AF_INET;
    s_in.sin_port = htons(cl->port_client);
    s_in.sin_addr.s_addr = inet_addr(cl->ip_client);
    setsockopt(cl->sock_data, SOL_SOCKET, SO_RCVTIMEO, (char *)&t, sizeof(t));
    if (connect(cl->sock_data, (SOCKADDR *)&s_in, sizeof(s_in)) == -1)
        return (perror("conect"), strdup("500 Connect Failed"CRLF));
    if ((pid = fork()) == -1)
        return (perror("fork"), strdup("500 Fork Failed"CRLF));
    else if (pid == 0)
    {
        func(cl->sock_data, cmd);
        exit(0);
    }
    close(cl->sock_data);
    wait(NULL);
    return (NULL);
}

char    *run_pasv(Client *cl, cmd_func func, const char *cmd)
{
    pid_t       pid;
    int         csock;
    SOCKADDR_IN csin;
    socklen_t   sinsize;
    struct timeval t;

    sinsize = sizeof(csin);
    t.tv_sec = 30;
    t.tv_usec = 0;
    setsockopt (cl->sock_data, SOL_SOCKET, SO_RCVTIMEO, (char *)&t, sizeof(t));
    if ((csock = accept(cl->sock_data, (SOCKADDR *)&csin, &sinsize)) == -1)
        return (perror("accept"), strdup("500 Accept Failed"CRLF));
    if ((pid = fork()) == -1)
        return (perror("fork"),  close(csock), strdup("500 Fork Failed"CRLF));
    else if (pid == 0)
    {
        close(cl->sock_data);
        func(csock, cmd);
        close(csock);
        exit(0);
    }
    wait(NULL);
    close(csock);
    return (NULL);
}
