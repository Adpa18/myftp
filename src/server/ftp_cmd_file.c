//
// Created by wery_a on 03/05/16.
//

#include <unistd.h>
#include <sys/stat.h>
#include "cmdlist.h"
#include "array.h"

char    *ftp_dele(const char *cmd_line, Client *client)
{
    char    **array;
    char    *ret;
    struct stat st;

    (void)client;
    if (!(array = split(cmd_line, " ")) || !array[1])
    {
        if (array)
            free_array(array);
        return (strdup(ERROR_CMD));
    }
    if (stat(array[1], &st) == -1)
        ret = concat("521 ", array[1], "file don't exist"CRLF);
    else
    {
        unlink(array[1]);
        ret = concat("250 ", array[1], " deleted"CRLF);
    }
    free_array(array);
    return (ret);
}

char    *ftp_mkd(const char *cmd_line, Client *client)
{
    char    **array;
    char    *ret;
    struct stat st;

    (void)client;
    if (!(array = split(cmd_line, " ")) || !array[1])
        return (strdup(ERROR_CMD));
    if (stat(array[1], &st) != -1)
        ret = concat("521 ", array[1], "directory already exists"CRLF);
    else
    {
        mkdir(array[1], 0744);
        ret = concat("257 ", array[1], "directory created"CRLF);
    }
    free_array(array);
    return (ret);
}

char    *ftp_rmd(const char *cmd_line, Client *client)
{
    char    **array;
    char    *ret;
    struct stat st;

    (void)client;
    if (!(array = split(cmd_line, " ")) || !array[1])
        return (strdup(ERROR_CMD));
    if (stat(array[1], &st) == -1)
        ret = concat("521 ", array[1], "directory don't exists"CRLF);
    else
    {
        rmdir(array[1]);
        ret = concat("250 ", array[1], "directory deleted"CRLF);
    }
    return (ret);
}

//char    *ftp_rnfr(const char *cmd_line, Client *client)
//{
//
//}
//
//char    *ftp_rnto(const char *cmd_line, Client *client)
//{
//
//}