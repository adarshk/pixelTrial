//
//  ExtractDescriptors.h
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/11/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#ifndef __pixelTrial__ExtractDescriptors__
#define __pixelTrial__ExtractDescriptors__

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

namespace ppc {
    
    enum Features{
        ORB_F,
        FAST,
        SURF_F

    };
    
    enum Descriptors{
        ORB_D,
        FREAK,
        SURF_D
    };
    
    enum Matchers{
        BRUTEFORCE,
        BFM
    };
    
    class ExtractDescriptors{
        
    private:
        void OrbFeatures(int noOfPoints);
        cv::Mat DescExc();
        std::vector<cv::KeyPoint> keypoints;
        cv::Mat query,descriptors,result;
        std::vector<cv::DMatch> matches,goodMatches;
        double maxDist = 0,  minDist = 100;
        void findDist();
        
    public:
        ExtractDescriptors(cv::Mat src){query = src;}
        cv::Mat detectFeatures(cv::Mat src,std::string featureDetector = "Orb",int noOfPoints = 4000);
        std::vector<cv::KeyPoint> detectFeatures(Features f=ORB_F,int noOfPoints = 4000);
        //cv::Mat detectFeatures(cv::Mat src,int f=0,int noOfPoints = 4000);
        cv::Mat identifyDescriptors(Descriptors d=ORB_D);
        std::vector<cv::DMatch> returnMatches(cv::Mat compareDesc);
        std::vector<cv::DMatch> returnGoodMatches(cv::Mat compareDesc);
        std::vector<cv::DMatch> returnGoodMatches(const double& dist);
        cv::Mat drawMatches(cv::Mat img2,std::vector<cv::KeyPoint>keypoints2);
        
    };
}

#endif /* defined(__pixelTrial__ExtractDescriptors__) */
