
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
 
 void detectRegions(int,void*);
 
 int main(int argc,char** argv){
 
 query = imread(argv[1],IMREAD_COLOR);
 testImg = imread(argv[1],IMREAD_COLOR);
 
 cvtColor(query, queryGreyscale, COLOR_BGR2GRAY);
 blur(queryGreyscale, queryGreyscale, Size(3,3));
     
 cvtColor(testImg, testImgGrayscale, COLOR_BGR2GRAY);
 blur(testImgGrayscale, testImgGrayscale, Size(3,3));
     Mat cannyClassResult,contourClass;
     
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
     contourClass = dc.drawRotatedRects();
     //contourClass = dc.drawContours();
     cout<< "cntrs Size - "<<cntrs.size() << endl;
 
     namedWindow("testImg",CV_WINDOW_AUTOSIZE);
     imshow("testImg", testImg);
     
     namedWindow("Contours",CV_WINDOW_AUTOSIZE);
     imshow("Contours", contourClass);
     
 String queryWindow = "Query";
// namedWindow(queryWindow,CV_WINDOW_AUTOSIZE);
 //imshow(queryWindow, query);
 //createTrackbar( "Threshold",queryWindow, &thresh,maxThreshold, detectRegions );
 
// detectRegions(0,0);
 
 waitKey(0);
 cout<<"success"<<endl;
 
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
 