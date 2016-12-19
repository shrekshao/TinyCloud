//
// Created by Frank Wu on 16/12/2016.
//
//#include "master_client.h"
#include "../grpc_backend/backend_client.h"

using namespace std;

//MasterClient msClient(grpc::CreateChannel(
//        "localhost:52013", grpc::InsecureChannelCredentials()));

BackendClient bClient(grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()));

int main(int argc, char *argv[])
{
    int a;
    if (bClient.GetMemTableInfo(a)) {
        cout << "Storage Metadata Retrived Success!\n";
        cout << "Buffer Size: " << a << "\n";
//                cout << "Buffer Size: " << res.buffer_length() << "\n";
//                mem_info_mapping[replica_mapping[ip_mapping[i]]] = res;
    } else {
        cout << "Storage Metadata Retrived Failed!\n";
    }

//    cout << "-------------------------------------\n";
//
//    string username = "wuzhengx";
//    if (msClient.CreateUser(username)) {
//        cout << "User: " << username << " is created!\n";
//    } else {
//        cout << "User: " << username << " is failed to create!\n";
//    }
//
//    cout << "-------------------------------------\n";
//
//    string username_2 = "jiangyueyi";
//    if (msClient.CreateUser(username_2)) {
//        cout << "User: " << username_2 << " is created!\n";
//    } else {
//        cout << "User: " << username_2 << " is failed to create!\n";
//    }
//
//    cout << "-------------------------------------\n";
//
//    string addr;
//    if (msClient.GetUserAddr(username, addr)) {
//        cout << "Retrived Info Successfully!\n";
//        cout << "User: " << username << " is re-directed to Node: " << addr << "\n";
//    } else {
//        cout << "Retrived Info Failed!\n";
//    }
//
//    cout << "-------------------------------------\n";
//
//    map<string, bool> res;
//    if (msClient.GetNodesStatus(res)) {
//        cout << "Retrived Node Status Successfully!\n";
//        for (auto it = res.begin(); it != res.end(); ++it)
//        {
//            if (!it->second) {
//                cout << "Node: " << it->first << ". Status: Online\n";
//            } else {
//                cout << "Node: " << it->first << ". Status: Offline\n";
//            }
//        }
//    } else {
//        cout << "Retrived Node Status Failed!\n";
//    }
//
//    cout << "-------------------------------------\n";
//
//    // trigger one node to be offline
//    if (msClient.DisableNode(0)) {
//        cout << "Disable Node Successfully!\n";
//    } else {
//        cout << "Disable Node Failed!\n";
//    }
//
//    cout << "-------------------------------------\n";
//
//    string addr_2;
//    if (msClient.GetUserAddr(username, addr_2)) {
//        cout << "Retrived Info Successfully!\n";
//        cout << "User: " << username << " is re-directed to Node: " << addr_2 << "\n";
//    } else {
//        cout << "Retrived Info Failed!\n";
//    }
//
//    cout << "-------------------------------------\n";
//
//    map<string, bool> res_2;
//    if (msClient.GetNodesStatus(res_2)) {
//        cout << "Retrived Node Status Successfully!\n";
//        for (auto it = res_2.begin(); it != res_2.end(); ++it)
//        {
//            if (!it->second) {
//                cout << "Node: " << it->first << ". Status: Online\n";
//            } else {
//                cout << "Node: " << it->first << ". Status: Offline\n";
//            }
//        }
//    } else {
//        cout << "Retrived Node Status Failed!\n";
//    }
//
//    cout << "-------------------------------------\n";
//
//    map<string, NodeInfo> res_3;
//    if (msClient.GetNodesInfo(res_3)) {
//        cout << "Retrived Node Info Successfully!\n";
//        for (auto it = res_3.begin(); it != res_3.end(); ++it)
//        {
//            cout << "Node: " << it->first << ". User Number: " << it->second.user_number() << "\n";
//            cout << "User List: " << it->second.user_list() << "\n";
//        }
//    } else {
//        cout << "Retrived Node Status Failed!\n";
//    }
//
//    cout << "-------------------------------------\n";

}