/*
** ftp_cmd_data.c for PSU_2015_myftp
**
** Made by	Adrien WERY
** Login	wery_a
**
** Started on	Wed May 04 14:30:55 2016 Adrien WERY
** Last update	Wed May 04 14:30:57 2016 Adrien WERY
*/

#include <stdlib.h>
#include "socket.h"
#include "array.h"
#include "cmdlist.h"

char    *data_cmd(const char *cmd_line, Client *client, bool send)
{
    char        **array;
    char        *ret;

    if (client->sock_data == -1 || client->mode != DATA_PASV)
        return (strdup(PASV_PORT));
    if (!(array = split(cmd_line, " ")) || !array[1])
    {
        if (array)
            free_array(array);
        return (strdup(ERROR_CMD));
    }
    array[1] = replace(array[1], '\n', '\0');
    ret = concat(DATA_PENDING, array[1], CRLF);
    write_socket(client->sock, ret);
    free(ret);
    if (send)
        ret = run_pasv(client->sock_data, &recv_file, array[1]);
    else
        ret = run_pasv(client->sock_data, &send_file, array[1]);
    free_array(array);
    client->use_mode = DATA_PASV;
    client->mode = NO_DATA;
    if (ret)
        return (ret);
    return (strdup(DATA_OK));
}

char    *ftp_stor(const char *cmd_line, Client *client)
{
    return (data_cmd(cmd_line, client, true));
}

char    *ftp_retr(const char *cmd_line, Client *client)
{
    return (data_cmd(cmd_line, client, false));
}
