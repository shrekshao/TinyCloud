//
// Created by tianli on 12/7/16.
//

#include "BigTabler.h"

// constructor
BigTabler::BigTabler (string s) {
    boost::filesystem::path dir(s);
    if(boost::filesystem::create_directory(dir)) {
        std::cout << "Create server folder " << s << "\n";
    } else {
        std::cout << "Create server folder " << s << " failed!\n";
    }
    cur_pt = 0;
    server_id = s;
    file_id = 0;
}

/*
 * Insert a file to the system, should have a lock, only one call at a time
 */
void BigTabler::put (string username, string file_name, unsigned char file_content[], string file_type, unsigned int file_size) {
    /*
    cout << "User Name: " << username << "\n";
    cout << "File Name: " << file_name << "\n";
    cout << "File Size: " << file_size << "\n";
    cout << "File Type: "<< file_type << "\n";
    cout << "Current SS Buffer Length = " << (sizeof(ss_buffer)/sizeof(*ss_buffer)) << endl;
    cout << "\n";
    */
    if (cur_pt + file_size >= MAX_BUFFER_SIZE) {
        // if the buffer is overflow, push to disk
        file_id++;

        // write the current buffer and the file to the disk first
        string file_path = server_id + "/" + to_string(file_id);

        ofstream outfile(file_path);
        outfile.write( (char*) &memtable[0], cur_pt);
        outfile.write( (char*) &file_content[0], file_size);

        // put the file metainfo in big table
        FileMeta* file_meta = new FileMeta(cur_pt, file_size, file_name, file_type, to_string(file_id), true, false);

        big_table[username][file_name] = file_meta;

        // clear buffer
        memset(&memtable[0], 0, cur_pt);
        cur_pt = 0;
    } else {
        // write the the new file to the buffer
        for (int i = 0; i < file_size; i++) {
            memtable[cur_pt + i] = file_content[i];
        }

        // put the file metainfo in big table
        FileMeta* file_meta = new FileMeta(cur_pt, file_size, file_name, file_type, NULL, false, false);
        big_table[username][file_name] = file_meta;

        cur_pt += file_size;
    }
}

/*
 * Search for a file
 */
byte* BigTabler::get (string username, string file_name) {
    FileMeta *file_meta = big_table[username][file_name];
    if (file_meta->is_deleted) {
        return NULL;
    } else if (file_meta->is_flushed) {
        // Read from the disk file
        string sstable_name = big_table[username][file_name]->sstable_name;

        // Lock the sstable
        boost::interprocess::file_lock flock;
        (sstable_name.c_str());

        FILE* f = fopen(filename.c_str(), "rb");
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