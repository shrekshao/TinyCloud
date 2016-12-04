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


using namespace std;

typedef void (*FunctionHandler)(int, const string &);





//#include "utils.h"

bool outputAuthor = false;
bool outputDebug = false;
int port = 10000;

const int BUFFER_SIZE = 50;
// unordered_set<int> fdSet;
const int NUM_FD = 1000; 
bool fdArray[NUM_FD] = {};  // all elements 0


enum ReceivingStatus{
    waiting_request,
    reading_request_get,
    reading_request_post,
    reading_request_header,
    reading_content
};

// ----------------- const status msg ----------------
const string CRLF = "\r\n";
const string LF = "\n";
const string HTTP_OK = "HTTP/1.1 200 OK\r\n";
const string HTTP_400 = "HTTP/1.1 400 BAD REQUEST\r\n";
const string HTTP_400_invalid_username = "HTTP/1.1 400 INVALID USERNAME OR PASSWORD\r\n";
const string HTTP_404 = "HTTP/1.1 404 NOT FOUND\r\n";
const string HTTP_HEADER_SERVER = "Server: tinycloud\r\n";

const string HTML_400_PAGE = "400 Bad Request\r\nWrong request URL\r\n";
const string HTML_404_PAGE = "404 Not Found\r\nThe requested URL was not found on this server.\r\n";


const string siteRoot = "_site/";



const unordered_set<string> textFileExtensions ({"txt", "html", "css", "js"});
const unordered_map<string, string> extToFileCat({
    {"jpg", "image"}
    ,{"jpeg", "image"}
    ,{"png", "image"}
    ,{"gif", "image"}
    ,{"bmp", "image"}

    ,{"pdf", "application"}
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
// unordered_map<string, FunctionHandler> uri_to_handlers;





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
    // separate line
    do_write(fd, &CRLF.at(0), CRLF.size());

    do_write(fd, &HTML_400_PAGE.at(0), HTML_400_PAGE.size());
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


/**
* @ return: if username and password is valid
*/
bool verifyUsernameAndPassword(const string & username, const string & password)
{
    // temp test
    // TODO: communicate with big table
    return (username == "ss") && (password == "123");
}








// ------------ post request handlers -------------

void loginHandler(int fd, const string & contentStr)
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




// {uri, FunctionHandler}
const unordered_map<string, FunctionHandler> postRequestHandlers({
    {"/", &loginHandler}
});