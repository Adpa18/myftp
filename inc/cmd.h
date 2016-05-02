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

# include "common.h"

# define UNKNOWN_CMD    "Unknow Command\n"

char    *pwd();
bool    cd(char *path);
char    *ls(char *path);

#endif /* !CMD_H */
