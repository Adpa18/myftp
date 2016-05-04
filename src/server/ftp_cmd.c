//
// Created by wery_a on 02/05/16.
//

#include <unistd.h>
#include <stdlib.h>
#include "common.h"
#include "array.h"
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

    if (client->sock_data == -1 || client->mode != DATA_PASV)
        return (strdup(PASV_PORT));
    if (!(array = split(cmd_line, " ")))
        return (strdup(ERROR_CMD));
    cmd = concat("ls -ln ", array[1], "\0");
    write_socket(client->sock, LIST_PENDING);
    ret = run_pasv(client->sock_data, &cmd_list, cmd);
    free(cmd);
    free_array(array);
    client->use_mode = DATA_PASV;
    client->mode = NO_DATA;
    if (ret)
        return (ret);
    return (strdup(LIST_OK));
}