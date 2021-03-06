TARGETS = http_server

CXX = g++ -O2
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
GRPC_CC_OUTPUT_M = grpc_master/

vpath %.proto $(PROTOS_PATH)
vpath %.pb.cc $(GRPC_CC_OUTPUT)
vpath %.grpc.pb.cc $(GRPC_CC_OUTPUT)
vpath %.pb.o $(GRPC_CC_OUTPUT)
vpath %.grpc.pb.o $(GRPC_CC_OUTPUT)


PB_OBJ_DEPENDENCY = $(GRPC_CC_OUTPUT)backend.pb.o $(GRPC_CC_OUTPUT)backend.grpc.pb.o $(GRPC_CC_OUTPUT_M)master.pb.o $(GRPC_CC_OUTPUT_M)master.grpc.pb.o


all: $(TARGETS)

http_server: $(PB_OBJ_DEPENDENCY) http_server.o
# http_server: $(GRPC_CC_OUTPUT)backend.pb.o $(GRPC_CC_OUTPUT)backend.grpc.pb.o http_server.h http_server_grpc.h http_server.cpp
	$(CXX) $^ $(LDFLAGS) -std=c++11 -o $@


.PRECIOUS: %.grpc.pb.cc
%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=$(GRPC_CC_OUTPUT) --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

.PRECIOUS: %.pb.cc
%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=$(GRPC_CC_OUTPUT) $<


clean:
	rm -fv $(TARGETS) *.o
