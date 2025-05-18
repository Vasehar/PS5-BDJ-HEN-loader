/*
 * Copyright (c) 2025 BenNox_XD
 *
 * This file is part of PS5-BDJ-HEN-loader and is licensed under the MIT License.
 * See the LICENSE file in the root of the project for full license information.
 */
#include "sendfile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 8096

int send_file(const char *ip, int port, const char *filepath) {
    int sockfd;
    struct sockaddr_in server_addr;
    int filefd;
    ssize_t bytes_read, bytes_sent;
    char buffer[BUFFER_SIZE];

    if (!ip || !filepath) {
        fprintf(stderr, "Invalid IP or file path\n");
        return -1;
    }

    filefd = open(filepath, O_RDONLY);
    if (filefd < 0) {
        perror("Failed to open file");
        return -1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Failed to create socket");
        close(filefd);
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        close(filefd);
        close(sockfd);
        return -1;
    }

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(filefd);
        close(sockfd);
        return -1;
    }

    while ((bytes_read = read(filefd, buffer, sizeof(buffer))) > 0) {
        ssize_t total_sent = 0;
        while (total_sent < bytes_read) {
            bytes_sent = send(sockfd, buffer + total_sent, bytes_read - total_sent, 0);
            if (bytes_sent < 0) {
                perror("Failed to send data");
                close(filefd);
                close(sockfd);
                return -1;
            }
            total_sent += bytes_sent;
        }
    }

    if (bytes_read < 0) {
        perror("Failed to read from file");
    }

    close(filefd);
    close(sockfd);

    return 0;
}