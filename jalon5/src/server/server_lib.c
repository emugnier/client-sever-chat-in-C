/////////////////////////////////////////////////////////////////////////////////////////
//BORDEAUX INP ENSEIRB-MATMECA
//DEPARTEMENT TELECOM
//RE216 PROGRAMMATION RESEAUX
//{daniel.negru,joachim.bruneau_-_queyreix,nicolas.herbaut}@ipb.fr
////////////////////////////////////////////////////////////////////////////////////////

#include <asm-generic/socket.h>
#include <regex.h>
#include <resolv.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdbool.h>
#include "user.h"
#include "../commons/network.h"
#include "server.h"
#include "callbacks.h"
#include <stdio.h>


void send_broadcast_by_fd(char test[256], int fd,struct client tab_client[]) {
	int p = 0;
	char * tmpmsg;
	tmpmsg = cpy_partchain(test,8);
	int i=0;
	while (tab_client[i].socket!=fd) {
		i++;
	}
	char msg[256];
	strcpy(msg,tab_client[i].name);
	strcat(msg,": ");
	strcat(msg,tmpmsg);
	for (p = 0; p < 20; p++) {
		if (strcmp(tab_client[p].name,"empty")!=0 && tab_client[p].socket != fd) {
			do_write(tab_client[p].socket,msg);
		}
	}
}

void send_msg_unicast(char test[256],int fd,struct client tab_client[]){
	char tmpnm[256];
	char * tmpmsg;
	char compte[256];
	sscanf(test,"%*s %s",tmpnm);
	strcpy(compte,"/msg ");
	strcat(compte,tmpnm);
	if(strlen(test)-1!=5+strlen(tmpnm)){
		tmpmsg = cpy_partchain(test,5+strlen(tmpnm)+1);
		memset(test,0,sizeof(test));
		send_unicast(tmpmsg,tmpnm, fd,tab_client);
	}
}

void send_unicast(char *buffer, char *uname_dest, int fd,struct client tab_client[]) {
	int p = 0;
	int i=0;
	while (tab_client[i].socket!=fd) {
		i++;
	}
	char msg[256];
	strcpy(msg,tab_client[i].name);
	strcat(msg,": ");
	strcat(msg,buffer);
	for (p = 0; p < 20; p++) {
		if (strcmp(tab_client[p].name,uname_dest)==0) {
			do_write(tab_client[p].socket,msg);
		}
	}
}
