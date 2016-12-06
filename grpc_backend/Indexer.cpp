//
// Created by tianli on 12/5/16.
//

#include "Indexer.h"

/*
 * File hierarchy indexer for all users, stored as a tree
 * dir format : /usr/username/../..
 */

Indexer::Indexer() : root("usr"){

}

/*
 *  Display the files and directories under the cur_dir
 */
map<string, Node> Indexer::display(string cur_dir) {
    map<string, Node> children_files; // return variable
    vector<string> path = directory_parser(cur_dir); // parse the path into vector<string>

    Node cur_node = root;
    // traverse all the sub dir
    for (string s : path) {
        Node *temp = node_finder(s, cur_node);
        if (temp == NULL) {
            // ERROR
            return NULL;
        }
        // iterated to next level
        cur_node = *temp;
    }

    for (map<string, Node>::iterator it = cur_node.children.begin(); it != cur_node.children.end(); ++it) {
        children_files[it->first] = it->second;
    }
    return children_files;
}

/*
 *  Create/Upload a file or a directory
 */
int Indexer::insert(string new_dir, bool is_file) {
    vector<string> path = directory_parser(new_dir);
    // traverse all the sub dir
    Node cur_node = root;
    int i;
    for (i = 0; i < path.size()-1;  i++) {
        Node *temp = node_finder(path[i], cur_node);
        if (temp == NULL) {
            // ERROR
            return -1;
        }
        // iterated to next level
        cur_node = *temp;
    }

    if (cur_node.is_file) {
        // Cannot create under a file
        return -1;
    }

    cur_node.children.emplace(path[i-1], path[i], is_file);

    return 1;
}

/*
 *  Delete a file/directory
 */
int Indexer::delet(string del_dir) {
    vector<string> path = directory_parser(del_dir);

    // traverse all the sub dir
    Node cur_node = root;
    int i;
    for (i = 0; i < path.size()-1;  i++) {
        Node *temp = node_finder(path[i], cur_node);
        if (temp == NULL) {
            // ERROR
            return -1;
        }
        // iterated to next level
        cur_node = *temp;
    }

    if (cur_node.is_file) {
        // Cannot delete under a file
        return -1;
    }

    // delete a file/directory
    cur_node.children.erase(path[i]);

    return 1;
}

// helper functions*************************************************************************************************

// parser
vector<string> Indexer::directory_parser(string directory) {
    // result vector
    vector<string> result;
    // parse the pass-in arguement
    string s = directory;
    string delim = "/";
    // parsing
    auto start = 0U;
    auto end = s.find(delim);
    while (end != string::npos) {
        result.push_back(s.substr(start, end - start));
        start = end + delim.length();
        end = s.find(delim, start);
    }
    result.push_back(s.substr(start, end));
    // return
    return result;
}

// node finder
Node* Indexer::node_finder(string target_name, Node node) {
    if (node.children.count(target_name)) {
        return &node.children[target_name];
    }
    return NULL;
}