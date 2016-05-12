/*
** common.h for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Tue Apr 19 21:45:38 2016 Adrien WERY
** Last update	Thu May 12 12:24:22 2016 Adrien WERY
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
    SOCKET      sock;
    SOCKET      sock_data;
    char        *root;
    char        *cwd;
    STATE       status;
    int         addr;
    MODE_DATA   mode;
    char        username[255];
    char        ip_client[255];
    int         port_client;
}               Client;

char    *run_pasv(Client *client, cmd_func func, const char *cmd);
char    *run_port(Client *client, cmd_func func, const char *cmd);
char    *concat(const char *s1, const char *s2, const char *lim);
char    *replace(char *s, char c, char by);
char    *getLine(int fd);
char    *exec(char *cmd_line);

#endif /* !COMMON_H */
