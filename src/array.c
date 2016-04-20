/*
** array.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Apr 20 21:52:23 2016 Adrien WERY
** Last update	Wed Apr 20 22:46:12 2016 Adrien WERY
*/

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "array.h"

inline int     array_len(char **array)
{
    int i;

    i = -1;
    while (array[++i]);
    return (i);
}

inline void     free_array(char **array)
{
    int i;

    i = -1;
    while (array[++i])
        free(array[i]);
    free(array);
}

inline void     print_array(char **array)
{
    int i;

    i = -1;
    while (array[++i])
    {
        write(1, array[i], strlen(array[i]));
        write(1, "\n", 1);
    }
}

inline char     **split(char *s, char *delim)
{
    char    **array;
    char    *copy;
    char    *str;
    int     nb_elems;
    int     i;

    nb_elems = 0;
    copy = strdup(s);
    str = strtok(copy, delim);
    while (str)
    {
        ++nb_elems;
        str = strtok(NULL, delim);
    }
    free(copy);
    if ((array = malloc(sizeof(char *) * (nb_elems + 1))) == NULL)
        return (NULL);
    str = strtok(s, delim);
    i = -1;
    while (str)
    {
        array[++i] = strdup(str);
        str = strtok(NULL, delim);
    }
    array[++i] = NULL;
    return (array);
}
