#include <iostream>
#include "reactor.hpp"

int main() {
    Reactor *reactor = Reactor::createReactor();

//    int fd1 = ... // Some file descriptor
//    reactor->addFd(fd1, POLLIN, [&]() {
//         Code to handle read event on fd1
//    });
//
//    int fd2 = ... // Some other file descriptor
//    reactor->addFd(fd2, POLLOUT, [&]() {
//         Code to handle write event on fd2
//    });
//
//    reactor->startReactor();

    // ... some time later ...

//    reactor->stopReactor();

//    delete reactor;

    return 0;
}
