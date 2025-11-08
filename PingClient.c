#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char *argv[]){
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <server-ip> <server-port>\n", argv[0]);
        return 1;
    }

    struct sockaddr_in server;

    //create the UDP socket:
    int socketFile = socket(AF_INET, SOCK_DGRAM, 0);
    int length = sizeof(server);

    //clear struct 
    bzero(&server, length);

    //binding
    server.sin_family = AF_INET;

    //get the IP from command line arg
    server.sin_addr.s_addr = inet_addr(argv[1]);

    server.sin_port = htons(atoi(argv[2]));

    int sequence_number = 1;

    char ping_message[] = "PING";
    printf("%s\n", ping_message);
    

    // for (int i = 0; i < 10; i++)
    // { 
    //     printf("%d\n", sequence_number);
    //     sequence_number += 1;
    // }


    sendto(socketFile, ping_message, strlen(ping_message), 0, (const struct sockaddr *)&server, length);
    
    
    // server_response
    char res[1024];
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);


    int message = recvfrom(socketFile, res, sizeof(res), 0, (struct sockaddr *)&from, &fromlen);

    if (message < 0)
    {
        perror("Error receiving from server");
    }

    // add null terminator at the end of the received bytes
    res[message] = '\0';

    // print the message from the server
    printf("From server: %s\n", res);


    close(socketFile);
    return 0;
}