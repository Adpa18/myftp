//
// Created by wery_a on 02/05/16.
//

#ifndef PSU_2015_MYFTP_CMDLIST_H
# define PSU_2015_MYFTP_CMDLIST_H

# include "common.h"

# define CRLF       "\r\n"
# define WELCOME        "220 Welcome to my house"CRLF
# define USERNAME_OK    "331 User name okay, need password"CRLF
# define LOGIN_SUCCESS  "230 Login successful"CRLF
# define LOGIN_FAILED   "530 Login failed"CRLF
# define UNKNOW_CMD     "500 Unknown command"CRLF

char    *ftp_noop(const char *cmd_line, Client *client);
char    *ftp_user(const char *cmd_line, Client *client);
char    *ftp_pass(const char *cmd_line, Client *client);

#endif //PSU_2015_MYFTP_CMDLIST_H
