//
//  main.cpp
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/3/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

/*

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

using namespace cv;
using namespace std;

int main(int argc, const char * argv[])
{
    
    if (argc != 3) {
        cout << "Image arguments missing" << endl;
        return -1;
    }
    
    Mat img1 = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
    Mat img2 = imread(argv[2],CV_LOAD_IMAGE_GRAYSCALE);
    
    if(!img1.data || !img2.data){
        cout << "Error -  images not in correct format" << endl;
        return -1;
    }
    
    
    int keyPoints = 15;
    
    FastFeatureDetector detector(keyPoints);
    vector<KeyPoint>keypoints1, keypoints2;
    
    detector.detect(img1, keypoints1);
    detector.detect(img2, keypoints2);
    
    FREAK extractor;
    Mat descriptors1,descriptors2;
    extractor.compute(img1, keypoints1, descriptors1);
    extractor.compute(img2, keypoints2, descriptors2);
    
    
    BruteForceMatcher<Hamming> matcher;
    vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);
    
    namedWindow("matches",CV_WINDOW_KEEPRATIO);
    Mat imgMatches;
    drawMatches(img1, keypoints1, img2, keypoints2, matches, imgMatches);
    imshow("matches", imgMatches);
    waitKey(0);
    
    std::cout << "Success!\n";
    return 0;
}

*/