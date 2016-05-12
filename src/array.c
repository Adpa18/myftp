/*
** array.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed May 04 14:31:31 2016 Adrien WERY
** Last update	Wed May 04 14:31:32 2016 Adrien WERY
*/

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "array.h"

inline int     array_len(const char **array)
{
    int i;

    i = -1;
    while (array[++i]);
    return (i);
}

inline void     free_array(char **array)
{
    int i;

    if (array)
    {
        i = -1;
        while (array[++i])
            free(array[i]);
        free(array);
    }
}

inline void     print_array(const char **array)
{
    int i;

    i = -1;
    while (array[++i])
    {
        write(1, array[i], strlen(array[i]));
        write(1, "\n", 1);
    }
}

int             getNbElems(const char *s, const char *delim)
{
    int     nb_elems;
    char    *copy;
    char    *str;

    nb_elems = 0;
    copy = strdup(s);
    str = strtok(copy, delim);
    while (str)
    {
        ++nb_elems;
        str = strtok(NULL, delim);
    }
    free(copy);
    return (nb_elems);
}

inline char     **split(const char *s, const char *delim)
{
    char    **array;
    char    *copy;
    char    *str;
    int     nb_elems;
    int     i;

    nb_elems = getNbElems(s, delim);
    copy = strdup(s);
    if ((array = malloc(sizeof(char *) * (nb_elems + 1))) == NULL)
        return (NULL);
    str = strtok(copy, delim);
    for (i = 0; str; ++i)
    {
        array[i] = strdup(str);
        str = strtok(NULL, delim);
    }
    free(copy);
    array[i] = NULL;
    return (array);
}
