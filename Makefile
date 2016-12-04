TARGETS = http_server

CXX = g++
CPPFLAGS += -I/usr/local/include -pthread
CXXFLAGS += -std=c++11

LDFLAGS += -L/usr/local/lib `pkg-config --libs grpc++ grpc`       \
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed \
           -lprotobuf -lpthread -ldl

PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

PROTOS_PATH = grpc_backend/protos

GRPC_CC_OUTPUT = grpc_backend/

vpath %.proto $(PROTOS_PATH)
vpath %.pb.cc $(GRPC_CC_OUTPUT)
vpath %.grpc.pb.cc $(GRPC_CC_OUTPUT)
vpath %.pb.o $(GRPC_CC_OUTPUT)
vpath %.grpc.pb.o $(GRPC_CC_OUTPUT)

all: $(TARGETS)

http_server: backend.pb.o backend.grpc.pb.o http_server.o
	$(CXX) $^ $(LDFLAGS) -std=c++11 -o $@


.PRECIOUS: %.grpc.pb.cc
%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=$(GRPC_CC_OUTPUT) --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

.PRECIOUS: %.pb.cc
%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=$(GRPC_CC_OUTPUT) $<


clean:
	rm -fv $(TARGETS) *.o *.pb.h *.pb.cc
