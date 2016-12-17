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

using namespace std;

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

const char*  server_ip = "0.0.0.0:50051";

// Indexer service in-memory storage
Indexer indexer_service;
Indexer indexer_service_backup;

// File service in-memory storage
BigTabler bigtable_service(server_ip);
BigTabler bigtable_service_backup(server_ip);

// Log FIFO counter
int primary_fifo = 0;
int replica_fifo = 0;

// Log mutex
mutex primary_mutex;
mutex replica_mutex;

// Logic and data behind the server's behavior.
class StorageServiceImpl final : public Storage::Service {
    Status GetFileList(ServerContext* context, const FileListRequest* request, FileListReply* reply) override {

        // Lock primary
        primary_mutex.lock();

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

            primary_mutex.unlock();
            return Status::OK;
        } else {
            primary_mutex.unlock();
            return Status::CANCELLED;
        }
    }

    Status InsertFileList(ServerContext* context, const FileListRequest* request, Empty* reply) override {

        // Write to log, lock primary
        primary_mutex.lock();
        ofstream primary_log(string("primary_log.txt"));
        string log("InsertFileList:insert(" + request->foldername() + ", false)\n");
        primary_log.write(log.c_str(), log.size());
        primary_log.close();

        int success = indexer_service.insert(request->foldername(), false);
        if (success == 1) {
            primary_mutex.unlock();
            return Status::OK;
        } else {
            primary_mutex.unlock();
            return Status::CANCELLED;
        }
    }

    Status PutFile(ServerContext* context, const FileChunk* request, Empty* reply) override {

        // Write to log, lock primary
        primary_mutex.lock();
        ofstream primary_log(string("primary_log.txt"));
        string log("PutFile:put(" + request->username() + request->filename() + ", " +  request->filetype() + ", " + to_string(request->length()) + "):insert(" + \
                request->username() + "/" + request->filename() + ", true)\n");
        primary_log.write(log.c_str(), log.size());
        primary_log.close();

        int success1 = bigtable_service.put(request->username(), request->filename(), (unsigned char *) request->data().c_str(), request->filetype(), request->length());
        int success2 = indexer_service.insert(request->username()+"/"+request->filename(), true);
        if (success1 == 1 && success2 == 1) {
            primary_mutex.unlock();
            return Status::OK;
        } else {
            primary_mutex.unlock();
            return Status::CANCELLED;
        }
    }

    Status UpdateFile(ServerContext* context, const FileChunk* request, Empty* reply) override {

        // Lock primary
        primary_mutex.lock();

        int success = bigtable_service.put(request->username(), request->filename(), (unsigned char *) request->orig_data().c_str(), (unsigned char *) request->data().c_str(), request->filetype(), request->orig_length(), request->length());
        if (success == 1) {
            primary_mutex.unlock();
            return Status::OK;
        } else {
            primary_mutex.unlock();
            return Status::CANCELLED;
        }
    }

    Status GetFile(ServerContext* context, const FileChunkRequest* request, FileChunk* reply) override {

        // Lock primary
        primary_mutex.lock();

        FileMeta* file_meta = bigtable_service.getMeta(request->username(), request->filename());

        if (file_meta == NULL) {
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
            primary_mutex.unlock();
            return Status::OK;
        } else {
            primary_mutex.unlock();
            return Status::CANCELLED;
        }
    }

    Status DeleteFile(ServerContext* context, const FileChunkRequest* request, Empty* reply) override {

        // Write to log, lock primary
        primary_mutex.lock();
        ofstream primary_log(string("primary_log.txt"));
        string log("DeleteFile:put(" + request->username() + ", " + request->filename() + ")\n");
        primary_log.write(log.c_str(), log.size());
        primary_log.close();

        pair<int, bool> file_info = indexer_service.checkIsFile(request->username()+"/"+request->filename());

        if (file_info.first == -1) {
            primary_mutex.unlock();
            return Status::CANCELLED;
        }

        if (file_info.second) {
            int success1 = indexer_service.delet(request->username()+"/"+request->filename());
            if (success1 == -1) {
                primary_mutex.unlock();
                return Status::CANCELLED;
            }
            int success2 = bigtable_service.delet(request->username(), request->filename());
            if (success1 == 1 && success2 == 1) {
                primary_mutex.unlock();
                return Status::OK;
            } else {
                primary_mutex.unlock();
                return Status::CANCELLED;
            }
        } else {
            vector<string> delete_candidates;
            int success = indexer_service.findAllChildren(request->username()+"/"+request->filename(), delete_candidates);
            if (success == -1) {
                primary_mutex.unlock();
                return Status::CANCELLED;
            }

            int success1 = indexer_service.delet(request->username()+"/"+request->filename());
            if (success1 == -1) {
                primary_mutex.unlock();
                return Status::CANCELLED;
            }

            int success2 = 1;
            for (string str : delete_candidates) {
                int delim = str.find("/");
                success2 = (success2 == 1 && bigtable_service.delet(str.substr(0, delim), str.substr(delim+1, str.length()-delim-1)) == 1) ? 1 : -1;
            }

            if (success1 == 1 && success2 == 1) {
                primary_mutex.unlock();
                return Status::OK;
            } else {
                primary_mutex.unlock();
                return Status::CANCELLED;
            }
        }
    }

    Status GetFileList_Backup(ServerContext* context, const FileListRequest* request, FileListReply* reply) override {

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

    Status InsertFileList_Backup(ServerContext* context, const FileListRequest* request, Empty* reply) override {

        // Write to log, lock primary
        replica_mutex.lock();
        ofstream replica_log(string("replica_log.txt"));
        string log("InsertFileList:insert(" + request->foldername() + ", false)\n");
        replica_log.write(log.c_str(), log.size());
        replica_log.close();

        int success = indexer_service.insert(request->foldername(), false);
        if (success == 1) {
            primary_mutex.unlock();
            return Status::OK;
        } else {
            primary_mutex.unlock();
            return Status::CANCELLED;
        }
    }

    Status PutFile_Backup(ServerContext* context, const FileChunk* request, Empty* reply) override {

        // Write to log, lock primary
        primary_mutex.lock();
        ofstream primary_log(string("primary_log.txt"));
        string log("PutFile:put(" + request->username() + request->filename() + ", " +  request->filetype() + ", " + to_string(request->length()) + "):insert(" + \
                request->username() + "/" + request->filename() + ", true)\n");
        primary_log.write(log.c_str(), log.size());
        primary_log.close();

        int success1 = bigtable_service.put(request->username(), request->filename(), (unsigned char *) request->data().c_str(), request->filetype(), request->length());
        int success2 = indexer_service.insert(request->username()+"/"+request->filename(), true);
        if (success1 == 1 && success2 == 1) {
            primary_mutex.unlock();
            return Status::OK;
        } else {
            primary_mutex.unlock();
            return Status::CANCELLED;
        }
    }

    Status UpdateFile_Backup(ServerContext* context, const FileChunk* request, Empty* reply) override {

        // Lock primary
        primary_mutex.lock();

        int success = bigtable_service.put(request->username(), request->filename(), (unsigned char *) request->orig_data().c_str(), (unsigned char *) request->data().c_str(), request->filetype(), request->orig_length(), request->length());
        if (success == 1) {
            primary_mutex.unlock();
            return Status::OK;
        } else {
            primary_mutex.unlock();
            return Status::CANCELLED;
        }
    }

    Status GetFile_Backup(ServerContext* context, const FileChunkRequest* request, FileChunk* reply) override {

        // Lock primary
        primary_mutex.lock();

        FileMeta* file_meta = bigtable_service.getMeta(request->username(), request->filename());

        if (file_meta == NULL) {
            return Status::CANCELLED;
        }

        reply->set_username(file_meta->username);
        reply->set_filename(file_meta->file_name);
        reply->set_length(file_meta->file_length);
        reply->set_filetype(file_meta->file_type);

        unsigned char temp[file_meta->file_length];
        int success = bigtable_service.get(request->username(), request->filename(), (unsigned char *) temp, file_meta->file_length);
        if (success == file_meta->file_length) {
            reply->set_data((char *)temp);
            primary_mutex.unlock();
            return Status::OK;
        } else {
            primary_mutex.unlock();
            return Status::CANCELLED;
        }
    }

    Status DeleteFile_Backup(ServerContext* context, const FileChunkRequest* request, Empty* reply) override {

        // Write to log, lock primary
        primary_mutex.lock();
        ofstream primary_log(string("primary_log.txt"));
        string log("DeleteFile:put(" + request->username() + ", " + request->filename() + ")\n");
        primary_log.write(log.c_str(), log.size());
        primary_log.close();

        pair<int, bool> file_info = indexer_service.checkIsFile(request->username()+"/"+request->filename());

        if (file_info.first == -1) {
            primary_mutex.unlock();
            return Status::CANCELLED;
        }

        if (file_info.second) {
            int success1 = indexer_service.delet(request->username()+"/"+request->filename());
            if (success1 == -1) {
                primary_mutex.unlock();
                return Status::CANCELLED;
            }
            int success2 = bigtable_service.delet(request->username(), request->filename());
            if (success1 == 1 && success2 == 1) {
                primary_mutex.unlock();
                return Status::OK;
            } else {
                primary_mutex.unlock();
                return Status::CANCELLED;
            }
        } else {
            vector<string> delete_candidates;
            int success = indexer_service.findAllChildren(request->username()+"/"+request->filename(), delete_candidates);
            if (success == -1) {
                primary_mutex.unlock();
                return Status::CANCELLED;
            }

            int success1 = indexer_service.delet(request->username()+"/"+request->filename());
            if (success1 == -1) {
                primary_mutex.unlock();
                return Status::CANCELLED;
            }

            int success2 = 1;
            for (string str : delete_candidates) {
                int delim = str.find("/");
                success2 = (success2 == 1 && bigtable_service.delet(str.substr(0, delim), str.substr(delim+1, str.length()-delim-1)) == 1) ? 1 : -1;
            }

            if (success1 == 1 && success2 == 1) {
                primary_mutex.unlock();
                return Status::OK;
            } else {
                primary_mutex.unlock();
                return Status::CANCELLED;
            }
        }
    }
};

void RunServer() {
    std::string server_address(server_ip);
    StorageServiceImpl service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    RunGC();
    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
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
