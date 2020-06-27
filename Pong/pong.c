#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "util.h"

#define PORTNO "1266"

int main(int argc, char **argv) {
  int nping = 1;
  int ch;
  char *pongport = strdup(PORTNO);
  
  while ((ch = getopt(argc, argv, "h:n:p:")) != -1) {
      switch (ch) {

      case 'n':
      nping = atoi(optarg);
      break;

      case 'p':
      pongport = strdup(optarg);
      break;
      
      default:
      fprintf(stderr, "usage: pong [-n #pings] [-p port]\n");

      }
  }
  
  struct sockaddr_in them;
  socklen_t themlen;
  
  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof hints);                    
  hints.ai_family = AF_INET;                          
  hints.ai_socktype = SOCK_DGRAM;                     
  hints.ai_flags = AI_PASSIVE;                        
  
  int addrerr = getaddrinfo( NULL, pongport, &hints, &result );
  if (addrerr != 0) { exit(1); }
  
  int socketfd = socket(AF_INET, result->ai_socktype, result->ai_protocol);
  if (socketfd == -1) { exit(1); };
  
  int binderr = bind(socketfd, result->ai_addr, result->ai_addrlen);
  if (binderr < 0) { exit(1); }

  freeaddrinfo(result); 
  
  for (int i = 0; i < nping; i++) {

      // Listen for ping message 
      int sizeofrecarray = 1500;
      char recipient_array[sizeofrecarray]; 
      
      themlen = sizeof(them); 
      
      long packetsize = recvfrom(socketfd,&recipient_array,sizeofrecarray,0,(struct sockaddr *)&them,&themlen);
      
      if (packetsize == -1) { 

          printf("pong[%i]: Error receiving packet\n", i);

      } else {

          // Send response back

          printf("pong[%i]: received packet from %s\n", i, inet_ntoa(them.sin_addr));
          
          for (int j = 0; j < sizeofrecarray; j++) { recipient_array[j]++; }
          
          long sent_packet_of_size = sendto(
              socketfd,                         
              recipient_array,                  
              sizeofrecarray,                   
              0,                                
              (struct sockaddr*)&them,             
              themlen                         
             );

             if (sent_packet_of_size == -1) {
                printf("Error sending packet number %i.\n", i);
             }
        }

    }

    return 0;
}

                  