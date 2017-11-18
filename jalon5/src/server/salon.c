#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "salon.h"

struct listchan* init_listchan(){
  struct listchan* listchannel;
  listchannel=malloc(sizeof(struct listchan));
  listchannel->premierchan=NULL;
  return listchannel;
}

void init_chan(struct listchan*listchannel){
  struct chan* channel;
  channel=malloc(sizeof(struct chan));
  channel->name=NULL;
  channel->nbmembers=0;
  listchannel->premierchan=channel;
  channel->next_chan=NULL;
}

struct chan* find_chan(char* name, struct listchan* listchannel){
  assert(listchannel->premierchan);
  struct chan *channel=listchannel->premierchan;
  while(channel!=NULL && channel->name!=NULL){
    if (strcmp(channel->name,name)==0){
      return channel;
    }
    channel=channel->next_chan;
  }
  return NULL;
}

char* disp_chan( struct listchan* listchannel){
  assert(listchannel->premierchan);
  char *name_salon;
  name_salon=malloc(sizeof(char)*256);
  strcpy(name_salon, "les salons:\n");
  struct chan *channel=listchannel->premierchan;
  while(channel!=NULL && channel->name!=NULL){
    strcat(name_salon, channel->name);
    channel=channel->next_chan;
  }
  return name_salon;
}

int create_chan_at_begin(struct listchan*listchannel,char *name, struct client* user){
  if(find_chan(name, listchannel)==NULL){
    struct chan* channel;
    channel=malloc(sizeof(struct chan));
    channel->name=name;
    strcpy(channel->members[0],user->name);

    strcpy(user->channel_name,name);

    int i=1;
    for(i=1;i<20;i++){
      strcpy(channel->members[i],"noone");
    }
    channel->nbmembers=1;
    struct chan* channeltmp=listchannel->premierchan;
    listchannel->premierchan=channel;
    channel->next_chan=channeltmp;
    return 0;
  }
  else{
    return 10;
  }
}

void erase_chan(struct listchan* listchannel, struct chan* channeltofind){
  assert(listchannel->premierchan);
  struct chan *channel=listchannel->premierchan;
  if  (channeltofind==channel){
    struct chan* channeltmp=channel->next_chan;
    free(channel);
    listchannel->premierchan=channeltmp;
  }
  else{
    channel=channel->next_chan;
    while(channel!=NULL){
      struct chan* channeltmp1=channel;

      if (channeltofind==channel){
        struct chan* channeltmp2=channel->next_chan;
        free(channel);
        channeltmp1->next_chan=channeltmp2;
      }
      channel=channel->next_chan;
    }
  }
}

void display_chan(char * name,struct listchan* listchannel){
  struct chan* channel=find_chan(name, listchannel);
  assert(channel);
  printf("Nom du channel : %s\n",channel->name );
  printf("Nombre de membres : %d\n",channel->nbmembers );
}

int add_user_chan(char * name,struct listchan* listchannel,struct client* user){
  struct chan* channel=find_chan(name, listchannel);
  if (channel==NULL){
    return 1;
  }
  int i=0;
  int newmb=0;
  while (i<20 && newmb==0) {
    if(strcmp(channel->members[i],"noone")==0){
      strcpy(channel->members[i],user->name);
      strcpy(user->channel_name,name);
      channel->nbmembers+=1;
      newmb=1;

    }
    i++;
  }
  return 0;
}

void remove_user_chan(struct client* user,char*name,struct listchan* listchannel){
  struct chan* channel=find_chan(name, listchannel);
  assert(channel);
  int i=0;
  int exmb=0;
  while (i<20 && exmb==0) {
    if(strcmp(channel->members[i],user->name)==0){
      strcpy(channel->members[i],"noone");
      strcpy(user->channel_name,"");
      channel->nbmembers-=1;
      if(channel->nbmembers==0){
        erase_chan(listchannel,channel);
      }
      exmb=1;
    }
    i++;
  }
}
