
#include <unistd.h>
#include <errno.h>
#include "contrib.h"
#include "assert.h"

char * cpy_partchain(char * chain, int start){
  assert(chain);
  char* new_chain=NULL;
  int lengthchain=strlen(chain);
  if (start<lengthchain){
    new_chain=malloc(sizeof(char)*(lengthchain-start));
    int i= start;
    for (i=start; i<=lengthchain-2;i++){
      new_chain[i-start]=chain[i];
    }
    new_chain[i-start]='\0';
  }
  else{
    error("probleme taille chaine copie" );
  }
  return new_chain;
}
