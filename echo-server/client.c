#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// TODO: Bufferサイズは何を基準にする？
#define BUFFER_SIZE 1024

int EchoRequest(int socketFd)
{
    char sendBuffer[BUFFER_SIZE], recvBuffer[BUFFER_SIZE];
    int sendSize, recvSize;

    while (1)
    {
        fgets(sendBuffer, BUFFER_SIZE, stdin);
        // TODO: 一度にすべて送れなかった場合の対応
        sendSize = send(socketFd, sendBuffer, BUFFER_SIZE, 0);
        if (sendSize == -1)
        {
            perror("Error: send() failed\n");
            if (close(socketFd) == -1)
            {
                perror("Error: close() failed\n");
                return 1;
            }
            return 1;
        }
        recvSize = recv(socketFd, recvBuffer, BUFFER_SIZE, 0);
        if (recvSize == -1)
        {
            perror("Error: recv() failed");
            if (close(socketFd) == -1)
            {
                perror("Error: close() failed\n");
                return 1;
            }
            return 1;
        }
        if (recvSize == 0)
        {
            puts("server did not respond\n");
            break;
        }
        puts(recvBuffer);
    }
    if (close(socketFd) == -1)
    {
        perror("Error: close() failed\n");
        return 1;
    }
    return 0;
}

int main()
{
    struct sockaddr_in serverAddr;

    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1)
    {
        perror("Error: socket() failed\n");
        return 1;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(8080);
    if (connect(socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("Error: connect() failed\n");
        return 1;
    }

    // echo
    if (EchoRequest(socketFd) == 1)
    {
        return 1;
    }
    return 0;
}
