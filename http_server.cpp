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





string msgGreeting = "+OK Server ready (Author: Shuai Shao / sshuai)\n";
string msgQuit = "+OK Goodbye!\n";
string msgErr = "-ERR Unknown command\n";
string msgServerOff = "-ERR Server shutting down\n";


const string CRLF = "\r\n";
const string LF = "\n";
const string HTTP_OK = "HTTP/1.1 200 OK\r\n";
const string HTTP_404 = "HTTP/1.1 404 NOT_FOUND\r\n";
const string HTTP_HEADER_SERVER = "Server: tinycloud\r\n";


const string HTML_404_PAGE = "404 Not Found\r\nThe requested URL /t.html was not found on this server.\r\n";

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


unordered_map<string, string> headers_received;
unordered_map<string, string> headers_tosend;

// uri, function handler
unordered_map<string, FunctionHandler> uri_to_handlers;





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






void sendTextFile(int comm_fd, const string& uri)
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


    

    ifstream in;
    in.open(filename);

    if (!in.is_open())
    {
        // 404
        printDebugMessage(comm_fd, uri + " " + filename + " 404\n");
        do_write(comm_fd, &HTTP_404.at(0), (int)HTTP_404.size());
        GeneralHeader header = {"text/html", (int)HTML_404_PAGE.size()};
        header.send(comm_fd);
        // separate line
        do_write(comm_fd, &CRLF.at(0), CRLF.size());

        do_write(comm_fd, &HTML_404_PAGE.at(0), HTML_404_PAGE.size());

        return;
    }

    // http 200 ok
    do_write(comm_fd, &HTTP_OK.at(0), HTTP_OK.size());

    // // file related headers
    // string contentType = "Content-Type: text/" + extensionName + "\r\n";
    // do_write(comm_fd, &contentType.at(0), contentType.size());

    in.seekg(0, in.end);
    int length = in.tellg();
    in.seekg(0, in.beg);


    // string contentLength = "Content-Length: " + to_string(length) + "\r\n";
    // do_write(comm_fd, &contentLength.at(0), contentLength.size());

    string content_type = "text/" + extensionName;
    GeneralHeader header = {content_type, length};
    header.send(comm_fd);



    // separate line
    do_write(comm_fd, &CRLF.at(0), CRLF.size());

    // content
    


    string line;
    while(getline(in, line))
    {
        line += "\n";
        do_write(comm_fd, &line.at(0), line.size());
    }

    in.close();
}

enum ReceivingStatus{
    wait,
    request_get,
    request_post, 
    content
};

void* httpClientThread(void* params)
{
  // int comm_fd = *((int *)params);
  int *p = (int *)params;
  int comm_fd = *p;
  delete p;

//   printDebugMessage(comm_fd, "New connection\n");

//   do_write(comm_fd, &msgGreeting.at(0), msgGreeting.size());


  stringstream ss;

  ReceivingStatus receivingStatus = wait;

  // receive message from client
  
  // unsigned short rlen;
  // do_read(comm_fd, (char*)&rlen, sizeof(rlen));
  // rlen = ntohs(rlen);
  char buf[BUFFER_SIZE];

  while (true)
  {
    
    string line;
    getline(ss, line, '\n');

    if ( ss.eof() )
    {
      ss.clear();
      ss << line;
      int n;
    
      n = read(comm_fd, &buf[0], BUFFER_SIZE);

      if (n > 0)
      {
        buf[n] = 0;
        ss << buf;
      } 
      continue;
    } 

    

    istringstream iss(line);
    string curInput;
    iss >> curInput;

    printDebugMessage(comm_fd, "C: " + line + "\n");



    if (curInput == "GET")
    {
        string uri;
        iss >> uri;

        // test
        sendTextFile(comm_fd, uri);

    }
    else if (curInput == "POST")
    {
        
    }
    // else if (curInput == CRLF || curInput == LF)
    else if (curInput == "")
    {

    }
    else
    {
    //   // error handling
      
    //   printDebugMessage(comm_fd, "S: " + msgErr);
    //   do_write(comm_fd, &msgErr.at(0), msgErr.size());

    //   ss.clear();


        // handle headers
    }


  }
  



  pthread_exit(NULL);
}



// void renderLoginPage(int comm_fd)
// {
//   // http 200 ok
//   do_write(comm_fd, &HTTP_OK.at(0), HTTP_OK.size());

//   // headers

//   // // date
//   // char timestamp[30];
//   // time_t rawtime;
//   // time(&rawtime);
//   // struct tm *timeinfo = localtime(&rawtime);
//   // strftime(timestamp, 30, "%c", timeinfo);

//   // do_write(comm_fd, &CRLF.at(0), CRLF.size());

//   sendTextFile(comm_fd, "index.html");
// }





// Ctrl-C
void endServerHandler(int s){
  //printf("Caught signal %d\n",s);
  // cerr << "\n\n\n" << fdSet.size() << "\n\n\n";

  // for (auto it = fdSet.begin(); it != fdSet.end(); ++it)
  // {
  //   // send server close messages
  //   do_write(*it, &msgServerOff.at(0), msgServerOff.size());

  //   // close all connections
  //   //cerr << "kill " << *it << '\n';
  //   close(*it);
  // }

  for (int i = 0; i < NUM_FD; i++)
  {
    if (fdArray[i]) 
    {
      // do_write(i, &msgServerOff.at(0), msgServerOff.size());
      close(i);
    }
  }
  

  exit(1); 
}






const int NUM_CLIENT_THREADS = 10;

int main(int argc, char *argv[])
{

  // parse cmd line arguments
  
  {
    if (argc > 5) {
        cerr << "too mand command line arguments!\n";
    }

    for (int i = 1; i < argc; ++i)
    {
        if (string(argv[i]) == "-p")
        {
            if (i + 1 < argc)
            {
                //cout << argv[i + 1] << '\n';
                port = atoi(argv[++i]);
            } else {
                cerr << "-p option requires one argument\n";
                return 1;
            }
        } else if (string(argv[i]) == "-a") {
            outputAuthor = true;
        } else if (string(argv[i]) == "-v") {
            outputDebug = true;
        } else {
            cerr << "unrecognized flag\n";
        }
    }
  }

  if (outputAuthor)
  {
    fprintf(stderr, "*** Author: Shuai Shao (sshuai)\n");
  }



//   // init page uri handlers
//   // TODO: Or parse all files transmissiable
//   uri_to_handlers.emplace("/", &sendTextFile);
//   uri_to_handlers.emplace("/main.css", &sendTextFile);






  // listen ports

  int listen_fd = socket(PF_INET, SOCK_STREAM, 0);

  // bind
  struct sockaddr_in servaddr;
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htons(INADDR_ANY);
  servaddr.sin_port = htons(port);
  bind(listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr));



  // listen to clients
  listen(listen_fd, 10);


  // Ctrl C handler
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = endServerHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  while (true) {
    struct sockaddr_in clientaddr;
    socklen_t clientaddrlen = sizeof(clientaddr);
    int* comm_fd = new int;
    *comm_fd = accept(listen_fd, (struct sockaddr*)&clientaddr, &clientaddrlen);
    
    // fdSet.insert(*comm_fd);
    fdArray[*comm_fd] = true;
    
    //printf("Connection from %s\n", inet_ntoa(clientaddr.sin_addr));


    pthread_t newPthread;
    pthread_create(&newPthread, NULL, httpClientThread, (void *)comm_fd);

    //fd2thread.push(make_pair(comm_fd, &newPthread));

  }


  // TODO: send messages -ERR Server shutting
  return 0;
}
