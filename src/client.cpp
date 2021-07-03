#include <iostream>
#include "client.h"

#define DEFAULT_PORT 2222
#define DEFAULT_IP_ADDRESS "192.168.2.13"

Client::Client()
{
    port = DEFAULT_PORT;
    strcpy(server_ip,DEFAULT_IP_ADDRESS);
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cout << "Error Open Socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    server = gethostbyname(DEFAULT_IP_ADDRESS);
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(uint16_t(DEFAULT_PORT));
}

Client::Client(char *ip, uint16_t port)
{
    this->port = port;
    strcpy(server_ip,ip);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cout << "Error Open Socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    server = gethostbyname(ip);
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(port);


}

bool Client::connected()
{
    int retval = 0;
    int n_times = 10;
    
     do
    {
        retval = connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
        if(retval >= 0)
            break;
    }while(n_times --);
    
    if (retval < 0)
    {
        std::cout << "Error Connecting" << std::endl;
        return false;
    }

    return true;
}

int Client::sendData(char *Data, size_t size)
{
    int retval = write(sockfd,Data, size);

    if(retval < 0)
    {
        std::cout << "Read error on socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Sending information : " << retval << " size byte written" << std::endl; 
    return retval;
}

int Client::readData(char *Data,size_t size)
{
    if(Data != nullptr)
    {
        delete Data;
        Data = nullptr;
    }

    size_t buffer_size = 4096;
    char buffer[buffer_size];
    int retval = 0;
    while (true)
    {
        retval = recv(sockfd, buffer, buffer_size, 0);
        if(retval < 0)
        {
            std::cout << "Read error on Socket " << std::endl;
            return retval; 
        }

        if(retval < buffer_size)
        {
            break;
        }
    }
    return 0;

}

int Client::conHandler(char *Data, size_t size)
{
    if (Data ==nullptr)
        return 0;
    
    int retval = 0;
    retval = sendData(Data, size);
    
    retval = readData(Data, size);
    return retval;
}

void Client::closeSocket()
{
    close(sockfd);
    serv_addr.~sockaddr_in();
}