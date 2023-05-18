#ifndef OS_EX4_REACTOR_H
#define OS_EX4_REACTOR_H

#include <poll.h>
#include <functional>
#include <map>
#include <vector>

class Reactor {
public:
    using handler_t = std::function<void()>;

    Reactor();

    ~Reactor();

    void addFD(int fd, handler_t handler);

    void startReactor();

    void stopReactor();

    void waitFor() const;

private:
    std::map<int, handler_t> handlers;
    bool stop;
    pthread_t reactorThread;

    static void *reactorLoop(void *arg);


};


#endif
