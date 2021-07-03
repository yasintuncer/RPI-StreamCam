#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


class Client
{
public:
    Client();
    Client(char *ip, uint16_t port);
    ~Client() {closeSocket();}
    bool connected();
    int conHandler(char *Data, size_t size);
    void closeSocket();

private:
    int sendData(char *data, size_t size);
    int readData(char *data, size_t size);
    
private:
    int sockfd;
    uint16_t port;
    struct  sockaddr_in serv_addr;
    struct hostent *server;
    char server_ip[INET_ADDRSTRLEN];
};