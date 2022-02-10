#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <fstream>

#define BACKLOG 10
using namespace std;

namespace Server
{
    class Socket
    {
        public:
        //DATA STRUCTURES
        struct sockaddr_in myaddr;
        struct sockaddr_storage storage;
        struct addrinfo hints;
        struct addrinfo *serverinfo;
        socklen_t size;
        int info, sock, listener, binder, newsock, msg_len, sender,receiver;
        char *message;
       //FUNCTIONS
        void CallError(int error);
        int CreateSocket(int domain, int type, int protocol);
        int Bind(int port);
        int Listening();
        int Accepting();
        int Sending();
        int Receiving();
        int End(struct addrinfo *var);
    };
}

//Error Handling 
void Server::Socket::CallError(int error)
{
        cout << " ERROR : " << strerror(errno) << '\n';
        exit(EXIT_FAILURE);
}

//Creation of socket and filling 
int Server::Socket::CreateSocket(int domain, int type, int protocol)
{
  
    sock = socket(domain,type,protocol);
    
    if ( sock == -1 )
    {
        CallError(sock);
    }

    return sock;
}

int Server::Socket::Bind(int port)
{
    //myaddr structure
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(port);
    // he uses htonl? should I? idk yet
    myaddr.sin_addr.s_addr = INADDR_ANY;

    binder = bind(sock,(struct sockaddr *)&myaddr, sizeof myaddr);
   
    if (binder == -1)
    {
        CallError(binder);
    }

   return binder;
}

int Server::Socket::Listening()
{
   listener = listen(sock,BACKLOG);
   if(listener == -1)
   {
       CallError(listener);
   }
   return listener;
}

//Accepting of connection
int Server::Socket::Accepting()
{
    size = sizeof storage;
    newsock = accept(sock,(struct sockaddr *)&storage,&size);
    return newsock;
}

int Server::Socket::Sending()
{
   msg_len = strlen(message);
   //returns the number of bytes that were sent
   sender = send(sock,message,msg_len,0);
   return sender;
}

int Server::Socket::Receiving()
{
    //TODO
   receiver = recv(sock,NULL,0,0);
   return receiver;
}

int Server::Socket::End(struct addrinfo *var)
{
    //free linked list
    freeaddrinfo(var);
    if(shutdown(newsock,2) == -1)
    {
        CallError(-1);
    }

   return 0;
}

int main(int argc, char *argv[])
{
    int port = stoi(argv[1]);
    return 0;
}