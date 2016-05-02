/*
** common.h for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Tue Apr 19 21:45:38 2016 Adrien WERY
** Last update	Sat Apr 30 18:17:21 2016 Adrien WERY
*/

#ifndef COMMON_H
# define COMMON_H

# include <string.h>
# include <stdio.h>

# define BUFF_SIZE  1024

typedef enum { false, true } bool;
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

typedef enum STATE
{
    NONE,
    USER_OK,
    LOGGED_IN
} STATE;

typedef struct
{
    SOCKET  sock;
    char    *path;
    STATE   status;
    char    username[255];
}           Client;

char    *concat(const char *s1, const char *s2, const char *lim);
char    *getLine(int fd);
char    *exec(char *cmd_line);

#endif /* !COMMON_H */
