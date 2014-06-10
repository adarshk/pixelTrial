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
    
    if (argc != 4) {
        cout << "Image arguments missing" << endl;
        return -1;
    }
    
    Mat img1 = imread(argv[1],CV_LOAD_IMAGE_ANYCOLOR);
    Mat img2 = imread(argv[2],CV_LOAD_IMAGE_ANYCOLOR);
    Mat img3 = imread(argv[3],CV_LOAD_IMAGE_ANYCOLOR);
    
    if(!img1.data || !img2.data || !img3.data){
        cout << "Error -  images not in correct format" << endl;
        return -1;
    }
    
    
    // Orb(4000 or 5000), Orb, BruteForce
    //Surf (400), Surf, Flann
    // Fast (40), Freak, Brute
    
    
    OrbFeatureDetector detector(5000);
    vector<KeyPoint>keypoints1, keypoints2,keypoints3;
    
    detector.detect(img1, keypoints1);
    detector.detect(img2, keypoints2);
    detector.detect(img3, keypoints3);
    
    
    OrbDescriptorExtractor extractor;
    Mat descriptors1,descriptors2,descriptors3;
    extractor.compute(img1, keypoints1, descriptors1);
    extractor.compute(img2, keypoints2, descriptors2);
    extractor.compute(img3, keypoints3, descriptors3);
    
    
    
    BFMatcher matcher(NORM_L2);
    BruteForceMatcher<Hamming> matcher2;
    vector<DMatch> matches,matches2;
    matcher.match(descriptors1, descriptors2, matches);
    matcher2.match(descriptors3, descriptors2, matches2);
    
    double maxDist = 0,  minDist = 100;
    
    for (int i=0; i<descriptors1.rows; i++) {
        double dist = matches[i].distance;
        if(dist< minDist) minDist = dist;
        if (dist > maxDist) maxDist = dist;
        
    }
    
    
    vector<DMatch> good_matches;
    
    for (int i=0; i<descriptors1.rows; i++) {
        if (matches[i].distance <= max(2*minDist , 0.05)) {
            good_matches.push_back(matches[i]);
        }
    }
    
    namedWindow("matches",CV_WINDOW_KEEPRATIO);
    Mat imgMatches,imgMatches2;
    //drawMatches(img1, keypoints1, img2, keypoints2, matches, imgMatches);
    drawMatches(img1, keypoints1, img2, keypoints2, good_matches, imgMatches,Scalar::all(-1), Scalar::all(-1),vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    
    drawMatches(img3, keypoints3, img2, keypoints2, matches2, imgMatches,Scalar::all(-1), Scalar::all(-1),vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    
    
    imshow("matches", imgMatches);
    waitKey(0);
    
    std::cout << "Success!\n";
    return 0;
}

*/