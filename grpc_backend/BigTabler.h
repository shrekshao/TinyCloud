//
// Created by tianli on 12/7/16.
//

#ifndef TINYCLOUD_BIGTABLER_H
#define TINYCLOUD_BIGTABLER_H

#include <iostream>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <map>
#include <stdio.h>
#include <fstream>
#include <mutex>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <thread>
#include <chrono>

// define const
const int MAX_BUFFER_SIZE = 1000000;
// define variables and space
using namespace std;

mutex m;

// file class
class File {
public:
    unsigned int buffer_start;
    unsigned int content_length;
    string content_type;
    bool is_flushed;
    string disk_filename;
    bool is_deleted;
    File () {
        buffer_start = 0;
        content_length = 0;
        is_flushed = false;
        is_deleted = false;
    }
};

vector<File*> deleted_files;

class BigTabler {

};


#endif //TINYCLOUD_BIGTABLER_H
