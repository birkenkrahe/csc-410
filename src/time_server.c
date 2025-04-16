#include "time_server.h"
#define PORT "8080" // for the `service` parameter of `getaddrinfo`
#define CONN 10 // max number of connections to `listen` to

int main(void)
{
  printf("Configuring local address...\n");
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  
  struct addrinfo *bind_address;
  getaddrinfo(0, PORT, &hints, &bind_address);
  printf("Creating socket...\n");
  int socket_listen;
  socket_listen = socket(bind_address->ai_family,
                         bind_address->ai_socktype,
                         bind_address->ai_protocol);
  if (!(socket_listen >=0)) {
    fprintf(stderr, "socket() failed, (%d)\n", errno);
    return 1;
   }
  printf("Binding socket to local address...\n");
  if (bind(socket_listen,
           bind_address->ai_addr,
           bind_address->ai_addrlen)) {
    fprintf(stderr, "bind() failed. (%d)\n", errno);
    return 1;
   }
  freeaddrinfo(bind_address);
  printf("Listening...\n");
  if (listen(socket_listen, CONN) < 0) {
    fprintf(stderr, "listen() failed. (%d)\n", errno);
    return 1;
   }
  printf("Waiting for connection...\n");
  struct sockaddr_storage client_address;
  socklen_t client_len = sizeof(client_address);
  int socket_client = accept(socket_listen,
  			   (struct sockaddr*) &client_address,
  			   &client_len);
  if (!(socket_client >= 0)) {
    fprintf(stderr, "accept() failed, (%d)\n", errno);
    return 1;
   }
  printf("Client is connected... ");
  char address_buffer[100];
  getnameinfo((struct sockaddr*) &client_address,
  	    client_len,
  	    address_buffer,
  	    sizeof(address_buffer),
  	    0,0,NI_NUMERICHOST);
  printf("%s\n", address_buffer);
  printf("Reading request...\n");
  char request[1024];
  int bytes_received = recv(socket_client,
                            request,
                            1024,0);
  printf("Received %d bytes.\n", bytes_received);
  printf("Sending response...\n");
  const char *response =
    "HTTP/1.1 200 OK\r\n"
    "Connection: close\r\n"
    "Content-Type: text/plain\r\n\r\n"
    "Local time is: ";
  int bytes_sent = send(socket_client,
  		      response,
  		      strlen(response),
  		      0);
  printf("Sent %d of %d bytes.\n", bytes_sent, (int) strlen(response));
  time_t timer; // holds the time
  time(&timer); // get the time defined in <time.h>
  char *time_msg = ctime(&timer); // convert time to string
  bytes_sent = send(socket_client,  // this is the 2nd socket
  		  time_msg,  // this is now the time msg string
  		  strlen(time_msg), // requires <string.h>
  		  0);
  printf("Sent %d of %d bytes.\n",
         bytes_sent, (int)strlen(time_msg));
  printf("Closing requesting (client) connection...\n");
  close(socket_client); // close the send/receive socket
  printf("Closing listening (server) connection...\n");
  close(socket_listen);
  printf("Finished.\n");

    return 0;
}
