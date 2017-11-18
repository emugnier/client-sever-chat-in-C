/////////////////////////////////////////////////////////////////////////////////////////
//BORDEAUX INP ENSEIRB-MATMECA
//DEPARTEMENT TELECOM
//RE216 PROGRAMMATION RESEAUX
//{daniel.negru,joachim.bruneau_-_queyreix,nicolas.herbaut}@ipb.fr
////////////////////////////////////////////////////////////////////////////////////////

#include "user.h"

void add_user(const int fd,struct client tab_client[],int * nbclient,struct sockaddr_in *server_adr,int* max_fd) {
  int i;
  for (i=0;i<20;i++){
    if (tab_client[i].socket==-1){
      tab_client[i].socket=fd;
      struct sockaddr_in *s = (struct sockaddr_in *)server_adr;
      tab_client[i].port = ntohs(s->sin_port);
      inet_ntop(AF_INET, &s->sin_addr, tab_client[i].adress, sizeof(tab_client[i].adress));
      strcpy(tab_client[i].channel_name,"");
      tab_client[i].portp2p=0;
      *nbclient+=1;
      *max_fd+=1;
      break;
    }
  }
}

void remove_user_by_fd(const int fd,int * nbclient,struct client tab_client[], fd_set fdsock) {
  do_write(fd,"You will be terminated");
  FD_CLR(fd,&fdsock);
  int l=0;
  *nbclient-=1;
  for(l=0;l<20;l++){
    if(tab_client[l].socket==fd){
      tab_client[l].socket=-1;
      memset(tab_client[l].name,0,sizeof(tab_client[l].name));
      strcpy(tab_client[l].name,"empty");
    }
  }
  do_close(fd);
}
