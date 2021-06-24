cmake_minimum_required( VERSION 3.7.2)

#set the project name 
project(StreamCam)

set(CMAKE_MODULE_PATH "/usr/local/lib/cmake/${CMAKE_MODULE_PATH}") 


FIND_PACKAGE(Boost)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

find_package( OpenCV REQUIRED )
IF (OpenCV_FOUND)
    INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIR})
    MESSAGE(STATUS "OPENCV FOUND")
ENDIF()

find_package(raspicam REQUIRED)
IF  (raspicam_FOUND)
    INCLUDE_DIRECTORIES(${raspicam_INCLUDE_DIR})
    MESSAGE(STATUS "RASPİCAM FOUND")
ENDIF()

include_directories(include)

 
# Add an executable with the above sources

# Create a sources variable with a link to all cpp files to compile 
set(SOURCES 
    src/client.cpp
    src/stream_cam.cpp
    src/main.cpp
) 
set(HEADERS
    include/client.h
    include/stream_cam.h)


add_executable(StreamCam ${SOURCES})


# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(StreamCam
    PRIVATE 
        ${OpenCV_LIBS} 
        ${raspicam_LIBS}
        ${raspicam_CV_LIBS}
)
target_link_libraries(StreamCam  
        ${OpenCV_LIBS} 
        ${raspicam_LIBS}
        ${raspicam_CV_LIBS} )

set(AppDirectory ${PROJECT_SOURCE_DIR}/App)

file(MAKE_DIRECTORY ${AppDirectory})

set(FILES /home/pi/Projects/StreamCam/config.json)

install(TARGETS StreamCam
        DESTINATION ${AppDirectory})
install(FILES ${FILES}
        DESTINATION ${AppDirectory})

