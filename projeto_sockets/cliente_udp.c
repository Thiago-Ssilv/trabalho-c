#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") 

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];
    FILE *file;
    int bytes_read;

  
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Falha na inicialização. Código de erro: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Winsock inicializado.\n");

   
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Erro ao criar o socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    printf("Socket UDP criado.\n");

    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");  // IP do servidor
    server.sin_port = htons(PORT);  // Porta do servidor

  
    file = fopen("arquivo_para_enviar.txt", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        if (sendto(sock, buffer, bytes_read, 0, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
            printf("Erro ao enviar pacote: %d\n", WSAGetLastError());
            fclose(file);
            closesocket(sock);
            WSACleanup();
            return 1;
        }
        printf("Enviando %d bytes ao servidor...\n", bytes_read);
    }

    printf("Arquivo enviado com sucesso.\n");

    
    fclose(file);
    closesocket(sock);
    WSACleanup();

    return 0;
}
