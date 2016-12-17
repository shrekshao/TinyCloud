//
// Created by tianli on 12/4/16.
//

#include <iostream>
#include <memory>
#include <map>
#include <unistd.h>

#include <grpc++/grpc++.h>

#include "backend.grpc.pb.h"
#include "Indexer.h"
#include "BigTabler.h"

void RunGC();
void writeToLog(string& msg);

using namespace std;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using backend::FileListRequest;
using backend::FileListReply;
using backend::Empty;
using backend::FileChunk;
using backend::FileChunkRequest;
using backend::Storage;

const char*  primary_server_ip = "0.0.0.0:50051";
const char*  replica_server_ip = "0.0.0.0:50052";

// Indexer service in-memory storage
Indexer indexer_service;

// File service in-memory storage
BigTabler bigtable_service(replica_server_ip);

// Log mutex
mutex replica_mutex;

// Logic and data behind the server's behavior.
class StorageServiceImpl final : public Storage::Service {
    Status GetFileList(ServerContext* context, const FileListRequest* request, FileListReply* reply) override {

        // Lock primary
        replica_mutex.lock();

        map<string, Node> res;
        int success = indexer_service.display(request->foldername(), res);
        //fprintf(stderr, "success: %d\n", success);
        if (success == 1) {
            for (map<string, Node>::iterator it = res.begin(); it != res.end(); ++it) {
                backend::FileInfo fi;
                fi.set_full_path(it->second.full_name);
                fi.set_name(it->second.node_name);
                fi.set_is_file(it->second.is_file);
                (*reply->mutable_filelist())[it->first] = fi;
            }

            replica_mutex.unlock();
            return Status::OK;
        } else {
            replica_mutex.unlock();
            return Status::CANCELLED;
        }
    }

    Status InsertFileList(ServerContext* context, const FileListRequest* request, Empty* reply) override {

        // Write to log, lock primary
        replica_mutex.lock();
        string log("InsertFileList:insert(" + request->foldername() + ", false)\n");
        writeToLog(log);

        int success = indexer_service.insert(request->foldername(), false);
        if (success == 1) {
            replica_mutex.unlock();
            return Status::OK;
        } else {
            replica_mutex.unlock();
            return Status::CANCELLED;
        }
    }

    Status PutFile(ServerContext* context, const FileChunk* request, Empty* reply) override {

        // Write to log, lock primary
        replica_mutex.lock();
        string log("PutFile:put(" + request->username() + request->filename() + ", " +  request->filetype() + ", " + to_string(request->length()) + "):insert(" + request->username() + "/" + request->filename() + ", true)\n");
        writeToLog(log);

        int success1 = bigtable_service.put(request->username(), request->filename(), (unsigned char *) request->data().c_str(), request->filetype(), request->length());
        int success2 = indexer_service.insert(request->username()+"/"+request->filename(), true);
        if (success1 == 1 && success2 == 1) {
            replica_mutex.unlock();
            return Status::OK;
        } else {
            replica_mutex.unlock();
            return Status::CANCELLED;
        }
    }

    Status UpdateFile(ServerContext* context, const FileChunk* request, Empty* reply) override {

        // Lock primary
        replica_mutex.lock();

        unsigned char *orig_data_temp = new unsigned char [request->orig_length()];
        unsigned char *data_temp = new unsigned char [request->length()];

        memcpy(orig_data_temp, request->orig_data(), request->orig_length());
        memcpy(data_temp, request->data(), request->length());

        int success = bigtable_service.cput(request->username(), request->filename(), (unsigned char *) request->orig_data().c_str(), (unsigned char *) request->data().c_str(), request->filetype(), request->orig_length(), request->length());
        if (success == 1) {

            // Write to log
            string log("UpdateFile:delet(" + request->username() + ", " + request->filename() + ")" + ":put(" + request->username() + ", " + request->filename() + ", " + request->filetype() + ", " + to_string(request->length()) + ")\n");
            writeToLog(log);

            int success1 = bigtable_service.delet(request->username(), request->filename());

            int success2 = 0;
            if (success1 == 1) {
                success2 = bigtable_service.put(request->username(), request->filename(), data_temp, request->filetype(), request->length());
            } else {
                replica_mutex.unlock();
                return Status::CANCELLED;
            }

            if (success2 == 1) {
                replica_mutex.unlock();
                return Status::OK;
            } else {
                replica_mutex.unlock();
                return Status::CANCELLED;
            }
        } else {
            replica_mutex.unlock();
            return Status::CANCELLED;
        }
    }

    Status GetFile(ServerContext* context, const FileChunkRequest* request, FileChunk* reply) override {

        // Lock primary
        replica_mutex.lock();

        FileMeta* file_meta = bigtable_service.getMeta(request->username(), request->filename());

        if (file_meta == NULL) {
            replica_mutex.unlock();
            return Status::CANCELLED;
        }

        reply->set_username(file_meta->username);
        reply->set_filename(file_meta->file_name);
        reply->set_length(file_meta->file_length);
        reply->set_filetype(file_meta->file_type);

        unsigned char temp[file_meta->file_length];
        int success = bigtable_service.get(request->username(), request->filename(), (unsigned char *) temp, file_meta->file_length);
        if (success == file_meta->file_length) {
            reply->set_data((char *)temp, file_meta->file_length);
            replica_mutex.unlock();
            return Status::OK;
        } else {
            replica_mutex.unlock();
            return Status::CANCELLED;
        }
    }

    Status DeleteFile(ServerContext* context, const FileChunkRequest* request, Empty* reply) override {

        // Write to log, lock primary
        replica_mutex.lock();
        string log("DeleteFile:put(" + request->username() + ", " + request->filename() + ")\n");
        writeToLog(log);

        pair<int, bool> file_info = indexer_service.checkIsFile(request->username()+"/"+request->filename());

        if (file_info.first == -1) {
            replica_mutex.unlock();
            return Status::CANCELLED;
        }

        if (file_info.second) {
            int success1 = indexer_service.delet(request->username()+"/"+request->filename());
            if (success1 == -1) {
                replica_mutex.unlock();
                return Status::CANCELLED;
            }
            int success2 = bigtable_service.delet(request->username(), request->filename());
            if (success1 == 1 && success2 == 1) {
                replica_mutex.unlock();
                return Status::OK;
            } else {
                replica_mutex.unlock();
                return Status::CANCELLED;
            }
        } else {
            vector<string> delete_candidates;
            int success = indexer_service.findAllChildren(request->username()+"/"+request->filename(), delete_candidates);
            if (success == -1) {
                replica_mutex.unlock();
                return Status::CANCELLED;
            }

            int success1 = indexer_service.delet(request->username()+"/"+request->filename());
            if (success1 == -1) {
                replica_mutex.unlock();
                return Status::CANCELLED;
            }

            int success2 = 1;
            for (string str : delete_candidates) {
                int delim = str.find("/");
                success2 = (success2 == 1 && bigtable_service.delet(str.substr(0, delim), str.substr(delim+1, str.length()-delim-1)) == 1) ? 1 : -1;
            }

            if (success1 == 1 && success2 == 1) {
                replica_mutex.unlock();
                return Status::OK;
            } else {
                replica_mutex.unlock();
                return Status::CANCELLED;
            }
        }
    }

};

void RunServer() {
    /*// Primary port
    std::string primary_server_address(primary_server_ip);
    StorageServiceImpl primary_service;

    ServerBuilder primary_builder;
    // Listen on the given address without any authentication mechanism.
    primary_builder.AddListeningPort(primary_server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    primary_builder.RegisterService(&primary_service);
    // Finally assemble the server.
    std::unique_ptr<Server> primary_server(primary_builder.BuildAndStart());
    std::cout << "Server primary listening on " << primary_server_address << std::endl;
    */

    // Replica port
    std::string replica_server_address(replica_server_ip);
    StorageServiceImpl replica_service;

    ServerBuilder replica_builder;
    // Listen on the given address without any authentication mechanism.
    replica_builder.AddListeningPort(replica_server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    replica_builder.RegisterService(&replica_service);
    // Finally assemble the server.
    std::unique_ptr<Server> replica_server(replica_builder.BuildAndStart());
    std::cout << "Server replica listening on " << replica_server_address << std::endl;

    RunGC();

    replica_server->Wait();

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.

    //primary_server->Wait();
}

void* gcHelper(void*) {
    int res = 1;
    while (res == 1) {
        sleep(DELETE_BUFFER_TIME);
        res = bigtable_service.gc();
    }
    pthread_exit(NULL);
}

void RunGC() {
    int rc = 0;
    pthread_t gcthread;

    pthread_create(&gcthread, NULL, &gcHelper, NULL);
    rc = pthread_join(gcthread, NULL);
}

int main(int argc, char** argv) {
    ///* Indexer test
    cout << indexer_service.insert("/tianli", false) << endl;
    cout << indexer_service.insert("/tianli/folder1", false) << endl;
    cout << indexer_service.insert("/tianli/folder2", false) << endl;
    cout << indexer_service.insert("/tianli/file3", true) << endl;
    cout << indexer_service.insert("/tianli/folder1/folder1.1", false) << endl;
    map<string, Node> res;
    int success = indexer_service.display("/tianli", res);
    cout << success << endl;
    for (map<string, Node>::iterator it = res.begin(); it != res.end(); ++it) {
        cout << it->first << " " << it->second.is_file << endl;
    }
    //*/

    ///* File storage test
    ifstream ifs1("file1.txt", ios::binary|ios::ate);
    ifstream::pos_type pos1 = ifs1.tellg();

    int length1 = pos1;

    char *pChars1 = new char[length1];
    ifs1.seekg(0, ios::beg);
    ifs1.read(pChars1, length1);
    ifs1.close();

    cout << "First put" << endl;
    cout << bigtable_service.put("tianli", "file1", (unsigned char *) pChars1, ".txt", length1) << endl;

    ifstream ifs2("file2.txt", ios::binary|ios::ate);
    ifstream::pos_type pos2 = ifs2.tellg();

    int length2 = pos2;

    char *pChars2 = new char[length2];
    ifs2.seekg(0, ios::beg);
    ifs2.read(pChars2, length2);
    ifs2.close();

    cout << "second put" << endl;
    cout << bigtable_service.put("tianli", "file2", (unsigned char *) pChars2, ".txt", length2) << endl;

    char *pChars3 = new char[length2+1];
    pChars3[length2] = '\0';
    cout << "get file2, length:" << length2 << endl;
    cout << bigtable_service.get("tianli", "file2", (unsigned char *) pChars3, length2) << endl;

    printf("%s\n", pChars3);
    //*/

    RunServer();

    return 0;
}

void writeToLog(string& msg) {
    ofstream replica_log(string("replica_log.txt"));
    replica_log.write(msg.c_str(), msg.size());
    replica_log.close();
}

