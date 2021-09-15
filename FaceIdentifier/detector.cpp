// Script that uses OpenCV's pretrained Haar Cascades to detected faces in a Camera Feed
// Import OpenCV modules
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "vector"
#include <iostream>

using namespace std;
using namespace cv;

// detect funtion
// Function that receives a frame, a CascadeClassider target and a Rect vector,
// and stores the instances of the the found object withnin the frame inside the vector,
// and draws a rectangle
void detect(Mat frame, CascadeClassifier target, vector<Rect>Instances){
    Mat grayFrame;
    cvtColor(frame, grayFrame, COLOR_BGR2GRAY );
    //equalizeHist( grayFrame, grayFrame );
    target.detectMultiScale(grayFrame, Instances);
    Scalar color = Scalar(0,0,255);

    for (int i = 0; i < Instances.size(); i++){
        rectangle(frame,Point(Instances[i].x,Instances[i].y),Point(Instances[i].x +Instances[i].width,Instances[i].y +Instances[i].height), color,7);
        imshow("Detector", frame);
    }
}

int main() {
    // Load Haar Cacade data for faces
    CascadeClassifier faces_haar;
    faces_haar.load("/home/paok/Documents/OpenCV_Practice/FaceIdentifier/haarcascade_frontalface_alt2.xml");
    // Create OpenCV frame object to store frame information
    Mat frame;
    // Create VideoCapture object, reading video device (USB camera)
    VideoCapture camera(0);
    vector<Rect>facesID;
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
        detect(frame,faces_haar,facesID);
        // Read key board input, setting esc as break key
        if(waitKey(10)== 27){
            break;
        }
    }
    
    return 0;
}