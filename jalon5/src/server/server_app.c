/////////////////////////////////////////////////////////////////////////////////////////
//BORDEAUX INP ENSEIRB-MATMECA
//DEPARTEMENT TELECOM
//RE216 PROGRAMMATION RESEAUX
//{daniel.negru,joachim.bruneau_-_queyreix,nicolas.herbaut}@ipb.fr
////////////////////////////////////////////////////////////////////////////////////////

#include <resolv.h>
#include <string.h>
#include <sys/select.h>
#include <stdlib.h>
#include "../commons/network.h"
#include "server.h"
#include "salon.h"
#include "exchange.h"


void main(int argc, char** argv) {

	if (argc != 2) {
		fprintf(stderr, "plese specify local port to bind to\n");
		exit(-1);
	}

	int sock;

	fd_set fdsock;  //init the fd_set

	struct sockaddr_in server_adr;

	memset(&server_adr,0,sizeof(server_adr));

	struct listchan* listchannel=init_listchan();
  init_chan(listchannel);

	struct exchange tab_exchange[10];
	int i=0;
	for(i=0; i<10;i++){
		init_exchange(&tab_exchange[i]);
	}

	//init a fresh socket
	//do_socket()
	sock=do_socket(AF_INET,SOCK_STREAM, 0);

	//init server address structure
	//init_serv_addr()
	init_serv_addr(argv[1],&server_adr);

	//perform the binding
	//do_bind()
	do_bind(sock, server_adr);

	//specify the socket to be a server socket and listen for at most 20 concurrent client
	if(listen(sock , 20)==-1){
		error("listen");
	}


	struct client	tab_client[20];     //A tab to stock all the client's sockets
	int k=0;
	for (k=0;k<20;k++){     //fill the tab with negatives values to avoid any confusion
		tab_client[k].socket=-1;
		strcpy(tab_client[k].name,"empty");
	}

	int nbclient=0;


	int max_fd=sock+1;


	//in an infinite loop, listen to the stored sockets
	for(;;){
		printf("nombre de clients:%d\n",nbclient );

		FD_ZERO(&fdsock);

		FD_SET(sock,&fdsock);

		int j=0;
		for(j=0;j<20;j++){
			if(tab_client[j].socket != -1){
				FD_SET(tab_client[j].socket,&fdsock);      //Fill the fdset with the client's sockets
			}
		}

		/*use select to listen to sockets*/
		int select_val=select(max_fd,&fdsock,NULL,NULL,NULL);
		if (select_val==-1) {
			error("select");
		}

		int i=0;
		for (i=0; i<max_fd;i++){
			if (FD_ISSET(i,&fdsock)) {
				//implement the listening for incoming sockets
				if(i==sock){
					int sock_client=do_accept(sock, &server_adr);
					if (nbclient==20){
						do_write(sock_client,"Server cannot accept incoming connections anymore. Try again later.");
						do_close(sock_client);
					}
					else{
					add_user(sock_client,tab_client,&nbclient,&server_adr,&max_fd);
					}
				}
				else {
					handle_msg(tab_client,i,fdsock,&nbclient,listchannel,tab_exchange);
					printf("message reçu\n" );
				}
			}
		}
	}
//clean up server socket
do_close(sock);
}
