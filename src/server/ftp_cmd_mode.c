//
// Created by wery_a on 03/05/16.
//

#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <common.h>
#include "server.h"
#include "cmdlist.h"

char    *ftp_pasv(const char *cmd_line, Client *client)
{
    IN_ADDR addr;
    char    *pasv;
    int     ports[2];
    char    buff[BUFF_SIZE];

    (void)cmd_line;
    srand(time(NULL));
    ports[0] = 128 + (rand() % 128);
    ports[1] = rand() % 0xff;
    addr.s_addr = client->addr;
    pasv = replace(inet_ntoa(addr), '.', ',');
    sprintf(buff, "%s,%d,%d", pasv, ports[0], ports[1]);
    if (client->sock_pasv != -1)
        close(client->sock_pasv);
    client->sock_pasv = init_connection(256 * ports[0] + ports[1]);
    printf("PASV port = %d\n", 256 * ports[0] + ports[1]);
    client->mode = DATA_PASV;
    return (concat("227 Entering Passive Mode (", buff, ")"CRLF));
}