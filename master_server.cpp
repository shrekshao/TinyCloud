//
// Created by Frank Wu on 12/12/2016.
//
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


using namespace std;

// const variables declare
int max_node_number;
map<int, string> ip_mapping;
// boolean table for expression the node situation
map<int, bool> crash_mapping;

class master_server {
public:

    map<string, int> inverse_ip_mapping;
    map<string, int> user_mapping;
    map<string, string> replica_mapping;
    master_server(string config_file) {
        ifstream file(config_file);
        string str;
        cout << "storage node list:\n";
        int index = 0;
        while (getline(file, str))
        {
            // reading the node ip with its replica ip
            //cout << str << "\n";
            string token;
            vector<string> vec;
            stringstream ss(str);
            while(std::getline(ss, token, ',')) {
                vec.push_back(token);
            }
            // set the index and node ip addr mapping
            ip_mapping[index] = vec[0];
            // set the node ip addr and replica addr mapping
            replica_mapping[vec[0]] = vec[1];
            // initialize all the node as un-crashed condition
            crash_mapping[index] = false;
            // put into inverse ip mapping as well
            inverse_ip_mapping[vec[0]] = index;
            index++;
        }
        max_node_number = index;
    }

    // getting the ip and port for a request to connect
    string get_user_node(string username) {
        string user_node_addr;
        if (user_mapping.count(username) != 0) {
            // look for primary of crashed
            if (crash_mapping[user_mapping[username]]) {
                // if it is crashed, lookup on the replica
                user_node_addr = replica_mapping[ip_mapping[user_mapping[username]]];
            } else {
                user_node_addr = ip_mapping[user_mapping[username]];
            }
        } else {
            // if it is a new user
            user_node_addr = "Err: Non-exist user. Create A User First.";
        }
        return user_node_addr;
    }

    // assigning new user to ips
    int create_user(string username) {
        int mapping_node_id = hash_user2node(username);
        // check if this node is down, if so, we need to adjust the server to assign
        if  (crash_mapping[mapping_node_id]) {
            // we simply loop through, find the avaliable server
            user_mapping[username] = inverse_ip_mapping[replica_mapping[ip_mapping[mapping_node_id]]];
        }
        user_mapping[username] = mapping_node_id;
        return mapping_node_id;
    }

    // suppliment function
    int hash_user2node(string username) {
        hash<string> hash_fn;
        unsigned int num = (unsigned int) hash_fn(username) % (max_node_number - 1);
        std::cout << "User: " << username << " is assigned to Node: #"<< num << '\n';
        // put the new user to the mapping
        return num;
    }


};


/*
* background garbage collection thread
*/
void failure_checking() {
    do {
        // this garbage collector will run every 100s
        cout << "Node Failure Scanning" <<endl;
        this_thread::sleep_for (chrono::seconds(1));

        // looping through all the primaries to see if a failure is detected
        for (int i = 0; i < max_node_number; i ++) {
            string token;
            vector<string> vec;
            stringstream ss(ip_mapping[i]);
            while(std::getline(ss, token, ':')) {
                vec.push_back(token);
            }
            cout << "Scanning IP: " << vec[0] << " Port: " << vec[1] << "\n";
            // trying to connect to this IP with this port
            int sockfd = socket(AF_INET, SOCK_STREAM, 0);

            struct sockaddr_in sin;
            sin.sin_family = AF_INET;
            sin.sin_port   = htons(atoi(vec[1].c_str()));  // Could be anything
            inet_pton(AF_INET, vec[0].c_str(), &sin.sin_addr);

            if (connect(sockfd, (struct sockaddr *) &sin, sizeof(sin)) == -1)
            {
                printf("Error connecting %s: %d (%s)\n", vec[0].c_str(), errno, strerror(errno));
                // mark this node as unavaliable
                crash_mapping[i] = false;
            }
        }
    }while(true);
}




int main(int argc, char *argv[]){
    //master_server.get_user_node("wuzhengx");
    master_server ms = master_server("master_config.txt");
    cout << "Replica Node of Node #1 : " << ms.replica_mapping[ip_mapping[0]] << "\n";
    cout << "Replica Node of Node #2 : " << ms.replica_mapping[ip_mapping[1]] << "\n";
    cout << "Replica Node of Node #3 : " << ms.replica_mapping[ip_mapping[2]] << "\n";
    cout << "Replica Node of Node #4 : " << ms.replica_mapping[ip_mapping[3]] << "\n";
    cout << "Replica Node of Node #5 : " << ms.replica_mapping[ip_mapping[4]] << "\n";
    // Constructs the new thread and runs it. Does not block execution.
    thread failure_scanning(failure_checking);
    // Makes the main thread wait for the new thread to finish execution, therefore blocks its own execution.
    failure_scanning.join();
};