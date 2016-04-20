/*
** common.h for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Tue Apr 19 21:45:38 2016 Adrien WERY
** Last update	Wed Apr 20 22:03:02 2016 Adrien WERY
*/

#ifndef COMMON_H
# define COMMON_H

# define BUFF_SIZE  4

typedef enum { false, true } bool;

char    *concat(const char *s1, const char *s2, const char *lim);
char    *getLine(int fd);
char    *exec(char **cmds);

#endif /* !COMMON_H */
