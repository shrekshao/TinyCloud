#include "http_server.h"

void* httpClientThread(void* params)
{
  // int comm_fd = *((int *)params);
  int *p = (int *)params;
  int comm_fd = *p;
  delete p;

//   printDebugMessage(comm_fd, "New connection\n");

//   do_write(comm_fd, &msgGreeting.at(0), msgGreeting.size());


  stringstream ss;

  ReceivingStatus receivingStatus = waiting_request;

  // receive message from client
  
  // unsigned short rlen;
  // do_read(comm_fd, (char*)&rlen, sizeof(rlen));
  // rlen = ntohs(rlen);
  char buf[BUFFER_SIZE];
  memset( buf, '\0', BUFFER_SIZE );

  unordered_map<string, string> headersReceived;
  string uri;


  while (true)
  {
    
    // printDebugMessage(comm_fd, "Server Status: " + to_string(receivingStatus) + "\n");

    if(reading_content == receivingStatus)
    {
        // printDebugMessage(comm_fd, "Server State: reading_content" + to_string(receivingStatus) + "\n");

        // int contentLength = stoi(headersReceived["Content-Length"]);    // TODO exception
        // string contentType = headersReceived["Content-Type"];

        auto it_content_length = headersReceived.find("Content-Length");
        if (it_content_length == headersReceived.end())
        {
            // ERROR, content-length not received
            printDebugMessage(comm_fd, "ERR: Content-Length not received");
            receivingStatus = waiting_request;
            ss.clear();
            continue;
        } 
        
        int contentLength = stoi(it_content_length->second);


        // test
        // printDebugMessage(comm_fd, "Content Line: " + line + "\r\n");
        
        // ss.seekg (0, ss.end);
        // int sslength = ss.tellg();
        // ss.seekg (0, ss.beg);

        // if (sslength >= contentLength)
        // {
            // string contentString;
            char * contentBuf = new char [ contentLength ];
            ss.read(contentBuf, contentLength);
            printDebugMessage(comm_fd, contentBuf);

            delete [] contentBuf;

            receivingStatus = waiting_request;

            // temp test, direct to new page
            sendFileToClient(comm_fd, "/profile.html");


            continue;
        // }
        
    }









    string line;

    // ? do a safe get line ?
    getline(ss, line, '\n');
    if ( ss.eof() )
    {
      ss.clear();
      ss << line;

      int n;
    
      n = read(comm_fd, &buf[0], BUFFER_SIZE);

    //   printDebugMessage(comm_fd, "eof " + line + " " + to_string(n) + "\n");

      if (n > 0)
      {
        buf[n] = 0;
        ss << buf;
      } 
      continue;
    } 

    

    

    printDebugMessage(comm_fd, "C: " + line + "\n");


    

    if (waiting_request == receivingStatus)
    {
        // printDebugMessage(comm_fd, "Server State: waiting_request " + to_string(receivingStatus) + "\n");

        istringstream iss(line);
        string curInput;
        iss >> curInput;

        if (curInput == "GET")
        {
            receivingStatus = reading_request_get;
            
            iss >> uri;

            headersReceived.clear();

        }
        else if (curInput == "POST")
        {
            receivingStatus = reading_request_post;

            
            iss >> uri;

            headersReceived.clear();

            // TODO
        }
        else
        {
            printDebugMessage(comm_fd, "Error in waiting_request" + curInput + "\n");
        }

        
    }
    else if(reading_request_get == receivingStatus
        || reading_request_post == receivingStatus )
    {
        // printDebugMessage(comm_fd, "Server State: reading_request " + to_string(receivingStatus) + "\n");

        istringstream iss(line);
        string curInput;
        iss >> curInput;

        if (curInput == "" || curInput == "\r")
        {
            printDebugMessage(comm_fd, "Content Separation line\n");
            // printDebugMessage(comm_fd, buf);
            // printDebugMessage(comm_fd, curInput.c_str());

            // ss >> curInput;
            // printDebugMessage(comm_fd, curInput.c_str());
            // ss >> curInput;
            // printDebugMessage(comm_fd, curInput.c_str());
            // ss >> curInput;
            // printDebugMessage(comm_fd, curInput.c_str());
            


            // auto it = headersReceived.find("Content-Length");
            if (receivingStatus == reading_request_post)
            {
                // has content
                receivingStatus = reading_content;
            }
            else if (receivingStatus == reading_request_get)
            {
                // no content
                receivingStatus = waiting_request;
                // test
                sendFileToClient(comm_fd, uri);
            }
            
        }
        else if (curInput.back() == ':')
        {
            // header
            string header = curInput.substr(0, curInput.size() - 1);
            string value;
            getline(iss, value);
            headersReceived.emplace(header, value);
        }
        else
        {
            printDebugMessage(comm_fd, "Error in reading_headers " + curInput + "\n");
        }

        
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

//   sendFileToClient(comm_fd, "index.html");
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
//   uri_to_handlers.emplace("/", &sendFileToClient);
//   uri_to_handlers.emplace("/main.css", &sendFileToClient);






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