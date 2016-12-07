//
// Created by Zhengxuan Wu on 4/12/2016.
//
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

typedef uint8_t byte;
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

class Storage_System
{
private:
    byte ss_buffer[MAX_BUFFER_SIZE];
    // define multilevel map. map within a map.
    // this is the in memory big table
    map<string, map<string, File*> > big_table;
    // the current pointer
    int curr_pointer;
    // the server index
    string server_index;
    // the disk file index
    int file_index;

public:
    // constructor
    Storage_System (string s) {
        curr_pointer = 0;
        server_index = s;
        file_index = 0;
    }
    // insert a file to the system
    void put (string username, string file_name, unsigned char content[]) {

        // getting file type
        string::size_type idx;
        string extension;
        idx = file_name.rfind('.');

        if(idx != string::npos)
            extension = file_name.substr(idx+1);
        else
            extension = "";

        // getting file size
        int curr_buffer_size = strlen((char*)content);

        cout << "\n";
        cout << "File Description: \n";
        cout << "User Name: " << username << "\n";
        cout << "File Name: " << file_name << "\n";
        cout << "File Size: " << curr_buffer_size << "\n";
        cout << "File Type: "<< extension << "\n";
        cout << "Current SS Buffer Length = " << (sizeof(ss_buffer)/sizeof(*ss_buffer)) << endl;
        cout << "\n";

        if (curr_buffer_size >= MAX_BUFFER_SIZE)
        {
            // if it is a big file (bigger than max buffer size)
            // the file name is concat from the server index and
            // the file index of this server

            m.lock();//lock the incrementation
            file_index++;//no other thread can access variable i until m.unlock() is called
            m.unlock();//release for other thread

            // push into the disk
            string filename = server_index + to_string(file_index);
            ofstream outfile(filename);
            outfile.write((char *)&content[0], curr_buffer_size);

            // put the file metainfo in big table
            File *file = new File();
            file->buffer_start = 0;
            file->content_length = curr_buffer_size;
            file->content_type = extension;
            file->is_flushed = true;
            file->disk_filename = filename;
            file->is_deleted = false;
            big_table[username][file_name] = file;

        } else if (curr_pointer + curr_buffer_size >= MAX_BUFFER_SIZE)
        {
            // if the buffer is overflow
            m.lock();//lock the incrementation
            file_index++;//no other thread can access variable i until m.unlock() is called
            m.unlock();//release for other thread

            // write the current buffer to the disk first
            string filename = server_index + to_string(file_index);
            ofstream outfile(filename);
            outfile.write((char *)&ss_buffer[0], strlen((char*)ss_buffer));
            curr_pointer = 0;

            // put the file metainfo in big table
            File *file = new File();
            file->buffer_start = 0;
            file->content_length = curr_buffer_size;
            file->content_type = extension;
            file->is_flushed = false;
            file->is_deleted = false;
            big_table[username][file_name] = file;

            // write the the new file to the buffer
            for (int i = 0; i < curr_buffer_size; i++)
            {
                ss_buffer[i] = content[i];
                curr_pointer++;
            }

        }
        else
        {
            // put the file metainfo in big table
            File *file = new File();
            file->buffer_start = curr_pointer;
            file->content_length = curr_buffer_size;
            file->content_type = extension;
            file->is_flushed = false;
            file->is_deleted = false;
            big_table[username][file_name] = file;

            // write the the new file to the buffer
            for (int i = 0; i < curr_buffer_size; i++) {
                //ss_buffer = [curr_pointer + i]
                ss_buffer[curr_pointer + i] = content[i];
            }
            curr_pointer += curr_buffer_size;

        }

    }

    // delete a file in the system
    void remove (string username, string file_name)
    {
        // this should wait till garbage collection
        big_table[username][file_name]->is_deleted = true;
        deleted_files.push_back(big_table[username][file_name]);
        //delete big_table[username][file_name];
    }

    // search a file
    void get (string username, string file_name, unsigned char* content)
    {
        File *file = big_table[username][file_name];
        if (file->is_deleted)
            return;
        if (file->is_flushed)
        {
            // read from the disk file
            string filename = big_table[username][file_name]->disk_filename;
            FILE *f = fopen(filename.c_str(), "rb");
            fseek(f, 0, SEEK_END);
            long fsize = ftell(f);
            fseek(f, 0, SEEK_SET);
            char *temp_ss_buffer = (char *)malloc(fsize + 1);
            fread(temp_ss_buffer, fsize, 1, f);
            fclose(f);
            temp_ss_buffer[fsize] = 0;

            int index = 0;
            for(int i = file->buffer_start; i < file->buffer_start + file->content_length; i++) {
                content[index] = (unsigned char)temp_ss_buffer[i];
                index++;
            }

        }
        else
        {
            // read from the buffer
            int index = 0;
            for (int i = file->buffer_start; i < file->buffer_start + file->content_length; i++) {
                content[index] = ss_buffer[i];
                index++;
            }
        }
    }

};

/*
 * background garbage collection thread
 */
void garbage_collector() {
    // this garbage collector will run every 100s
    cout << "Garbage Collecting" <<endl;
    this_thread::sleep_for (chrono::seconds(100));
    // get all the files in the deleted files vector
    for (File* ff : deleted_files) {

        // delete from the buffer or the file
        if (ff->is_flushed)
        {
            // delete from a file
            // read from the disk file
            string filename = ff->disk_filename;
            FILE *f = fopen(filename.c_str(), "rb");
            fseek(f, 0, SEEK_END);
            long fsize = ftell(f);
            fseek(f, 0, SEEK_SET);
            char *temp_ss_buffer = (char *)malloc(fsize + 1);
            fread(temp_ss_buffer, fsize, 1, f);
            fclose(f);
            temp_ss_buffer[fsize] = 0;

            // update all pointers of all the other
            // files saved in this file

        }
        else
        {
            // delete in memory

        }
        // we need to delete from the map

        // delete from the deleted map

        // delete the referenced object at the end

    }
}

/*
 * background file partitioner
 * this is helpful for merging small files
 * distributed large files in local and
 * in replica machines
 */
void file_partitioner() {

}

int main(int argc, char *argv[]) {

    // initialize the storage system
    Storage_System *storage_system = new Storage_System("127.0.0.1:8000");
    // test
    unsigned char c[7] = {120, 'a', '!', 123, 255, 111, '\0'};
    storage_system->put("wuzhengx", "folder10/folder21/index.html", c);
    // running the garbage collection thread at the same time

    // Constructs the new thread and runs it. Does not block execution.
    thread garbage_collection(garbage_collector);
    // Makes the main thread wait for the new thread to finish execution, therefore blocks its own execution.
    garbage_collection.join();
}


