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

// using this name space
using namespace std;

// design a class for saving a node
class StorageNodeInfo {
public:
    int user_number;
    int storage_size;
    vector<string> user_list;
    StorageNodeInfo() {
        storage_size = 0;
        user_number = 0;
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
public:
    // constructor
    MasterNode(string config_file);
    // get user address
    string get_user_node(string username);
    // create a new user
    int create_user(string username);
    // checking if a storage node is down
    int failure_checking();
    // getting a map indicating the status of all the nodes
    int get_status(map<string, bool> &status);
    // getting a map indicating the user distribution accross the nodes
    int get_info(map<string, StorageNodeInfo> &info);
    // disenable a node from master
    int disenable_node(int index);
    // suppliment function
    int hash_user2node(string username) {
        hash<string> hash_fn;
        unsigned int num = (unsigned int) hash_fn(username) % (max_node_number - 1);
        std::cout << "[Code]User: " << username << " is assigned to Node: #"<< num << '\n';
        // put the new user to the mapping
        return num;
    }
};


#endif //TINYCLOUD_MASTER_H
