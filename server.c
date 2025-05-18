#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

// Function to handle client communication
void doprocessing(int sock);

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int pid;

    // Automatically clean up zombie child processes
    signal(SIGCHLD, SIG_IGN);

    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // Allow reusing address quickly after program restarts
    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // Initialize server address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5001;  // Server port number

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any interface
    serv_addr.sin_port = htons(portno);      // Convert to network byte order

    // Bind the socket to the port
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    // Listen for incoming connections
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // Accept connections indefinitely
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            exit(1);
        }

        // Fork a child process to handle the client
        pid = fork();
        if (pid < 0) {
            perror("ERROR on fork");
            exit(1);
        }

        if (pid == 0) {
            // In child process
            close(sockfd);  // Child doesn't need the listening socket
            doprocessing(newsockfd);  // Handle client communication
            exit(0);
        } else {
            // In parent process
            close(newsockfd);  // Parent doesn't need the connected socket
        }
    }
}

// Function to handle communication with the client
void doprocessing(int sock) {
    int n;
    char buffer[256];

    while (1) {
        bzero(buffer, 256);
        n = read(sock, buffer, 255);  // Read client message
        if (n < 0) {
            perror("ERROR reading from socket");
            break;
        }
        if (n == 0) {
            printf("Client disconnected.\n");
            break;
        }

        printf("Here is the message: %s\n", buffer);

        // Respond to client
        n = write(sock, "I got your message", 18);
        if (n < 0) {
            perror("ERROR writing to socket");
            break;
        }
    }

    close(sock);  // Close the connection
}
