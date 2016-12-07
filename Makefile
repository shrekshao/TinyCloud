TARGETS = http_server directory_service file_storage_service

all: $(TARGETS)

http_server: http_server.h http_server.cpp
	g++ $^ -lpthread -std=c++11 -o $@

directory_service: directory_service.cpp
    g++ $^ -lpthread -std=c++11 -o $@

file_storage_service: file_storage_service.cpp
    g++ $^ -lpthread -std=c++11 -o $@

clean:
	rm -fv $(TARGETS)
