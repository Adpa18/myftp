/*
** ftp_cmd_wd.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed May 04 14:31:14 2016 Adrien WERY
** Last update	Thu May 12 12:24:43 2016 Adrien WERY
*/

#include <unistd.h>
#include <stdlib.h>
#include <common.h>
#include "socket.h"
#include "cmdlist.h"
#include "array.h"

char    *ftp_pwd(const char *cmd_line, Client *client)
{
    char    *path;

    (void)cmd_line;
    (void)client;
    path = &client->cwd[strlen(client->root)];
    if (!path || path[0] == 0)
    {
        return (strdup("257 \"/\" is working directory"CRLF));
    }
    return (concat("257 \"", path, "\" is working directory"CRLF));
}

char    *ftp_cdup(const char *cmd_line, Client *client)
{
    char    *path;
    char    *tmp;

    (void)cmd_line;
    path = client->cwd;
    tmp = path;
    path = &path[strlen(client->root)];
    if (path && path[0] != 0 && (path[0] != '/' || path[1]))
        chdir("../");
    free(tmp);
    client->cwd = getcwd(NULL, 0);
    return (strdup(CWD_OK));
}

char    *ftp_cwd(const char *cmd_line, Client *client)
{
    char    **ar;
    char    *last_path;
    char    *path;

    if (!(ar = split(cmd_line, " ")) || !ar[1])
    {
        return (free_array(ar), strdup(ERROR_CMD));
    }
    last_path = getcwd(NULL, 0);
    path = (ar[1][0] == '/') ? concat(client->root, ar[1], 0) : strdup(ar[1]);
    if (chdir(path) == -1)
    {
        free_array(ar);
        return (free(last_path), free(path), strdup(FAILED_DIR));
    }
    free(path);
    path = getcwd(NULL, 0);
    if (strlen(path) < strlen(client->root))
        chdir(last_path);
    free_array(ar);
    free(path);
    free(last_path);
    free(client->cwd);
    client->cwd = getcwd(NULL, 0);
    return (strdup(CWD_OK));
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

    if (client->sock_data == -1 || client->mode == DATA_NO)
        return (strdup(PASV_PORT));
    if (!(array = split(cmd_line, " ")))
        return (strdup(ERROR_CMD));
    cmd = concat("ls -ln ", array[1], "\0");
    write_socket(client->sock, LIST_PENDING);
    if (client->mode == DATA_PASV)
        ret = run_pasv(client, &cmd_list, cmd);
    else
        ret = run_port(client, &cmd_list, cmd);
    free(cmd);
    free_array(array);
    client->mode = DATA_NO;
    if (ret)
        return (ret);
    return (strdup(LIST_OK));
}
