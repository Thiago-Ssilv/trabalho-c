#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET sockfd;
    struct sockaddr_in server, client;
    char buffer[BUFFER_SIZE];
    int client_len, bytes_read;
    FILE *file;

  
    printf("Inicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Erro ao iniciar Winsock. Código de erro: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Winsock inicializado.\n");

   
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("Erro ao criar o socket UDP: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    printf("Socket UDP criado.\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

   
    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Erro no bind: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    printf("Bind realizado.\n");

  
    printf("Aguardando pacotes UDP...\n");
    client_len = sizeof(client);

    
    file = fopen("arquivo_recebido_udp.txt", "wb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

   
    while ((bytes_read = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client, &client_len)) > 0) {
        fwrite(buffer, sizeof(char), bytes_read, file);
        printf("Recebidos %d bytes do cliente.\n", bytes_read);
    }

    printf("Arquivo recebido com sucesso.\n");

    
    fclose(file);
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
