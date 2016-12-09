//
// Created by tianli on 12/7/16.
//

#ifndef TINYCLOUD_BIGTABLER_H
#define TINYCLOUD_BIGTABLER_H

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
#include <boost/filesystem.hpp>
#include <boost/interprocess/sync/file_lock.hpp>

using namespace std;
typedef unsigned char byte;

const int MAX_BUFFER_SIZE = 1000000000; // define const

// file class
class FileMeta {
public:
    unsigned int buffer_start;
    unsigned int file_length;
    string file_name;
    string file_type;
    string sstable_name;
    bool is_flushed;
    bool is_deleted;
    FileMeta (unsigned int buffer_start, unsigned int file_length, string file_name, string file_type, string sstable_name, bool is_flushed = false, bool is_deleted = false) {
        this->buffer_start = buffer_start;
        this->file_length = file_length;
        this->file_name = file_name;
        this->file_type = file_type;
        this->sstable_name = sstable_name;
        this->is_flushed = is_flushed;
        this->is_deleted = is_deleted;
    }
};

class BigTabler {
    mutex m;
    vector<FileMeta*> deleted_files;

    string server_id; // the server id
    int file_id; // the file id on disk

    byte memtable[MAX_BUFFER_SIZE];
    unsigned int cur_pt; // the current buffer pointer

    map<string, map<string, FileMeta>> big_table; // define multilevel map. map within a map.
public:
    BigTabler (string s);
    void put (string username, string file_name, unsigned char content[], string type, unsigned int file_size);
    byte* get (string username, string file_name);
};


#endif //TINYCLOUD_BIGTABLER_H
