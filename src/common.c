/*
** common.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Tue Apr 19 22:32:37 2016 Adrien WERY
** Last update	Wed May 04 14:31:39 2016 Adrien WERY
*/

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"

inline char    *getLine(int fd)
{
    char    *dest;
    char    *tmp;
    char    buff[BUFF_SIZE + 1];
    bool    done;

    done = false;
    dest = NULL;
    while (!done)
    {
        memset(buff, 0, BUFF_SIZE + 1);
        if (read(fd, buff, BUFF_SIZE) <= 0)
            break;
        for (size_t i = 0; i < BUFF_SIZE; i++)
            if (buff[i] == '\n')
            {
                buff[i] = 0;
                done = true;
                break;
            }
        tmp = dest;
        dest = concat(tmp, buff, NULL);
        if (tmp)
            free(tmp);
    }
    return (dest);
}

inline char *concat(const char *s1, const char *s2, const char *s3)
{
    size_t  len1;
    size_t  len2;
    size_t  len3;
    char    *dest;

    len1 = (s1) ? strlen(s1) : 0;
    len2 = (s2) ? strlen(s2) : 0;
    len3 = (s3) ? strlen(s3) : 0;
    if ((dest = malloc(len1 + len2 + len3 + 1)) == NULL)
        return (NULL);
    if (s1)
        memcpy(dest, s1, len1);
    if (s2)
        memcpy(dest + len1, s2, len2);
    if (s3)
        memcpy(dest + len1 + len2, s3, len3 + 1);
    return (dest);
}

inline char *replace(char *s, char c, char by)
{
    int     i;

    i = -1;
    while (s[++i])
    {
        if (s[i] == c)
        s[i] = by;
    }
    return (s);
}
