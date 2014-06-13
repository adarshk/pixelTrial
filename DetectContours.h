//
//  FindContours.h
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/10/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#ifndef __pixelTrial__FindContours__
#define __pixelTrial__FindContours__

#include <iostream>
#include <opencv2/opencv.hpp>

namespace ppc{
    
    class DetectContours{
        
    private:
        cv::Mat query,result;
        //std::vector<cv::Vec4i> hierarchy;
        //std::vector<std::vector<cv::Point>> contours;
        
         static std::vector<std::vector<cv::Point>> contours;
         static std::vector<cv::Vec4i> hierarchy;
        
        std::vector<std::vector<cv::Point>> nonStaticContours;
        std::vector<cv::Vec4i> nonStaticHierarchy;
        std::vector<cv::RotatedRect> minRect;
        void formRectangles();

        
    public:
        DetectContours(cv::Mat src, std::vector<std::vector<cv::Point>> cntrs = contours,std::vector<cv::Vec4i> hrchy = hierarchy){
            
            query = src;
            nonStaticContours = cntrs;
            nonStaticHierarchy = hrchy;
            
        }
        
        cv::vector<std::vector<cv::Point>> findContours();
        cv::Mat drawContours();
        cv::Mat drawRotatedRects();
        void printHierarchy();

        
        
        //cv::vector<std::vector<cv::Point>> findContours(cv::Mat src, std::vector<std::vector<cv::Point>> cntrs,std::vector<cv::Vec4i> hrchy);
//        cv::vector<std::vector<cv::Point>> findContours(cv::Mat src, std::vector<std::vector<cv::Point>> cntrs = contours,std::vector<cv::Vec4i> hrchy = hierarchy);
        
//        cv::Mat findContoursAndDrawRotatedRects(cv::Mat src, std::vector<std::vector<cv::Point>> contours,std::vector<cv::Vec4i> hierarchy);
        
    };
}

#endif /* defined(__pixelTrial__FindContours__) */
