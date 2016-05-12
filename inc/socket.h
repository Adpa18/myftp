/*
** socket.h for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed May 04 14:30:23 2016 Adrien WERY
** Last update	Wed May 04 14:30:26 2016 Adrien WERY
*/

#ifndef PSU_2015_MYFTP_SOCKET_H
# define PSU_2015_MYFTP_SOCKET_H

# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include "common.h"

bool    write_socket(SOCKET sock, const char *buffer);
char    *read_socket(SOCKET sock);
void	cat(int in, int out);
void    send_file(int sock, const char *filename);
void    recv_file(int sock, const char *filename);

#endif //PSU_2015_MYFTP_SOCKET_H
