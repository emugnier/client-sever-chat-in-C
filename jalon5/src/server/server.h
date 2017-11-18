/////////////////////////////////////////////////////////////////////////////////////////
//BORDEAUX INP ENSEIRB-MATMECA
//DEPARTEMENT TELECOM
//RE216 PROGRAMMATION RESEAUX
//{daniel.negru,joachim.bruneau_-_queyreix,nicolas.herbaut}@ipb.fr
////////////////////////////////////////////////////////////////////////////////////////

#ifndef SERVER_H_QSGPNSGPOQNSPGOSNDFG
#define SERVER_H_QSGPNSGPOQNSPGOSNDFG
#include "user.h"


//Send a message to all users, except on the fd for the sender.


void send_broadcast_by_fd(char test[256], int fd,struct client tab_client[]);

//Send a message to a user by username
void send_msg_unicast(char test[256],int fd,struct client tab_client[]);

//Send a message
void send_unicast(char *buffer, char *uname_dest,int fd,struct client tab_client[]);


#endif //SERVER_H_QSGPNSGPOQNSPGOSNDFG
