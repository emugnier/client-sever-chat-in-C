#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include "network.h"
#include "client.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void get_addr_info1(char* port, char* address, struct sockaddr_in * adr_server ) {
  struct hostent* res;
  struct in_addr* addr;
  res=gethostbyname(address);
  addr=(struct in_addr*)res->h_addr_list[0];
  adr_server->sin_addr=*addr;
  adr_server->sin_family= AF_INET;
  adr_server->sin_port=htons(atoi(port));
}

void handle_client_message(int sock,char receiver[256], char *file_path) {
  char msg[256]="";
  readline(sock,msg);
  if (strncmp("/send ",msg,6)==0 && strlen(msg)>9){
    sscanf(msg,"%*s %s %*s",receiver);
    sscanf(msg,"%*s %*s %s",file_path);
  }
}

int handle_server_message(int sock,char receiver[256],char * file_path) {
  char test[256]="";
  do_read(sock,test);
  printf("%s\n",test);
  if ( strcmp(receiver,"")!=0){
    char tmp[256];
    strcpy(tmp,receiver);
    strcat(tmp," accept the file");
    if(strcmp(test,tmp)==0){
      char info[256]="";
      do_read(sock,info);
      char address[256];
      char port[256];
      sscanf(info,"%*s %s %*s %*s",port);
      sscanf(info,"%*s %*s %*s %s",address);
      char file[256]="le fichier est envoyé et pas par le serveur";
      printf("%s\n",file );
      struct sockaddr_in receiver_addr;
      int sock_send;
      memset(&receiver_addr,0,sizeof(receiver_addr));
      get_addr_info1(port,address,&receiver_addr);
      sock_send=do_socket(AF_INET,SOCK_STREAM, 0);
      do_connect(sock_send,receiver_addr);
      handle_send_file( sock_send,file_path);
      close(sock_send);
    }
  }
  if (strcmp("Server cannot accept incoming connections anymore. Try again later.",test)==0){
    do_close(sock);
    return 10;
  }
  if (strcmp("You will be terminated",test)==0){
    do_close(sock);
    return 10;
  }
  return 0;
}

void handle_send_file( int socket, char* file_path) {
  int file;
  struct stat *stat_file=malloc(sizeof(struct stat));
  file=open(file_path,O_RDONLY);
  if(fstat(file,stat_file)==-1){
    error("fstat");
  }
  int length_file=stat_file->st_size;
  char file_path2[256];
  strcpy(file_path2,file_path);
  write(socket,file_path2,256);
  printf("Taille du fichier : %d octets\n",(int)stat_file->st_size );
  if (write(socket,&length_file,sizeof(int))==-1) {
    error("write");
  }
  int i=1;
  while (i*256<length_file){
    char* buffer=malloc(sizeof(char)*256);
    read(file,buffer,256);
    write(socket,buffer,256);
    i++;
    free(buffer);
  }
  int rest=length_file-(i-1)*256;
  char* buffer=malloc(sizeof(char)*rest);
  read(file,buffer,rest);
  write(socket,buffer,rest);
  free(buffer);
  close(file);
}

void handle_receive_file( int socket){
  int file;
  char file_name1[256];
  read(socket,file_name1,256);
  char path[256]="receive";
  strcat(path,file_name1);
  printf("fichier reçu: %s\n",path );
  file=open(path,O_CREAT|O_WRONLY,S_IRWXU);
  int length_file;
  read(socket,&length_file,sizeof(int));
  int i=1;
  while (i*256<length_file){
    char* buffer=malloc(sizeof(char)*256);
    read(socket,buffer,256);
    write(file,buffer,256);
    i++;
    free(buffer);
  }
  int rest=length_file-(i-1)*256;
  char* buffer=malloc(sizeof(char)*rest);
  read(socket,buffer,rest);
  if(write(file,buffer,rest)==-1){
    error("write");
  }
  free(buffer);
  close(file);
}

void readline(int sock,char *message){
  char * msg=message;
  size_t size_msg=256;
  getline(&msg,&size_msg,stdin);
  if (write(sock,message,256)==-1) {
    error("write");
  }
}
