/*
** cmd.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Tue Apr 19 21:34:01 2016 Adrien WERY
** Last update	Wed Apr 20 23:10:13 2016 Adrien WERY
*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "common.h"
#include "array.h"

char    *pwd()
{
    char    *path;

    if ((path = getcwd(NULL, 0)) == NULL)
    {
        perror("getcwd");
        return (NULL);
    }
    return (concat(path, "\n", NULL));
}

char    *cd(char *path)
{
    if (chdir(path) == -1)
    {
        perror("chdir");
        return (NULL);
    }
    return (concat(path, "\n", NULL));
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
        return (NULL);
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
    closedir(dir);
    tmp = dest;
    dest = concat(tmp, "\n", NULL);
    free(tmp);
    return (dest);
}

char    *exec(char **cmds)
{
    // print_array(cmds);
    if (!strcmp(cmds[0], "ls"))
        return (ls(cmds[1]));
    else if (!strcmp(cmds[0], "pwd"))
        return (pwd());
    else if (!strcmp(cmds[0], "cd"))
        return (cd(cmds[1]));
    return ("Unknow Command");
}
