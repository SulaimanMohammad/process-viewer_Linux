#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdio.h>
#include "capteur.h"

typedef struct client_info
{
  size_t id;
  int socket_client;
  struct sockaddr *info;
  machine_info_t *machine;
}client_info_t;

typedef struct server_s
{
  size_t nb_user;
  size_t user_max;
}server_t;

#endif
