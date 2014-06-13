//
//  FindContours.cpp
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/10/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#include "DetectContours.h"


namespace ppc {
    
    std::vector<std::vector<cv::Point>> DetectContours::contours;
    std::vector<cv::Vec4i> DetectContours::hierarchy;
    
    
    /*
    cv::vector<std::vector<cv::Point>> findContours(cv::Mat src, std::vector<std::vector<cv::Point>> cntrs,std::vector<cv::Vec4i> hrchy){
        
        if (!src.data) {
            std::cout<< "No Image data" << std::endl;
        }
        
        findContours(src, cntrs, hrchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,cv::Point(0,0));
        
        return cntrs;
    }
     */
    
    
    
    void DetectContours::printHierarchy(){
        
        for (std::vector<cv::Vec4i>::iterator itr = nonStaticHierarchy.begin(); itr!=nonStaticHierarchy.end(); ++itr) {
            std::cout  << *itr << std::endl;
                 }
    }
    
    
    cv::vector<std::vector<cv::Point>> DetectContours::findContours(){
        
        if (!query.data) {
            std::cout<< "No Image data" << std::endl;
        }
        
        cv::findContours(query, nonStaticContours, nonStaticHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,cv::Point(0,0));
        
    return nonStaticContours;
    
    }
   
    cv::Mat DetectContours::drawContours(){
        
        result = cv::Mat::zeros(query.size(), CV_8UC3);
        
        for (size_t i=0; i<nonStaticContours.size(); i++) {
            cv::drawContours(result, nonStaticContours, (int)i, cv::Scalar(255),2,8,hierarchy,0,cv::Point());
        }
        
        
        
    return result;
    
    }
    
    void DetectContours::formRectangles(){
        
        
        minRect.resize(nonStaticContours.size());
        
        for (size_t i=0; i<nonStaticContours.size(); i++) {
            minRect[i]  = minAreaRect(cv::Mat(nonStaticContours[i]));
        }
    }
    
    cv::Mat DetectContours::drawRotatedRects(){
        
        DetectContours::formRectangles();
        
        result = cv::Mat::zeros(query.size(), CV_8UC3);
        cv::Point2f rectPoints[4];
        
        for (size_t i=0; i<nonStaticContours.size(); i++) {
            cv::drawContours(result, nonStaticContours, (int)i, cv::Scalar(255),2,8,hierarchy,0,cv::Point());
            
            minRect[i].points(rectPoints);
            
            for (int j=0; j<4; j++) {
                cv::line(result, rectPoints[j], rectPoints[(j+1)%4], cv::Scalar(0,0,255),1,8);
            }
        }
        
        
        return result;
    }
    
}
