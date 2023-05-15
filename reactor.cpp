#include "reactor.hpp"

#include <utility>

Reactor* Reactor::createReactor() {
    return new Reactor();
}

void Reactor::stopReactor() {
    running_ = false;
}

void Reactor::startReactor() {
    running_ = true;
    this->WaitFor();
}

void Reactor::addFd(int fd, short events, handler_t handler) {
    struct pollfd pfd = {fd, events, 0};
    fds_.push_back(pfd);
    handlers_.push_back(handler);
}

void Reactor::WaitFor() {
    while (running_) {
        int ret = poll(fds_.data(), fds_.size(), -1);
        if (ret > 0) {
            for (size_t i = 0; i < fds_.size(); i++) {
                if (fds_[i].revents != 0) {
                    handlers_[i]();
                    fds_[i].revents = 0; // Reset revents
                }
            }
        }
    }
}
