//
//  findEdges.cpp
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/10/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#include "findEdges.h"

namespace ppc {
    
    cv::Mat FindEdges::cannyEdges(cv::Mat src_img,int lower_threshold,int upper_threshold){
    
        if (!src_img.data) {
            std::cout<< "No Image data" << std::endl;
        }
        
        cv::Canny(src_img, result_img, lower_threshold, upper_threshold);
        return result_img;
    }
    
}