#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define TIMEOUT_SEC 1

int main(int argc, char *argv[]){
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <server-ip> <server-port>\n", argv[0]);
        return 1;
    }

    // create the UDP socket:
    int socketFile = socket(AF_INET, SOCK_DGRAM, 0);
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;
    setsockopt(socketFile, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    struct sockaddr_in server;
    int length = sizeof(server);
    //clear struct 
    bzero(&server, length);

    //binding
    server.sin_family = AF_INET;

    //get the IP from command line arg
    server.sin_addr.s_addr = inet_addr(argv[1]);

    server.sin_port = htons(atoi(argv[2]));
    
    
    int num_received = 0;


    for (int seq_num = 1; seq_num <= 10; seq_num++)
    {
        char send_msg[1024];
        char res_buffer[1024];
        struct timeval start_time, end_time;
        int server_res;

        

        snprintf(send_msg, 1204, "PING %d %ld.%06ld",
                 seq_num, start_time.tv_sec, (long)start_time.tv_usec);

        //need to get the time before the send
        gettimeofday(&start_time, NULL);
        

        
        //send datagram
        sendto(socketFile, send_msg, strlen(send_msg), 0, (const struct sockaddr *)&server, length);


       
        struct sockaddr_in from;
        socklen_t fromlen = sizeof(from);

        bzero(res_buffer, 1024);

        server_res = recvfrom(socketFile, res_buffer, 1024 - 1, 0,
                     (struct sockaddr *)&from, &fromlen);
        
        //get end time
        gettimeofday(&end_time, NULL);


        //recvfrom returns -1 if there was no response
        if (server_res > 0)
        {
            
            double rtt = (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                         (end_time.tv_usec - start_time.tv_usec) / 1000.0;

            //printf("Duration: %.3f ms\n", rtt);


            res_buffer[server_res] = '\0'; 
           

            printf("PING received from %s: seq#=%d time=%.3f ms\n",
                   argv[1], seq_num, rtt);

            // Update stats
            num_received++;
            // rtt_sum += rtt;
            // if (rtt_min == -1 || rtt < rtt_min)
            //     rtt_min = rtt;
            // if (rtt > rtt_max)
            //     rtt_max = rtt;
        }
        else if (server_res < 0)
        {
            // Timeout
            printf("Request timeout for: seq#=%d\n",
                   seq_num);
            

        }
    }


    close(socketFile);
    return 0;
}