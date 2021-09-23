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
    Mat contrast;
    int fingers;
    bool isHand;
public:
    Hand(Mat frame);
    Hand(Mat frame, Mat background);
    Mat returnImage();
};

Hand::Hand(Mat frame){
    frame = 1.45*frame; //Aument saturation by multiplying integer
    blur(frame,contrast,Size(3,3));
    cvtColor(contrast, contrast, COLOR_BGR2GRAY);
    threshold(contrast, image,255,255,THRESH_OTSU);
}

Hand::Hand(Mat frame, Mat background){
    Mat noise, fmask;
    frame = 1.35*frame; //Aument saturation by multiplying integer
    absdiff(background,frame, contrast);
    //Set pixels bellow 15 (detected as noise) to black
    noise = abs(contrast)<15;
    noise = 0;
    bitwise_or(contrast,noise,contrast);
    // Blur Image
    blur(contrast,contrast,Size(3,3));
    // Turn Image to grayscale
    cvtColor(contrast, contrast, COLOR_BGR2GRAY );
    // Set grays as blacks;
    noise = abs(contrast)<10;
    noise = 0;
    bitwise_or(contrast,noise, contrast);
    // Aument saturarion
    contrast = 3.5*contrast;
    // Apply Otsu Thresholding
    threshold(contrast, image,70,255,THRESH_OTSU);
}


Mat Hand::returnImage(){
    return image;
}


int main() {
    bool hasBackground = false;
    // Create OpenCV frame object to store frame information
    Mat frame;
    Mat Region;
    Mat background;
    // Scalar object represent red
    Scalar red = Scalar(0,0,255);
    // Rectangle that limits the Region of Interest (ROI)
    Rect ROI(410,45,225,225);

    // Create VideoCapture object, reading video device (USB camera)
    VideoCapture camera(0);
    
    
    // Check if the camera is readable
    if(!camera.isOpened()){
        cout<<"Error reading camera feed";
        return -1;
    }

    for (;;){
        if (hasBackground){
            camera.read(frame);
            // Check if selected device is sending information
            if(frame.empty()){
                cout<<"NULL frame ";
                break;
            }
            rectangle(frame,ROI,red,5);
            Hand h1(frame(ROI), background);
            imshow("Camera Feed", frame);
            imshow("Region of Interest", h1.returnImage());
            // Read key board input, setting esc as break key
            if(waitKey(10)== 27){
                break;
            }

             if(waitKey(10)== 32){
                cout<<"Background Erased"<<endl;
                hasBackground = false;
            }
        }

        if (!hasBackground){
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

            if(waitKey(10)== 32){
                frame(ROI).copyTo(background);
                cout<<"Background Registered"<<endl;
                hasBackground = true;
            }
        }

    }
    return 0;
}


