#include <iostream>
#include <netinet/in.h>
#include <csignal>
#include "src/reactor.hpp"

// Handler function for processing client messages
void handleClientMessage() {
    // Implement your server logic here
    // Read client's message, process it, and send a response if needed
}

int main() {
    // Create Reactor instance
    Reactor* reactor = Reactor::createReactor();

    // Create server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        delete reactor;
        return 1;
    }

    // Bind the server socket to a port
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);  // Replace with your desired port number
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        close(serverSocket);
        delete reactor;
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == -1) {
        std::cerr << "Failed to listen on socket" << std::endl;
        close(serverSocket);
        delete reactor;
        return 1;
    }

    // Add server socket to the reactor
    reactor->addFd(serverSocket, POLLIN, handleClientMessage);

    // Start the reactor
    reactor->startReactor();

    // Cleanup
    close(serverSocket);
    delete reactor;

    return 0;
}
