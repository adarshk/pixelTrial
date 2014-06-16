
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string.h>
#include "findEdges.h"
#include "DetectContours.h"


using namespace cv;
using namespace std;
using namespace ppc;

Mat query,queryGreyscale,thresholdResult,testImg,testImgGrayscale;
RNG rng(12345);
int thresh(100);
int maxThreshold(255);

Mat src_image,src_image_grayscale,src_image_threshold,src_image_edges,src_image_contours;
const bool showImg = true;
FindEdges find_edges;

void detectRegions(int,void*);
void ApplyThreshold(int,void*);
void ShowImage(Mat im);
void HoughTransform(Mat dst);

int main(int argc,char** argv){
    
    

    //string dir = argv[2] + string("/layout4/") + "layout4.png";
    string dir = argv[2] + string("/bootstrap2/") + "bootstrap2.png";
    cout << "dir - " << dir << endl;
    src_image = imread(dir);
    if(!src_image.data){
        cout << "No image data" <<endl;
    }
    namedWindow("Layout1",CV_WINDOW_AUTOSIZE);
    imshow("Layout1", src_image);
    
    cvtColor(src_image, src_image_grayscale, COLOR_BGR2GRAY);
    //blur(src_image_grayscale, src_image_grayscale, Size(3,3));
    GaussianBlur(src_image_grayscale,src_image_grayscale,Size(3,3),0);
    
    
    
    
    
    namedWindow("Detected Contours",CV_WINDOW_AUTOSIZE);
    createTrackbar( "Threshold","Detected Contours", &thresh,maxThreshold, ApplyThreshold );
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
    
    
    
    /*
    DetectContours detect_contours(src_image_edges);
    vector<vector<Point>> detected_contours;
    detected_contours = detect_contours.findContours();
    src_image_contours = detect_contours.drawRotatedRects();
    
    
    
    
    
    
    /*
    
    query = imread(argv[1],IMREAD_COLOR);
    testImg = imread(argv[1],IMREAD_COLOR);
    
    cvtColor(query, queryGreyscale, COLOR_BGR2GRAY);
    blur(queryGreyscale, queryGreyscale, Size(3,3));
    
    cvtColor(testImg, testImgGrayscale, COLOR_BGR2GRAY);
    blur(testImgGrayscale, testImgGrayscale, Size(3,3));
    Mat cannyClassResult,contour_result;
    
    FindEdges fe;
    //FindEdges fe(testImg);
    //testImg = fe.cannyEdges();
    cannyClassResult = fe.cannyEdges(testImgGrayscale);
    
    
    vector<vector<Point>> cntrs;
    vector<Vec4i> hrchy;
    DetectContours dc(cannyClassResult);
    //DetectContours dc(cannyClassResult,cntrs,hrchy);
    //cntrs = dc.findContours(cannyClassResult, cntrs, hrchy);
    cntrs = dc.findContours();
    contour_result = dc.drawRotatedRects();
    dc.printHierarchy();
    //contourClass = dc.drawContours();
    cout<< "cntrs Size - "<<cntrs.size() << endl;
    
    namedWindow("testImg",CV_WINDOW_AUTOSIZE);
    imshow("testImg", testImg);
    
    namedWindow("Contours",CV_WINDOW_AUTOSIZE);
    imshow("Contours", contour_result);
    
    String queryWindow = "Query";
    // namedWindow(queryWindow,CV_WINDOW_AUTOSIZE);
    //imshow(queryWindow, query);
    //createTrackbar( "Threshold",queryWindow, &thresh,maxThreshold, detectRegions );
    
    // detectRegions(0,0);
    
     */
    waitKey(0);
    cout<<"success"<<endl;
    
}

void ApplyThreshold(int,void*){
    
    cout << "thresh - " << thresh<<endl;
    
    threshold(src_image_grayscale, src_image_threshold, thresh, maxThreshold, CV_THRESH_BINARY_INV);
    
    src_image_edges = find_edges.cannyEdges(src_image_threshold,thresh);
    
    HoughTransform(src_image_edges);
    
    vector<vector<Point>> contours;
    findContours(src_image_edges, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    
    for (vector<vector<Point>>::iterator itr = contours.begin(); itr!=contours.end(); ++itr) {
        Rect r = boundingRect(Mat(*itr));
        rectangle(src_image_edges, r, Scalar(255), 2);
    }
    
//    namedWindow("Detected Contours",CV_WINDOW_AUTOSIZE);
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
