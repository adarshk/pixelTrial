//
//  findEdges.h
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/10/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#ifndef __pixelTrial__findEdges__
#define __pixelTrial__findEdges__

#include <iostream>
#include <opencv2/opencv.hpp>
namespace ppc {
    
    class FindEdges{
        
    private:
        cv::Mat result_img;
        
    public:
        cv::Mat cannyEdges(cv::Mat src_img,int lower_threshold=0,int upper_threshold=255);
        
    };
}

#endif /* defined(__pixelTrial__findEdges__) */
