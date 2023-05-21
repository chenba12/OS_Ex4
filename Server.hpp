#ifndef OS_EX4_SERVER_H
#define OS_EX4_SERVER_H

#include "Reactor.hpp"

class Server {
private:
    Reactor reactor;
    int server_fd;
    const int port = 9034;

public:
    Server();

    ~Server();

    void acceptConnection();

    static void echoBack(int socket, char *ip, int port);

    void start();
};

#endif //OS_EX4_SERVER_H
