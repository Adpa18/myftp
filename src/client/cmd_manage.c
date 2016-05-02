//
// Created by wery_a on 02/05/16.
//

#include <unistd.h>
#include <stdlib.h>
#include "client.h"
#include "array.h"

extern bool killed;

bool    fileManage(const char *cmd, int sock)
{
    char    **cmds;
    bool    ret;

    ret = true;
    if (!(cmds = split(cmd, " ")))
        return (false);
    if (cmds[0] && cmds[1])
    {
        if (!strcmp(cmds[0], "put"))
            send_file(cmds[1], sock);
        else if (!strcmp(cmds[0], "get"))
            recv_file(cmds[1], sock);
        else if (!strcmp(cmds[0], "mput"))
            for (int i = 1; cmds[i]; ++i)
                send_file(cmds[i], sock);
        else if (!strcmp(cmds[0], "mget"))
            for (int i = 1; cmds[i]; ++i)
                recv_file(cmds[i], sock);
        else
            ret = false;
    }
    free_array(cmds);
    return (ret);
}

bool        local_cmd(char *cmd)
{
    char    *ret;

    if (cmd[0] != 'l' || !cmd[1] || (ret = exec(&cmd[1])) == NULL)
        return (false);
    write(1, ret, strlen(ret));
    free(ret);
    return (true);
}

void    shell(int sock)
{
    char    *cmd_line;

    if (!(cmd_line = getLine(0)))
        return;
    if (!strcmp(cmd_line, "quit"))
        killed = true;
    else if (!local_cmd(cmd_line))
    {
        fileManage(cmd_line, sock);
        write_socket(sock, cmd_line);
    }
    free(cmd_line);
}