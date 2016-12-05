//
// Created by Zhengxuan Wu on 4/12/2016.
//
#include <iostream>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <map>

using namespace std;

// node class
class Node {
public:
    string file_name;
    map<string, Node*> children;
    bool is_file;
    Node (string name) {
        file_name = name;
    }
};

// file system class
class File_System {
private:
    Node* root;
public:
    File_System () {
        root = new Node("usr");
        root->is_file = false;
    }

    // dir format : /usr/username/../..

    // search & display
    vector<string> display_directory (string username, string current_directory)
    {
        // parse out the name
        vector<string> child_filenames;
        vector<string> path = directory_parser(current_directory);
        path.insert(path.begin(), username);
        // first, we need to make sure that the user has its own folder
        // under root directory
        Node* curr_node = root;
        // traverse all the sub dir
        for (string s : path)
        {
            Node* temp = node_finder(s, curr_node);
            if (temp == NULL)
            {
                // err
                return child_filenames;
            }
            // iterated to next level
            curr_node = temp;
        }

        for (map<string, Node*>::iterator it=curr_node->children.begin(); it!=curr_node->children.end(); ++it)
            child_filenames.push_back(it->first);
        return child_filenames;
    }

    // insert
    void insert_directory (string username, string new_directory)
    {

        vector<string> path = directory_parser(new_directory);
        path.insert(path.begin(), username);
        // first, we need to make sure that the user has its own folder
        // under root directory
        Node* curr_node = root;
        // traverse all the sub dir
        for (string s : path)
        {
            cout << s + "\n";
            Node* temp = node_finder(s, curr_node);
            if (temp == NULL)
            {
                // create a new user node
                temp = new Node(s);
                temp->is_file = false;
                // insert element to the map
                curr_node->children[s] = temp;
            }
            // iterated to next level
            curr_node = temp;
        }
        // just file is the last layer is a file or not
        curr_node->is_file = is_file(new_directory);
    }

    // delete
    void delete_directory (string username, string delete_directory)
    {
        vector<string> path = directory_parser(delete_directory);
        // delete the file
    }

    // suppliments function

    // parser
    vector<string> directory_parser (string directory)
    {
        // result vector
        vector<string> result;
        // parse the pass-in arguement
        string s = directory;
        string delim = "/";
        // parsing
        auto start = 0U;
        auto end = s.find(delim);
        while (end != string::npos)
        {
            result.push_back(s.substr(start, end - start));
            start = end + delim.length();
            end = s.find(delim, start);
        }
        result.push_back(s.substr(start, end));
        // return
        return result;
    }

    // node finder
    Node* node_finder (string target_name, Node* node)
    {
        if (node->children.count(target_name))
            return node->children[target_name];
        return NULL;
    }

    // file or directory
    bool is_file (string path)
    {
        struct stat s;
        if( stat(path.c_str(),&s) == 0 )
        {
            if( s.st_mode & S_IFDIR )
                return false;
            else if( s.st_mode & S_IFREG )
                return true;
            else
                return false;
        }
        else
            return false;
    }

};

int main(int argc, char *argv[]) {
    // create a root "/usr" as a the root node
    // root : usr, is_file : false
    File_System* file_system = new File_System();
    file_system->insert_directory("wuzhengx", "file10/file20/index.html");
    file_system->insert_directory("wuzhengx", "file10/file21/index.html");
    file_system->insert_directory("wuzhengx", "file10/file22/index.html");
    file_system->insert_directory("wuzhengx", "file10/file23/index.html");

    vector<string> result = file_system->display_directory("wuzhengx", "file10");
    for (string s : result)
    {
        cout << s + "\n";
    }

}
