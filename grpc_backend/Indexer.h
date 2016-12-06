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

using namespace std;

/*
 * Node object to store directories and files for Indexer when requiring user's file hierarchy
 */
class Node {
public:
    string name;
    bool is_file;
    map<string, Node*> children;
    Node(string name, bool is_file = false) {
        this->name = name;
        this->is_file = is_file; // Default to be directory
    }
};

class Indexer {
    Node root;
    vector<string> directory_parser (string directory);
    Node* node_finder(string target_name, Node *node);
public:
    Indexer();
    int display(string cur_dir, map<string, Node*> res);
    int insert(string new_dir, bool is_file = false);
    int delet(string del_dir);
};



#endif //TINYCLOUD_STORAGE_SERVICE_H
