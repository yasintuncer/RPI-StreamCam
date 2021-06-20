#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <raspicam/raspicam_still_cv.h>
#include "client.h"

namespace pt = boost::property_tree;
enum Stream_Mode
{
    DISPLAY = 0,
    PORT    = 1
};
struct CameraParams
{
    /* Camera specs */
        int width;          
        int height;         
        int format;       
        int fps;       
        int brightness;
        int contrast;      
        int saturation;      
        int gain;
        int exposure;
        /* Stream spec */       
        int mode;
        int port;
        std::string ip_address;
        std::string media_directory;
}CameraParams;
class ParameterEngine
{
    public:
    ParameterEngine(std::string config_json)
    {
    
        // Create a root
        pt::ptree root;

    // Load the json file in this ptree
    pt::read_json(config_json, root);
    
    this->width             = root.get<int>("width",0);
    this->height            = root.get<int>("height",0);
    this->format            = root.get<int>("format",0);
    this->fps               = root.get<int>("fps",0);
    this->brightness        = root.get<int>("brightness",0);
    this->saturation        = root.get<int>("saturation",0);
    this->gain              = root.get<int>("gain", 0);
    this->exposure          = root.get<int>("exposure", 0);
    this->mode              = root.get<int>("mode", 0);
    this->port              = root.get<int>("port",0);
    this->ip_address        = root.get<std::string>("ip_address",0);
    this->media_directory   = root.get<std::string>("media_directory",0);
    }
    
    
    public:

    CameraParams cs;
        
};



class StreamCam
{
    public:
        StreamCam();
        bool init_camera_properties();

    private:
        bool init_mode_requirements();
    private:    
        ParameterEngine *params;
        raspicam::RaspiCam_Still_Cv *cam;
        tcp_client *client;
};

