/*
** ftp_cmd_wd.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed May 04 14:31:14 2016 Adrien WERY
** Last update	Wed May 04 14:31:16 2016 Adrien WERY
*/

#include <unistd.h>
#include <stdlib.h>
#include "socket.h"
#include "cmdlist.h"
#include "array.h"

char    *ftp_pwd(const char *cmd_line, Client *client)
{
    char    *path;
    char    *tmp;

    (void)cmd_line;
    (void)client;
    tmp = getcwd(NULL, 0);
    path = &tmp[strlen(client->root)];
    if (!path || path[0] == 0)
    {
        free(tmp);
        return (strdup("257 \"/\" is working directory"CRLF));
    }
    path = concat("257 \"", path, "\" is working directory"CRLF);
    free(tmp);
    return (path);
}

char    *ftp_cdup(const char *cmd_line, Client *client)
{
    char    *path;
    char    *tmp;

    (void)cmd_line;
    path = getcwd(NULL, 0);
    tmp = path;
    path = &path[strlen(client->root)];
    if (path && path[0] != 0 && (path[0] != '/' || path[1]) )
        chdir("../");
    free(tmp);
    return (strdup(CWD_OK));
}

char    *ftp_cwd(const char *cmd_line, Client *client)
{
    char    **array;
    char    *last_path;
    char    *path;

    if (!(array = split(cmd_line, " ")) || !array[1])
    {
        if (array)
            free_array(array);
        return (strdup(ERROR_CMD));
    }
    last_path = getcwd(NULL, 0);
    if (chdir(array[1]) == -1)
    {
        free_array(array);
        free(last_path);
        return (strdup(FAILED_DIR));
    }
    path = getcwd(NULL, 0);
    if (strlen(path) < strlen(client->root))
        chdir(last_path);
    free_array(array);
    free(path);
    free(last_path);
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
    client->use_mode = client->mode;
    client->mode = DATA_NO;
    if (ret)
        return (ret);
    return (strdup(LIST_OK));
}
