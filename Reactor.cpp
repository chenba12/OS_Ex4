#include <unistd.h>
#include <thread>
#include <cstdio>
#include "Reactor.hpp"

/**
 * reactor ctr
 */
Reactor::Reactor() : stop(false), reactorThread() {}

/**
 * close the file descriptors
 */
Reactor::~Reactor() {
    for (const auto &pair: handlers) {
        close(pair.first);
    }
}

/**
 * add a new file descriptor to the reactor
 * @param fd file descriptor
 * @param handler function to use
 */
void Reactor::addFD(int fd, handler_t handler) {
    handlers[fd] = std::move(handler);
}

/**
 * starts the reactor on a new thread
 */
void Reactor::startReactor() {
    if (pthread_create(&reactorThread, nullptr, &Reactor::reactorLoop, this) != 0) {
        perror("pthread_create failed");
    }
}

/**
 * stop the reactor
 */
void Reactor::stopReactor() {
    stop = true;
}

/**
 * joins the reactor thread
 */
void Reactor::waitFor() const {
    if (pthread_join(reactorThread, nullptr) != 0) {
        perror("pthread_join failed");
    }
}

/**
 * reactor main loop that happens inside a separate thread
 * @param arg arguments passed to the thread ()
 */
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
            perror("poll failed");
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