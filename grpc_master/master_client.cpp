//
// Created by Frank Wu on 16/12/2016.
//
#include "master_client.h"

using namespace std;

static master_client msClient(grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()));

int main(int argc, char *argv[])
{
    string username = "wuzhengx";
    if (msClient.CreateUser(username)) {
        cout << "User: " << username << " is created!\n";
    } else {
        cout << "User: " << username << " is failed to create!\n";
    }

    string addr;
    if (msClient.GetUserAddr(username, addr)) {
        cout << "Retrived Info Successfully!\n";
        cout << "User: " << username << " is re-directed to Node: " << addr << "\n";
    } else {
        cout << "Retrived Info Failed!\n";
    }

}