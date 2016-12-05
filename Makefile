TARGETS = http_server storage_server

all: $(TARGETS)

http_server: http_server.h http_server.cpp
	g++ $^ -lpthread -std=c++11 -o $@

storage_server: storage_server.cpp
    g++ $^ -lpthread -std=c++11 -o $@

clean:
	rm -fv $(TARGETS)
