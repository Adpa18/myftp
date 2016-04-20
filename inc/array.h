/*
** array.h for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed Apr 20 21:54:53 2016 Adrien WERY
** Last update	Wed Apr 20 22:43:47 2016 Adrien WERY
*/

#ifndef ARRAY_H
#define ARRAY_H

int     array_len(char **array);
void    free_array(char **array);
void    print_array(char **array);
char    **split(char *s, char *delim);

#endif /* !ARRAY_H */
