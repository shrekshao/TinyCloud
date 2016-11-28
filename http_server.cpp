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

#include <unordered_set>


using namespace std;


bool do_write(int fd, const char *buf, int len)
{
    int sent = 0;
    while (sent < len)
    {
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


//#include "utils.h"

bool outputAuthor = false;
bool outputDebug = false;
int port = 10000;

const int BUFFER_SIZE = 50;


// unordered_set<int> fdSet;
const int NUM_FD = 1000; 
bool fdArray[NUM_FD] = {};  // all elements 0




void printDebugMessage(int fd, const string & msg)
{
  //stderr << "[" << fd << "]"
  if (outputDebug)
  {
    fprintf(stderr, "[%d] %s", fd, msg.c_str()); 
  }
}


string msgGreeting = "+OK Server ready (Author: Shuai Shao / sshuai)\n";
string msgQuit = "+OK Goodbye!\n";
string msgErr = "-ERR Unknown command\n";
string msgServerOff = "-ERR Server shutting down\n";

void* clientThread(void* params)
{
  // int comm_fd = *((int *)params);
  int *p = (int *)params;
  int comm_fd = *p;
  delete p;

  //cerr << "\n\nthread created!!!!    fd=" << comm_fd << "\n\n";

  printDebugMessage(comm_fd, "New connection\n");

  do_write(comm_fd, &msgGreeting.at(0), msgGreeting.size());


  stringstream ss;

  // receive message from client
  
  // unsigned short rlen;
  // do_read(comm_fd, (char*)&rlen, sizeof(rlen));
  // rlen = ntohs(rlen);
  char buf[BUFFER_SIZE];

  while (true)
  {
    
    string cmd;
    getline(ss, cmd, '\n');

    if ( ss.eof() )
    {
      ss.clear();
      ss << cmd;
      int n;
    
      n = read(comm_fd, &buf[0], BUFFER_SIZE);

      if (n > 0)
      {
        buf[n] = 0;
        ss << buf;
      } 
      continue;
    } 

    

    istringstream iss(cmd);
    string curInput;
    iss >> curInput;

    printDebugMessage(comm_fd, "C: " + cmd + "\n");

    if (curInput == "QUIT")
    {
      
      printDebugMessage(comm_fd, "S: " + msgQuit);
      // send to client
      do_write(comm_fd, &msgQuit.at(0), msgQuit.size());

      printDebugMessage(comm_fd, "Connection closed\n");

      // ? thread safe?
      // fdSet.erase(comm_fd);
      fdArray[comm_fd] = false;

      close(comm_fd);
      break;
    }
    else if (curInput == "ECHO")
    {
      //iss >> curInput;  // message
      getline(iss, curInput); // has a space at the begining

      // string msg = "+OK " + curInput + "\n";
      string msg = "+OK" + curInput + "\n";
      printDebugMessage(comm_fd, "S: " + msg);
      // send to client
      do_write(comm_fd, &msg.at(0), msg.size());
    }
    else
    {
      // error handling
      
      printDebugMessage(comm_fd, "S: " + msgErr);
      do_write(comm_fd, &msgErr.at(0), msgErr.size());

      ss.clear();
    }


  }
  



  pthread_exit(NULL);
}

const string CRLF = "\r\n";
const string LF = "\n";
string HTTP_OK = "HTTP/1.1 200 OK\r\n";

void sendTextFile(int comm_fd, const string& filename)
{
    ifstream in;
    in.open(filename);

    string line;
    while(getline(in, line))
    {
        line += "\n";
        do_write(comm_fd, &line.at(0), line.size());
    }

    in.close();
}

enum ReceivingStatus{
    request, content
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

  ReceivingStatus receivingStatus = request;

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
        // get all headers
        string uri;

        iss >> uri;

        // test
        do_write(comm_fd, &HTTP_OK.at(0), HTTP_OK.size());

        // what if a empty line

        do_write(comm_fd, &CRLF.at(0), CRLF.size());
        
        do_write(comm_fd, &CRLF.at(0), CRLF.size());

        sendTextFile(comm_fd, "index.html");

    }
    else if (curInput == CRLF || curInput == LF)
    {

    }
    else
    {
    //   // error handling
      
    //   printDebugMessage(comm_fd, "S: " + msgErr);
    //   do_write(comm_fd, &msgErr.at(0), msgErr.size());

    //   ss.clear();
    }


  }
  



  pthread_exit(NULL);
}









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
      do_write(i, &msgServerOff.at(0), msgServerOff.size());
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



  /* Your code here */

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
