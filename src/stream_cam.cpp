#include <iostream>
#include "stream_cam.h"
#include <opencv2/core/core.hpp>

#define NAMED_WINDOW "STREAM"
const std::string config = "/home/pi/Projects/StreamCam/config.json";

static inline void int_to_char_array( char *a, int value)
{
    a[0] = (value& 0xFF);
    a[1] = ((value >>8)&0xFF);
    a[2] = ((value >>16)&0xFF);
    a[3] = ((value >>24)&0xFF);
    
    return;
}


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
    if(cam->set(cv::CAP_PROP_FORMAT, pm->camera.format))
    {
        std::cout << "Invalid format : " << pm->camera.format << std::endl;
        pm->camera.width = cam->get(cv::CAP_PROP_FORMAT);
        std::cout << "Frame width set default :" << pm->camera.format << std::endl;
    }
    if(cam->set(cv::CAP_PROP_FRAME_WIDTH, pm->camera.width))
    {
        std::cout << "Invalid Frame Width : " << pm->camera.width << std::endl;
        pm->camera.width = cam->get(cv::CAP_PROP_FRAME_WIDTH);
        std::cout << "Frame width set default :" << pm->camera.width << std::endl;
    }
    if(cam->set(cv::CAP_PROP_FRAME_HEIGHT, pm->camera.height))
    {
        std::cout << "Invalid Frame Height : " << pm->camera.height << std::endl;
        pm->camera.height = cam->get(cv::CAP_PROP_FRAME_HEIGHT);
        std::cout << "Frame Height set default :" << pm->camera.height << std::endl;
    }
    if(cam->set(cv::CAP_PROP_FPS, pm->camera.fps))
    {
        std::cout << "Invalid Frame Per Second : " << pm->camera.fps << std::endl;
        isCamInit = false;
    }
    return ;
}


void Stream::initClient()
{
    client = new tcp_client(pm->stream.ip_address, pm->stream.port);
    
    if( client->isOpened)
        isClientInit = true;
    else 
        isClientInit = false;
}

void Stream::updateOutputBuffer(cv::Mat image)
{
    int width    = image.cols;
    int heigth   = image.rows;
    int channels = image.channels();
    int new_buffer_size =  sizeof(width) + sizeof(heigth) + sizeof(channels) + width * heigth * channels;

    if (output_buffer != nullptr)
        delete output_buffer;
     output_buffer = new char[new_buffer_size]();
    char a[4];
    int_to_char_array(a,width);
    std::memcpy(output_buffer, a,sizeof(int));
    int_to_char_array(a,heigth);
    std::memcpy(output_buffer + sizeof(int), a, sizeof(int));
    int_to_char_array(a, channels);
    std::memcpy(output_buffer + sizeof(int) * 2, a, sizeof(int));

    std::memcpy(output_buffer + 3 * sizeof(int), image.data, width*heigth*channels);
    buffer_size = new_buffer_size;

}
int Stream::streamOnTcp()
{   
    if(!client->conn())
    {
        std::cout << "Connection refused.." << std::endl;
        return -1;
    }
    updateOutputBuffer(frame);
    client->send_data(output_buffer, buffer_size);
    
    client->close_socket();
    return 0;
}
int Stream::streamOnDisplay()
{
    cv::namedWindow(NAMED_WINDOW, cv::WINDOW_NORMAL);
    cv::imshow(NAMED_WINDOW,frame);
    return 0;
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
        int n = 0;
        while(n >= 0)
        {
            cam->grab();
            cam->retrieve(frame);
            if(!frame.empty())
                n = streamOnTcp();
        }
        cam->release();
        client->close_socket();
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
            cam->grab();
            cam->retrieve(frame);
            if(!frame.empty())
                n = streamOnDisplay();
        }

        cam->release();
        cv::destroyAllWindows();
    }


    return;
}

