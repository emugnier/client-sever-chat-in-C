#include "exchange.h"

int init_exchange(struct exchange* exchange) {
	strcpy(exchange->sender,"");
	strcpy(exchange->receiver,"");
	strcpy(exchange->pathname,"");
	exchange->accepted=0;
	exchange->finish=0;
	return 0;
}

void disp_exchange(struct exchange exchange){
	printf("Sender : %s\n",exchange.sender );
	printf("Pathname : %s\n",exchange.pathname );
	printf("Receiver : %s\n",exchange.receiver );
	printf("Finish : %d\n",exchange.finish);
	printf("Accepted : %d\n",exchange.accepted );
}

int define_exchange(struct exchange tab_exchange[],char sender[256],char* receiver,char *pathname){
	int i =0;
	while (strcmp(tab_exchange[i].sender,"")!=0 && i<10) {
		i++;
	}
	strcpy(tab_exchange[i].sender,sender);
	strcpy(tab_exchange[i].receiver,receiver);
	strcpy(tab_exchange[i].pathname,pathname);
	disp_exchange(tab_exchange[i]);
}

int find_exchange_sender(struct client tab_client[],int fd,struct exchange tab_exchange[]){
	int i=0;
	while(tab_client[i].socket!=fd){
		i++;
	}
	int j=0;
	for(j=0;j<10;j++){
		if(strcmp(tab_exchange[j].receiver,tab_client[i].name)==0 && tab_exchange[j].accepted==0){
			return j;
		}
	}
	return -1;
}
