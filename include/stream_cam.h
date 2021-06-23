#include <ctype.h>
#include <raspicam/raspicam_still_cv.h>
#include "client.h"
#include "ParameterEngine.h"
#include <opencv2/highgui/highgui.hpp>

class Stream
{
    public:
        Stream();
        void run();
    public:
        bool isCamInit;
        bool isClientInit;
        bool isStreamOnDisplay;
        bool isStreamOnTCP;
            
    private:
        ParameterEngine *pm;
        raspicam::RaspiCam_Still_Cv *cam;
        tcp_client *client;
        char *output_buffer;
        int buffer_size;
        cv::Mat frame;
    private:    
        void initCam();
        void initClient();
        void updateOutputBuffer(cv::Mat);
        int streamOnTcp();
        int streamOnDisplay();
        

        
};

