#ifndef CLIENT_H_sqdfqsdfqsdf
#define CLIENT_H_sqdfqsdfqsdf

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/**
 * a function that given the address and the port fill the addrinfo
 */
void get_addr_info1(char* port, char* address, struct sockaddr_in * adr_server );

void get_addr_info(const char* address, const char* port, struct addrinfo** res);

/**
 * a function that is called when we receive a message from the server
 */
int handle_server_message(int sock,char receiver[256],char * file_path);
/**
 * a function called with user input
 */
void handle_client_message(int sock,char receiver[256], char *file_path);

/**
 * function called when a file is received
 */
void handle_file_receive(const char *file_path, const char* addr,
		const char* port);

/**
 * function called when we need to send a file
 */
void handle_send_file(int socket, char* file_path);

void handle_receive_file( int socket);

void readline(int sock,char *message);
#endif /* CLIENT_H_ */
