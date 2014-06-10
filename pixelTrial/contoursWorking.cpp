/*

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;


int threshold_value = 100;
int const max_value = 255;
int const max_BINARY_value = 150;
bool const showImg = true;

Mat img1,img2,greyImg,thresholdedImg;
//Mat img1,img2,greyImg,thresholdedImg;
const char* window_name = "Threshold";



void Threshold(int,void*);
void showImage(Mat);

int main(int argc, const char * argv[])
{
    
    if (argc != 3) {
        cout << "Image arguments missing" << endl;
        return -1;
    }
    
    img1 = imread(argv[1],CV_LOAD_IMAGE_COLOR);
    img2 = imread(argv[2],CV_LOAD_IMAGE_COLOR);
    
    
    if(!img1.data || !img2.data ){
        cout << "Error -  images loaded" << endl;
        return -1;
    }
    
    
    
    cvtColor(img1, greyImg, COLOR_RGB2GRAY);
    
    
    namedWindow(window_name,CV_WINDOW_NORMAL);
    
    
    createTrackbar( "Value",window_name, &threshold_value,max_value, Threshold );
    
    Threshold(0, 0);
    
    //imwrite("newImg.jpeg", imgMatches);
    
    //Canny(greyImg, greyImg, 0, 255);
    
    vector<vector<Point>> contours;
    findContours(greyImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    //drawContours(greyImg, contours, -1, Scalar(255),CV_FILLED);
    
//    
//     for (int i =0; i<contours.size(); i++) {
//     if (contours[i].size() > 50) {
//     Rect r = boundingRect(Mat(contours[i]));
//     rectangle(greyImg, r, Scalar(255), 2);
//     }
//     
//     }
    
    
    
    for (vector<vector<Point>>::iterator itr = contours.begin(); itr!=contours.end(); ++itr) {
        Rect r = boundingRect(Mat(*itr));
        rectangle(greyImg, r, Scalar(255), 2);
    }
    
    
    showImage(greyImg);
    
    waitKey(0);
    
    std::cout << "Success!\n";
    
    
    return 0;
}




void CannyFilter(int,void*){
    
    
}

void Threshold (int,void*){
    cout << "Threshold value" << threshold_value << endl;
    
    //threshold( greyImg, thresholdedImg, threshold_value, max_BINARY_value,threshold_type );
    threshold(greyImg, thresholdedImg, threshold_value, max_BINARY_value, 0);
    
    
    
}

void showImage(Mat grImg){
    
    if (showImg) {
        imshow(window_name, grImg);
    }
    
}

*/