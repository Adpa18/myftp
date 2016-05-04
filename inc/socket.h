//
// Created by wery_a on 26/04/16.
//

#ifndef PSU_2015_MYFTP_SOCKET_H
# define PSU_2015_MYFTP_SOCKET_H

# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include "common.h"

bool    write_socket(SOCKET sock, const char *buffer);
int     read_socket(SOCKET sock, char *buffer);
void	cat(int in, int out);
void    send_file(int sock, const char *filename);
void    recv_file(int sock, const char *filename);

#endif //PSU_2015_MYFTP_SOCKET_H
