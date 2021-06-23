#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "client.h"
tcp_client::tcp_client(std::string ip_adress, int portno)
{
    this->ip_adress = ip_adress;
    this->portno = portno;
}

bool tcp_client::conn()
{
    this->isOpened = true;
    // create socket
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(this->sockfd < 0)
    {
        perror("Error openning socket");
        this->isOpened = false;
    }

    this->server = gethostbyname(ip_adress.c_str());
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host\n");
        isOpened = false;
    }
    bzero((char *)&this->server_addr, sizeof(this->server_addr));
    this->server_addr.sin_family = AF_INET;
    bcopy((char*)this->server->h_addr, (char *)&this->server_addr.sin_addr.s_addr, this->server->h_length);
    this->portno = portno;
    server_addr.sin_port = htons(this->portno);

    if (connect(this->sockfd, (struct sockaddr *)&this->server_addr, sizeof(this->server_addr)) < 0)
        this->isConnected = false;
    else
    this->isConnected = true;

    return this->isConnected;
}

int tcp_client::send_data(char *data, int data_length)
{
    if (data == NULL)
    {
        perror("Sending data cannot be NULL");
        return -1;
    }
    if(write(this->sockfd, data, data_length) < 0)
    {
        perror("ERROR writing to socket");
        return -1;
    }

    return 0;
}

int tcp_client::read_data(char *data, int &length_of_data)
{

    std::vector<char> stack = std::vector<char>();
    size_t package_size = 4096;
    char *tmp;
    size_t current_size = 0;
    do
    {
        if(current_size < 0)
        {
            perror("ERROR reading from socket\n");
            stack.clear();
            return -1;
        }
        tmp = new  char[package_size];
        current_size = read(this->sockfd, tmp, package_size);
        stack.insert(stack.end(),tmp, tmp + current_size);
        delete [] tmp;

    }
    while(current_size != 0);
    
    length_of_data = stack.size();

    data = new  char[length_of_data];

    std::copy(stack.begin(), stack.end(), data);
    stack.clear();
    return 0;
}


void tcp_client::close_socket()
{
    close(this->sockfd);
    this->isConnected = false;
    
}
