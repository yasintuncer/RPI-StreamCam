#include <iostream>
#include "stream_cam.h"
#include <opencv2/core/core.hpp>

const std::string config = "param.json";

StreamCam::StreamCam()
{
    this->params = new ParameterEngine(config);
    
    this->cam = new raspicam::RaspiCam_Still_Cv();
}
bool StreamCam::init_camera_properties()
{
    if ( ! this->cam->set(cv::CAP_PROP_FRAME_WIDTH, this->params->width))
    {
        std::cout << "Unknown properties Width : " << this->params->width << std::endl;
        return false;
    }
    if (! this->cam->set(cv::CAP_PROP_FRAME_HEIGHT, this->params->height))
    {
        std::cout << "Unknown properties Height : " << this->params->height << std::endl;
        return false;
    }
    if (! this->cam->set(cv::CAP_PROP_FORMAT, this->params->format))
    {
        std::cout << "Unknown properties Format : " << this->params->format << std::endl;
        return false;
    }
    if (! this->cam->set(cv::CAP_PROP_BRIGHTNESS, this->params->brightness))
    {
        std::cout << "Unknown properties Brightness : " << this->params->brightness << std::endl;
        return false;
    }
    if (! this->cam->set(cv::CAP_PROP_CONTRAST, this->params->contrast))
    {
        std::cout << "Unknown properties Contrast : " << this->params->contrast << std::endl;
        return false;
    }
    if (! this->cam->set(cv::CAP_PROP_CONTRAST, this->params->saturation))
    {
        std::cout << "Unknown properties Saturation : " << this->params->saturation << std::endl;
        return false;
    }
    if (! this->cam->set(cv::CAP_PROP_GAIN, this->params->gain))
    {
        std::cout << "Unknown properties gain : " << this->params->gain << std::endl;
        return false;
    }
    if (! (this->params->mode == Stream_Mode::DISPLAY || this->params->mode == Stream_Mode::PORT))
    {
        std::cout << "Unknown Work Mode : " << this->params->mode << std::endl;
        std::cout << "Please correct the mode parameter in the JSON file.\navailable choices:\n\tDISPLAY : 0\n\tPORT: 1 "; 
        return false;
    }
    if((this->params->ip_address.empty()) & this->params->mode == Stream_Mode::PORT)
    {
        std::cout << "İp address or port error" << std::endl;
        return false;
    }

}

bool StreamCam::get_camera_properties()
{
    if ( ! this->cam->set(cv::CAP_PROP_FRAME_WIDTH, this->params->width))
    {
        std::cout << "Unknown properties Width : " << this->params->width << std::endl;
        return false;
    }
    if (! this->cam->set(cv::CAP_PROP_FRAME_HEIGHT, this->params->height))
    {
        std::cout << "Unknown properties Height : " << this->params->height << std::endl;
        return false;
    }
    if (! this->cam->set(cv::CAP_PROP_FORMAT, this->params->format))
    {
        std::cout << "Unknown properties Format : " << this->params->format << std::endl;
        return false;
    }
    if (! this->cam->set(cv::CAP_PROP_BRIGHTNESS, this->params->brightness))
    {
        std::cout << "Unknown properties Brightness : " << this->params->brightness << std::endl;
        return false;
    }
    if (! this->cam->set(cv::CAP_PROP_CONTRAST, this->params->contrast))
    {
        std::cout << "Unknown properties Contrast : " << this->params->contrast << std::endl;
        return false;
    }
    if (! this->cam->set(cv::CAP_PROP_CONTRAST, this->params->saturation))
    {
        std::cout << "Unknown properties Saturation : " << this->params->saturation << std::endl;
        return false;
    }
    if (! this->cam->set(cv::CAP_PROP_GAIN, this->params->gain))
    {
        std::cout << "Unknown properties gain : " << this->params->gain << std::endl;
        return false;
    }
    if (! (this->params->mode == Stream_Mode::DISPLAY || this->params->mode == Stream_Mode::PORT))
    {
        std::cout << "Unknown Work Mode : " << this->params->mode << std::endl;
        std::cout << "Please correct the mode parameter in the JSON file.\navailable choices:\n\tDISPLAY : 0\n\tPORT: 1 "; 
        return false;
    }
    if((this->params->ip_address.empty()) & this->params->mode == Stream_Mode::PORT)
    {
        std::cout << "İp address or port error" << std::endl;
        return false;
    }

}