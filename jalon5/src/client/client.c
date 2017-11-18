/////////////////////////////////////////////////////////////////////////////////////////
//BORDEAUX INP ENSEIRB-MATMECA
//DEPARTEMENT TELECOM
//RE216 PROGRAMMATION RESEAUX
//{daniel.negru,joachim.bruneau_-_queyreix,nicolas.herbaut}@ipb.fr
////////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>
#include <resolv.h>
#include <string.h>
#include <sys/select.h>
#include "server.h"
#include "client.h"
#include "../commons/network.h"
#include "../commons/contrib.h"

int main(int argc, char** argv) {
  if (argc != 4) {
    fprintf(stderr, "usage: RE216_CLIENT hostname port portclient\n");
    return 1;
  }

  int sock;
  int sock_client;
  struct sockaddr_in sock_server;
  memset(&sock_server,0,sizeof(sock_server));
  struct sockaddr_in client_addr;
  memset(&sock_server,0,sizeof(client_addr));
  //get the add info
  get_addr_info1(argv[2], argv[1], &sock_server);
  init_serv_addr(argv[3],&client_addr);
  //get the socket
  sock_client=do_socket(AF_INET,SOCK_STREAM, 0);
  sock = do_socket(AF_INET,SOCK_STREAM, 0);

  //fill the socket
  do_bind(sock_client,client_addr);

  if(listen(sock_client , 1)==-1){
		error("listen");
	}
  //connect to remote socket
  do_connect(sock, sock_server);
  //in an infinite loop, use select to listen to open sockets
  fd_set fd_client;

  int peer2peer=-1;
  char receiver[256]="";
  char *file_path=malloc(sizeof(char)*256);
  int max_fd = sock + 1;
  for (;;) {
    //clean the set before adding file descriptors
    FD_ZERO(&fd_client);
    //add the fd for server connection
    FD_SET(sock, &fd_client);
    //add the fd for user-input
    FD_SET(fileno(stdin), &fd_client);

    FD_SET(sock_client,&fd_client);

    if (peer2peer>-1){
      FD_SET(peer2peer,&fd_client);
    }

    int select_ret_val = select(max_fd, &fd_client, NULL, NULL, NULL);
    if (select_ret_val== -1) {
        error("select client");
    }

    int i = 0;

    for (i = 0; i < max_fd && select_ret_val > 0; i++) {

      if (FD_ISSET(i, &fd_client)) {
        if (i == fileno(stdin)) {
          handle_client_message(sock,receiver,file_path);
        }

        else if (i==sock) {
          int end=handle_server_message(sock,receiver,file_path);
          if (end==10){
            return 0;
          }
        }
        else if (i==sock_client){
          peer2peer=do_accept(sock_client,&client_addr);
          max_fd+=1;
        }
        else{
          handle_receive_file(i);
          do_close(peer2peer);
          peer2peer=-1;
        }
        select_ret_val--;
      }
    }
  }
  return 0;
}
