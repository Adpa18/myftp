/*
** cmd.h for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Tue Apr 19 22:32:42 2016 Adrien WERY
** Last update	Tue Apr 19 22:32:43 2016 Adrien WERY
*/

#ifndef CMD_H
# define CMD_H

# include "common.h"

char    *pwd();
bool    cd(char *path);
char    *ls(char *path);

#endif /* !CMD_H */
