/////////////////////////////////////////////////////////////////////////////////////////
//BORDEAUX INP ENSEIRB-MATMECA
//DEPARTEMENT TELECOM
//RE216 PROGRAMMATION RESEAUX
//{daniel.negru,joachim.bruneau_-_queyreix,nicolas.herbaut}@ipb.fr
////////////////////////////////////////////////////////////////////////////////////////

#ifndef USER_H_
#define USER_H_

#include <stddef.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//that's the struture you should use to store your users
struct client{
	int socket;
	char name[256];
	char timeuh[256];
	char adress[INET6_ADDRSTRLEN];
	int port;
	char channel_name[256];
	int portp2p;
};

typedef struct client* puser;


//function to add an user in tab stored in the server
void add_user(const int fd,struct client tab_client[],int * nbclient,struct sockaddr_in *server_adr,int* max_fd);

//function to erase an user stored in the tab
void remove_user_by_fd(const int fd,int * nbclient,struct client tab_client[], fd_set fdsock);


#endif /* USER_H_ */
