//
// Created by Frank Wu on 16/12/2016.
//

#ifndef TINYCLOUD_MASTER_H
#define TINYCLOUD_MASTER_H

// import all the libs
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <map>
#include <functional>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <thread>
#include <chrono>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <boost/thread.hpp>
#include <mutex>

#include <iostream>
#include <memory>
#include <map>
#include <unistd.h>
#include <string>
#include <string.h>
#include <pthread.h>

#include <cstdio>

#include <grpc++/grpc++.h>

#include "../grpc_backend/backend.grpc.pb.h"

using namespace std;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using backend::Storage;
using backend::MemTableInfo;

// design a class for saving a node
class StorageNodeInfo {
public:
    int user_number;
    int storage_size;
    vector<string> user_list;
    bool crashed;
    StorageNodeInfo() {
        storage_size = 0;
        user_number = 0;
        crashed = false;
    }
};

class MasterNode {
    // storage number from the txt config file
    int max_node_number;
    // storage node ip mapping - #, IP:Port
    map<int, string> ip_mapping;
    // storage node inverse ip mapping - IP:Port, #
    map<string, int> inverse_ip_mapping;
    // storage node replica mapping - IP:Port, IP:Port
    map<string, string> replica_mapping;
    // storage node crashing indexing table - #, boolean
    map<int, bool> crash_mapping;
    // user mapping to the storage node - username: #
    map<string, int> user_mapping;
    // meta-info about all the storage node - IP:Port, Struct
    map<string, MemTableInfo> mem_info_mapping;
public:
    // constructor
    MasterNode(string config_file);

    // methods
    // get user address
    string get_user_node(string username);
    // create a new user
    int create_user(string username);
    // getting a map indicating the status of all the nodes
    int get_status(map<string, bool> &status);
    // getting a map indicating the user distribution accross the nodes
    int get_info(map<string, StorageNodeInfo> &info);
    // disenable a node from master
    int disenable_node(int index);
    // disenable a node from master
    int enable_node(int index);
    // suppliment function
    int hash_user2node(string username) {
        hash<string> hash_fn;
        unsigned int num = (unsigned int) hash_fn(username) % (max_node_number);
        // put the new user to the mapping
        return num;
    }
    // send stored storage node meta info to front server
    int send_node_date(map<string, MemTableInfo> &res);

    // background thread
    // for replaying the logs
    int replay();
    // for adding checkpoint, it essentially checking all the
    // in memory data strcuture to the disk, in different files
    int checking_to_disk();
    // for reaching the check point file
    int checking_out_disk();
    // checking if a storage node is down
    int failure_checking();
    // supp function
    int construct_crash_map();
    int construct_user_map();
    // for getting meta info from storage node
    int checking_node_data();
};

#endif //TINYCLOUD_MASTER_H
