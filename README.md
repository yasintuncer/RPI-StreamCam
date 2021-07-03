**RaspberyyPi Stream Camera project**

This project streams images from taken on the raspberry pi CSI camera module. The streams process is provided with TCP connection. Raspicam CV library was used for the process of capturing images.

The Cmake was used as project manager on this project.For the build:
    
    $ cd repository
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make -j4
    $ make install

Now Builded Project.For the run:
    
    $cd ../App
    $./StreamCam

if you want to be configure runtime parameters like Stream type and camera parameters. open "config.json" file and edit.