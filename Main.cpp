#include "Server.hpp"

int main() {
    Server server;
    server.start();

    Reactor reactor;
    reactor.startReactor();
    reactor.waitFor();
    return 0;
}
