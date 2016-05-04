//
// Created by wery_a on 02/05/16.
//

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <common.h>
#include "array.h"
#include "common.h"
#include "socket.h"
#include "cmdlist.h"
#include "cmd_help.h"

char    *ftp_no_cmd(const char *cmd_line, Client *client)
{
    (void)cmd_line;
    (void)client;
    return (strdup(NOT_IMP_CMD));
}

char    *ftp_noop(const char *cmd_line, Client *client)
{
    (void)cmd_line;
    (void)client;
    return (strdup(NOOP_CMD));
}

char    *ftp_help(const char *cmd_line, Client *client)
{
    char    *out;
    char    *tmp;

    (void) cmd_line;
    (void) client;
    out = strdup("214 - The following are the FTP commands:\n");
    for (int i = 0; i < NB_CMD; ++i) {
        tmp = out;
        out = concat(out, cmdlist_help[i], "\n");
        free(tmp);
    }
    return (out);
}

char    *run_pasv(int sock_pasv, cmd_func func, const char *cmd)
{
    pid_t       pid;
    int         csock;
    SOCKADDR_IN csin;
    socklen_t   sinsize;
    struct timeval timeout;

    sinsize = sizeof(csin);
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    setsockopt (sock_pasv, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    if ((csock = accept(sock_pasv, (SOCKADDR *)&csin, &sinsize)) == -1)
    {
        perror("accept");
        return (strdup("500 Accept Failed"CRLF));
    }
    if ((pid = fork()) == -1)
    {
        perror("fork");
        return (strdup("500 Fork Failed"CRLF));
    }
    else if (pid == 0)
        func(csock, cmd);
    else
    {
        wait(NULL);
        close(csock);
    }
    return (NULL);
}

void    cmd_list(int sock, const char *cmd)
{
    dup2(sock, 1);
    execlp("/bin/sh", "/bin/sh", "-c", cmd, NULL);
}

char    *ftp_list(const char *cmd_line, Client *client)
{
    char        **array;
    char        *cmd;
    char        *ret;

    if (client->sock_pasv == -1 || client->mode != DATA_PASV)
        return (strdup(PASV_PORT));
    if (!(array = split(cmd_line, " ")))
        return (strdup(ERROR_CMD));
    cmd = concat("ls -ln ", array[1], "\0");
    write_socket(client->sock, LIST_PENDING);
    ret = run_pasv(client->sock_pasv, &cmd_list, cmd);
    free(cmd);
    free_array(array);
    client->use_mode = DATA_PASV;
    if (ret)
        return (ret);
    return (strdup(LIST_OK));
}