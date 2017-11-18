#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "user.h"

struct chan {
  char* name;
  char members[20][256];
  int nbmembers;
  struct chan * next_chan;
};

//chained list to have as many channel as we want
struct listchan{
  struct chan *premierchan;
};

//used to initiate the chained list
struct listchan* init_listchan();

//to init a channel
void init_chan(struct listchan*listchannel);

// permit to find a channel in the chained list
struct chan* find_chan(char* name, struct listchan* listchannel);

// permit to create a chan
int create_chan_at_begin(struct listchan*listchannel,char *name, struct client *user);

// erase a chan
void erase_chan(struct listchan* listchannel, struct chan* channeltofind);

//display informations about a chan
void display_chan(char * name,struct listchan* listchannel);

//add a user to a chan
int add_user_chan(char * name,struct listchan* listchannel,struct client* user);

//remove a user from a chan
void remove_user_chan(struct client* user,char*name,struct listchan* listchannel);
