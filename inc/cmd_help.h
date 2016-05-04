/*
** cmd_help.h for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed May 04 14:30:45 2016 Adrien WERY
** Last update	Wed May 04 14:30:47 2016 Adrien WERY
*/

#ifndef PSU_2015_MYFTP_CMD_HELP_H
#define PSU_2015_MYFTP_CMD_HELP_H

static const char *cmdlist_help[] =
        {
                "ABOR <CRLF>",
                "ACCT <SP> <account-information> <CRLF>",
                "ALLO <SP> <decimal-integer>",
                "APPE <SP> <pathname> <CRLF>",
                "CDUP <CRLF>",
                "CWD  <SP> <pathname> <CRLF>",
                "DELE <SP> <pathname> <CRLF>",
                "HELP [<SP> <string>] <CRLF>",
                "LIST [<SP> <pathname>] <CRLF>",
                "MKD  <SP> <pathname> <CRLF>",
                "MODE <SP> <mode-code> <CRLF>",
                "NLST [<SP> <pathname>] <CRLF>",
                "NOOP <CRLF>",
                "PASS <SP> <password> <CRLF>",
                "PASV <CRLF>",
                "PORT <SP> <host-port> <CRLF>",
                "PWD  <CRLF>",
                "QUIT <CRLF>",
                "REIN <CRLF>",
                "REST <SP> <marker> <CRLF>",
                "RETR <SP> <pathname> <CRLF>",
                "RMD  <SP> <pathname> <CRLF>",
                "RNFR <SP> <pathname> <CRLF>",
                "RNTO <SP> <pathname> <CRLF>",
                "SITE <SP> <string> <CRLF>",
                "SIZE <CRLF>",
                "SMNT <SP> <pathname> <CRLF>",
                "STAT [<SP> <pathname>] <CRLF>",
                "STOR <SP> <pathname> <CRLF>",
                "STOU <CRLF>",
                "STRU <SP> <structure-code> <CRLF>",
                "SYST <CRLF>",
                "TYPE <SP> <type-code> <CRLF>",
                "USER <SP> <username> <CRLF>"
        };

#endif //PSU_2015_MYFTP_CMD_HELP_H
