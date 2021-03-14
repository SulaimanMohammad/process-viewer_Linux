#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#include "client.h"
#include "capteur.h"
#include "server.h"

void my_client()
{
	struct addrinfo *addr_info =NULL;
  struct addrinfo hints;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

  int ret = getaddrinfo(NULL, SERVER_PORT, &hints, &addr_info);

  if (ret < 0)
    {
      herror("getaddrinfo");
      exit(EXIT_FAILURE);
    }

    struct addrinfo *tmp = NULL;
    int sock = -1;
    int connected = 0;

    for (tmp = addr_info; tmp != NULL; tmp = tmp->ai_next)
      {
        sock = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);

        if (sock < 0)
          {
            perror("socket");
            continue;
          }

        int ret = connect(sock, tmp->ai_addr, tmp->ai_addrlen);

        if (ret < 0)
          {
            perror("connect");
            continue;
          }

        connected = 1;
        break;
      }

    shutdown(sock, SHUT_RDWR);

    close(sock);
}
