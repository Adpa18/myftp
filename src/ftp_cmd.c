/*
** ftp_cmd.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed May 04 14:31:18 2016 Adrien WERY
** Last update	Wed May 04 14:31:20 2016 Adrien WERY
*/

#include <unistd.h>
#include <stdlib.h>
#include <common.h>
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

char    *ftp_type(const char *cmd_line, Client *client)
{
    (void)cmd_line;
    (void)client;
    return (strdup(TYPE_OK));
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
