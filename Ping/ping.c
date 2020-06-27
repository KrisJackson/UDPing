#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdbool.h>

#include "util.h"
#define PORTNO "1266"

int main(int argc, char **argv) {
  int nping = 1, errors = 0;
  int ch;
  char *ponghost = strdup("localhost");
  char *pongport = strdup(PORTNO);
  struct sockaddr_in them;
  socklen_t   themlen;
  int arraysize = 100;

  while ((ch = getopt(argc, argv, "h:n:p:s:")) != -1) {
    switch (ch) {
      case 'h':
        ponghost = strdup(optarg);
        break;
      case 'n':
        nping = atoi(optarg);
        break;
      case 'p':
        pongport = strdup(optarg);
        break;
      case 's':
        arraysize = atoi(optarg);
        break;
      default:
        fprintf(stderr, "usage: ping [-h host] [-n #pings] [-p port] [-s size]\n");
    }
  }
  char array[arraysize];
  for (int i = 0; i < arraysize; i++) { array[i] = 100; }
  
  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof hints);  
  hints.ai_family = AF_INET;        
  hints.ai_socktype = SOCK_DGRAM;   
  
  int addrinfo_err = getaddrinfo(ponghost, pongport, &hints, &result);
  if (addrinfo_err != 0) { exit(1); }
  
  int socketfd = socket(AF_INET, result->ai_socktype, result->ai_protocol);
  if (socketfd == -1) { exit(1); }
  
  double totaltime = 0;
  for (int i = 0; i < nping; i++) {
      
      // Send packet 
      double start_time = get_wctime();
      long sent_packet_of_size = sendto( 
          socketfd, 
          array, 
          sizeof(array), 
          result->ai_flags, 
          result->ai_addr, 
          result->ai_addrlen
      );
      
      if (sent_packet_of_size == -1) { 
          printf("Error sending packet number %i.\n", i); 
          errors++;
      }
      
      freeaddrinfo(result); 
      
      // Wait for response 
      int sizeofrecarray = 1500;
      char recipient_array[sizeofrecarray];
      
      themlen = sizeof(them);
      
      long packetsize = recvfrom(
          socketfd,                   
          &recipient_array,           
          sizeofrecarray,             
          0,                         
          (struct sockaddr *)&them,  
          &themlen                    
      );

      if (packetsize == -1) {
        printf("Error receiving packet number %i.\n", i);
        errors++;
      }

      // Check that each value in the array is equal to 101
      bool hasAllValues = true;
      for (int j = 0; j < arraysize; j++) {
        if (recipient_array[j] != 101) {
            printf("%c", recipient_array[j]);
            hasAllValues = false;
        }
      }

      double end_time = get_wctime();
      double elapsed_time = (end_time - start_time) * 1000;
      totaltime += elapsed_time;

      if (!hasAllValues) {
        printf("ping[%i] : Value does not equal 101. Round-trip time: %lf ms\n", i, elapsed_time);
      } else {
        printf("ping[%i] : round-trip time: %lf ms\n", i, elapsed_time);
      }
    }
    
    if (errors == 0) {
        printf("No errors detected.\n");
        printf("Time to send %i packets of %i bytes %lf ms (%lf avg per packet)\n", nping, arraysize,totaltime, (totaltime/nping));
    } else {
        printf("%i errors detected.\n", errors);
    }
    
    return 0;
}
