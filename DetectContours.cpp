//
//  FindContours.cpp
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/10/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#include "DetectContours.h"


namespace ppc {
    cv::Mat findContours(cv::Mat src, std::vector<std::vector<cv::Point>> contours,std::vector<cv::Vec4i> hierarchy){
        
        if (!src.data) {
            std::cout<< "No Image data" << std::endl;
        }
        
        findContours(src, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,cv::Point(0,0));
        
        return src;
    }
}
