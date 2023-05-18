#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include "Server.hpp"

Server::Server() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        // handle error
        return;
    }

    struct sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9034);

    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        // handle error
        close(server_fd);
        return;
    }

    if (listen(server_fd, 10) < 0) {
        // handle error
        close(server_fd);
        return;
    }

    reactor.addFD(server_fd, [this]() { this->acceptConnection(); });
}

Server::~Server() {
    close(server_fd);
}

void Server::acceptConnection() {
    struct sockaddr_in address{};
    socklen_t addrlen = sizeof(address);

    int new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    if (new_socket < 0) {
        // handle error
        return;
    }

    char *connected_ip = inet_ntoa(address.sin_addr);
    int connected_port = ntohs(address.sin_port);

    std::cout << "New client connected: IP - " << connected_ip << ", Port - " << connected_port << "\n";

    reactor.addFD(new_socket, [new_socket, connected_ip, connected_port]() {
        Server::echoBack(new_socket, connected_ip, connected_port);
    });
}


void Server::echoBack(int socket, char *ip, int port) {
    char buffer[1024] = {0};
    ssize_t readValue = read(socket, buffer, 1024);
    if (readValue <= 0) {
        // handle error or client disconnection
        std::cout << "Client disconnected: IP - " << ip << ", Port - " << port << "\n";
        close(socket);
        return;
    }

    // Print the client's IP and port before the message
    std::cout << "Message from client: IP - " << ip << ", Port - " << port << ": " << buffer << std::endl;
}


void Server::start() {
    reactor.startReactor();
}
