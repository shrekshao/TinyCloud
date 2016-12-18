//
// Created by tianli on 12/7/16.
//

#ifndef TINYCLOUD_BIGTABLER_H
#define TINYCLOUD_BIGTABLER_H

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <map>
#include <stdio.h>
#include <fstream>
#include <mutex>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <string.h>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_types.hpp>
#include <boost/thread/shared_mutex.hpp>

using namespace std;

typedef unsigned char byte;

const int MAX_BUFFER_SIZE = 1000000000; // define const
const int DELETE_BUFFER_TIME = 345600; // 4 days

// file class
class FileMeta {
public:
    unsigned int buffer_start;
    unsigned int file_length;
    string username;
    string file_name;
    string file_type;
    string sstable_name;
    bool is_flushed;
    bool is_deleted;
    FileMeta (unsigned int buffer_start, unsigned int file_length, string username, string file_name, string file_type, string sstable_name, bool is_flushed = false, bool is_deleted = false) {
        this->buffer_start = buffer_start;
        this->file_length = file_length;
        this->username = username;
        this->file_name = file_name;
        this->file_type = file_type;
        this->sstable_name = sstable_name;
        this->is_flushed = is_flushed;
        this->is_deleted = is_deleted;
    }
};

// minHeap compare class in gc
class Compare {
public:
    bool operator() (pair<unsigned int, string> one, pair<unsigned int, string> two) {
        return one.first > two.first;
    }
};

class BigTabler {
    string server_id; // the server id
    int file_id; // the file id on disk

    byte memtable[MAX_BUFFER_SIZE];
    unsigned int cur_pt; // the current buffer pointer

    map<string, map<string, FileMeta>> big_table; // define multilevel map. map within a map.

    vector<string> memtable_file; // Record file in memory
    map<string, vector<pair<time_t, FileMeta>>> deleted_files; // Record deleted files, gc will handle the rest of work and update this vector
    map<string, vector<string>> sstable_indexer; // Record files in each sstable, used for gc update

    map<string, boost::shared_mutex> sstable_mutex; // mutexs for sstable files on disk
    map<string, mutex> deleted_files_mutex; // mutexs for deleted_files of each sstable

    // predict function for remove_if in gc
    struct predicate {
        bool operator()(const pair<time_t, FileMeta> &pX) const {
            time_t timer;
            time(&timer);
            return timer - DELETE_BUFFER_TIME > (&pX)->first;
        }
    };
public:
    mutex log_mutex; // mutex for write to log between server and gc
    BigTabler (string s);
    int createuser (string usrname, string password);
    int getpassword (string username, string& res);
    int put (string username, string file_name, unsigned char content[], string type, unsigned int file_size);
    int cput (string username, string file_name, unsigned char orig_file_content[], unsigned char content[], string type, unsigned int orig_file_size, unsigned int file_size);
    FileMeta* getMeta (string username, string file_name);
    int get (string username, string file_name, unsigned char* res, unsigned int res_size);
    int delet (string username, string file_name);
    int gc(string& log_file);
    bool needClear(vector<pair<time_t, FileMeta>> &vec);
    int clearSSTable(map<string, vector<pair<time_t, FileMeta>>>::iterator it, vector<pair<time_t, FileMeta>>::iterator ite, ofstream& replica_log);
    unsigned int getCur_pt();
    int getMemtable(unsigned char* result);
    int put (string username, string file_name, string file_type, unsigned int file_size);
    void setMemtable(string& temp);
    void gcLog(string sstable, string temp[], int length);
    //void writeToLog(string& msg);
};


#endif //TINYCLOUD_BIGTABLER_H
