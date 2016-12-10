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
    file_id = 1;
}

/*
 * Insert a file to the system, should have a lock, only one call at a time
 * return: 1    success
 *         -1   fail
 */
int BigTabler::put (string username, string file_name, unsigned char file_content[], string file_type, unsigned int file_size) {
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

        // write the current buffer and the file to the disk first
        string file_path = server_id + "/" + to_string(file_id);

        ofstream outfile(file_path);
        outfile.write((char*) &memtable[0], cur_pt);
        outfile.write((char*) &file_content[0], file_size);
        outfile.close();
        // put the file metainfo in big table

        big_table.emplace(piecewise_construct, forward_as_tuple(username), forward_as_tuple());
        big_table.at(username).emplace(piecewise_construct, forward_as_tuple(file_name), forward_as_tuple(cur_pt, file_size, file_name, file_type, to_string(file_id), true, false));

        // Change the memtable file_meta
        for(vector<string>::iterator it = memtable_file.begin(); it != memtable_file.end(); ++it) {
            big_table.at(username).at(*it).sstable_name = to_string(file_id);
            big_table.at(username).at(*it).is_flushed = true;
        }
        memtable_file.clear();

        // Clear buffer
        memset(&memtable[0], 0, cur_pt);
        cur_pt = 0;

        file_id++;
    } else {
        // write the the new file to the buffer
        for (int i = 0; i < file_size; i++) {
            memtable[cur_pt + i] = file_content[i];
        }
        cur_pt += file_size;

        // put the file metainfo in big table
        big_table.emplace(piecewise_construct, forward_as_tuple(username), forward_as_tuple());
        big_table.at(username).emplace(piecewise_construct, forward_as_tuple(file_name), forward_as_tuple(cur_pt, file_size, file_name, file_type, string(), false, false));

        memtable_file.emplace(memtable_file.end(), file_name);
    }
}

/*
 * Search for a file, return in res
 * return: file size written >= 0    success
 *         -1   fail
 */
int BigTabler::get (string username, string file_name, unsigned char* res, unsigned int res_size) {
    FileMeta file_meta = big_table.at(username).at(file_name);

    if (file_meta.is_deleted) {
        return -1;
    } else if (file_meta.is_flushed) {
        // Read from the disk sstable
        string sstable_name = big_table.at(username).at(file_name).sstable_name;

        // Sharable lock the sstable
        boost::interprocess::file_lock flock(sstable_name.c_str());
        boost::interprocess::sharable_lock<boost::interprocess::file_lock> sh_lock(flock);

        int fd = open(sstable_name.c_str(), O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "open");
            return -1;
        }

        struct stat sb;
        if (fstat(fd, &sb) == -1) {
            /* To obtain file size */
            fprintf(stderr, "fstat");
            return -1;
        }

        off_t pa_offset = file_meta.buffer_start & ~(sysconf(_SC_PAGE_SIZE) - 1); /* offset for mmap() must be page aligned */
        if (file_meta.buffer_start >= sb.st_size) {
            fprintf(stderr, "offset is past end of file\n");
            return -1;
        }

        int length = file_meta.file_length;
        if (file_meta.buffer_start + length > sb.st_size) {
            length = sb.st_size - file_meta.buffer_start; /* Can't display bytes past end of file */
        }

        unsigned char *addr = (unsigned char *) mmap(NULL, length + file_meta.buffer_start - pa_offset, PROT_READ, MAP_PRIVATE, fd, pa_offset);
        if (addr == MAP_FAILED) {
            fprintf(stderr, "mmap");
            return -1;
        }

        int len = min(file_meta.file_length, res_size);
        for (int i = 0; i < len; i++) {
            res[i] = *(addr + file_meta.buffer_start - pa_offset + i);
        }

        munmap(addr, length + file_meta.buffer_start - pa_offset);
        close(fd);

        return len;
    } else {
        // Read from the memtable
        int len = min(file_meta.file_length, res_size);
        for (int i = 0; i < len; i++) {
            res[i] = memtable[file_meta.buffer_start+i];
        }
        return len;
    }
}
