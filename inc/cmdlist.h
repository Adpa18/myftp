//
// Created by wery_a on 02/05/16.
//

#ifndef PSU_2015_MYFTP_CMDLIST_H
# define PSU_2015_MYFTP_CMDLIST_H

# include "common.h"

# define NB_CMD         34
# define CRLF           "\r\n\0"
# define NOOP_CMD       "200 NOOP ok"CRLF
# define HELP_CMD       "214 Help Me"CRLF
# define WELCOME        "220 Welcome to my house"CRLF
# define BYE_BYE        "221 Bye Bye from my house"CRLF
# define LIST_PENDING   "150 Here comes the directory listing"CRLF
# define LIST_OK        "226 Directory send OK"CRLF
# define DATA_PENDING   "150 Opening BINARY mode data connection for "
# define DATA_OK        "226 Transfer complete"CRLF
# define CWD_OK         "250 Directory successfully changed"CRLF
# define USERNAME_OK    "331 User name okay, need password for "
# define PASV_PORT      "425 Use PORT or PASV first"CRLF
# define INVALID_USER   "530 Invalid username"CRLF
# define INVALID_PASS   "530 Invalid password"CRLF
# define CANT_USER      "530 530 Can't change"CRLF
# define LOGIN_ALREADY  "230 Already Logged in with "
# define LOGIN_SUCCESS  "230 Login successful with "
# define LOGIN_FAILED   "530 Login failed"CRLF
# define LOGIN_FIRST    "503 Login with USER first"CRLF
# define CURRENT_DIR    "257 Current Working Directory is "
# define UNKNOW_CMD     "500 Unknown command"CRLF
# define NOT_IMP_CMD    "502 Command not implemented"CRLF
# define ERROR_CMD      "503 Bad sequence of commands"CRLF
# define FAILED_DIR     "550 Failed to change directory"CRLF

char    *ftp_no_cmd(const char *cmd_line, Client *client);
char    *ftp_noop(const char *cmd_line, Client *client);
char    *ftp_user(const char *cmd_line, Client *client);
char    *ftp_pass(const char *cmd_line, Client *client);
char    *ftp_quit(const char *cmd_line, Client *client);
char    *ftp_pwd(const char *cmd_line, Client *client);
char    *ftp_help(const char *cmd_line, Client *client);
char    *ftp_cdup(const char *cmd_line, Client *client);
char    *ftp_cwd(const char *cmd_line, Client *client);
char    *ftp_pasv(const char *cmd_line, Client *client);
char    *ftp_dele(const char *cmd_line, Client *client);
char    *ftp_mkd(const char *cmd_line, Client *client);
char    *ftp_rmd(const char *cmd_line, Client *client);
char    *ftp_list(const char *cmd_line, Client *client);
char    *ftp_stor(const char *cmd_line, Client *client);
char    *ftp_retr(const char *cmd_line, Client *client);

#endif //PSU_2015_MYFTP_CMDLIST_H
