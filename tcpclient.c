/*Ankeet Presswala
 * CMSC417
 * Proj0 - UDP Client
 */
#include<arpa/inet.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<time.h>

#define SERVER_IP   "127.0.0.1"
//"128.8.126.92"
#define SERVER_PORT 9033
#define DELIM       "#"

int main(int argc, char *argv[])
{
    
    int sockfd;
    struct sockaddr_in serv_addr;
    char *username, *message;
    char response[512];
    char data[512];
    
//    char *nonce, *raw_timestamp, *digest;
//    time_t timestamp;
//    char *timestring;
    
    //parse arguments
    if (argc<3){
        printf("usage: %s <username> <message>\n", argv[0]);
        exit(1);
    }
    username = argv[1];
    message = argv[2];
    
    //should ensure that the final message size is no larger than 256
    if ((strlen(username) + strlen(message)) > 255){
        printf("Combined user/message size too large.\n");
        exit(1);
    }
    
    /* Create a UDP socket */
    // Remember to use SOCK_DGRAM and IPPROTO_UDP
    if ((sockfd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))<0){
        printf("Error creating socket.\n");
        exit(1);
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) < 0){
        printf("Error in inet_pton(); possible invalid address string.\n");
        exit(1);
    }
    memset(&(serv_addr.sin_zero), '\0', 8);
    
    //establish connectoin
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        printf("Error establishing connection with %s.\n", SERVER_IP);
    
    strcpy(data, username);
    strcat(data, "#");
    strcat(data, message);
    strcat(data, "\n\n\r");
    
    //    printf("Sending %s...\n", data);
    if (send(sockfd, &data, strlen(data), 0) < 0){
        printf("Error sending %s to %s.\n", data, SERVER_IP);
        exit(1);
    }
    printf("Sending to %s:\t%s\n", SERVER_IP, data);
    /* Receive the server's response */
    // have to use recvfrom
    //    printf("Receiving...\n");
    if (recv(sockfd, response, 92, 0)<0){
        printf("Error receiving bytes.\n");
        exit(1);
    }
    //should this have to check server response for length?
    printf("Server response:\t%s\n", response);
    
    //parsing response
    //remember that strtok is destructive

//    nonce = strtok(response, DELIM);
//    digest = strtok(NULL, DELIM);
//    raw_timestamp = strtok(NULL, DELIM);
//    timestamp = (time_t) atoi(raw_timestamp);
//    timestring = ctime(&timestamp);
//    printf("nonce:%s timestamp:%s digest:%s\n", nonce, timestring, digest);
    close(sockfd);
    return 0;
}

