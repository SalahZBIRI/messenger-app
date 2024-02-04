#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#define BUFFER_SIZE 1024

void *receive_message(void *arg) {
    int client_socket = *((int *)arg);
    char message[BUFFER_SIZE];
    int read_size;
    
    while ((read_size = recv(client_socket, message, BUFFER_SIZE, 0)) > 0) {
        message[read_size] = '\0';
        printf("%s", message);
        memset(message, 0, sizeof(message));
    }
    
    pthread_exit(NULL);
}

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    pthread_t receive_thread;
    
    // Create client socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Failed to create socket");
        exit(1);
    }
    
    // Set server details
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");  //local host
    server_address.sin_port = htons(8888);   // un exemple de port
    
    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(1);
    }
    
    printf("Connected to the server.\n");
    
    // Receive and store the username
    char username[20];
    memset(username, 0, strlen(username)); 

    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    
    username[strcspn(username, "\n")] = '\0';
    write(client_socket, username, strlen(username)); // send username to the server 
    
    // Create thread to receive messages from the server
    pthread_create(&receive_thread, NULL, receive_message, &client_socket); 
    
    char message[BUFFER_SIZE];
    
    while (1) {
    printf("Enter a message (or 'quit server' to quit): ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';  // Supprimer le caractère de saut de ligne

    if (strcmp(message, "quit server") == 0) {
        //write(client_socket, message, strlen(message));
        write(client_socket, "has left the server", strlen("has left the server"));
        break;
    }

    write(client_socket, message, strlen(message));
    memset(message, 0, sizeof(message));
}

    
    close(client_socket);
    pthread_cancel(receive_thread);
    return 0;
}


