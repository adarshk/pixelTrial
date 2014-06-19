
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "find_edges.h"
#include "detect_contours.h"
#include "display_window.h"
#include "load_image.h"
#include "apply_threshold.h"


using namespace cv;
using namespace std;
using namespace ppc;

Mat query,queryGreyscale,thresholdResult,testImg,testImgGrayscale;
RNG rng(12345);
int thresh(100);
int maxThreshold(255);

Mat src_image,src_image_grayscale,src_image_threshold,src_image_edges,src_image_contours;
const bool showImg = true;


void detectRegions(int,void*);
void ApplyThreshold(int,void*);
void ShowImage(Mat im);
void HoughTransform(Mat dst);

Threshold th(thresh,maxThreshold,1);
//Threshold th;
Edges fe;
Contours ct,sit;
Mat contour_test;

int main(int argc,char** argv){
    
    
    
    //string dir = argv[2] + string("/layout4/") + "layout4.png";
    string dir = argv[2] + string("/bootstrap1/") + "bootstrap1.png";
    cout << "dir - " << dir << endl;
    
    
    LoadImage li(dir,"Layout1");
    src_image = li.get_image();
    //    src_image = imread(dir);
    if(!src_image.data){
        cout << "No image data" <<endl;
    }

    li.show();
    
    
//     or use this method if theres no need to pass in the directory or if theres only an image and no directory
//    LoadImage li1;
//    li1.set_image(src_image);
//    li1.set_image_name("Layout1");
//    li1.show();
    
    //DisplayWindow dw("Layout1", src_image,CV_WINDOW_AUTOSIZE);
    //dw.show();
    
    cvtColor(src_image, src_image_grayscale, COLOR_BGR2GRAY);
    //blur(src_image_grayscale, src_image_grayscale, Size(3,3));
    GaussianBlur(src_image_grayscale,src_image_grayscale,Size(3,3),0);
    
    
    th.set_source_image(src_image_grayscale);
    ct.set_contour_mode(CV_RETR_TREE);
    ct.set_approximaiton_type(CV_CHAIN_APPROX_SIMPLE);
    sit.set_contour_mode(CV_RETR_EXTERNAL);
    sit.set_approximaiton_type(CV_CHAIN_APPROX_NONE);

    
    
    
    namedWindow("Detected Contours",CV_WINDOW_AUTOSIZE);
    namedWindow("contourTest",CV_WINDOW_AUTOSIZE);
    createTrackbar( "Threshold","contourTest", &thresh,maxThreshold, ApplyThreshold );
    ApplyThreshold(0, 0);
    
    //    ShowImage(src_image_threshold);
    /*
     vector<vector<Point>> contours;
     findContours(src_image_threshold, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
     
     for (vector<vector<Point>>::iterator itr = contours.begin(); itr!=contours.end(); ++itr) {
     Rect r = boundingRect(Mat(*itr));
     rectangle(src_image_threshold, r, Scalar(255), 2);
     }
     
     //    imshow("Detected Contours", src_image_threshold);
     
     //    imshow("Detected Contours", src_image_threshold);
     //    ShowImage(src_image_threshold);
     */
     
     
     /*
     DetectContours detect_contours(src_image_edges);
     vector<vector<Point>> detected_contours;
     detected_contours = detect_contours.findContours();
     src_image_contours = detect_contours.drawRotatedRects();
    */
    waitKey(0);
    cout<<"success"<<endl;
    
}

void ApplyThreshold(int,void*){
    
    cout << "thresh - " << thresh << endl;
    
    
    th.set_minimum_threshold(thresh);
    src_image_threshold = th.apply();
    
    //threshold(src_image_grayscale, src_image_threshold, thresh, maxThreshold, CV_THRESH_BINARY_INV);
    
    fe.set_lower_threshold(thresh);
    fe.set_source_image(src_image_threshold);
    src_image_edges = fe.applyCanny();
    
    HoughTransform(src_image_edges);
    
    
    contour_test = ct.set_source_image(src_image_edges).find().draw().draw_rotated_rectangles().get_result_image();
    
    vector<cv::Vec4i> contour_hierarchy;
    contour_hierarchy = ct.get_hierarchy();
    for (std::vector<cv::Vec4i>::iterator itr = contour_hierarchy.begin(); itr!=contour_hierarchy.end(); ++itr) {
        std::cout  << *itr << std::endl;
        
    }
    
    cout << "new hierarchy - " << endl;
    
    
    imshow("contourTest", contour_test);
    
    src_image_edges = sit.set_source_image(src_image_edges).find_without_hierarchy().draw_rectangles().get_result_image();


    imshow("Detected Contours", src_image_edges);
    
    namedWindow("Thresh",CV_WINDOW_AUTOSIZE);
    imshow("Thresh", src_image_threshold);
}

void HoughTransform(Mat dst){
    
    Mat cdst;
    cvtColor(dst, cdst, CV_GRAY2BGR);
    
    vector<Vec2f> lines;
    // detect lines
    HoughLines(dst, lines, 1, CV_PI/180, 150, 0, 0 );
    
    // draw lines
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
    }
    
    namedWindow("HoughLines",CV_WINDOW_AUTOSIZE);
    imshow("HoughLines", cdst);
}

void ShowImage(Mat im){
    
    if (showImg) {
        imshow("Detected Contours", im);
    }
    
}

void detectRegions(int,void*){
    
    
    
    // hierarchy
    
    
    //     for (vector<Vec4i>::iterator itr = hierarchy.begin(); itr!=hierarchy.end(); ++itr) {
    //            cout  << *itr << endl;
    //     }
    
    //Warping image -- still work to do
    
    /*
     
     Point2f perspectiveOutput[4];
     Mat pOut = getPerspectiveTransform(rectPoints, perspectiveOutput);
     Mat unWarped;
     warpPerspective(queryGreyscale, unWarped, pOut, cannyResult.size());
     
     // namedWindow("Contours",CV_WINDOW_AUTOSIZE);
     // imshow("Contours", drawBoundries);
     //
     //     namedWindow("unWarp",CV_WINDOW_AUTOSIZE);
     //     imshow("unWarp", unWarped);
     */
    
}
