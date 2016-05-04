//
// Created by wery_a on 03/05/16.
//

#include <unistd.h>
#include <stdlib.h>
#include "cmdlist.h"
#include "array.h"

char    *ftp_pwd(const char *cmd_line, Client *client)
{
    char    *path;
    char    *tmp;

    (void)cmd_line;
    (void)client;
    path = getcwd(NULL, 0);
    tmp = path;
    path = &path[strlen(client->root)];
    if (!path || path[0] == 0)
    {
        free(tmp);
        return (strdup("257 \"/\" is working directory"CRLF));
    }
    free(tmp);
    return (concat("257 \"", path, "\" is working directory"CRLF));
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

    if (chdir(replace(array[1], '\n', 0)) == -1)
    {
        free_array(array);
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