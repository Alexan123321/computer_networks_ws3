/***************************************************************/
/**************CLIENT-SIDE IMPLEMENTATION OF RFC868*************/
/***************************************************************/
/*  BY: Jeppe Stjernholm Schildt & Alexander Stæhr Johansen
*   STUDY: Computer Engineering, 4th semester
*   COURSE: Computer Networking
*   DATE: 28-04-2021
*   DEADLINE: 14-04-2021
*
*   INSPIRATION: https://www.geeksforgeeks.org/udp-server-client-implementation-c/
*/

/* Standard libraries */
#include <stdio.h>                                                              //printf, scanf, fgets
#include <stdlib.h>                                                             //exit
#include <string.h>                                                             //string

/* Networking libraries */
#include <arpa/inet.h>                                                          //inet_addr
#include <netinet/in.h>                                                         //sockaddr_in
  
#define MAX_SIZE 1024
  
// Driver code
int main() {
    int PORT;                                                                   //port declaration
    char IP[MAX_SIZE];                                                          //ip declaration
    int socketDescriptor;                                                       //socket descriptor
    char requestBuffer[MAX_SIZE];                                               //request buffer
    uint32_t reply;                                                             //reply buffer

    struct sockaddr_in serverAddress;                                           //struct handy for system calls related to internet operations

    /* Interaction with user */
    printf("Please enter IP: ");
    fgets(IP, sizeof(IP), stdin);                                               //user specifies IP(v4)
    printf("Please enter port: ");          
    scanf("%d", &PORT);                                                         //user specifies port

    /* Creation of a socket file descriptor */
    socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);                          //AF_INET is denoting the IPv4 address family, where SOCK_DGRAM denotes the datagram-based protocol, UDP.                           
    if(socketDescriptor < 0 ) {                                                 
        printf("Critical error: socket was not created successfully...\n");
        exit(EXIT_FAILURE);
    }
      
    /* Server data is specified */
    serverAddress.sin_family = AF_INET;                                         //IPv4 address specification
    serverAddress.sin_addr.s_addr = inet_addr(IP);                              //IP is attached to server
    serverAddress.sin_port = htons(PORT);                                       //port is attached to server   
      
    long int n;                                                                 //int storing server reply
    int len;                                                                    //int size of appllication buffer
      
    /* Request timestamp */
    sendto(socketDescriptor, (const char *)requestBuffer, strlen(requestBuffer), MSG_CONFIRM, (const struct sockaddr *) &serverAddress, sizeof(serverAddress));
    printf("Timestamp request sent...\n"); 

    /* Receive timestamp */
    n = recvfrom(socketDescriptor, &reply, sizeof(reply), MSG_WAITALL, (struct sockaddr *) &serverAddress, &len);
    
    /* Print timestamp */
    printf("Timestamp received: %u \n", ntohl(reply));

    return 0;
}
