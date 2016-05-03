/*
** cmd.h for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Tue Apr 19 22:32:42 2016 Adrien WERY
** Last update	Tue May 03 11:44:50 2016 Adrien WERY
*/

#ifndef CMD_H
# define CMD_H

# include "cmdlist.h"

typedef enum COMMAND
{
    ABOR, ACCT, ALLO, APPE, CDUP, CWD, DELE, EPRT, EPSV, FEAT, HELP,
    LIST, MDTM, MKD, MODE, NLST, NOOP, OPTS, PASS, PASV, PORT, PWD,
    QUIT, REIN, REST, RETR, RMD, RNFR, RNTO, SITE, SIZE, SMNT, STAT,
    STOR, STOU, STRU, SYST, TYPE, USER, XCUP, XCWD, XMKD, XPWD, XRMD,
    NO_CMD
} COMMAND;

static const char *cmdlist_str[] =
{
    "ABOR", "ACCT", "ALLO", "APPE", "CDUP", "CWD", "DELE", "EPRT", "EPSV", "FEAT",
    "HELP", "LIST", "MDTM", "MKD", "MODE", "NLST", "NOOP", "OPTS",  "PASS", "PASV",
    "PORT", "PWD", "QUIT", "REIN", "REST", "RETR", "RMD",  "RNFR", "RNTO", "SITE",
    "SIZE", "SMNT", "STAT", "STOR", "STOU", "STRU",  "SYST", "TYPE", "USER", "XCUP",
    "XCWD", "XMKD", "XPWD", "XRMD"
};

typedef char *(*cmdlist_ptr)(const char *, Client *);

static cmdlist_ptr cmdlist_func[] =
{
    &ftp_no_cmd, &ftp_no_cmd, &ftp_no_cmd, &ftp_no_cmd, &ftp_cdup,
    &ftp_cwd, &ftp_no_cmd, &ftp_no_cmd, &ftp_no_cmd, &ftp_no_cmd,
    &ftp_help, &ftp_no_cmd, &ftp_no_cmd, &ftp_no_cmd, &ftp_no_cmd,
    &ftp_no_cmd, &ftp_noop, &ftp_no_cmd, &ftp_pass, &ftp_pasv,
    &ftp_no_cmd, &ftp_pwd, &ftp_quit, &ftp_no_cmd, &ftp_no_cmd,
    &ftp_no_cmd, &ftp_no_cmd, &ftp_no_cmd, &ftp_no_cmd, &ftp_no_cmd,
    &ftp_no_cmd, &ftp_no_cmd, &ftp_no_cmd, &ftp_no_cmd, &ftp_no_cmd,
    &ftp_no_cmd, &ftp_no_cmd, &ftp_no_cmd, &ftp_user, &ftp_no_cmd,
    &ftp_no_cmd, &ftp_no_cmd, &ftp_no_cmd, &ftp_no_cmd
};

#endif /* !CMD_H */
