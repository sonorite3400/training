#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// TODO: Bufferサイズは何を基準にする？
#define BUFFER_SIZE 1024

int TryClose(int socketFd) {
  while (1) {
    if (errno == 4) {
      continue;
    }
    if (close(socketFd) == -1) {
      perror("Error: close() failed\n");
      return 1;
    }
    return 0;
  }
}

int Communicate(int socketFd) {
  char sendBuffer[BUFFER_SIZE], recvBuffer[BUFFER_SIZE];
  int sendSize, recvSize;

  while (1) {
    if (fgets(sendBuffer, BUFFER_SIZE, stdin) == NULL) {
      perror("Error: fgets() failed\n");
    }

    while (1) {
      sendSize = send(socketFd, sendBuffer, BUFFER_SIZE, 0);
      if (sendSize == sizeof(sendBuffer)) {
        break;
      }
      if (sendSize == -1) {
        perror("Error: send() failed\n");
        TryClose(socketFd);
        return 1;
      }
    }

    recvSize = recv(socketFd, recvBuffer, BUFFER_SIZE, 0);
    if (recvSize == -1) {
      perror("Error: recv() failed\n");
      TryClose(socketFd);
      return 1;
    }
    if (recvSize == 0) {
      puts("server did not respond");
      break;
    }
    if (puts(recvBuffer) == EOF) {
      perror("Error: puts failed\n");
      return 1;
    }
  }
  if (TryClose(socketFd) == 1) {
    return 1;
  }
  return 0;
}

int main() {
  struct addrinfo hints, *serverAddr;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo("::1", "8080", &hints, &serverAddr) != 0) {
    perror("Error: getaddrinfo() failed\n");
    return 1;
  }

  int socketFd = socket(serverAddr->ai_family, serverAddr->ai_socktype,
                        serverAddr->ai_protocol);
  if (socketFd == -1) {
    perror("Error: socket() failed\n");
    return 1;
  }

  if (connect(socketFd, serverAddr->ai_addr, serverAddr->ai_addrlen) == -1) {
    perror("Error: connect() failed\n");
    return 1;
  }

  // echo
  if (Communicate(socketFd) == 1) {
    return 1;
  }
  return 0;
}
