//
//  Edges.h
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/10/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
namespace ppc {
    
    class Edges{
        
    private:
        cv::Mat result_img;
        cv::Mat source,result;
        int lower_threshold,upper_threshold,aperture_size;
        
    public:
        Edges();
        Edges(const int& set_lower,const int& set_upper);
        Edges(cv::Mat query,cv::Mat result_image,const int& set_lower,const int& set_upper);
        ~Edges();
        Edges& set_source_image(cv::Mat query);
        const cv::Mat get_source_image() const;
        const cv::Mat get_result_image() const;
        Edges& set_lower_threshold(const int& set_lower);
        int get_lower_threshold() const;
        Edges& set_upper_threshold(const int& set_upper);
        int get_upper_threshold() const;
        Edges& set_aperture_size(int aperture);
        Edges& applyCanny();
        Edges& applyCanny(cv::Mat query,cv::Mat result_image,const int& set_lower,const int& set_upper);
    };
}


