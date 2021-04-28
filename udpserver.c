/***************************************************************/
/**************SERVER-SIDE IMPLEMENTATION OF RFC868*************/
/***************************************************************/
/*  BY: Jeppe Stjernholm Schildt & Alexander Stæhr Johansen
*   STUDY: Computer Engineering, 4th semester
*   COURSE: Computer Networking
*   DATE: 12-04-2021
*   DEADLINE: 14-04-2021
*
*   INSPIRATION: https://www.geeksforgeeks.org/udp-server-client-implementation-c/
*/

/* Standard libraries */
#include <stdio.h>                                                              //printf, scanf, fgets
#include <stdlib.h>                                                             //exit
#include <string.h>                                                             //string
#include <time.h>                                                               //time

/* Networking libraries */
#include <netinet/in.h>                                                         //sockaddr_in
#include <arpa/inet.h>                                                          //inet_addr

/* Constant */
#define MAX_SIZE 1024                                                           //maximum buffer size
#define TIME_1900_TO_1970 2208988800                                            //time in seconds between 1900 and 1970 as specified in RFC868
#define TIMESTAMP_SIZE 4                                                         // 4 bytes as specified in RFC868

/* Driver program */
int main() {        
    int PORT;                                                                   //port declaration
    char IP[MAX_SIZE];                                                          //ip declaration
    int socketDescriptor;                                                       //socket descriptor
    char timestampBuffer[TIMESTAMP_SIZE];                                       //message buffer containing the timestamp

    struct sockaddr_in serverAddress;                                           //struct handy for system calls related to internet operations
    struct sockaddr_in clientAddress;

    /* Interaction with user */
    printf("Please enter IP: ");
    fgets(IP, sizeof(IP), stdin);                                               //user specifies IP(v4)
    printf("Please enter port: ");          
    scanf("%d", &PORT);                                                         //user specifies port
    printf("Awaiting timestamp requests...\n");
      
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
      
    /* Socket and server is bound together */
    int correctBound = bind(socketDescriptor, (const struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (correctBound < 0)
    {
        printf("Critical error: socket was not bound successfully...\n");
        exit(EXIT_FAILURE);
    }
  
    int len = sizeof(clientAddress);                                   

    /* Client request */
    recvfrom(socketDescriptor, (char *)timestampBuffer, MAX_SIZE, MSG_WAITALL, (struct sockaddr*) &clientAddress, &len);
    printf("Timestamp request received...\n");

    /* Server reply */
    uint32_t serverTime = time(NULL) + TIME_1900_TO_1970;                   //1900 - 1970 time constant is added to the current time in seconds
    sprintf(timestampBuffer, "%ld", (long) serverTime);                     //the long int is converted into a string 
    printf("Timestamp reply: %s\n", timestampBuffer);                       //timestamp is printed to the terminal for verification purposes
    serverTime = htonl(serverTime);                                         //timestamp is converted into network byte order
    printf("Timestamp size: %lu\n", sizeof(serverTime));                    //the size of the timestamp is printed

    sendto(socketDescriptor, &serverTime, sizeof(serverTime), MSG_CONFIRM, (const struct sockaddr *) &clientAddress, len); //timestamp is transmitted
    return 0;
}
