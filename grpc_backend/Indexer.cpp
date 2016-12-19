//
// Created by tianli on 12/5/16.
//

#include "Indexer.h"

/*
 * File hierarchy indexer for all users, stored as a tree
 * dir format : /usr/username/../..
 */

Indexer::Indexer() : root("usr", "usr"){

}

/*
 *  Display the files and directories under the cur_dir
 */
int Indexer::display(string cur_dir, map<string, Node> &res) {
    boost::filesystem::path p1(cur_dir);

    // traverse all the sub dir
    Node* cur_node = &root;
    Node* prev_node;
    for (auto const& element : p1) {
        if (element.string().compare("/") == 0) {
            continue;
        }
        Node* temp = node_finder(element.string(), cur_node);
        if (temp == NULL) {
            // ERROR
            fprintf(stderr, "Folder does not exist!\n");
            return -1;
        }
        // iterated to next level
        prev_node = cur_node;
        cur_node = temp;
    }

    if (cur_node->is_file) {
        fprintf(stderr, "Cannot display file list under a file!\n");
        return -1;
    }

    for (map<string, Node>::iterator it = cur_node->children.begin(); it != cur_node->children.end(); ++it) {
        res.emplace(it->first, it->second);
    }
    if (prev_node->full_name.compare("usr") != 0) {
        res.emplace("..", *prev_node);
    }

    return 1;
}

/*
 *  Check if there is such a file and if it is a file or dir
 */
pair<int, bool> Indexer::checkIsFile(string cur_dir) {
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
            return make_pair(-1, false);
        }
        // iterated to next level
        cur_node = temp;
    }

    if (cur_node->is_file) {
        return make_pair(1, true);
    } else {
        return make_pair(1, false);
    }
}

/*
 * Find all the files under a directory
 * return: 1    success
 *         -1   fail
 */
int Indexer::findAllChildren(string dir, vector<string>& res) {
    boost::filesystem::path p1(dir);

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

    return findAllChildrenHelper(cur_node, res);

}

/*
 * Helper function for findAllChildren
 * return: 1    success
 *         -1   faile
 */
int Indexer::findAllChildrenHelper(Node* cur_node, vector<string>& res) {
    int response = 1;
    for (map<string, Node>::iterator it = cur_node->children.begin(); it != cur_node->children.end(); ++it) {
        if (it->second.is_file) {
            res.push_back(it->second.full_name);
        } else {
            response = (findAllChildrenHelper(&(it->second), res) == 1 && response == 1) ? 1 : -1;
        }
    }
    return response;
}

/*
 *  Create/Upload a file or a directory
 */
int Indexer::insert(string new_dir, bool is_file) {

    fprintf(stderr, "Insert: %s\n", new_dir.c_str());

    boost::filesystem::path p1(new_dir);

    // traverse all the sub dir
    Node *cur_node = &root;
    for (auto const &element : p1.parent_path()) {
        if (element.string().compare("/") == 0) {
            continue;
        }
        Node *temp = node_finder(element.string(), cur_node);
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

    if (new_dir.find("/") == 0) {
        cur_node->children.emplace(piecewise_construct, forward_as_tuple( p1.filename().string()), forward_as_tuple(p1.filename().string(), new_dir.substr(1, new_dir.length()-1), is_file));
    } else {
        cur_node->children.emplace(piecewise_construct, forward_as_tuple(p1.filename().string()), forward_as_tuple(p1.filename().string(), new_dir, is_file));
    }

    return 1;
}

/*
 *  Delete a file/directory in the hierarchy structure
 */
int Indexer::delet(string del_dir) {
    fprintf(stderr, "Delet: %s\n", del_dir.c_str());

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