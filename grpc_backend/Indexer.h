//
// Created by tianli on 12/5/16.
//

#ifndef TINYCLOUD_STORAGE_SERVICE_H
#define TINYCLOUD_STORAGE_SERVICE_H

#include <iostream>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <map>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

using namespace std;

/*
 * Node object to store directories and files for Indexer when requiring user's file hierarchy
 */
class Node {
public:
    string key;
    string filename;
    bool is_file;
    map<string, Node> children;
    Node(string key, string filename, bool is_file = false) {
        this->key = key;
        this->filename = filename;
        this->is_file = is_file; // Default to be directory
    }
};

class Indexer {
    Node root;
    Node* node_finder(string target_name, Node* node);
public:
    Indexer();
    int display(string cur_dir, map<string, Node> &res);
    int insert(string new_dir, bool is_file = false);
    int delet(string del_dir);
    pair<int, bool> checkIsFile(string cur_dir);
    int findAllChildren(string dir, vector<string>& res);
    int findAllChildrenHelper(Node* cur_node, vector<string>& res);
};



#endif //TINYCLOUD_STORAGE_SERVICE_H
