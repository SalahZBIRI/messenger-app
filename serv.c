#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 2050

typedef struct {
    int socket;
    char username[20];
} Client;

Client clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

int get_color(int num);
void replace_emoji(char *message, const char *pattern, const char *emoji);

int get_color(int num) {
    switch (num) {
        case 0:
            return 97;    // Blanc
        case 1:
            return 33;    // Marron
        case 2:
            return 31;    // Rouge
        case 3:
            return 32;    // Vert
        case 4:
            return 34;    // Bleu
        case 5:
            return 95;    // Rose
        case 6:
            return 35;    // Violet
        case 7:
            return 90;    // Gris foncé
        case 8:
            return 30;    // Noir
        case 9:
            return 93;    // Jaune
        default:
            return -1;            
    }
}



void replace_emoji(char *message, const char *pattern, const char *emoji) {
    char *match;
    while ((match = strstr(message, pattern))) {
        size_t pattern_len = strlen(pattern);
        size_t emoji_len = strlen(emoji);
        memmove(match + emoji_len, match + pattern_len, strlen(match + pattern_len) + 1);
        memcpy(match, emoji, emoji_len);
    }
}




void send_message(char *message, int current_client) {
    pthread_mutex_lock(&clients_mutex);
    char buffer[1024]; // Taille du tampon augmentée pour accueillir les emojis
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].socket != -1 && i != current_client) {
            strcpy(buffer, message);
            replace_emoji(buffer, "<3", "\u2764\uFE0F");    // Cœur rouge
            replace_emoji(buffer, "<2", "\U0001F62D");      // Visage qui pleure
            replace_emoji(buffer, "<1", "\U0001F621");      // Visage en colère
            replace_emoji(buffer, "<4", "\U0001F602");      // Visage qui pleure de rire
            strcat(buffer, "\n"); // Ajouter un saut de ligne à la fin
            write(clients[i].socket, buffer, strlen(buffer));
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void send_private_message(char *message, char *receiver) {
    pthread_mutex_lock(&clients_mutex);
    char buffer[1024]; // Taille du tampon augmentée pour accueillir les emojis
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].socket != -1 && strcmp(clients[i].username, receiver) == 0) {
            strcpy(buffer, message);
            replace_emoji(buffer, "<3", "\u2764\uFE0F");    // Cœur rouge
            replace_emoji(buffer, "<2", "\U0001F62D");      // Visage qui pleure
            replace_emoji(buffer, "<1", "\U0001F621");      // Visage en colère
            replace_emoji(buffer, "<4", "\U0001F602");      // Visage qui pleure de rire
            strcat(buffer, "\n"); // Ajouter un saut de ligne à la fin
            write(clients[i].socket, buffer, strlen(buffer));
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}




void *client_thread(void *arg) {
    char message[BUFFER_SIZE];
    char private_message[BUFFER_SIZE];
    int current_client = *((int*)arg);
    int read_size;
    int couleur;
    
    while ((read_size = recv(clients[current_client].socket, message, BUFFER_SIZE, 0)) > 0) {
        message[read_size] = '\0'; 
        //couleur = get_color(current_client); 
        
        couleur =get_color(current_client);
        if (message[0] == '@') {
            // Private message
            char *receiver = strtok(message, " ");
            char *msg = strtok(NULL, "\n");
            if((strcmp(msg,">like") == 0)){
            sprintf(private_message, "\033[%dm[Private] %s: %s\033[0m", couleur, clients[current_client].username, "liked your message \U0001F44D");
            send_private_message(private_message, &receiver[1]);
            }
            else if((strcmp(msg,">dislike")) == 0){
            sprintf(private_message, "\033[%dm[Private] %s: %s\033[0m", couleur, clients[current_client].username, "disliked your message \U0001F44E");
            send_private_message(private_message, &receiver[1]);
            }
            else{
            sprintf(private_message, "\033[%dm[Private] %s: %s\033[0m", couleur, clients[current_client].username, msg);
            send_private_message(private_message, &receiver[1]);
            }
        } 
        
        else {
            // Broadcast message
            sprintf(private_message, "\033[%dm%s: %s\033[0m", couleur, clients[current_client].username, message);

            send_message(private_message, current_client);
        }
        
        memset(message, 0, sizeof(message));
        memset(private_message, 0, sizeof(private_message));
    }
    
    pthread_mutex_lock(&clients_mutex);
    printf("Client %s disconnected.\n", clients[current_client].username);
    close(clients[current_client].socket);
    clients[current_client].socket = -1;
    pthread_mutex_unlock(&clients_mutex);
    
    pthread_exit(NULL);
}



int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    pthread_t client_threads[MAX_CLIENTS];
    
    // Initialize client array
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        clients[i].socket = -1;
    }
    
    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create socket");
        exit(1);
    }
    
    // Set server details
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8888);
    
    // Bind the socket to the specified IP and port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        exit(1);
    }
    
    // Start listening for incoming connections
    listen(server_socket, MAX_CLIENTS);
    printf("Server started. Waiting for incoming connections...\n");
    
    int client_count = 0;
    
    while (1) {
        socklen_t client_address_len = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
        
        pthread_mutex_lock(&clients_mutex);
        
        // Check if maximum clients limit has been reached
        if (client_count == MAX_CLIENTS) {
            printf("Maximum clients reached. Connection rejected.\n");
            close(client_socket);
            pthread_mutex_unlock(&clients_mutex);
            continue;
        }
        
        // Find the first available slot in the clients array
        int i;
        for (i = 0; i < MAX_CLIENTS; ++i) {
            if (clients[i].socket == -1) {
                clients[i].socket = client_socket;
                break;
            }
        }
        
        // Receive and store the username
        char username[20];
        memset(username, 0, strlen(username));
        read(client_socket, username, sizeof(username));
        strcpy(clients[i].username, username);
        
        // Create a new thread for the client
        pthread_create(&client_threads[i], NULL, client_thread, &i);
        
        printf("New client connected: %s\n", clients[i].username);
        client_count++;
        
        pthread_mutex_unlock(&clients_mutex);
    }
    
    return 0;
}
