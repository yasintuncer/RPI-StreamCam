#include <iostream>
#include "stream_cam.h"
#include <opencv2/core/core.hpp>

#define NAMED_WINDOW "STREAM"
const std::string config = ".config.json";



struct image_buffer
{
    int buffer_size;
    char *data;
    char * get_buffer(){    return data;}
    void set(cv::Mat &image)
    {
        if (data != nullptr)
        {
            delete data;
            data = nullptr;
        }
        if(image.empty())
            return;
        
        buffer_size = image.elemSize() + sizeof(int) * 3;
        
        data = new char[buffer_size]();
        int a = image.channels();
        memcpy(data, &image.rows, sizeof(int));
        memcpy(data + sizeof(int), &image.cols, sizeof(int));
        
        memcpy(data + 2 * sizeof(int), &a,sizeof(int));
        memcpy(data + 3 * sizeof(int), image.data, image.elemSize());
    }
};


Stream::Stream()
{
    pm = new ParameterEngine(config);
    if ( pm->source == SourceMode::CAPTURE)
    {
        initCam();
        std::cout << "Camera initilazed..\n"; 
    }
    else if (pm->source == SourceMode::MEDIA || pm->source == SourceMode::VIDEO)
    {/** this will be reserved media file streaming **/
        exit(0);
    }
    else 
    {std::cout << "Undefined Source Type" << std::endl;
        exit(0);
    }
    if(pm->stream.mode == StreamMode::TCP)
    {
        initClient();
        isStreamOnTCP = true;
    } 
    else if(pm->stream.mode == StreamMode::DISPLAY)
    {
            isStreamOnDisplay = true;
    }
    else 
    {
        std::cout <<  "Undefined Streamn Type" << std::endl;
        exit(0);
    }
        
}

void Stream::initCam()
{
    this->cam = new raspicam::RaspiCam_Still_Cv();
    isCamInit = true;
    if(!cam->set(cv::CAP_PROP_FORMAT, pm->camera.format))
    {
        std::cout << "Invalid format : " << pm->camera.format << std::endl;
        pm->camera.width = cam->get(cv::CAP_PROP_FORMAT);
        std::cout << "Frame width set default :" << pm->camera.format << std::endl;
    }
    if(!cam->set(cv::CAP_PROP_FRAME_WIDTH, pm->camera.width))
    {
        std::cout << "Invalid Frame Width : " << pm->camera.width << std::endl;
        pm->camera.width = cam->get(cv::CAP_PROP_FRAME_WIDTH);
        std::cout << "Frame width set default :" << pm->camera.width << std::endl;
    }
    if(!cam->set(cv::CAP_PROP_FRAME_HEIGHT, pm->camera.height))
    {
        std::cout << "Invalid Frame Height : " << pm->camera.height << std::endl;
        pm->camera.height = cam->get(cv::CAP_PROP_FRAME_HEIGHT);
        std::cout << "Frame Height set default :" << pm->camera.height << std::endl;
    }
    if(!cam->set(cv::CAP_PROP_FPS, pm->camera.fps))
    {
        std::cout << "Invalid Frame Per Second : " << pm->camera.fps << std::endl;
        isCamInit = false;
    }
    return ;
}


void Stream::initClient()
{
    //char ip[INET_ADDRSTRLEN];
    //strcpy(ip,pm->stream.ip_address.c_str());
    //client = new Client(ip,1234);
    client = new Client();    
}

void Stream::run()
{   
    if (pm->stream.mode == StreamMode::TCP && pm->source == SourceMode::CAPTURE)
    {
        if(!cam->open())
        {
            std::cout << "Camera Couldn't opened.\n";
            exit(0);    
        }
        // Warmup Cam
        cv::Mat temp;
        for(int i =0; i < 20; i++)
        {
            cam->grab();
            cam->retrieve(temp);
        }
        temp.release();
        
        if(!client->connected())
        {
            std::cout << "Connection refused.\n";
            exit(0);
        }
        

        for(;;)
        {
            cv::Mat frame;
            
            cam->grab();
            cam->retrieve(frame);

            if(!frame.empty())
            {
                image_buffer buffer;
                buffer.set(frame);
                client->conHandler(buffer.data, buffer.buffer_size);
            }
        }
        cam->release();
        client->closeSocket();

    }
    else if(pm->stream.mode == StreamMode::TCP && pm->source != SourceMode::CAPTURE)
    {
        
        
    }
    else if(pm->stream.mode == StreamMode::DISPLAY && pm->source == SourceMode::CAPTURE)
    {
        if(!cam->open())
        {
            std::cout << "Camera Couldn't opened.\n";
            exit(0);    
        }
        int n = 0;
        while(n >= 0)
        {
            cv::Mat frame;
            cam->grab();
            cam->retrieve(frame);
        }

        cam->release();
        cv::destroyAllWindows();
    }


    return;
}

