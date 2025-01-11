#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// TODO: Bufferサイズは何を基準にする？
#define BUFFER_SIZE 1024

int Echo(int clientFd)
{
    char recvBuffer[BUFFER_SIZE];

    while (1)
    {
        int recvSize = recv(clientFd, recvBuffer, BUFFER_SIZE, 0);
        if (recvSize == -1)
        {
            perror("Error: recv() failed\n");
            if (close(clientFd) == -1)
            {
                perror("Error: close() failed\n");
                return 1;
            }
            break;
        }
        else if (recvSize == 0)
        {
            if (close(clientFd) == -1)
            {
                perror("Error: close() failed\n");
                return 1;
            }
            break;
        }
        // TODO: 一度にすべて送れなかった場合の対応
        if (send(clientFd, recvBuffer, recvSize, 0) == -1)
        {
            perror("Error: send() failed\n");
            if (close(clientFd) == -1)
            {
                perror("Error: close() failed\n");
                return 1;
            }
            break;
        }
    }
    return 0;
}

int main()
{
    struct sockaddr_in serverAddr, clientAddr;

    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1)
    {
        perror("Error: socket() failed\n");
        return 1;
    }
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);
    if (bind(socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("Error: bind() failed\n");
        if (close(socketFd) == -1)
        {
            perror("Error: close() failed\n");
            return 1;
        }
        return 1;
    }

    // TODO: backlogに入れるべき大きさは何を基準にする？
    if (listen(socketFd, 5) == -1)
    {
        perror("Error: listen() failed\n");
        if (close(socketFd) == -1)
        {
            perror("Error: close() failed\n");
            return 1;
        }
        return 1;
    };

    // TODO: マルチスレッドにする
    while (1)
    {
        unsigned int clientAddrLength = sizeof(clientAddr);
        int acceptedSocketFd = accept(socketFd, (struct sockaddr *)&clientAddr, &clientAddrLength);
        if (acceptedSocketFd == -1)
        {
            perror("Error: accept() failed\n");
            if (close(socketFd) == -1)
            {
                perror("Error: close() failed\n");
                return 1;
            }
            continue;
        }
        if (Echo(acceptedSocketFd) == 1)
        {
            return 1;
        }
    }
}
