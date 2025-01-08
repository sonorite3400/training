#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// TODO: Bufferサイズは何を基準にする？
#define BUFFER_SIZE 1024

int main()
{
    struct sockaddr_in serverAddr, clientAddr;
    char recvBuffer[BUFFER_SIZE];

    // AF_INET: IP protocol family
    // SOCK_STREAM: Sequenced, reliable, connection-based byte streams
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    // INADDR_ANY: Address to accept any incoming messages
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);
    bind(socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    // TODO: backlogに入れるべき大きさは何を基準にする？
    listen(socketFd, 5);

    unsigned int clientAddrLength = sizeof(clientAddr);
    int acceptSocketFd = accept(socketFd, (struct sockaddr *)&clientAddr, &clientAddrLength);

    // echo
    int recvSize = recv(acceptSocketFd, recvBuffer, BUFFER_SIZE, 0);
    send(acceptSocketFd, recvBuffer, recvSize, 0);

    close(socketFd);
    return 0;
}
