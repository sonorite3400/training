#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// TODO: Bufferサイズは何を基準にする？
#define BUFFER_SIZE 1024

int main()
{
    struct sockaddr_in serverAddr;
    char sendBuffer[BUFFER_SIZE], recvBuffer[BUFFER_SIZE];

    int socketFd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(8080);
    connect(socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    // echo
    fgets(sendBuffer, BUFFER_SIZE, stdin);
    send(socketFd, sendBuffer, BUFFER_SIZE, 0);
    recv(socketFd, recvBuffer, BUFFER_SIZE, 0);
    puts(recvBuffer);

    close(socketFd);
    return 0;
}
