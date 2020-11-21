/**
 *
 * Program: tcpEchoserv_addr
 * Author: Benedikt Stonig
 * Date: 21.11.2020
 * Description: This Program acts as a serv_addr that recieves data from a cli_addr and sends it back.
 * Git-repo: https://github.com/Essigschurke13/School.git
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

//Defines
#define port 60400
#define buf_size 1024

//Global Variables
char buf[buf_size];


/* Funtions */
//This function is just for printing out errors and exiting the program on error
void error(char *msg, int excode)
{
    perror(msg);
    exit(excode);
}


int main()
{
    int sockfd, clientfd;
    struct sockaddr_in serv_addr, cli_addr;
    int len;
    int val = 1;

    //Create socket and check for failure
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        error("ERROR: Couldn't create socket", 1);
    }

    //Setup the sochaddr_in
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    len = sizeof(serv_addr);
    
    //Bind socket and check for failure
    if (bind(sockfd, (struct sockaddr *)&serv_addr, len) < 0) 
    {
        error("ERROR: Couldn't bind socket", 3);
    }
    
    //Start listening
    if (listen(sockfd, 10) < 0) 
    {
        error("ERROR: Listen error", 4);
    }
    
    //handle connection
    while (1) 
    {
        len = sizeof(cli_addr);
        printf("waiting for cli_addrs\n");
        
        //Accept connection and check for failure
        clientfd = accept(sockfd, (struct sockaddr *)&cli_addr, &len);
        if (clientfd < 0) 
        {
            error("ERROR: accept error", 5);
        }
        
        //print out connection details
        char *cli_addr_ip = inet_ntoa(cli_addr.sin_addr);
        printf("Accepted new connection from a cli_addr %s:%d\n", cli_addr_ip, ntohs(cli_addr.sin_port));

        //Recieve and send back data until disconnect
        while(1)
        {
            memset(buf, 0, sizeof(buf));

            //recieve data
            int rcvMsg = read(clientfd, buf, sizeof(buf));
            if ( rcvMsg < 0 ) 
            {
                error("ERROR: read error", 6);
            }
        
            printf("received %s from cli_addr\n", buf);

            //send data back to client
            if (write(clientfd, buf, rcvMsg) < 0) 
            {
                error("ERROR: write error", 7);
            }

            //break if client disconnects
            if( rcvMsg == 0)
            {
                break;
            }
        }
        close(clientfd);
    }
    close(sockfd);
    return 0;
}