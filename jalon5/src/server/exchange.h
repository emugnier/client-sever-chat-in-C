#include <resolv.h>
#include <string.h>
#include <sys/select.h>
#include <stdlib.h>
#include "user.h"

//struct exchange to store the exchange in the server
struct exchange{
	char sender[256];
	char receiver[256];
	int accepted;
	char pathname[256];
	int finish;
};

//initiation on the struct
int init_exchange(struct exchange* exchange);

//used to display informations about the channel
void disp_exchange(struct exchange exchange);

// define an exchange with sender receiver and pathname of the file
int define_exchange(struct exchange tab_exchange[],char sender[256],char* receiver,char *pathname);

//permit to find the sender for an exchange
int find_exchange_sender(struct client tab_client[],int fd,struct exchange tab_exchange[]);
