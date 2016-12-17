//
// Created by Frank Wu on 16/12/2016.
//

#include "MasterNode.h"

// Constructor
MasterNode::MasterNode(string config_file) {
    ifstream file(config_file);
    string str;
    cout << "storage node list:\n";
    int index = 0;
    while (getline(file, str))
    {
        // reading the node ip with its replica ip
        cout << str << "\n";
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

    // open the log file for flushing in the log file
}

// Get User Node IP:Port As A String
string MasterNode::get_user_node(string username) {
    cout << "[MASTER]:Receiving GET USER Call\n";
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
        user_node_addr = "ERR: Non-exist user. Create A User First.";
    }
    return user_node_addr;
}

// Create A New User, Assign To A Node Internally
int MasterNode::create_user(string username) {
    cout << "[MASTER]:Receiving CREATE USER Call\n";
    int mapping_node_id = hash_user2node(username);
    // check if this node is down, if so, we need to adjust the server to assign
    if  (crash_mapping[mapping_node_id]) {
        // we simply loop through, find the avaliable server
        user_mapping[username] = inverse_ip_mapping[replica_mapping[ip_mapping[mapping_node_id]]];
    }
    user_mapping[username] = mapping_node_id;
    return mapping_node_id;
}

// Getting The Status Live/Die Of Each Node
int MasterNode::get_status(map<string, bool> &status) {
    cout << "[MASTER]:Receiving GET STATUS Call\n";
    // looping through, get the basic status of each node
    for (int i = 0; i < max_node_number; i++) {
        status[ip_mapping[i]] = crash_mapping[i];
    }
    return 1;
}

// Getting An Info Struct For Each Node
int MasterNode::get_info(map<string, StorageNodeInfo> &info) {
    cout << "[MASTER]:Receiving GET INFO Call\n";
    // loop through all the users
    for (auto it = user_mapping.begin(); it != user_mapping.end(); ++it)
    {
        // push the user into the node user lists
        info[ip_mapping[it->second]].user_list.push_back(it->first);
        // adding the user count for this node
        info[ip_mapping[it->second]].user_number++;
    }
    // getting the status of the server
    for (int i = 0; i < max_node_number; i++) {
        info[ip_mapping[i]].crashed = crash_mapping[i];
    }
    return 1;
}

// Disenable One Node For Testing
int MasterNode::disenable_node(int index) {
    cout << "[MASTER]:Receiving DISENABLE Call\n";
    if (!crash_mapping[index]) {
        crash_mapping[index] = true;
        return 1;
    } else {
        return 0;
    }
}

// Enable One Node For Testing
int MasterNode::enable_node(int index) {
    cout << "[MASTER]:Receiving ENABLE Call\n";
    if (crash_mapping[index]) {
        crash_mapping[index] = false;
        return 1;
    } else {
        return 0;
    }
}

// Background Thread Pinning To Check Availability Of Nodes
int MasterNode::failure_checking() {
    // this garbage collector will run every 100s
    //cout << "Node Failure Scanning" <<endl;
    this_thread::sleep_for (chrono::seconds(1));

    // looping through all the primaries to see if a failure is detected
    for (int i = 0; i < max_node_number; i ++) {
        string token;
        vector<string> vec;
        stringstream ss(ip_mapping[i]);
        while(std::getline(ss, token, ':')) {
            vec.push_back(token);
        }
        //cout << "Scanning IP: " << vec[0] << " Port: " << vec[1] << "\n";
        // trying to connect to this IP with this port
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in sin;
        sin.sin_family = AF_INET;
        sin.sin_port   = htons(atoi(vec[1].c_str()));  // Could be anything
        inet_pton(AF_INET, vec[0].c_str(), &sin.sin_addr);

        if (connect(sockfd, (struct sockaddr *) &sin, sizeof(sin)) == -1)
        {
            //printf("Error connecting %s:%s\n", vec[0].c_str(), vec[1].c_str());
            // mark this node as unavaliable
            crash_mapping[i] = false;
        }
    }
    return 1;
}