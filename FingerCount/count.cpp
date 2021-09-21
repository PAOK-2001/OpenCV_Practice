// Scrip that counts number of fingers displayED

// Import OpenCV modules
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <sys/timeb.h>
#include "vector"
#include <iostream>

using namespace std;
using namespace cv;




int main() {
    // Create OpenCV frame object to store frame information
    Mat frame;
    Mat Region;
    // Scalar object represent red
    Scalar red = Scalar(0,0,255);
    // Rectangle that limits the Region of Interest (ROI)
    Rect ROI(410,45,190,190);

    // Create VideoCapture object, reading video device (USB camera)
    VideoCapture camera(2);
    
    
    // Check if the camera is readable
    if(!camera.isOpened()){
        cout<<"Error reading camera feed";
        return -1;
    }

    for (;;){
        camera.read(frame);
        // Check if selected device is sending information
        if(frame.empty()){
            cout<<"NULL frame ";
            break;
        }
        
        rectangle(frame,ROI,red,5);
        Region = frame(ROI);
        imshow("Camera Feed", frame);
        imshow("Region of Interest",Region);
        // Read key board input, setting esc as break key
        if(waitKey(10)== 27){
            break;
        }
    }
    
    return 0;
}


