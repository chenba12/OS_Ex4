#include <unistd.h>
#include <thread>
#include "Reactor.hpp"

Reactor::Reactor() : stop(false), reactorThread() {}

Reactor::~Reactor() {
    for (const auto &pair: handlers) {
        close(pair.first);
    }
}

void Reactor::addFD(int fd, handler_t handler) {
    handlers[fd] = std::move(handler);
}

void Reactor::startReactor() {
    if (pthread_create(&reactorThread, nullptr, &Reactor::reactorLoop, this) != 0) {
        // handle error
    }
}

void Reactor::stopReactor() {
    stop = true;
}

void Reactor::waitFor() const {
    if (pthread_join(reactorThread, nullptr) != 0) {
        // handle error
    }
}

void *Reactor::reactorLoop(void *arg) {
    auto *reactor = static_cast<Reactor *>(arg);
    auto *fds = new pollfd[reactor->handlers.size()];

    while (!reactor->stop) {
        int i = 0;
        for (const auto &pair: reactor->handlers) {
            fds[i].fd = pair.first;
            fds[i].events = POLLIN;
            ++i;
        }

        int ret = poll(fds, reactor->handlers.size(), -1);
        if (ret < 0) {
            // handle error
        }

        for (i = 0; i < reactor->handlers.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                reactor->handlers[fds[i].fd]();
            }
        }
    }

    delete[] fds;
    return nullptr;
}