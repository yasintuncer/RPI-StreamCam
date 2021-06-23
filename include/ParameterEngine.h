#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <raspicam/raspicam_still_cv.h>

namespace pt = boost::property_tree;

enum StreamMode: int
{
    DISPLAY = 0,
    TCP     = 1
};
enum SourceMode: int
{
    MEDIA = 0,
    VIDEO = 1,
    CAPTURE = 2
};

struct CameraParams
{
    /* Camera specs */
    int width;          
    int height;         
    int format;       
    int fps;       
        
};
struct StreamParams
{
    /* Stream spec */       
    StreamMode mode;
    int port;
    std::string ip_address;
};
    
  

class ParameterEngine
{
    public:
    ParameterEngine(std::string config_json)
    {
        LoadParams(config_json);
    }
    
    
    public:
    SourceMode source;
    std::string  media_directory;
    CameraParams default_camera;
    CameraParams camera;
    StreamParams stream;     
    
    public:

    void LoadParams(std::string file_json)
    {
        pt::ptree root;
        
        pt::read_json(file_json, root);
        /* Source
        /* camera  params */
        camera.width            = root.get<int>("Width",0);
        camera.height           = root.get<int>("Height",0);
        camera.format           = root.get<int>("Format",0);
        camera.fps       = root.get<int>("Fps",0);
        /* Streaming params */
        std::cout << StreamMode::DISPLAY << std::endl;
        stream.mode              = ((root.get<int>("StreamMode")) == StreamMode::DISPLAY) ? StreamMode::DISPLAY : StreamMode::TCP;
        stream.ip_address        = root.get<std::string>("IpAdress");
        stream.port              = root.get<int>("Port");
        if (root.get<int>("SourceMode") == SourceMode::MEDIA)
            source = SourceMode::MEDIA;
        else if(root.get<int>("SourceMode") == SourceMode::VIDEO)
            source = SourceMode::VIDEO;
        else
            source = SourceMode::CAPTURE;
        
        media_directory = root.get<std::string>("MediaDirectory");
    }
};