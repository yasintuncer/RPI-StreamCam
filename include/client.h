#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>



class tcp_client
{
private:
    int sockfd, portno,n;
    struct sockaddr_in server_addr;
    struct hostent *server;
    std::string response_data = "";
public:
    tcp_client(std::string, int);
    ~tcp_client();

    bool conn();
    int send_data(char *data, int data_length);
    int read_data(char *data, int &length_of_data);
    void close_socket();
public:
    bool isOpened;
    bool isConnected;

};