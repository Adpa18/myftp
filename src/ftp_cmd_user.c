/*
** ftp_cmd_user.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed May 04 14:31:08 2016 Adrien WERY
** Last update	Thu May 12 13:37:09 2016 Adrien WERY
*/

#include <pwd.h>
#include <crypt.h>
#include "array.h"
#include "cmdlist.h"

char    *ftp_user(const char *cmd_line, Client *client)
{
    char    **array;

    if (client->status == LOGGED_IN)
        return (strdup(CANT_USER));
    if ((array = split(cmd_line, " ")) == NULL || !array[1])
    {
        free_array(array);
        return (strdup(INVALID_USER));
    }
    client->status = USER_OK;
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
    if (strncasecmp(client->username, "Anonymous", 9) || array[1])
    {
        if ((pw = getpwnam(client->username)) == NULL
            || (pw->pw_passwd != '\0' && (!array[1] ||
                strcmp(crypt(array[1], pw->pw_passwd), pw->pw_passwd))))
        {
            free_array(array);
            return (strdup(LOGIN_FAILED));
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
    return (strdup(BYE_BYE));
}
