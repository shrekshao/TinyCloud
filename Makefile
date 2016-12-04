TARGETS = http_server

CPPFLAGS += -I/usr/local/include -pthread
CXXFLAGS += -std=c++11

LDFLAGS += -L/usr/local/lib -lpthread


all: $(TARGETS)

http_server: http_server.o
	g++ $^ $LDFLAGS -std=c++11 -o $@

clean:
	rm -fv $(TARGETS)
