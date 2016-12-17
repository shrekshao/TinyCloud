#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <pthread.h>
#include <signal.h>

#include <sstream>
#include <iostream>
#include <fstream>
#include <time.h>

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "http_server_grpc.h"


using namespace std;


static FileSystemClient fsClient(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));







typedef void (*FunctionHandlerGet)(int fd);
typedef void (*FunctionHandlerPost)(int fd, const string & contentStr, string & threadUsername);





//#include "utils.h"

static bool outputAuthor = false;
static bool outputDebug = false;
static int port = 10000;

static const int BUFFER_SIZE = 50;
// unordered_set<int> fdSet;
static const int NUM_FD = 1000; 
static bool fdArray[NUM_FD] = {};  // all elements 0


enum ReceivingStatus{
    waiting_request,
    reading_request_get,
    reading_request_post,
    reading_request_header,
    reading_content,
    reading_content_multipart
};

// ----------------- const status msg ----------------
static const string CRLF = "\r\n";
static const string LF = "\n";
static const string HTTP_OK = "HTTP/1.1 200 OK\r\n";
static const string HTTP_400 = "HTTP/1.1 400 BAD REQUEST\r\n";
static const string HTTP_400_invalid_username = "HTTP/1.1 400 INVALID USERNAME OR PASSWORD\r\n";
static const string HTTP_404 = "HTTP/1.1 404 NOT FOUND\r\n";
static const string HTTP_HEADER_SERVER = "Server: tinycloud\r\n";

static const string HTML_400_PAGE = "400 Bad Request\r\nWrong request URL\r\n";
static const string HTML_404_PAGE = "404 Not Found\r\nThe requested URL was not found on this server.\r\n";


static const string siteRoot = "_site/";



static const unordered_set<string> textFileExtensions ({"txt", "html", "css", "js"});
static const unordered_map<string, string> extToFileCat({
    {"jpg", "image"}
    ,{"jpeg", "image"}
    ,{"png", "image"}
    ,{"gif", "image"}
    ,{"bmp", "image"}

    ,{"pdf", "application"}
    ,{"ppt", "application"}
    ,{"pptx", "application"}
});




#define HttpDebugLog(fd, a...) do { \
if (outputDebug) {\
struct timeval tv; \
fprintf(stdout, "[%d] ", fd); \
fprintf(stdout, a); \
fprintf(stdout, "\n"); } } while(0)

// TODO: http struct header

bool do_write(int fd, const char *buf, int len);

struct GeneralHeader
{
    
    string content_type = "";
    int content_length = 0;

    string set_cookie = "";



    void clear()
    {
        content_type = "";
        content_length = 0;
        set_cookie = "";
    }

    void send(int fd)
    {
        // TODO: date

        do_write(fd, &HTTP_HEADER_SERVER.at(0), (int)HTTP_HEADER_SERVER.size());


        if (set_cookie != "")
        {
            string setCookieStr = "Set-Cookie: " + set_cookie + "\r\n";
            do_write(fd, &setCookieStr.at(0), setCookieStr.size());
        }

        if (content_type != "")
        {
            string contentTypeStr = "Content-Type: " + content_type + "\r\n";
            do_write(fd, &contentTypeStr.at(0), contentTypeStr.size());
        }
        
        if (content_length != 0)
        {
            string contentLengthStr = "Content-Length: " + to_string(content_length) + "\r\n";
            do_write(fd, &contentLengthStr.at(0), contentLengthStr.size());
        }
        
    }
};

GeneralHeader header;

// uri, function handler
// unordered_map<string, FunctionHandlerPost> uri_to_handlers;





void printDebugMessage(int fd, const string & msg)
{
  //stderr << "[" << fd << "]"
  if (outputDebug)
  {
    fprintf(stderr, "[%d] %s", fd, msg.c_str()); 
  }
}



bool do_write(int fd, const char *buf, int len)
{
    int sent = 0;
    while (sent < len)
    {
      // if (outputDebug)
      // {
      //   fprintf(stderr, "[%d] %s", fd, buf); 
      // }
        int n = write(fd, &buf[sent], len - sent);
        if (n < 0)
        {
            cerr << "write message error\n";
            return false;
        }
        sent += n;
    }
    return true;
}



// std::istream& safeGetline(std::istream& is, std::string& t)
// {
//     t.clear();

//     // The characters in the stream are read one-by-one using a std::streambuf.
//     // That is faster than reading them one-by-one using the std::istream.
//     // Code that uses streambuf this way must be guarded by a sentry object.
//     // The sentry object performs various tasks,
//     // such as thread synchronization and updating the stream state.

//     std::istream::sentry se(is, true);
//     std::streambuf* sb = is.rdbuf();

//     for(;;) {
//         int c = sb->sbumpc();
//         switch (c) {
//         case '\n':
//             return is;
//         case '\r':
//             if(sb->sgetc() == '\n')
//                 sb->sbumpc();
//             return is;
//         case EOF:
//             // Also handle the case when the last line has no line ending
//             if(t.empty())
//                 is.setstate(std::ios::eofbit);
//             return is;
//         default:
//             t += (char)c;
//         }
//     }
// }


void send404Page(int fd)
{
    // 404
    do_write(fd, &HTTP_404.at(0), (int)HTTP_404.size());
    // GeneralHeader header404 = {"text/html", (int)HTML_404_PAGE.size()};
    GeneralHeader header404;
    header404.content_type = "text/html";
    header404.content_length = HTML_404_PAGE.size();
    header404.send(fd);
    // separate line
    do_write(fd, &CRLF.at(0), CRLF.size());

    do_write(fd, &HTML_404_PAGE.at(0), HTML_404_PAGE.size());
}

void send400Page(int fd)
{
    // 400
    do_write(fd, &HTTP_400.at(0), (int)HTTP_400.size());
    GeneralHeader header400;
    header400.content_type = "text/html";
    header400.content_length = HTML_400_PAGE.size();
    header400.send(fd);
    // separate line
    do_write(fd, &CRLF.at(0), CRLF.size());

    do_write(fd, &HTML_400_PAGE.at(0), HTML_400_PAGE.size());
}

void sendData(int fd, const string & dataType, const string & data)
{
    // HttpDebugLog( fd, "get file list: %s", folder.c_str());

    do_write(fd, &HTTP_OK.at(0), (int)HTTP_OK.size());

    GeneralHeader header200Data;
    header200Data.content_type = dataType;
    header200Data.content_length = data.size();
    header200Data.send(fd);
    // separate line
    do_write(fd, &CRLF.at(0), CRLF.size());

    do_write(fd, &data.at(0), data.size());
}


void sendTextFile(int fd, const string & filename, const string & extensionName)
{
    ifstream in;
    in.open(siteRoot + filename);

    if (!in.is_open())
    {
        // 404
        printDebugMessage(fd, filename + " 404\n");
        send404Page(fd);
        return;
    }

    // http 200 ok
    do_write(fd, &HTTP_OK.at(0), HTTP_OK.size());

    in.seekg(0, in.end);
    int length = in.tellg();
    in.seekg(0, in.beg);

    string content_type = "text/" + extensionName;
    // GeneralHeader header = {content_type, length};
    header.content_type = content_type;
    header.content_length = length;
    header.send(fd);

    // separate line
    do_write(fd, &CRLF.at(0), CRLF.size());

    // content

    string line;
    while(getline(in, line))
    {
        line += "\n";
        do_write(fd, &line.at(0), line.size());
    }

    in.close();
}

void sendBinaryFile(int fd, const string &filename, const string & extensionName)
{
    ifstream in;
    in.open(siteRoot + filename, ios::binary);

    if (!in.is_open())
    {
        // 404
        printDebugMessage(fd, filename + " 404\n");
        send404Page(fd);
        return;
    }

    // http 200 ok
    do_write(fd, &HTTP_OK.at(0), HTTP_OK.size());

    in.seekg(0, in.end);
    int length = in.tellg();
    in.seekg(0, in.beg);


    vector<char> buffer((
            istreambuf_iterator<char>(in)), 
            (istreambuf_iterator<char>()));

    in.close();

    string file_cat = extToFileCat.find(extensionName)->second;
    string content_type = file_cat + "/" + extensionName;
    // GeneralHeader header = {content_type, length};
    header.content_type = content_type;
    header.content_length = length;
    header.send(fd);

    // separate line
    do_write(fd, &CRLF.at(0), CRLF.size());

    // content

    do_write(fd, &buffer.at(0), buffer.size());

}

void sendFileToClient(int fd, const string & uri)
{
    string filename;
    string extensionName;
    if (uri == "/")
    {
        filename = "index.html";
    }
    else if (uri.at(0) =='/')
    {
        // TODO: add custom site root
        filename = uri.substr(1);
    }

    size_t dotPos = filename.find_last_of('.');
    
    if (dotPos == string::npos)
    {
        filename += ".html";
        dotPos = filename.find_last_of('.');
    }

    extensionName = filename.substr(dotPos + 1);


    if (textFileExtensions.find(extensionName) != textFileExtensions.end())
    {
        // text file
        sendTextFile(fd, filename, extensionName);
    }
    else
    {
        // binary file
        sendBinaryFile(fd, filename, extensionName);
    }
}



// // --------- get request handlers --------------
// void getFileListHandler(int fd)
// {

// }
// // {uri, FunctionHandlerGet}
// static const unordered_map<string, FunctionHandlerGet> getRequestHandlers({
//     {"/drive-get-list", &getFileListHandler}
// });


// void handleGetRequest(int fd, const string & uri)
// {
//     auto it = getRequestHandlers.find(uri);

//     if (it == getRequestHandlers.end())
//     {
//         // pure get file
//         sendFileToClient(fd, uri);
//     }
//     else
//     {
//         // functional url
//         (*(it->second))(fd);
//     }
// }





/**
* @ return: if username and password is valid
*/
bool verifyUsernameAndPassword(const string & username, const string & password)
{
    // temp test
    // TODO: communicate with big table
    return 
        (username == "ss") && (password == "123")
        || (username == "tianli") && (password == "123");
}








// ------------ post request handlers -------------

void loginHandler(int fd, const string & contentStr, string & threadUsername)
{
    istringstream iss_content(contentStr);
    string username, password, tmp;
    getline(iss_content, tmp, '='); // username
    getline(iss_content, username, '&');
    getline(iss_content, tmp, '='); // password
    getline(iss_content, password, '&');

    HttpDebugLog( fd, "%s, %s", username.c_str(), password.c_str());

    if (verifyUsernameAndPassword(username, password))
    {
        header.clear();
        header.set_cookie = "username=" + username;

        threadUsername = username;


        // direct to new page
        sendFileToClient(fd, "/profile.html");
    }
    else
    {
        // TODO: invalid username or password
        HttpDebugLog( fd, "Invalid %s %s", username.c_str(), password.c_str());

        // do_write(fd, &HTTP_400_invalid_username.at(0), (int)HTTP_400_invalid_username.size());
        // GeneralHeader header400;
        // header400.content_type = "text/html";
        // header400.content_length = HTML_400_PAGE.size();
        // separate line
        // do_write(fd, &CRLF.at(0), CRLF.size());

        header.clear();
        sendFileToClient(fd, "/index-login");


        // do_write(fd, &HTML_400_PAGE.at(0), HTML_400_PAGE.size());
    }
}

void registerHandler(int fd, const string & contentStr, string & threadUsername)
{
    istringstream iss_content(contentStr);
    string username, password, tmp;
    getline(iss_content, tmp, '='); // username
    getline(iss_content, username, '&');
    getline(iss_content, tmp, '='); // password
    getline(iss_content, password, '&');
    // confirm password

    HttpDebugLog( fd, "Register New User: %s, %s", username.c_str(), password.c_str());

    header.clear();
    sendFileToClient(fd, "/");
}




void getFilelistHandler(int fd, const string & folder, string & threadUsername)
{
    // Current for test
    // TODO: grpc ask for file list

    // cerr << "\n\n" << folder << "\n\n";

    HttpDebugLog( fd, "get file list: %s %s", threadUsername.c_str(), folder.c_str());

    string path;
    // if (folder.back() == '/')
    if (folder == "/")
    {
        path = "/" + threadUsername;
    }
    else
    {
        // path is now absolute full path with username
        path = folder;
    }

    HttpDebugLog( fd, "request path: %s", path.c_str());
    // cerr << "\n\npath: " << path << "\n"; 

    // temp test
    
    ostringstream oss;

    oss << "{";

    std::map<std::string, FileInfo> fileList;
    if (fsClient.GetFileList(path, fileList))
    {
        bool isFirst = true;
        for (const auto & f : fileList)
        {
            if (isFirst)
            {
                isFirst = false;
            }
            else
            {
                oss << ",";
            }

            // FileInfo.full_path  /tianli/path/to
            // FileInfo.name (relative) == first (key) except for key=..


            HttpDebugLog( fd, "<%s , (%s, %d) >"
                , f.second.full_path().c_str(), f.first.c_str(), f.second.is_file());

            oss << "\"";
            // oss << f.first;
            oss << f.second.full_path();
            oss << "\"";
            oss << ":";

            oss << "{";

            oss << "\"name\":";

            oss << "\"";
            // oss << f.second.name();
            oss << f.first;
            oss << "\"";

            oss << ",";
            
            oss << "\"folder\":";

            oss << ( !f.second.is_file() ) ? "1" : "0";
            
            oss << "}";

        }
    }
    else
    {
        HttpDebugLog( fd, "get file list fail");
    }

    oss << "}";


    string file_list_json = oss.str();



    // file_list_json += "{";

    // if (folder == "/")
    // {
    //     // file-key , display info
    //     file_list_json += "\"av\"";
    //     file_list_json += ":";
    //     file_list_json += "{\"name\":\"av\", \"date\":\"12-01-2015\", \"folder\":1}";

    //     file_list_json += ",";
    //     file_list_json += "\"download_test/cis505project.pdf\"";
    //     file_list_json += ":";
    //     file_list_json += "{\"name\":\"cis505project.pdf\", \"date\":\"12-05-2016\"}";

    //     file_list_json += ",";
    //     file_list_json += "\"download_test/lecture18.pptx\"";
    //     file_list_json += ":";
    //     file_list_json += "{\"name\":\"lecture18.pptx\", \"date\":\"12-01-2016\"}";

    //     file_list_json += ",";
    //     file_list_json += "\"download_test/profile.html\"";
    //     file_list_json += ":";
    //     file_list_json += "{\"name\":\"profile.html\", \"date\":\"12-19-2016\"}";
    // }
    // else if (folder == "av")
    // {
    //     file_list_json += "\"/\"";
    //     file_list_json += ":";
    //     file_list_json += "{\"name\":\"..\", \"date\":\"10-01-2015\", \"folder\":1}";

    //     file_list_json += ",";
    //     file_list_json += "\"download_test/di_pose.png\"";
    //     file_list_json += ":";
    //     file_list_json += "{\"name\":\"di_pose.png\", \"date\":\"11-22-2015\"}";
    // }

    // file_list_json += "}";


    HttpDebugLog( fd, "send file list json:\n %s", file_list_json.c_str());

    // header.clear();
    sendData(fd, "application/json", file_list_json);
    // sendData(fd, "json", file_list_json);
    // sendData(fd, "text", file_list_json);
}




void insertFolderHandler(int fd, const string & contentStr, string & threadUsername)
{
    istringstream iss_content(contentStr);
    string curFolder, foldername;
    getline(iss_content, curFolder, '&');
    getline(iss_content, foldername, '&');


    // string fullPathFolder = "/" + threadUsername; // with user name
    string fullPathFolder;

    if (curFolder == "/")
    {
        fullPathFolder = "/" + threadUsername + "/" + foldername;
        // curFolder += threadUsername;
    }
    else
    {
        fullPathFolder = curFolder + "/" + foldername;
    }

    HttpDebugLog( fd, "insert full path: %s", fullPathFolder.c_str());

    fsClient.InsertFolder(fullPathFolder);

    // need to parse from folder (content) sent


    HttpDebugLog( fd, "cur (parent) full path, get file list : %s", curFolder.c_str());

    getFilelistHandler(fd, curFolder, threadUsername);
}

void uploadFileToStorage(const string & threadUsername, const string & curFolder, const string & filename, const string & data)
{

    string fullPathFolder;

    if (curFolder == "/")
    {
        fullPathFolder = "/" + threadUsername + "/" + filename;
        // curFolder += threadUsername;
    }
    else
    {
        fullPathFolder = curFolder + "/" + filename;
    }

    HttpDebugLog( 999, "fullPathFolder: %s, filename: %s", fullPathFolder.c_str(), filename.c_str());
    fsClient.UploadFile(threadUsername, fullPathFolder, data);
}




// {uri, FunctionHandlerPost}
static const unordered_map<string, FunctionHandlerPost> postRequestHandlers({
    {"/", &loginHandler}
    , {"/register", &registerHandler}
    // , {"/drive", &uploadFileHandler}
    , {"/get-file-list", &getFilelistHandler}

    , {"/insert-folder", &insertFolderHandler}
});


void handlePostRequest(int fd, const string & uri, const string & contentStr, string & threadUsername)
{
    auto it = postRequestHandlers.find(uri);
    if (it == postRequestHandlers.end())
    {
        // 400 not valid uri post
        HttpDebugLog( fd, "Invalid post url request: %s", uri.c_str());
        send400Page(fd);
    }
    else
    {
        (*(it->second))(fd, contentStr, threadUsername);
    }
}








void uploadFileHandler(int fd, const string & contentStr, const string & boundary, const string & threadUsername)
{
    // TODO: where to put folder info? 
    HttpDebugLog( fd, "upload file handler");

    // string boundary_end = boundary + "--";
    size_t boundary_size = boundary.size();
    // size_t boundary_end_size = boundary_end.size();


    size_t cur_part_start = 0;
    size_t cur_part_end = -boundary_size;

    vector<string> parts;



    cur_part_start = contentStr.find(boundary, cur_part_end + boundary_size);

    // if (cur_part_start == string::npos)
    // {
    //     HttpDebugLog( fd, "end spliting multipart content");
    //     break;
    // }

    do
    {
        
        cur_part_end = contentStr.find(boundary, cur_part_start + boundary_size);

        if (cur_part_end == string::npos)
        {
            HttpDebugLog( fd, "no more boundary!!!");
            break;
        }

        parts.emplace_back(contentStr.substr(cur_part_start + boundary_size, cur_part_end - cur_part_start - boundary_size));
        cur_part_start = cur_part_end;
    }
    while (cur_part_end != string::npos);
    
    

    const string separation_line = "\r\n\r\n"; 
    const string form_input_name = "; name=\"";
    const string form_filename = "; filename=\"";


    string filename_value;
    string file_bytes;
    // for (const string & str : parts)
    {
        // hardcode: file
        const string & str = parts[0];

        auto p_sep = str.find(separation_line);

        string headers = str.substr(0, p_sep - 0);
        string file_bytes = str.substr(p_sep + separation_line.size());

        // HttpDebugLog( fd, "*******headers:\n%s\nbytes:\n%s", headers.c_str(), file_bytes.c_str());

        // hardcode header part
        auto p_name_start = headers.find(form_input_name) + form_input_name.size();
        auto p_name_end = headers.find( "\"", p_name_start );
        

        string name_value = headers.substr(p_name_start, p_name_end - p_name_start);
        HttpDebugLog( fd, "name=%s", name_value.c_str());
        // auto p_name = header.find(form_)

        // assume first is file
        auto p_filename_start = headers.find(form_filename) + form_filename.size();
        auto p_filename_end = headers.find( "\"", p_filename_start );

        filename_value = headers.substr(p_filename_start, p_filename_end - p_filename_start);
        HttpDebugLog( fd, "filename=%s", filename_value.c_str());
        // HttpDebugLog( fd, "%d %d ", filename_value.c_str());

    }

    HttpDebugLog( fd, "parts.size() = %d", (int)parts.size());

    string curFolder;
    {
        //hardcode: cur-folder
        const string & str = parts[1];

        auto p_sep = str.find(separation_line);

        string headers = str.substr(0, p_sep - 0);
        curFolder = str.substr(p_sep + separation_line.size());

        // hardcode header part
        auto p_name_start = headers.find(form_input_name) + form_input_name.size();
        auto p_name_end = headers.find( "\"", p_name_start );

        string name_value = headers.substr(p_name_start, p_name_end - p_name_start);
        HttpDebugLog( fd, "name=%s", name_value.c_str());
    }



    // upload file call rpc
    uploadFileToStorage(threadUsername, curFolder, filename_value, file_bytes);



    // refresh page
    // TODO: ajax refresh
    sendFileToClient(fd, "/drive.html");
}