//
// Created by wery_a on 03/05/16.
//

#include "array.h"
#include "cmdlist.h"
#include <pwd.h>
#include <crypt.h>

char    *ftp_user(const char *cmd_line, Client *client)
{
    char    **array;

//    if (client->status == USER_OK)
//        return (strdup(USERNAME_OK));
    if (client->status == LOGGED_IN)
        return (strdup(CANT_USER));
    if ((array = split(cmd_line, " ")) == NULL || !array[1])
    {
        if (array)
            free_array(array);
        return (strdup(INVALID_USER));
    }
    client->status = USER_OK;
    array[1] = replace(array[1], '\n', '\0');
    strcpy(client->username, array[1]);
    free_array(array);
    return (concat(USERNAME_OK, client->username, CRLF));
}

char    *ftp_pass(const char *cmd_line, Client *client)
{
    char    **array;
    struct passwd   *pw;

    if (client->status == NONE)
        return (strdup(LOGIN_FIRST));
    if (client->status == LOGGED_IN)
        return (concat(LOGIN_ALREADY, client->username, CRLF));
    if ((array = split(cmd_line, " ")) == NULL)
        return (strdup(INVALID_PASS));
    if (strncmp(client->username, "Anonymous", 9) || array[1])
    {
        if ((pw = getpwnam(client->username)) == NULL)
        {
            free_array(array);
            return (strdup(LOGIN_FAILED));
        }
        if (pw->pw_passwd != '\0')
        {
            if (!array[1] || strcmp(crypt(array[1], pw->pw_passwd), pw->pw_passwd))
            {
                free_array(array);
                return (strdup(LOGIN_FAILED));
            }
        }
    }
    client->status = LOGGED_IN;
    free_array(array);
    return (concat(LOGIN_SUCCESS, client->username, CRLF));
}

char    *ftp_quit(const char *cmd_line, Client *client)
{
    (void)cmd_line;
    (void)client;
//    make remove of client
    return (strdup(BYE_BYE));
}