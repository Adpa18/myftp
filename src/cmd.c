/*
** cmd.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed May 04 14:31:35 2016 Adrien WERY
** Last update	Wed May 04 14:31:37 2016 Adrien WERY
*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include "common.h"
#include "array.h"

char    *pwd()
{
    char    *path;

    if ((path = getcwd(NULL, 0)) == NULL)
    {
        perror("getcwd");
        return (strdup(strerror(errno)));
    }
    return (concat(path, "\n", NULL));
}

char    *cd(char *path)
{
    if (chdir(path) == -1)
    {
        perror("chdir");
        return (strdup(strerror(errno)));
    }
    return (concat(path, "\n", NULL));
}

char    *end_ls(DIR *dir, char *dest)
{
    char    *tmp;

    closedir(dir);
    tmp = dest;
    dest = concat(tmp, "\n", NULL);
    free(tmp);
    return (dest);
}

char    *ls(char *path)
{
    DIR             *dir;
    struct dirent	*curr_file;
    char            *dest;
    char            *tmp;

    if (!path)
        path = ".";
    if (!(dir = opendir(path)))
    {
        perror("opendir");
        return (strdup(strerror(errno)));
    }
    dest = NULL;
    while ((curr_file = readdir(dir)))
    {
        if (curr_file->d_name[0] != '.')
        {
            tmp = dest;
            dest = concat(tmp, curr_file->d_name, (tmp) ? "\n" : NULL);
            if (tmp)
                free(tmp);
        }
    }
    return (end_ls(dir, dest));
}

char    *exec(char *cmd_line)
{
    char    **cmds;
    char    *ret;

    if (!cmd_line || !(cmds = split(cmd_line, " ")))
        return (NULL);
    if (!strcmp(cmds[0], "ls"))
        ret = ls(cmds[1]);
    else if (!strcmp(cmds[0], "pwd"))
        ret = pwd();
    else if (!strcmp(cmds[0], "cd"))
        ret = cd(cmds[1]);
    else
        ret = NULL;
    free_array(cmds);
    return (ret);
}
