TARGETS = http_server

all: $(TARGETS)

http_server: http_server.cpp
	g++ $^ -lpthread -std=c++11 -o $@

clean::
	rm -fv $(TARGETS)
