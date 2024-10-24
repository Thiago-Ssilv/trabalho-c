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
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];
    FILE *file = NULL;
    int bytes_read;

    printf("Inicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Falha na inicialização. Código de erro: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Winsock inicializado.\n");

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Erro ao criar o socket: %d\n", WSAGetLastError());
        cleanup(sock, file);
        return 1;
    }
    printf("Socket criado.\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Erro na conexão: %d\n", WSAGetLastError());
        cleanup(sock, file);
        return 1;
    }
    printf("Conectado ao servidor.\n");

    file = fopen("arquivo_para_enviar.txt", "rb");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        cleanup(sock, file);
        return 1;
    }

    while ((bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
        if (send(sock, buffer, bytes_read, 0) == SOCKET_ERROR) {
            printf("Erro ao enviar o arquivo: %d\n", WSAGetLastError());
            cleanup(sock, file);
            return 1;
        }
    }

    if (ferror(file)) {
        printf("Erro ao ler o arquivo.\n");
        cleanup(sock, file);
        return 1;
    }

    printf("Arquivo enviado com sucesso.\n");
    cleanup(sock, file);
    return 0;
}
