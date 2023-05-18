CC := g++
CFLAGS := -std=c++17 -fPIC
LDFLAGS := -shared
RPATH := -Wl,-rpath=./
LIBRARY := libst_reactor.so
SERVER := react_server
SRCS := Reactor.cpp Server.cpp
OBJS := $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(LIBRARY) $(SERVER)

$(LIBRARY): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER): Main.cpp $(LIBRARY)
	$(CC) $(CFLAGS) $(RPATH) -o $@ $< -L. -lst_reactor

clean:
	rm -f $(OBJS) $(LIBRARY) $(SERVER)
