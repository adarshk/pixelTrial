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

        
    public:
        cv::Mat findContours(cv::Mat src, std::vector<std::vector<cv::Point>> contours,std::vector<cv::Vec4i> hierarchy);
        
        cv::Mat findContoursAndDrawRotatedRects(cv::Mat src, std::vector<std::vector<cv::Point>> contours,std::vector<cv::Vec4i> hierarchy);
        
    };
}

#endif /* defined(__pixelTrial__FindContours__) */
