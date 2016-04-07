#include "pcrypto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>

#define LOCAL_PORT  9033
#define BUFSIZE     4096
#define DELIM       "#"

static const int MAXPENDING = 5; // Maximum outstanding connection requests 11
int servSock;

void SIGTERMHandler (int signum) {
    close(servSock);
    exit(0);
}

void failure(char *message){
    printf("%s", message);
    close(servSock);
    exit(1);
}

void handleClient(int clientSock){
    char buffer[BUFSIZE];
    char data[4146]; // 40 (nonce) + (4096) username/message +
    char response[93]; //90 bytes + # + # + \0
    char randnum[20], timestring[20];
    char *nonce, *username, *message, *digest;
    time_t timestamp;
    
    if (recv(clientSock, buffer, BUFSIZE, 0) <0)
        failure("recv() failed\n");

    
    if ((timestamp = time(NULL)) <0)
        failure("Error in telling time.");
    
    snprintf(randnum, 10, "%d", rand());
    snprintf(timestring, 10, "%d", (int) timestamp);

//    printf("time: %s\trandnum %s\n", timestring, randnum);

    //nonce, username, message, timestring
    nonce = (char *) p_ascii_hash(randnum, sizeof(randnum));
    username = strtok(buffer, DELIM);
    message = strtok(NULL, DELIM);
    
//    printf("This just in from %s: %s\n", username, message);
    //put together nonce/username/message/timestring and hash
    strcpy(data, nonce);
    strcat(data, username);
    strcat(data, message);
    strcat(data, timestring);
    digest = (char *)p_ascii_hash(data, sizeof(data));
    
    //put together response
    strcpy(response, nonce);
    strcat(response, "#");
    strcat(response, digest);
    strcat(response, "#");
    strcat(response, timestring);
//    printf("sending response:\n%s\n", response);
    
    //send
    if (send(clientSock, response, strlen(response), 0) < 0){
        printf("Error sending %s.\n", response);
        exit(1);
    }
    
}

int main() {
    
    struct sockaddr_in servAddr;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSock;
    fd_set socketSet;

    // Create incoming socket
    if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        failure("socket() failed\n");
    
    //Ctrl-C socket cleanup
    signal(SIGTERM,SIGTERMHandler);
    signal(SIGSEGV,SIGTERMHandler);
    
    // Construct local address structure
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(LOCAL_PORT);
    memset(&(servAddr.sin_zero), '\0', 8);
    
    // Bind to local address
    if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
        failure("bind() failed\n");
    
    // Shh, listen   
    if (listen(servSock, MAXPENDING) < 0)
        failure("listen() failed\n");
    
    for (;;) {
        FD_ZERO(&socketSet);
        FD_SET(servSock, &socketSet);
        
        if (select(servSock + 1, &socketSet, NULL, NULL, NULL) == 0)
            printf("No echo requests for...Server still alive\n");
        
        if (FD_ISSET(servSock, &socketSet)){
            clientSock = accept(servSock,(struct sockaddr *) &clientAddr, &clientAddrLen);
            if (clientSock < 0)
                failure("accept() failed\n");
            char clientName[INET_ADDRSTRLEN];
            if (inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, clientName, sizeof(clientName)) == NULL)
                failure("Unable to get client address.\n");
            
            printf("Handling client %s/¡%d\n",clientName,ntohs(clientAddr.sin_port));
            handleClient(clientSock);
            
        }
        
    }
}
