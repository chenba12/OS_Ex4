CC := g++
CFLAGS := -std=c++17 -fPIC
LDFLAGS := -shared
LIBRARY := libst_reactor.so
SERVER := server
SRCDIR := src
OBJDIR := obj
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

.PHONY: all clean

all: $(LIBRARY) $(SERVER)

$(LIBRARY): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER): $(SRCDIR)/server.cpp $(LIBRARY)
	$(CC) $(CFLAGS) -o $@ $< -L. -lst_reactor

clean:
	rm -rf $(OBJDIR) $(LIBRARY) $(SERVER)