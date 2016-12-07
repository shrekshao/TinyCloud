//
// Created by Zhengxuan Wu on 4/12/2016.
//
#include <iostream>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <map>

// define const
const int MAX_BUFFER_SIZE = 1000000;
// define variables and space
using namespace std;

typedef uint8_t byte;
// file class
class File {
public:
    unsigned int buffer_start;
    unsigned int content_length;
    string content_type;
    bool is_flushed;
    File () {
        buffer_start = 0;
        content_length = 0;
        is_flushed = false;
    }
};

class Storage_System
{
private:
    byte ss_buffer[MAX_BUFFER_SIZE];
    // define multilevel map. map within a map.
    // this is the in memory big table
    map<string, map<string, File> > big_table;
    // deleted map
    map<string, File> deleted_table;
    // the current pointer
    int curr_pointer;
public:
    // constructor
    Storage_System () {
        curr_pointer = 0;
    }
    // insert a file to the system
    void put (string username, string file_name, string content) {

        // getting file type
        string::size_type idx;
        string extension;
        idx = file_name.rfind('.');

        if(idx != string::npos)
        {
            extension = file_name.substr(idx+1);
        }
        else
        {
            // No extension found
        }

        // getting file size
        int curr_buffer_size = content.len();

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

        } else if (curr_pointer + curr_buffer_size >= MAX_BUFFER_SIZE)
        {
            // if the buffer is overflow

        }
        else
        {
            // if the buffer is under-flow
            for (int i = 0; i < curr_buffer_size; i++) {
                //ss_buffer = [curr_pointer + i]
            }

        }

    }
    // delete a file in the system

    // search a file

};

int main(int argc, char *argv[]) {
    Storage_System *storage_system = new Storage_System();

    string s = "This is test.\n";
    storage_system->put("wuzhengx", "folder10/folder21/index.html", s);
}


