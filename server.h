#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdio.h>
#include "capteur.h"

#define MAX 30
#define SERVER_PORT "8080"

typedef struct client_info
{
  size_t id;
  int socket_client;
  struct sockaddr *info;
  proc_info_t machine_info;
}client_info_t;

typedef struct server_s
{
  size_t nb_user;
  size_t user_max;
  client_info_t client[MAX];
}server_t;

void serveur();
#endif
