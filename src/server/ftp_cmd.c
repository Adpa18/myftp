//
// Created by wery_a on 02/05/16.
//

#include <common.h>
#include <pwd.h>
#include <crypt.h>
#include "array.h"
#include "cmdlist.h"

char    *ftp_noop(const char *cmd_line, Client *client)
{
    printf("%s\n", cmd_line);
    (void)client;
    return (concat(UNKNOW_CMD, NULL, NULL));
}

char    *ftp_user(const char *cmd_line, Client *client)
{
    char    **array;

    if ((array = split(cmd_line, " ")) == NULL || !array[1])
        return (strdup("530 Invalid username"));
    client->status = USER_OK;
    strcpy(client->username, array[1]);
    return (strdup(USERNAME_OK));
}

char    *ftp_pass(const char *cmd_line, Client *client)
{
    char    **array;
    struct passwd   *pw;

    if ((array = split(cmd_line, " ")) == NULL)
        return (strdup("530 Invalid password"));
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
    return (strdup(LOGIN_SUCCESS));
}