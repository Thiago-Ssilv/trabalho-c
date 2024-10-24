#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

void cleanup(SOCKET sock, FILE *file) {
    if (file) fclose(file);
    if (sock != INVALID_SOCKET) closesocket(sock);
    WSACleanup();
}

int main() {
    WSADATA wsa;
    SOCKET server_fd, new_socket;
    struct sockaddr_in server, client;
    int client_len, bytes_read;
    char buffer[BUFFER_SIZE];

    printf("Inicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Falha na inicialização. Código de erro: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Winsock inicializado.\n");

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Erro ao criar o socket: %d\n", WSAGetLastError());
        cleanup(server_fd, NULL);
        return 1;
    }
    printf("Socket criado.\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Erro no bind: %d\n", WSAGetLastError());
        cleanup(server_fd, NULL);
        return 1;
    }
    printf("Bind realizado.\n");

    listen(server_fd, 3);
    printf("Aguardando conexões...\n");

    while (1) {
        client_len = sizeof(struct sockaddr_in);
        if ((new_socket = accept(server_fd, (struct sockaddr *)&client, &client_len)) == INVALID_SOCKET) {
            printf("Erro no accept: %d\n", WSAGetLastError());
            cleanup(new_socket, NULL);
            continue; 
        }
        printf("Conexão aceita.\n");

        FILE *file = fopen("arquivo_recebido.txt", "wb");
        if (file == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            cleanup(new_socket, NULL);
            continue;
        }

        
        while ((bytes_read = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0) {
            fwrite(buffer, sizeof(char), bytes_read, file);
        }
        if (bytes_read < 0) {
            printf("Erro ao receber dados: %d\n", WSAGetLastError());
        } else {
            printf("Arquivo recebido com sucesso.\n");
        }

       
        fclose(file);
        closesocket(new_socket);
    }

    
    cleanup(server_fd, NULL);
    return 0;
}

