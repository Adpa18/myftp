/*
** cmd.h for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Tue Apr 19 22:32:42 2016 Adrien WERY
** Last update	Sat Apr 30 18:17:24 2016 Adrien WERY
*/

#ifndef CMD_H
# define CMD_H

# include "cmdlist.h"

typedef enum COMMAND
{
    ABOR, CWD, DELE, LIST, MDTM, MKD, NLST, PASS, PASV,
    PORT, PWD, QUIT, RETR, RMD, RNFR, RNTO, SITE, SIZE,
    STOR, TYPE, USER, NOOP
} COMMAND;

static const char *cmdlist_str[] =
        {
                "ABOR", "CWD", "DELE", "LIST", "MDTM", "MKD", "NLST", "PASS", "PASV",
                "PORT", "PWD", "QUIT", "RETR", "RMD", "RNFR", "RNTO", "SITE", "SIZE",
                "STOR", "TYPE", "USER", "NOOP"
        };

typedef char *(*cmdlist_ptr)(const char *, Client *);

static cmdlist_ptr cmdlist_func[] =
        {
                &ftp_noop, &ftp_noop, &ftp_noop, &ftp_noop, &ftp_noop, &ftp_noop,
                &ftp_noop, &ftp_pass, &ftp_noop, &ftp_noop, &ftp_noop, &ftp_noop,
                &ftp_noop, &ftp_noop, &ftp_noop, &ftp_noop, &ftp_noop, &ftp_noop,
                &ftp_noop, &ftp_noop, &ftp_user, &ftp_noop
        };

#endif /* !CMD_H */
