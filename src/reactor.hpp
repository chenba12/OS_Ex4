#ifndef OS_EX4_REACTOR_H
#define OS_EX4_REACTOR_H

#include <unordered_map>
#include <functional>
#include <thread>

#include <vector>
#include <functional>
#include <sys/poll.h>

using handler_t = std::function<void()>;

class Reactor {
private:
    Reactor() : running_(false) {}

    void WaitFor();

    std::vector<struct pollfd> fds_;
    std::vector<handler_t> handlers_;
    bool running_;
public:
    static Reactor *createReactor();

    void stopReactor();

    void startReactor();

    void addFd(int fd, short events, handler_t handler);

    ~Reactor();
};

#endif
