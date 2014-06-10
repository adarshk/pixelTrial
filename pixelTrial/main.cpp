
 #include <iostream>
 #include <stdio.h>
 #include <stdlib.h>
 #include <opencv2/opencv.hpp>
 #include <opencv2/highgui/highgui.hpp>
 #include <opencv2/imgproc/imgproc.hpp>
 #include <string.h>
#include "findEdges.h"
 
 
 using namespace cv;
 using namespace std;
using namespace ppc;

 Mat query,queryGreyscale,thresholdResult,testImg,testImgGrayscale;
 RNG rng(12345);
 int thresh(100);
 int maxThreshold(255);
 
 void detectRegions(int,void*);
 
 int main(int argc,char** argv){
 
 query = imread(argv[1],IMREAD_COLOR);
 testImg = imread(argv[1],IMREAD_COLOR);
 
 cvtColor(query, queryGreyscale, COLOR_BGR2GRAY);
 blur(queryGreyscale, queryGreyscale, Size(3,3));
     
 cvtColor(testImg, testImgGrayscale, COLOR_BGR2GRAY);
 blur(testImgGrayscale, testImgGrayscale, Size(3,3));
     Mat cannyClassResult;
     
     FindEdges fe;
     //FindEdges fe(testImg);
     //testImg = fe.cannyEdges();
     cannyClassResult = fe.cannyEdges(testImgGrayscale);
 
     namedWindow("testImg",CV_WINDOW_AUTOSIZE);
     imshow("testImg", cannyClassResult);
     
 String queryWindow = "Query";
 namedWindow(queryWindow,CV_WINDOW_AUTOSIZE);
 //imshow(queryWindow, query);
 //createTrackbar( "Threshold",queryWindow, &thresh,maxThreshold, detectRegions );
 
 detectRegions(0,0);
 
 waitKey(0);
 cout<<"success"<<endl;
 
 }
 
 
 void detectRegions(int,void*){
 
 
 Mat cannyResult,thresholdResult;
 vector<vector<Point>> contours;
 
 vector<Vec4i> hierarchy;
     
 Point2f rectPoints[4];
 
 Canny(queryGreyscale, cannyResult, thresh, 255);
 //threshold(queryGreyscale, thresholdResult, thresh, 200, CV_THRESH_BINARY_INV);
 findContours(cannyResult, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,Point(0,0));
 
     cout<< "contours Size - "<<contours.size() << endl;
     
     
     vector<RotatedRect> minRect(contours.size());
     
     for (size_t i=0; i<contours.size(); i++) {
         minRect[i]  = minAreaRect(Mat(contours[i]));
     }
     
     Mat drawBoundries = Mat::zeros(cannyResult.size(), CV_8UC3);
 for (size_t i=0; i<contours.size(); i++) {
    //Scalar color = Scalar(rng.uniform(0, 255),rng.uniform(0, 255),rng.uniform(0, 255));
    drawContours(drawBoundries, contours, (int)i, Scalar(255),2,8,hierarchy,0,Point());
     
     minRect[i].points(rectPoints);
     
     for (int j=0; j<4; j++) {
         line(drawBoundries, rectPoints[j], rectPoints[(j+1)%4], Scalar(0,0,255),1,8);
     }
     
 }
     
     for (vector<Vec4i>::iterator itr = hierarchy.begin(); itr!=hierarchy.end(); ++itr) {
         
         cout  << *itr << endl;
     }

     
     
     Point2f perspectiveOutput[4];
     Mat pOut = getPerspectiveTransform(rectPoints, perspectiveOutput);
     Mat unWarped;
     warpPerspective(queryGreyscale, unWarped, pOut, cannyResult.size());

// namedWindow("Contours",CV_WINDOW_AUTOSIZE);
// imshow("Contours", drawBoundries);
//     
//     namedWindow("unWarp",CV_WINDOW_AUTOSIZE);
//     imshow("unWarp", unWarped);
 
 }
 