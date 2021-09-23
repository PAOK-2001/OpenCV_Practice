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
Scalar red   = Scalar(0,0,255);
Scalar green = Scalar(0,255,0);

class Hand{
private:
    Mat image;
    int handID;
    vector<vector<Point>> countours;
    vector<Vec4i> hierchy;
    int fingers;
    bool isHand;
public:
    Hand(Mat frame);
    Hand(Mat frame, Mat background);
    void drawHand(Mat frame);
    void getHand();
    Mat returnImage();
};

Hand::Hand(Mat frame){
    Mat contrast;
    frame = 1.45*frame; //Aument saturation by multiplying integer
    blur(frame,contrast,Size(3,3));
    cvtColor(contrast, contrast, COLOR_BGR2GRAY);
    threshold(contrast, image,255,255,THRESH_OTSU);
}

Hand::Hand(Mat frame, Mat background){
    Mat contrast;
    Mat noise, fmask;
    frame = 1.35*frame; //Aument saturation by multiplying integer
    absdiff(background,frame, contrast);
    //Set pixels bellow 15 (detected as noise) to black
    noise = abs(contrast)<15;
    noise = 0;
    bitwise_or(contrast,noise,contrast);
    // Blur Image
    blur(contrast,contrast,Size(7,7));
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

void Hand::getHand(){
    
    findContours(image,countours,hierchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    double largestArea = 0;
    for (int i = 0; i < countours.size(); i++){
        if(contourArea(countours[i])>largestArea){
            handID = i;
            largestArea = contourArea(countours[i]);        }
    }
}

void Hand::drawHand(Mat frame){
    drawContours(frame, countours, -1,green,5,8,hierchy);
    drawContours(image,countours,-1,red,7,8,hierchy);

    
}

int main() {
    bool hasBackground = false;
    // Create OpenCV frame object to store frame information
    Mat frame;
    Mat Region;
    Mat background;
    Scalar color;
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
        if (hasBackground){
            color = green;
            camera.read(frame);
            // Check if selected device is sending information
            if(frame.empty()){
                cout<<"NULL frame ";
                break;
            }
            rectangle(frame,ROI,color,5);
            Hand h1(frame(ROI), background);
            h1.getHand();
            h1.drawHand(frame);
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
            color = red;
            camera.read(frame);
            // Check if selected device is sending information
            if(frame.empty()){
                cout<<"NULL frame ";
                break;
            }
            putText(frame,"No background!",Point(10,450),FONT_HERSHEY_TRIPLEX,1,color,3);
            rectangle(frame,ROI,color,5);
            Hand h1(frame(ROI));
            imshow("Camera Feed", frame);
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
