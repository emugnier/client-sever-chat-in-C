/////////////////////////////////////////////////////////////////////////////////////////
//BORDEAUX INP ENSEIRB-MATMECA
//DEPARTEMENT TELECOM
//RE216 PROGRAMMATION RESEAUX
//{daniel.negru,joachim.bruneau_-_queyreix,nicolas.herbaut}@ipb.fr
////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <regex.h>
#include <resolv.h>
#include <sys/socket.h>

#include <stdbool.h>




#include "network.h"
#include "user.h"
#include "callbacks.h"
#include "colors.h"


void error(const char* msg) {
	perror(msg);
	exit(-1);
}

/**************************************************
 * In this file, you should implement every code that
 * can be used by both client and server
 */

int do_socket(int domain, int type, int protocol) {
	int sock;
	int yes;
	if ((sock= socket(AF_INET,SOCK_STREAM, 0))==-1){ //or IPPROTO_TCP à changer AF_INET,SOCK_STREAM, 0
		perror("creation socket");
		exit(EXIT_FAILURE);
	}
	 if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
				perror("ERROR setting socket options");
				exit(EXIT_FAILURE);
	}
	return sock;
}

void do_connect(int sock,struct sockaddr_in sock_server){
  if(connect(sock,(struct sockaddr *) &sock_server, sizeof(sock_server))==-1){
    perror("connect");
    exit(EXIT_FAILURE);
  }
}



void init_serv_addr(const char* port, struct sockaddr_in *adr_server) {
	struct in_addr addr;
	addr.s_addr=htons(INADDR_ANY);
	adr_server->sin_addr=addr;
	adr_server->sin_family= AF_INET;
	adr_server->sin_port=htons(atoi(port));

}

void do_bind(int sock, struct sockaddr_in server_adr){
  if(bind( sock,(struct sockaddr*)&server_adr,sizeof(server_adr))==-1){
    error("bind");
  }
}

int do_accept(int sock, struct sockaddr_in *adr_server){
	int size=sizeof(*adr_server);
	int acceptok=accept(sock, (struct sockaddr*)adr_server,&size);
	return acceptok;
}

int do_read(const int socket, char buffer[256]) {
	int sent =0;
	if(read(socket,buffer,256)==-1){
		error("read");
	}
}

int do_write(const int socket, const char buffer[256]) {
	if (write(socket,buffer,256)==-1) {
    error("write");
  }
}

void do_close(int sock){
  close(sock);
}
