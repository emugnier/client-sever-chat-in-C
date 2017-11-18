/////////////////////////////////////////////////////////////////////////////////////////
//BORDEAUX INP ENSEIRB-MATMECA
//DEPARTEMENT TELECOM
//RE216 PROGRAMMATION RESEAUX
//{daniel.negru,joachim.bruneau_-_queyreix,nicolas.herbaut}@ipb.fr
////////////////////////////////////////////////////////////////////////////////////////

#ifndef CALLBACKS_H_SQFGSPDFGNSPDOFGSPDOFG
#define CALLBACKS_H_SQFGSPDFGNSPDOFGSPDOFG
#include "user.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../commons/network.h"
#include "../commons/contrib.h"
#include "salon.h"
#include "exchange.h"

//you may want to organise all your functions handling keyworks like this

int handle_msg(struct client tab_client[], const int i,fd_set fdsock,int* nbclient,struct listchan* listchannel,struct exchange tab_exchange[]);


int handle_who( char test[256], struct client tab_client[]);

int handle_whois(char test[256], struct client tab_client[]);

int handle_nick( char test[256], struct client tab_client[],const int fd);

int handle_port( char test[256], struct client tab_client[],const int fd);

int handle_join( char test[256], struct client tab_client[],const int fd,struct listchan* listchannel);

int handle_create( char test[256], struct client tab_client[] ,const int fd,struct listchan* listchannel);

int handle_sendchan(char test[256],struct client tab_client[],const int fd,struct listchan* listchannel);

int handle_sendfile(char buffer[256], int fd,struct exchange tab_exchange[],struct client tab_client[]);

int handle_help(char test[256]);

int handle_answer_file(int fd, int exchange, char answer[256],struct client tab_client[],struct exchange tab_exchange[]);
#endif //CALLBACKS_H_SQFGSPDFGNSPDOFGSPDOFG
