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
typedef void    (*cmd_func)(int sock, const char *cmd);

typedef enum STATE
{
    NONE,
    USER_OK,
    LOGGED_IN
} STATE;

typedef enum MODE_DATA
{
    DATA_NO,
    DATA_PASV,
    DATA_PORT
} MODE_DATA;

typedef struct
{
    SOCKET      server_sock;
    SOCKET      sock;
    SOCKET      sock_data;
    char        *root;
    STATE       status;
    int         addr;
    MODE_DATA   mode;
    MODE_DATA   use_mode;
    char        username[255];
}               Client;

char    *run_pasv(int sock_data, cmd_func func, const char *cmd);
char    *concat(const char *s1, const char *s2, const char *lim);
char    *replace(char *s, char c, char by);
char    *getLine(int fd);
char    *exec(char *cmd_line);

#endif /* !COMMON_H */
