//
//  watershed_marker.h
//  pixelTrial
//
//  Created by Adarsh Kosuru on 7/3/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

namespace ppc{
    
    class WatershedMarker{
    private:
        cv::Mat markers;
    public:
        
        void set_markers(const cv::Mat& marker_image){
            marker_image.convertTo(markers, CV_32S);
        }
        
        cv::Mat apply_watershed(const cv::Mat& source_image){
            cv::watershed(source_image, markers);
            markers.convertTo(markers, CV_8U);
            return markers;
        }
    };
}


