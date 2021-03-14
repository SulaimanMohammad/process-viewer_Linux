#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <proc/readproc.h>
#include "server.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

void *client_loop(void *param)
{
	client_info_t *client = (client_info_t *)param;

	printf("Connection \n");

	write(client->socket_client, "Bienvenu \n", 20);
	close(client->socket_client);
	free(client);

	return NULL;
}
void serveur()
{
	struct addrinfo hints;
	struct addrinfo *addr_info =NULL;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int ret = getaddrinfo(NULL,SERVER_PORT, &hints, &addr_info);

	if(ret < 0)
	{
		herror("getaddrinfo");
	}

	struct addrinfo *tmp;
	int listen_socket=-1;
	int binded=0;

	for(tmp = addr_info; tmp != NULL; tmp->ai_next)
	{
		listen_socket = socket(tmp->ai_family, tmp->ai_socktype,tmp->ai_protocol);

		if(listen_socket < 0)
		{
			perror("socket");
			continue;
		}
		int ret = bind(listen_socket, tmp->ai_addr, tmp->ai_addrlen);

		if (ret < 0)
			{
				perror("bind");
				continue;
			}

		binded = 1;
		break;
	}

	ret = listen(listen_socket, 2);

  if (ret < 0)
    {
      perror("listen");
      exit(EXIT_FAILURE);
    }

  struct sockaddr client_info;
  socklen_t addrlen;

  while (1)
    {
      int client_socket = accept(listen_socket, &client_info, &addrlen);

      if (client_socket < 0)
        {
          perror("accept");
          exit(EXIT_FAILURE);
        }

      client_info_t *client = malloc(sizeof(client_info_t));

      if (!client)
        {
          fprintf(stderr, "Erreur d'allocationn");
          exit(EXIT_FAILURE);
        }

      client->socket_client = client_socket;

      pthread_t th;

      pthread_create(&th, NULL, client_loop, client);
      pthread_detach(th);
    }

  close(listen_socket);

}
