/*
** client.h for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Sun Apr 10 16:43:22 2016 Adrien WERY
** Last update	Sat Apr 30 09:47:44 2016 Adrien WERY
*/

#ifndef CLIENT_H
# define CLIENT_H

# include "socket.h"

# define USAGE          "Usage : ./client host port\n"
# define SHELL_PS1      "ftp> "
# define EOT_SERVER     "Server disconnected !\n"
# define KILL_SIGINT    "Client was kiled by SIGINT\n"

void    shell(int sock);

#endif /* !CLIENT_H */
