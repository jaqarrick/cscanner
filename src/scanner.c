#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include "config.h"

extern ScannerArgs scanner_args;

int scan_port(int port)
{
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    if (inet_pton(AF_INET, scanner_args.ip, &server_address.sin_addr) <= 0)
    {
        printf("Invalid address/ Address not supported\n");
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("Socket creation error\n");
        return 1;
    }

    int connection = connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address));
    close(sockfd);
    return connection;
}