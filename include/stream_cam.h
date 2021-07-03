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
        Client *client;
        
    private:    
        void initCam();
        void initClient();
        

        
};

