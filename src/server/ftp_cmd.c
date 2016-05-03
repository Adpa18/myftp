//
// Created by wery_a on 02/05/16.
//

#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <crypt.h>
#include <errno.h>
#include <common.h>
#include <time.h>
#include "common.h"
#include <arpa/inet.h>
#include "array.h"
#include "cmdlist.h"
#include "server.h"

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

char    *ftp_user(const char *cmd_line, Client *client)
{
    char    **array;

    if (client->status == USER_OK)
        return (strdup(USERNAME_OK));
    if (client->status == LOGGED_IN)
        return (strdup(CANT_USER));
    if ((array = split(cmd_line, " ")) == NULL || !array[1])
        return (strdup(INVALID_USER));
    client->status = USER_OK;
    strcpy(client->username, array[1]);
    return (concat(USERNAME_OK, client->username, NULL));
}

char    *ftp_pass(const char *cmd_line, Client *client)
{
    char    **array;
    struct passwd   *pw;

    if (client->status == NONE)
        return (strdup(LOGIN_FIRST));
    if (client->status == LOGGED_IN)
        return (concat(LOGIN_ALREADY, client->username, NULL));
    if ((array = split(cmd_line, " ")) == NULL)
        return (strdup(INVALID_PASS));
    if (strncmp(client->username, "Anonymous", 9) || array[1])
    {
        if ((pw = getpwnam(client->username)) == NULL)
            return (strdup(LOGIN_FAILED));
        if (pw->pw_passwd != '\0')
        {
            if (!array[1] || strcmp(crypt(array[1], pw->pw_passwd), pw->pw_passwd))
                return (strdup(LOGIN_FAILED));
        }
    }
    client->status = LOGGED_IN;
    return (concat(LOGIN_SUCCESS, client->username, NULL));
}

char    *ftp_quit(const char *cmd_line, Client *client)
{
    (void)cmd_line;
    (void)client;
//    make remove of client
    return (strdup(BYE_BYE));
}

char    *ftp_help(const char *cmd_line, Client *client)
{
    (void)cmd_line;
    (void)client;
    close(client->sock);
    return (strdup(HELP_CMD));
}

char    *ftp_pwd(const char *cmd_line, Client *client)
{
    char    *path;

    (void)cmd_line;
    (void)client;
    path = getcwd(NULL, 0);
    path = &path[strlen(client->root)];
    if (!path || path[0] == 0)
        path[0] = '/';
    return (concat("257 \"", path, "\" is working directory"CRLF));
}

char    *ftp_cdup(const char *cmd_line, Client *client)
{
    char    *path;

    (void)cmd_line;
    path = getcwd(NULL, 0);
    path = &path[strlen(client->root)];
    if (path && path[0] != 0 && (path[0] != '/' || path[1]) )
        chdir("../");
    return (strdup(CWD_OK));
}

char    *ftp_cwd(const char *cmd_line, Client *client)
{
    char    **array;
    char    *last_path;
    char    *path;

    if (!(array = split(cmd_line, " ")) || !array[1])
        return (strdup(ERROR_CMD));
    last_path = getcwd(NULL, 0);

    if (chdir(replace(array[1], '\n', 0)) == -1)
        return (strdup(FAILED_DIR));
    path = getcwd(NULL, 0);
    if (strlen(path) < strlen(client->root))
        chdir(last_path);
    return (strdup(CWD_OK));
}

char    *ftp_pasv(const char *cmd_line, Client *client)
{
    char    *pasv;
    int     ports[2];
    char    buff[BUFF_SIZE];

    (void)cmd_line;
    srand(time(NULL));
    ports[0] = 128 + (rand() % 128);
    ports[1] = rand() % 0xff;
    pasv = replace(inet_ntoa(client->addr->sin_addr), '.', ',');
    sprintf(buff, "%s,%d,%d", pasv, ports[0], ports[1]);
    close(client->sock_pasv);
    client->sock_pasv = init_connection(256 * ports[0] + ports[1]);
    client->mode = DATA_PASV;
    return (concat("227 Entering Passive Mode (", buff, ")"CRLF));
}