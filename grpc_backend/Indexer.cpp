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
int Indexer::display(string cur_dir, map<string, Node> &res) {
    boost::filesystem::path p1(cur_dir);

    // traverse all the sub dir
    Node* cur_node = &root;
    for (auto const& element : p1) {
        if (element.string().compare("/") == 0) {
            continue;
        }
        Node* temp = node_finder(element.string(), cur_node);
        if (temp == NULL) {
            // ERROR
            return -1;
        }
        // iterated to next level
        cur_node = temp;
    }

    for (map<string, Node>::iterator it = cur_node->children.begin(); it != cur_node->children.end(); it++) {
        res.emplace(it->first, it->second);
    }
    return 1;
}

/*
 *  Create/Upload a file or a directory
 */
int Indexer::insert(string new_dir, bool is_file) {
    boost::filesystem::path p1(new_dir);

    // traverse all the sub dir
    Node* cur_node = &root;
    for (auto const& element : p1.parent_path()) {
        if (element.string().compare("/") == 0) {
            continue;
        }
        Node* temp = node_finder(element.string(), cur_node);
        if (temp == NULL) {
            // ERROR
            return -1;
        }
        // iterated to next level
        cur_node = temp;
    }

    if (cur_node->is_file) {
        // Cannot create under a file
        return -1;
    }

    cur_node->children.emplace(p1.filename().string(), Node(p1.filename().string(), is_file));

    return 1;
}

/*
 *  Delete a file/directory in the hierarchy structure
 */

int Indexer::delet(string del_dir) {
    boost::filesystem::path p1(del_dir);

    // traverse all the sub dir
    Node* cur_node = &root;
    for (auto const& element : p1.parent_path()) {
        if (element.string().compare("/") == 0) {
            continue;
        }
        Node* temp = node_finder(element.string(), cur_node);
        if (temp == NULL) {
            // ERROR
            return -1;
        }
        // iterated to next level
        cur_node = temp;
    }

    if (cur_node->is_file) {
        // Cannot delete under a file
        return -1;
    }

    // delete a file/directory
    cur_node->children.erase(p1.filename().string());

    return 1;
}

// helper functions*************************************************************************************************

// node finder
Node* Indexer::node_finder(string target_name, Node* node) {
    auto it = node->children.find(target_name);
    if (it != node->children.end()) {
        return &(it->second);
    }
    return NULL;
}