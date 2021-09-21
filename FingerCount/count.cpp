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


class Hand{
private:
    Mat image;
    int fingers;
    bool isHand;
public:
    Hand();
    Hand(Mat frame);
    Mat returnImage();
};

Hand::Hand(Mat frame){
    cvtColor(frame, frame, COLOR_BGR2GRAY );
    threshold(frame, image,70,255,THRESH_BINARY);
}


Mat Hand::returnImage(){
    return image;
}



int main() {
    // Create OpenCV frame object to store frame information
    Mat frame;
    Mat Region;
    // Scalar object represent red
    Scalar red = Scalar(0,0,255);
    // Rectangle that limits the Region of Interest (ROI)
    Rect ROI(410,45,225,225);

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
        Hand h1(frame(ROI));
        imshow("Camera Feed", frame);
        imshow("Region of Interest", h1.returnImage());
        // Read key board input, setting esc as break key
        if(waitKey(10)== 27){
            break;
        }
    }
    
    return 0;
}


