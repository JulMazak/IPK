//***INCLUDES***
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <string>
//***DEFINES***
#define BACKLOG 10
#define BUFFER_SIZE 30000
#define PROTOCOL 0
using namespace std;
// definitions of classes
namespace MyServer
{
    class Socket
    {
        public:
        //DATA STRUCTURES
        struct sockaddr_in myaddr;
        struct sockaddr_storage storage;
        struct addrinfo *MyServerinfo;
        socklen_t size;
        int info, sock, listener, binder, newsock, msg_len, sender,receiver,optval,hostnum,req,i;
        string messag,respond,first_line; 
        string cpu[10],cpu2[10];
        char request[100],hostname[100];
        char buffer[BUFFER_SIZE] = {0};
       //FUNCTIONS
        void CallError();
        int CreateSocket(int domain, int type, int protocol);
        void Binding(int port);
        void Listening();
        void Accepting();
        void Responding();
        void End();
        void Server(int port);
        void Hostname();
        void CPU_Name();
        void Load_read(string cpu[10]);
        void Load();
    };

}
//Error Handling 
void MyServer::Socket::CallError()
{
        cout << " ERROR : " << strerror(errno) << '\n';
        exit(EXIT_FAILURE);
}
//Creation of socket and filling 
int MyServer::Socket::CreateSocket(int domain, int type, int protocol)
{
    sock = socket(domain,type,protocol);
    if ( sock == -1 )
        CallError();
    optval = 1;
    // solve "ALREADY IN USE " error
    setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
    setsockopt (sock, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof optval);
    return sock;
}
// Associate with port
void MyServer::Socket::Binding(int port)
{
    //myaddr structure
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(port);
    myaddr.sin_addr.s_addr = INADDR_ANY;
    memset(myaddr.sin_zero, '\0', sizeof myaddr.sin_zero);

    binder = bind(sock,(struct sockaddr *)&myaddr, sizeof(myaddr));
   if (binder == -1)
        CallError();
}
//Listen to requests
void MyServer::Socket::Listening()
{
   listener = listen(sock,BACKLOG);
   if(listener == -1)
       CallError();
}
//Accepting of connection
void MyServer::Socket::Accepting()
{
    size = sizeof(storage);
    newsock = accept(sock,(struct sockaddr *)&storage,&size);
    read(newsock,buffer,BUFFER_SIZE);
    strncpy (request,buffer,14);
    if(request[0] != 'G')
    messag = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/plain;\r\n\r\nINVALID METHOD";
    string request2(request);
    request2 = request2.substr(request2.find("/")+1,strlen(request));
    request2 = request2.substr(0,10);
    req = 99;
    if(request2.compare("hostname ") == 0)
    req = 1;
    else if(request2.compare("cpu-name ") == 0)
    req = 2;
    else if (request2.compare("load HTTP") == 0)
    req = 3;
}
 // Respond accordingly
 void MyServer::Socket::Responding()
 {
    switch(req) {
  case 1 :
    Hostname();
    messag = "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n"+respond;
    break;
  case 2 :
    CPU_Name();
    messag = "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n"+respond;
    break;
  case 3:
  Load();
  messag = "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n"+respond;
  break;
  default:
  messag = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain;\r\n\r\nINVALID REQUEST";
}
const char *message = messag.c_str();
write(newsock , message , strlen(message));
 
 }
// End connection
void MyServer::Socket::End()
{
    close(newsock);
}
// SERVER class
void MyServer::Socket::Server(int port)
{
    CreateSocket(AF_INET,SOCK_STREAM,PROTOCOL);
    Binding(port);
    Listening();
    while(true)
    {
    Accepting();
    Responding();
    End();
    }
    
}

// GET Hostname
void MyServer::Socket::Hostname()
{
    if ((hostnum = gethostname(hostname, 100)) != 0 )
    CallError();
    respond = hostname;
}
// Reading information about CPU usage
void MyServer::Socket::Load_read(string cpu[10])
{
    ifstream infile;
    string file("/proc/stat");
    infile.open(file);
    getline(infile,first_line);
    i = 0;
    stringstream ssin(first_line);
    while(ssin.good() && i < 10)
    {
        ssin >> cpu[i];
        ++i;
    }
    infile.close();
}
// GET load
void MyServer::Socket::Load()
{
    Load_read(cpu);
    sleep(1);
    Load_read(cpu2);
    long double idle = stol(cpu2[4]);
    long double previdle = stol(cpu[4]);
    long double total = stol(cpu2[1])+stol(cpu2[2])+stol(cpu2[3])+stol(cpu2[4])+stol(cpu2[5])+stol(cpu2[6])+stol(cpu2[7]);
    long double prevtotal = stol(cpu[1])+stol(cpu[2])+stol(cpu[3])+stol(cpu[4])+stol(cpu[5])+stol(cpu[6])+stol(cpu[7]);
    long double loadavg = 1 - ( (idle - previdle) / (total - prevtotal));
    string temp = to_string(loadavg*100);
    respond = temp.substr(0,temp.find(".")+3)+"%";
}
// GET cpu-name
void MyServer::Socket::CPU_Name()
{
    ifstream infile;
    string file("/proc/cpuinfo");
    infile.open(file);
    string respond2 = "BAD REQUEST";
    while(getline(infile, first_line))
{
    if (first_line.find("name") != string::npos)
        respond2 = first_line;

}
int size = respond2.length();
respond2 = respond2.substr(respond2.find(":")+2,size);
respond = respond2;
}
int main(int argc, char *argv[])
{
    if(argc == 2)
    {
    int port = stoi(argv[1]);
    MyServer::Socket obj;
    obj.Server(port);
    }
    return 0;
}
