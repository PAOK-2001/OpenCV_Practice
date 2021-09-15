// Simple script that reads and displays camera feed using OPENCV

// Import OpenCV modules
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main() {
    // Create OpenCV frame object to store frame information
    Mat frame;
    // Create VideoCapture objet
    VideoCapture camera(0);
    
    // Check if the camera is readable
    if(!camera.isOpened()){
        cout<<"Error reading camera feed";
        return -1;
    }

    for (;;){
        camera.read(frame);

        if(frame.empty()){
            cout<<"NULL frame ";
            break;
        }
        imshow("Camera Feed", frame);

        if(waitKey(10)== 27){
            break;
        }
    }
    
    return 0;
}


