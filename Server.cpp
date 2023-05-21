#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include "Server.hpp"

/**
 * constructor starts the server TCP socket on port 9034
 * and start the reactor
 */
Server::Server() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket opening failed");
        return;
    }

    struct sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);


    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        return;
    }

    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return;
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        close(server_fd);
        return;
    }
    reactor.addFD(server_fd, [this]() { this->acceptConnection(); });
    start();

}

/**
 * destructor
 */
Server::~Server() {
    reactor.stopReactor();
    close(server_fd);
}

/**
 * accepts a new connection to the server
 */
void Server::acceptConnection() {
    struct sockaddr_in address{};
    socklen_t addrlen = sizeof(address);

    int new_socket = accept(server_fd, (struct sockaddr *) &address, &addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        reactor.stopReactor();
        return;
    }

    char *connected_ip = inet_ntoa(address.sin_addr);
    int connected_port = ntohs(address.sin_port);

    std::cout << "New client connected: IP - " << connected_ip << ", Port - " << connected_port << std::endl;

    reactor.addFD(new_socket, [new_socket, connected_ip, connected_port]() {
        Server::echoBack(new_socket, connected_ip, connected_port);
    });
}

/**
 * echo what the clients sent to the server on stdin
 * @param socket client socket
 * @param ip of the client
 * @param port of the client
 */
void Server::echoBack(int socket, char *ip, int port) {
    char buffer[1024] = {0};
    ssize_t readValue = read(socket, buffer, 1024);
    if (readValue <= 0) {
        std::cout << "Client disconnected: IP - " << ip << ", Port - " << port << std::endl;
        close(socket);
        return;
    }

    std::string str(buffer);
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    std::copy(str.begin(), str.end(), buffer);
    buffer[str.size()] = '\0';

    std::cout << "Message from client: IP - " << ip << ", Port - " << port << ": " << buffer << std::endl;
}


/**
 * start the reactor
 */
void Server::start() {
    reactor.startReactor();
    reactor.waitFor();
}
