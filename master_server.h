//
// Created by Frank Wu on 12/12/2016.
//

#include <string>

#ifndef FILE_STORAGE_FINAL_PROJ_MASTER_SERVER_H
#define FILE_STORAGE_FINAL_PROJ_MASTER_SERVER_H


class master_server {
public:
    master_server();
private:
    string get_user_node(string username) const;
};


#endif //FILE_STORAGE_FINAL_PROJ_MASTER_SERVER_H
