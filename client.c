#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    // Ensure hostname and port are provided
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);  // Convert port from string to int

    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // Get server address by hostname
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    // Set up server address structure
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    // Copy server address to structure
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);  // Convert port to network byte order

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }

    // Communication loop
    while (1) {
        printf("Please enter the message: ");
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);

        // Allow the client to exit by typing "exit"
        if (strncmp(buffer, "exit", 4) == 0)
            break;

        // Send message to server
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("ERROR writing to socket");
            break;
        }

        // Read response from server
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) {
            perror("ERROR reading from socket");
            break;
        }

        printf("Server: %s\n", buffer);  // Print server response
    }

    close(sockfd);  // Close the socket before exiting
    return 0;
}
