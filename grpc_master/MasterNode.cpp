//
// Created by Frank Wu on 16/12/2016.
//

#include "MasterNode.h"
#include "master_client.h"
#include "../grpc_backend/backend_client.h"


mutex _logging_mutex;

typedef boost::shared_mutex Lock;
typedef boost::unique_lock< Lock > WriteLock;
typedef boost::shared_lock< Lock > ReadLock;

Lock myLock;

// const string
string log_filename = "master_log_file.txt";
string crash_checkpoint_filename = "crash_checkpoint.txt";
string user_checkpoint_filename = "user_checkpoint.txt";

// ------------------------LOGGING------------------------
// the logging method
void _logging( const std::string &text )
{
    _logging_mutex.lock();
    std::ofstream log_file(
            log_filename, std::ios_base::out | std::ios_base::app );
    log_file << text << std::endl;
    _logging_mutex.unlock();
}

// ------------------------SUPP------------------------

// side function
inline bool ifexists (const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}

// ------------------------STATE MACHINE------------------------

// Replaying All The Command In File
int MasterNode::replay() {

    ifstream file_2("master_log_file.txt");
    string str;
    int line_index = 1;
    while (getline(file_2, str))
    {
        if(str==""){
            break;
        }
        string line_number = to_string(line_index);
        // reading the node ip with its replica ip
        string token;
        vector<string> vec;
        stringstream ss(str);
        while(std::getline(ss, token, ':')) {
            vec.push_back(token);
        }
        // adding a switch statement
        if (vec[0] == "CREATE") {
            cout<<"[Replaying"<< " Line:"<<line_number <<"] Create User\n";
            MasterNode::create_user(vec[1]);
        } else if (vec[0] == "ENABLE") {
            cout<<"[Replaying"<< " Line:"<<line_number <<"] Enable Node\n";
            int node_index = stoi(vec[1]);
            if (MasterNode::enable_node(node_index) == 0)
                cout<<"[Replaying] Enable Replay Failed\n";
        } else if (vec[0] == "DISENABLE") {
            cout<<"[Replaying"<< " Line:"<<line_number <<"] Disenable Node\n";
            int node_index = stoi(vec[1]);
            if (MasterNode::disenable_node(node_index) == 0)
                cout<<"[Replaying] Disenable Replay Failed\n";
        }
        line_index++;
    }
    file_2.close();
    return 1;
}

// Checking Into Disk Constantly For All In-memory Raw Data
int MasterNode::checking_to_disk() {

    remove(crash_checkpoint_filename.c_str());
    ofstream checkpoint_1(crash_checkpoint_filename, std::ios_base::out | std::ios_base::app );
    // writing to the check point file
    for (auto it = crash_mapping.begin(); it != crash_mapping.end(); ++it)
    {
        if (it->second) {
            checkpoint_1 << ip_mapping[it->first] << ":true" << std::endl;
        } else {
            checkpoint_1 << ip_mapping[it->first] << ":false" << std::endl;
        }

    }

    remove(user_checkpoint_filename.c_str());
    ofstream checkpoint_2(user_checkpoint_filename, std::ios_base::out | std::ios_base::app );
    // writing to the check point file
    for (auto it = user_mapping.begin(); it != user_mapping.end(); ++it)
    {
        checkpoint_2 << it->first << ":" << to_string(it->second) << std::endl;
    }

    //remove(log_filename.c_str());

    return 1;
}

// sub function call for reconstruct
int MasterNode::construct_crash_map() {
    // we simply open two checkpoint files
    // parsing the txt, and save in memory
    ifstream file("crash_checkpoint.txt");
    string str;
    while (getline(file, str))
    {
        // reading the node ip with its replica ip
        string token;
        vector<string> vec;
        stringstream ss(str);
        while(std::getline(ss, token, ':')) {
            vec.push_back(token);
        }
        // adding to the map
        if (vec[1] == "true") {
            crash_mapping[stoi(vec[0])] = true;
        } else {
            crash_mapping[stoi(vec[0])] = false;
        }
    }
    return 1;
}

int MasterNode::construct_user_map() {
    // we simply open two checkpoint files
    // parsing the txt, and save in memory
    ifstream file("user_checkpoint.txt");
    string str;
    while (getline(file, str))
    {
        // reading the node ip with its replica ip
        string token;
        vector<string> vec;
        stringstream ss(str);
        while(std::getline(ss, token, ':')) {
            vec.push_back(token);
        }
        // adding to the map
        user_mapping[vec[0]] = stoi(vec[1]);
    }
    return 1;
}

// Checking Out Disk Constantly When Rebooting
int MasterNode::checking_out_disk() {
    if(MasterNode::construct_crash_map() == 1 &&
            MasterNode::construct_user_map() ==1 ) {
        return 1;
    }
    return 1;
}

// ------------------------MASTER NDOE------------------------

// Constructor
MasterNode::MasterNode(string config_file) {
    ifstream file(config_file);
    string str;
    //cout << "storage node list:\n";
    int index = 0;
    while (getline(file, str))
    {
        if(str==""){
            continue;
        }
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
        // initialize all the buffer size to be zero
        mem_info_mapping[ip_mapping[index]].buffer_length = 0;
        index++;
    }
    max_node_number = index;

    file.close();

    remove(log_filename.c_str());
    //state machine reboot

    //checking point file?
    if (ifexists("./" + crash_checkpoint_filename) &&
            ifexists("./" + user_checkpoint_filename)) {
        cout << "[MASTER]:Check Point Detected! Cache Into Memory\n";
        MasterNode::checking_out_disk();
        // replay the rest
        if (ifexists(log_filename))
            MasterNode::replay();
    } else {
        // if not check point file, log file?
        if (ifexists("./" + log_filename)) {
            cout << "[MASTER]:Log File Detected! Replay All Calls\n";
            // only reply
            MasterNode::replay();
        }
    }
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
    _logging("CREATE:" + username);
    int mapping_node_id = hash_user2node(username);
    // check if this node is down, if so, we need to adjust the server to assign

    // dynamically allocate users for balancing work load
    // if the current node work load is twice higher than the min work load
    // assign to the one with min work load
    int index = 0;
    int min_index = 0;
    long min_load = 999999;
    for (auto it = mem_info_mapping.begin(); it != mem_info_mapping.end(); ++it)
    {
        //cout << "buffer length: " << it->second.buffer_length();
        long temp_size = (long) it->second.buffer_length;
        if (temp_size < min_load) {
            min_load = (long) it->second.buffer_length;
            min_index = index;
        }
        index++;
    }

    if (min_index == mapping_node_id || (long) mem_info_mapping[ip_mapping[mapping_node_id]].buffer_length <= min_load*2) {
        // we only assign user to the primary server
        user_mapping[username] = mapping_node_id;
        std::cout << "[Code]User: " << username << " is assigned to Node: "<< ip_mapping[mapping_node_id] << '\n';
    } else {
        mapping_node_id = min_index;
        if  (crash_mapping[mapping_node_id]) {
            // we simply loop through, find the avaliable server
            user_mapping[username] = inverse_ip_mapping[replica_mapping[ip_mapping[mapping_node_id]]];
        }
        user_mapping[username] = mapping_node_id;
    }
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
    _logging("DISENABLE:" + ip_mapping[index]);
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
    _logging("ENABLE:" + ip_mapping[index]);
    if (crash_mapping[index]) {
        crash_mapping[index] = false;
        return 1;
    } else {
        return 0;
    }
}

//int MasterNode::send_node_date(map<string, RawDataFromNode> &res) {
//    cout << "[MASTER]:Receiving GETTING NODE DATA Call\n";
//    // loop through all the nodes with its metainfo
////    res = std::map<string, MemTableInfo>(mem_info_mapping.begin(), mem_info_mapping.end());
//    // TODO
//    return 1;
//}

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
            printf("Node Failed Detected %s:%s\n", vec[0].c_str(), vec[1].c_str());
            // mark this node as unavaliable
            crash_mapping[i] = true;
        } else {
            printf("Node Successfully Detected %s:%s\n", vec[0].c_str(), vec[1].c_str());
            crash_mapping[i] = false;
        }
    }
    return 1;
}

// Background Thread For Retrieving Meta-info From Storage Node
int MasterNode::checking_node_data() {
    // looping through all the storage node to fetch info
    for (int i = 0; i < max_node_number; i ++) {
        // formulate a client to storage server sequentially
        if (crash_mapping[i]) {
            // go to replica IP for fetching meta data
            BackendClient bClient(grpc::CreateChannel(
                    replica_mapping[ip_mapping[i]], grpc::InsecureChannelCredentials()));
//            backend::MemTableInfo res;
            RawDataFromNode res;
            cout << "Primary Failed! Redirected ->" << replica_mapping[ip_mapping[i]] << "\n";
            if (bClient.GetMemTableInfo(res.buffer_length)) {
                cout << "Storage Metadata Retrived Success!\n";
                cout << "Buffer Size: "<< res.buffer_length << "\n";
//                cout << "Buffer Size: " << res.buffer_length() << "\n";
//                mem_info_mapping[replica_mapping[ip_mapping[i]]] = res;
            } else {
                cout << "Storage Metadata Retrived Failed!\n";
            }
        } else {
            // go to primary to get data
            BackendClient bClient(grpc::CreateChannel(
                    ip_mapping[i], grpc::InsecureChannelCredentials()));
            RawDataFromNode res;
            if (bClient.GetMemTableInfo(res.buffer_length)) {
                cout << "Storage Metadata Retrived Success!\n";
                cout << "Buffer Size: " << res.buffer_length << "\n";
                mem_info_mapping[ip_mapping[i]] = res;
            } else {
                cout << "Storage Metadata Retrived Failed!\n";
            }
        }

    }
    return 1;
}
