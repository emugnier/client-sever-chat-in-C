#include "callbacks.h"

int handle_who( char test[256], struct client tab_client[]) {
  memset(test,0,sizeof(test));
  int ji = 0;
  for(ji = 0;ji < 20; ji++) {
    if(strcmp(tab_client[ji].name,"empty")!=0){
      strcat(test,tab_client[ji].name);
      strcat(test,"\n");
    }
  }
  return 0;
}

int handle_msg(struct client tab_client[], const int i,fd_set fdsock,int *nbclient,struct listchan* listchannel,struct exchange tab_exchange[]) {
  int pquit=0;
  char test[256]="";
  int exchange=-1;
  do_read(i,test);
  if (strcmp(tab_client[i-4].name,"empty")==0) {
    if (strcmp("/quit\n",test)==0){
      remove_user_by_fd(i,nbclient,tab_client,fdsock);
      pquit=10;
    }
    if (strncmp("/nick ",test,6)==0) {
      handle_nick( test, tab_client,i);
    }
    else {
      memset(test,0,sizeof(test));
      strcpy(test,"Veuillez rentrer un pseudo à l'aide de /nick \n");
    }
  }
  else if (tab_client[i-4].portp2p==0) {
    if (strcmp("/quit\n",test)==0){
      remove_user_by_fd(i,nbclient,tab_client,fdsock);
      pquit=10;
    }
    if (strncmp("/p2p ",test,5)==0) {
      handle_port( test, tab_client,i);
    }
    else {
      memset(test,0,sizeof(test));
      strcpy(test,"Veuillez rentrer un port pour le p2p à l'aide de /p2p \n");
    }
  }
  exchange=find_exchange_sender(tab_client,i,tab_exchange);
  if(exchange>-1){
    handle_answer_file(i,exchange,test,tab_client,tab_exchange);
  }
  if (strcmp("/who\n",test)==0){
    handle_who(test, tab_client);
  }
  if (strncmp("/nick ",test,6)==0) {
    handle_nick( test, tab_client,i);
  }
  if (strncmp("/create ",test,8)==0){
    handle_create(test,tab_client ,i,listchannel);
    pquit=10;
  }
  if (strncmp("/join ",test,6)==0){
    handle_join(test,tab_client ,i,listchannel);
    pquit=10;
  }
  if(strncmp("/listsalon",test,10)==0){
    char* msg=disp_chan(listchannel);
    do_write(i,msg);
  }
  if (strncmp("/msg ",test,5)==0) {
    send_msg_unicast(test,i,tab_client);
    pquit = 18;
  }
  if (strncmp("/msgall ",test,8)==0) {
    pquit = 19;
    send_broadcast_by_fd(test, i,tab_client);
  }
  if (strncmp("/whois ",test,7)==0){
    handle_whois(test,tab_client);
  }
  int k=0;
  if (strncmp("/quit ",test,6)==0){
    char channel[256];
    sscanf(test,"%*s %s",channel);
    for (k=0;k<20;k++){
      if(tab_client[k].socket == i){
        remove_user_chan(&tab_client[k],channel,listchannel);
        pquit=19;
      }
    }
  }
  for (k=0;k<20;k++){
    if(tab_client[k].socket == i && pquit!=10){
      if (strcmp(tab_client[k].channel_name,"")!=0){
        handle_sendchan(test,tab_client,i,listchannel);
        pquit=20;
      }
    }
  }
  if(strncmp("/help",test,5)==0){
    handle_help(test);
  }

  if(strncmp("/send ",test,6)==0){
    handle_sendfile(test,i,tab_exchange,tab_client);
    pquit =10;
  }
  if (strcmp("/quit\n",test)==0){
    remove_user_by_fd(i,nbclient,tab_client,fdsock);
    pquit=10;
  }
  else if(pquit==0){
    do_write(i,test);
  }
}

int handle_whois( char test[256], struct client tab_client[]) {
  char tmpname[256];
  sscanf(test,"%*s %s",tmpname);
  memset(test,0,sizeof(test));
  int q = 0;
  for (q = 0;q<20;q++) {
    if(strcmp(tmpname,tab_client[q].name)==0){
      strcat(test,"L'adresse de cette personne est : ");
      strcat(test,tab_client[q].adress);
      strcat(test," et son port est : ");
      char port[256]="";
      sprintf(port,"%d",tab_client[q].port);
      strcat(test,port);
      strcat(test," \n");
    }
  }
  return 0;
}

int handle_help(char test[256]) {
    memset(test,0,sizeof(test));
    strcat(test,"Liste de commande disponible : \n");
    strcat(test,"/nick {name} \n");
    strcat(test,"/who \n");
    strcat(test,"/whois {name} \n");
    strcat(test,"/msg {name} {message} \n");
    strcat(test,"/send {name} {fichier} \n");
    strcat(test,"/msgall {message} \n");
    strcat(test,"/create {salon_name} \n");
    strcat(test,"/quit  \n");
    return 0;
}

int handle_nick( char test[256], struct client tab_client[],const int fd) {
  char name[256];
  if (strncmp("/nick ",test,6)==0) {
    sscanf(test,"%*s %s",name);
    memset(test,0,sizeof(test));
    int q = 0;
    for (q = 0;q<20;q++) {
      if(strcmp(name,tab_client[q].name)==0){
        memset(test,0,sizeof(test));
        strcpy(test,"Pseudo déjà pris, Recommencez /nick \n");
        break;
      }
      else{
        strcpy(tab_client[fd-4].name,name);
        strcpy(test,tab_client[fd-4].name);
        strcat(test," sera votre nouveau pseudonyme \n");
        break;
      }
    }

  }
  return 0;
}

int handle_port( char test[256], struct client tab_client[],const int fd) {
  char portt[256];
  if (strncmp("/p2p ",test,5)==0) {
    sscanf(test,"%*s %s",portt);
    int port=atoi(portt);
    memset(test,0,sizeof(test));
    int q = 0;
    for (q = 0;q<20;q++) {
      if(port==tab_client[q].portp2p){
        memset(test,0,sizeof(test));
        strcpy(test,"port déjà pris, Recommencez /p2p \n");
        break;
      }
      else{
        tab_client[fd-4].portp2p=port;
        strcpy(test,portt);
        strcat(test," sera votre port \n");
        break;
      }
    }
  }
  return 0;
}

int handle_join( char test[256], struct client tab_client[] ,const int fd,struct listchan* listchannel){
  char *name;
  name = cpy_partchain(test,6);
  int i=0;
  while(tab_client[i].socket!=fd){
    i++;
  }
  int ok;
  ok=add_user_chan(name,listchannel,&tab_client[i]);
  if (ok==0){
    display_chan(name,listchannel);
    char msg[256];
    strcpy(msg,"Vous avez rejoint le salon ");
    strcat(msg,name);
    do_write(fd,msg);
  }
  else{
    char msg[256];
    strcpy(msg,"ce salon n'existe pas:  ");
    strcat(msg,name);
    do_write(fd,msg);
  }
  return 0;
}

int handle_create( char test[256], struct client tab_client[] ,const int fd,struct listchan* listchannel){
  char *name;
  name = cpy_partchain(test,8);
  int i=0;
  while(tab_client[i].socket!=fd){
    i++;
  }
  if(find_chan(name,listchannel)==NULL){
    create_chan_at_begin(listchannel,name,&tab_client[i]);
    display_chan(name,listchannel);
    char msg[256];
    strcpy(msg,"Vous avez créé le salon:  ");
    strcat(msg,name);
    do_write(fd,msg);
    return 0;
  }
  else{
    char msg[256];
    strcpy(msg,"Ce salon existe déjà:  ");
    strcat(msg,name);
    do_write(fd,msg);
  }
}

int handle_sendchan(char test[256],struct client tab_client[],const int fd,struct listchan* listchannel){
  int i=0;
  while(tab_client[i].socket!=fd){
    i++;
  }
  char *name=tab_client[i].channel_name;
  struct chan* channel=find_chan(name, listchannel);
  char msg[256];

  strcpy(msg,name);
  strcat(msg,": ");
  strcat(msg,test);

  for(i=0;i<20;i++){
    send_unicast(msg, channel->members[i],fd,tab_client);
  }
}

int handle_sendfile(char buffer[256], int fd,struct exchange tab_exchange[],struct client tab_client[]){
  char* receiver=malloc(sizeof(char)*256);
  char* pathname=malloc(sizeof(char)*256);
  sscanf(buffer,"%*s %s %*s",receiver);
  sscanf(buffer,"%*s %*s %s",pathname);
  int exist=0;
  int i=0;
  int fd_sender=0;
  for(i=0;i<20;i++){
    if(strcmp(receiver,tab_client[i].name)==0){
      fd_sender=tab_client[i].socket;
      exist=1;
    }
  }
  if(exist==0){
    return -10;
  }
  else{
    int i=0;
    while(tab_client[i].socket!=fd){
      i++;
    }
    define_exchange(tab_exchange,tab_client[i].name,receiver,pathname);
    char msg[256];
    strcpy(msg,tab_client[i].name);
    strcat(msg, " wants you to accept the transfer of the file " );
    strcat(msg,pathname);
    strcat(msg,"[Y/N]");
    do_write(fd_sender,msg);
    return 0;
  }
}



int handle_answer_file(int fd, int exchange, char answer[256],struct client tab_client[],struct exchange tab_exchange[]){
  char ans[256];
  int i=0;
  int k=0;
  while(tab_client[i].socket!=fd){
    i++;
  }
  int j=0;
  while(strcmp(tab_client[j].name,tab_exchange[exchange].sender)!=0){
    j++;
  }
  k=0;
  if(strcmp(answer,"Y\n")==0){
    char info[256];
    strcpy(info,"port ");
    char buffer[10];
    sprintf(buffer,"%d",tab_client[i].portp2p);
    strcat(info,buffer);
    strcat(info," adresse ");
    strcat(info,tab_client[i].adress);
    strcpy(ans,tab_client[i].name);
    strcat(ans," accept the file");
    do_write(tab_client[j].socket,ans);
    do_write(tab_client[j].socket,info);
    tab_exchange[exchange].accepted=1;
  }
  else if(strcmp(answer,"N\n")==0){
    strcpy(ans,tab_client[i].name);
    strcat(ans," cancel file transfer");
    do_write(tab_client[j].socket,ans);
    init_exchange(&tab_exchange[exchange]); //pas sur
  }
  else{
    strcpy(ans,"please enter [Y/N]");
    do_write(fd,ans);
  }
  return 0;
}
