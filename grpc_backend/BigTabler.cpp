//
// Created by tianli on 12/7/16.
//

#include <queue>
#include "BigTabler.h"

// constructor
BigTabler::BigTabler (string s) {
    boost::filesystem::path dir(s);
    if (!boost::filesystem::exists(dir)) {
        if(boost::filesystem::create_directory(dir)) {
            std::cout << "Create server folder " << s << "\n";
        } else {
            std::cout << "Create server folder " << s << " failed!\n";
        }
    } else {
        std::cout << "Server folder " << s << " already exists!\n";
    }
    cur_pt = 0;
    server_id = s;
    file_id = 1;
    deleted_files_mutex.emplace(piecewise_construct, forward_as_tuple(to_string(file_id)), forward_as_tuple());
    // Add new delete vector for this sstable
    deleted_files.emplace(piecewise_construct, forward_as_tuple(to_string(file_id)), forward_as_tuple());
}

/*
 * Create user account in bigtable under col password
 * return: 1    success
 *         -1   fail
 */
int BigTabler::createuser (string username, string password) {
    // Check if there is already a file with this name
    auto it = big_table.find(username);
    if (it == big_table.end()) {
        big_table.emplace(piecewise_construct, forward_as_tuple(username), forward_as_tuple());
        // put the file metainfo in big table
        big_table.at(username).emplace(piecewise_construct, forward_as_tuple("password"), forward_as_tuple(0, 0, username, password, "password", to_string(0), false, false));
        return 1;
    } else {
        return -1;
    }
}

/*
 * Get user password in bigtable under col password
 * return: 1    success
 *         -1   fail
 */
int BigTabler::getpassword (string username, string& res) {
    // Check if there is already a file with this name
    auto it = big_table.find(username);
    if (it == big_table.end()) {
        return -1;
    } else {
        res = big_table.at(username).at("password").file_name;
        return 1;
    }
}

/*
 * Insert a file to the system, should have a lock, only one call at a time
 * return: 1    success
 *         -1   fail
 */
int BigTabler::put (string username, string file_name, unsigned char file_content[], string file_type, unsigned int file_size) {

    cout << "User Name: " << username << "\n";
    cout << "File Name: " << file_name << "\n";
    cout << "File Size: " << file_size << "\n";
    cout << "File Type: "<< file_type << "\n";
    cout << "\n";


    //printf("cur_pt: %d\n%s\n", cur_pt, file_content);

    // Check if there is already a file with this name
    if (big_table.find(username) == big_table.end()) {
        big_table.emplace(piecewise_construct, forward_as_tuple(username), forward_as_tuple());
    } else if (big_table.at(username).find(file_name) != big_table.at(username).end()) {
        if (!big_table.at(username).at(file_name).is_deleted) {
            return -1;
        } else {
            big_table.at(username).erase(file_name);
        }
    }

    if (cur_pt + file_size >= MAX_BUFFER_SIZE) {
        // if the buffer is overflow, push to disk

        // write the current buffer and the file to the disk first
        string file_path = server_id + "/" + to_string(file_id);

        ofstream outfile(file_path);
        outfile.write((char*) &memtable[0], cur_pt);
        outfile.write((char*) &file_content[0], file_size);
        outfile.close();

        // Add new mutex for this sstable
        sstable_mutex.emplace(piecewise_construct, forward_as_tuple(to_string(file_id)), forward_as_tuple());

        // put the file metainfo in big table
        big_table.at(username).emplace(piecewise_construct, forward_as_tuple(file_name), forward_as_tuple(cur_pt, file_size, username, file_name, file_type, to_string(file_id), true, false));

        // Change the memtable file_meta
        for(vector<string>::iterator it = memtable_file.begin(); it != memtable_file.end(); ++it) {
            big_table.at(username).at(*it).is_flushed = true;
        }

        memtable_file.emplace(memtable_file.end(), username+"/"+file_name);
        sstable_indexer.emplace(to_string(file_id), memtable_file);

        memtable_file.clear();

        // Clear buffer
        memset(&memtable[0], 0, cur_pt);
        cur_pt = 0;

        file_id++;
        deleted_files_mutex.emplace(piecewise_construct, forward_as_tuple(to_string(file_id)), forward_as_tuple());
        deleted_files.emplace(piecewise_construct, forward_as_tuple(to_string(file_id)), forward_as_tuple());
    } else {
        // write the the new file to the buffer
        for (int i = 0; i < file_size; i++) {
            memtable[cur_pt + i] = file_content[i];
        }

        // put the file metainfo in big table
        big_table.at(username).emplace(piecewise_construct, forward_as_tuple(file_name), forward_as_tuple(cur_pt, file_size, username, file_name, file_type, to_string(file_id), false, false));

        memtable_file.emplace(memtable_file.end(), username+"/"+file_name);

        cur_pt += file_size;
    }

    return 1;
}

int BigTabler::put (string username, string file_name, string file_type, unsigned int file_size) {
    cout << "User Name: " << username << "\n";
    cout << "File Name: " << file_name << "\n";
    cout << "File Size: " << file_size << "\n";
    cout << "File Type: "<< file_type << "\n";
    cout << "\n";


    //printf("cur_pt: %d\n%s\n", cur_pt, file_content);

    // Check if there is already a file with this name
    if (big_table.find(username) == big_table.end()) {
        big_table.emplace(piecewise_construct, forward_as_tuple(username), forward_as_tuple());
    } else if (big_table.at(username).find(file_name) != big_table.at(username).end()) {
        if (!big_table.at(username).at(file_name).is_deleted) {
            return -1;
        } else {
            big_table.at(username).erase(file_name);
        }
    }

    if (cur_pt + file_size >= MAX_BUFFER_SIZE) {
        // if the buffer is overflow, push to disk

        // Add new mutex for this sstable
        sstable_mutex.emplace(piecewise_construct, forward_as_tuple(to_string(file_id)), forward_as_tuple());

        // put the file metainfo in big table
        big_table.at(username).emplace(piecewise_construct, forward_as_tuple(file_name), forward_as_tuple(cur_pt, file_size, username, file_name, file_type, to_string(file_id), true, false));

        // Change the memtable file_meta
        for(vector<string>::iterator it = memtable_file.begin(); it != memtable_file.end(); ++it) {
            big_table.at(username).at(*it).is_flushed = true;
        }

        memtable_file.emplace(memtable_file.end(), username+"/"+file_name);
        sstable_indexer.emplace(to_string(file_id), memtable_file);

        memtable_file.clear();

        cur_pt = 0;

        file_id++;
        deleted_files_mutex.emplace(piecewise_construct, forward_as_tuple(to_string(file_id)), forward_as_tuple());
        deleted_files.emplace(piecewise_construct, forward_as_tuple(to_string(file_id)), forward_as_tuple());
    } else {
        // write the the new file to the buffer

        // put the file metainfo in big table
        big_table.at(username).emplace(piecewise_construct, forward_as_tuple(file_name), forward_as_tuple(cur_pt, file_size, username, file_name, file_type, to_string(file_id), false, false));

        memtable_file.emplace(memtable_file.end(), username+"/"+file_name);

        cur_pt += file_size;
    }

    return 1;
}

/*
 * Update a file in the system, should have a lock, only one call at a time
 * return: 1    do update
 *         -1   no update
 */
int BigTabler::cput (string username, string file_name, unsigned char orig_file_content[], unsigned char file_content[], string file_type, unsigned int orig_file_size, unsigned int file_size) {
    /*
    cout << "User Name: " << username << "\n";
    cout << "File Name: " << file_name << "\n";
    cout << "File Size: " << file_size << "\n";
    cout << "File Type: "<< file_type << "\n";
    cout << "Current SS Buffer Length = " << (sizeof(ss_buffer)/sizeof(*ss_buffer)) << endl;
    cout << "\n";
    */
    // Check if there is already a file with this name
    if (big_table.find(username) == big_table.end()) {
        return -1;
    }

    if (big_table.at(username).find(file_name) == big_table.at(username).end()) {
        return -1;
    } else {
        if (orig_file_size != big_table.at(username).at(file_name).file_length) {
            return -1;
        } else {
            unsigned char temp[big_table.at(username).at(file_name).file_length];
            get(username, file_name, temp, big_table.at(username).at(file_name).file_length);
            for (int i = 0; i < big_table.at(username).at(file_name).file_length; i++) {
                if (orig_file_content[i] != temp[i]) {
                    return -1;
                }
            }

            delet(username, file_name);

            return put(username, file_name, file_content, file_type, file_size);
        }
    }
}

/*
 * Get file metainfo
 * return: file_meta    success
 *         NULL   fail
 */
FileMeta* BigTabler::getMeta (string username, string file_name) {
    if (big_table.find(username) == big_table.end() || big_table.at(username).find(file_name) == big_table.at(username).end()) {
        return NULL;
    }

    return &big_table.at(username).at(file_name);
}

/*
 * Search for a file, return in res
 * return: file size written >= 0    success
 *         -1   fail
 */
int BigTabler::get (string username, string file_name, unsigned char* res, unsigned int res_size) {


    cout << "User Name: " << username << "\n";
    cout << "File Name: " << file_name << "\n";
    cout << "File Size: " << res_size << "\n";
    cout << "\n";


    if (big_table.find(username) == big_table.end() || big_table.at(username).find(file_name) == big_table.at(username).end()) {
        return -1;
    }

    FileMeta *file_meta = &big_table.at(username).at(file_name);

    if (file_meta->is_deleted) {
        return -1;
    } else if (file_meta->is_flushed) {
        // Read from the disk sstable
        string sstable_name = big_table.at(username).at(file_name).sstable_name;

        // Sharable lock the sstable
        boost::shared_lock<boost::shared_mutex> lock(sstable_mutex.at(sstable_name));

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

        int start = big_table.at(username).at(file_name).buffer_start;

        off_t pa_offset = start & ~(sysconf(_SC_PAGE_SIZE) - 1); /* offset for mmap() must be page aligned */
        if (start >= sb.st_size) {
            fprintf(stderr, "offset is past end of file\n");
            return -1;
        }

        int length = big_table.at(username).at(file_name).file_length;
        if (start + length > sb.st_size) {
            length = sb.st_size - start; /* Can't display bytes past end of file */
        }

        unsigned char *addr = (unsigned char *) mmap(NULL, length + file_meta->buffer_start - pa_offset, PROT_READ, MAP_PRIVATE, fd, pa_offset);
        if (addr == MAP_FAILED) {
            fprintf(stderr, "mmap");
            return -1;
        }

        int len = min(file_meta->file_length, res_size);
        for (int i = 0; i < len; i++) {
            res[i] = *(addr + file_meta->buffer_start - pa_offset + i);
        }

        munmap(addr, length + file_meta->buffer_start - pa_offset);
        close(fd);

        return len;
    } else {
        // Read from the memtable
        int len = min(file_meta->file_length, res_size);
        for (int i = 0; i < len; i++) {
            res[i] = memtable[file_meta->buffer_start+i];
            //printf("hello %d, %c", file_meta->buffer_start+i, (char) res[i]);
        }
        return len;
    }
}

/*
 * Delete a file, only one call at a time
 * return: 1    success
 *         -1   fail
 */
int BigTabler::delet(string username, string file_name) {
    cout << "username: " << username << endl;
    cout << "file_name: " << file_name << endl;
    if (big_table.find(username) == big_table.end() || big_table.at(username).find(file_name) == big_table.at(username).end()) {
        return -1;
    }

    if (big_table.at(username).at(file_name).is_deleted) {
        return 1;
    } else {
        big_table.at(username).at(file_name).is_deleted = true;
        string sstable = big_table.at(username).at(file_name).sstable_name;
        time_t timer;
        time(&timer);
        deleted_files.at(sstable).insert(deleted_files.at(sstable).end(), make_pair(timer, big_table.at(username).at(file_name)));
        deleted_files_mutex.at(sstable).unlock();
        big_table.at(username).erase(file_name);

        return 1;
    }
}

/*
 * Garbage collection, run every *** hours
 * return: 1    success
 *         -1   error
 */
int BigTabler::gc(string& log_file) {
    for (map<string, vector<pair<time_t, FileMeta>>>::iterator it = deleted_files.begin(); it != deleted_files.end(); ++it) {
        if (it->first.compare(to_string(file_id)) != 0) {
            // Only clear sstable files, not memtable files
            if (needClear(it->second)) {
                // Exclusively lock the sstable
                boost::upgrade_lock<boost::shared_mutex> lock(sstable_mutex.at(it->first));
                boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

                log_mutex.lock();
                ofstream replica_log(log_file, ofstream::app);
                string msg("GC:"+it->first+",");
                replica_log.write(msg.c_str(), msg.size());

                deleted_files_mutex.at(it->first).lock();
                vector<pair<time_t, FileMeta>>::iterator ite = remove_if(it->second.begin(), it->second.end(), predicate());
                if (clearSSTable(it, ite, replica_log) == -1) {
                    string msg2("\n");
                    replica_log.write(msg2.c_str(), msg2.size());
                    replica_log.close();
                    log_mutex.unlock();
                    continue;
                }

                string msg2("\n");
                replica_log.write(msg2.c_str(), msg2.size());
                replica_log.close();
                log_mutex.unlock();

                // Update deleted_files
                it->second.erase(ite, it->second.end());
                deleted_files_mutex.at(it->first).unlock();
            }
        }
    }
    return 1;
}

unsigned int BigTabler::getCur_pt() {
    fprintf(stderr, "cur_pt in getCur_pt: %u\n", cur_pt);
    return cur_pt;
}

int BigTabler::getMemtable(unsigned char* result) {
    strcpy((char *) result, (char *) memtable);

    return 1;
}

void BigTabler::setMemtable(string& temp) {
    strcpy((char *) memtable, temp.c_str());
}

// Helper functions************************************************************************************************************

/*
 * Check if there is file need to be cleared from disk under a sstable
 */
bool BigTabler::needClear(vector<pair<time_t, FileMeta>> &vec) {
    time_t timer;
    time(&timer);
    for (vector<pair<time_t, FileMeta>>::iterator ite = vec.begin(); ite != vec.end(); ++ite) {
        if (timer-DELETE_BUFFER_TIME > ite->first) {
            return true;
        }
    }
    return false;
}

/*
 * Clear part of the sstable
 * return: 1    success
 *         -1   fail
 */
int BigTabler::clearSSTable(map<string, vector<pair<time_t, FileMeta>>>::iterator it, vector<pair<time_t, FileMeta>>::iterator ite, ofstream& replica_log) {
    priority_queue<pair<unsigned int, string>, vector<pair<unsigned int, string>>, Compare> minHeap;
    for (vector<pair<time_t, FileMeta>>::iterator myite = ite; myite != it->second.end(); ++myite) {
        minHeap.push(make_pair(myite->second.buffer_start, myite->second.username + "/" + myite->second.file_name));
    }

    string msg("," + minHeap.size());
    replica_log.write(msg.c_str(), msg.size());

    string file_path = server_id + "/" + it->first;
    ifstream infile(file_path, ifstream::binary);
    if (!infile.is_open()) {
        return -1;
    }
    // get length of file:
    infile.seekg (0, infile.end);
    int length = infile.tellg();
    infile.seekg (0, infile.beg);

    // allocate memory:
    unsigned char *buffer = new unsigned char[length];

    // read data as a block:
    infile.read((char*) buffer, length);

    infile.close();

    ofstream outfile(file_path + "_tmp", ofstream::binary);
    if (!outfile.is_open()) {
        return -1;
    }

    int pt = 0;
    vector<string>::iterator iter = sstable_indexer.at(it->first).begin();
    for ( ; iter != sstable_indexer.at(it->first).end(); ) {
        if ((*iter).compare(minHeap.top().second) == 0) {
            minHeap.pop();

            string msg("," + *iter);
            replica_log.write(msg.c_str(), msg.size());

            // Update sstable_indexer
            iter = sstable_indexer.at(it->first).erase(iter);
        } else {
            string username = (*iter).substr(0, (*iter).find("/"));
            string file_name = (*iter).substr((*iter).find("/")+1, (*iter).length()-(*iter).find("/")-1);
            outfile.write((char*) &buffer[big_table.at(username).at(file_name).buffer_start], big_table.at(username).at(file_name).file_length);
            big_table.at(username).at(file_name).buffer_start = pt;
            pt += big_table.at(username).at(file_name).file_length;
            ++iter;
        }
    }

    outfile.close();
    delete[] buffer;

    remove(file_path.c_str());
    rename((file_path+"_tmp").c_str(), file_path.c_str());

    return 1;
}

void BigTabler::gcLog(string sstable, vector<string> temp, int length) {
    int pt = 0;
    int i = 0;
    vector<string>::iterator iter = sstable_indexer.at(sstable).begin();
    for ( ; iter != sstable_indexer.at(sstable).end(), i < length; ) {
        if ((*iter).compare(temp[i]) == 0) {
            i++;

            // Update sstable_indexer
            iter = sstable_indexer.at(sstable).erase(iter);
        } else {
            string username = (*iter).substr(0, (*iter).find("/"));
            string file_name = (*iter).substr((*iter).find("/")+1, (*iter).length()-(*iter).find("/")-1);
            big_table.at(username).at(file_name).buffer_start = pt;
            pt += big_table.at(username).at(file_name).file_length;
            ++iter;
        }
    }

    vector<pair<time_t, FileMeta>>::iterator myite = deleted_files.at(sstable).begin();
    for ( ; myite != deleted_files.at(sstable).end(); ) {
        int flag = 0;
        for (int i = 0; i < length; i++) {
            if (temp[i].compare(myite->second.username+"/"+myite->second.file_name) == 0) {
                myite = deleted_files.at(sstable).erase(myite);
                flag = 1;
                break;
            }
        }

        if (flag == 0) {
            ++myite;
        }
    }
}

/*
void BigTabler::writeToLog(string& msg) {
    log_mutex.lock();
    ofstream replica_log(string(log_file, ofstream::app));
    replica_log.write(msg.c_str(), msg.size());
    replica_log.close();
    log_mutex.unlock();
}
 */