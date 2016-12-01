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
    reading_content
};

// ----------------- const status msg ----------------
const string CRLF = "\r\n";
const string LF = "\n";
const string HTTP_OK = "HTTP/1.1 200 OK\r\n";
const string HTTP_404 = "HTTP/1.1 404 NOT_FOUND\r\n";
const string HTTP_HEADER_SERVER = "Server: tinycloud\r\n";

const string HTML_404_PAGE = "404 Not Found\r\nThe requested URL /t.html was not found on this server.\r\n";


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


// TODO: http struct header

bool do_write(int fd, const char *buf, int len);

struct GeneralHeader
{
    
    string content_type;
    int content_length;

    void send(int fd)
    {
        // TODO: date

        do_write(fd, &HTTP_HEADER_SERVER.at(0), (int)HTTP_HEADER_SERVER.size());

        string contentTypeStr = "Content-Type: " + content_type + "\r\n";
        do_write(fd, &contentTypeStr.at(0), contentTypeStr.size());

        string contentLengthStr = "Content-Length: " + to_string(content_length) + "\r\n";
        do_write(fd, &contentLengthStr.at(0), contentLengthStr.size());
    }
};


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





void sendTextFile(int fd, const string & filename, const string & extensionName)
{
    ifstream in;
    in.open(siteRoot + filename);

    if (!in.is_open())
    {
        // 404
        printDebugMessage(fd, filename + " 404\n");
        do_write(fd, &HTTP_404.at(0), (int)HTTP_404.size());
        GeneralHeader header = {"text/html", (int)HTML_404_PAGE.size()};
        header.send(fd);
        // separate line
        do_write(fd, &CRLF.at(0), CRLF.size());

        do_write(fd, &HTML_404_PAGE.at(0), HTML_404_PAGE.size());

        return;
    }

    // http 200 ok
    do_write(fd, &HTTP_OK.at(0), HTTP_OK.size());

    in.seekg(0, in.end);
    int length = in.tellg();
    in.seekg(0, in.beg);

    string content_type = "text/" + extensionName;
    GeneralHeader header = {content_type, length};
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
        do_write(fd, &HTTP_404.at(0), (int)HTTP_404.size());
        GeneralHeader header = {"text/html", (int)HTML_404_PAGE.size()};
        header.send(fd);
        // separate line
        do_write(fd, &CRLF.at(0), CRLF.size());

        do_write(fd, &HTML_404_PAGE.at(0), HTML_404_PAGE.size());

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
    GeneralHeader header = {content_type, length};
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