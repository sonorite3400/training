#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// TODO: Bufferサイズは何を基準にする？
#define BUFFER_SIZE 1024

int TryClose(int socketFd)
{
    while (1)
    {
        if (errno == 4)
        {
            continue;
        }
        if (close(socketFd) == -1)
        {
            perror("Error: close() failed\n");
            return 1;
        }
        return 0;
    }
}

// clientFd will be closed at the end
int Echo(int clientFd)
{
    char recvBuffer[BUFFER_SIZE];
    int sendSize;

    while (1)
    {
        int recvSize = recv(clientFd, recvBuffer, BUFFER_SIZE, 0);
        if (recvSize == -1)
        {
            perror("Error: recv() failed\n");
            if (TryClose(clientFd) == 1)
            {
                return 1;
            }
            break;
        }
        else if (recvSize == 0)
        {
            if (TryClose(clientFd) == 1)
            {
                return 1;
            }
            break;
        }
        while (1)
        {
            sendSize = (clientFd, recvBuffer, recvSize, 0);
            if (sendSize == recvSize)
            {
                break;
            }
            if (sendSize == -1)
            {
                perror("Error: send() failed\n");
                if (TryClose(clientFd) == 1)
                {
                    return 1;
                }
                break;
            }
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
        TryClose(socketFd);
        return 1;
    }

    // TODO: backlogに入れるべき大きさは何を基準にする？
    if (listen(socketFd, 5) == -1)
    {
        perror("Error: listen() failed\n");
        TryClose(socketFd);
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
            if (TryClose(socketFd) == 1)
            {
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
