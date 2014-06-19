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
        int lower_threshold,upper_threshold;
        
    public:
        Edges();
        Edges(const int& set_lower,const int& set_upper);
        Edges(cv::Mat query,cv::Mat result_image,const int& set_lower,const int& set_upper);
        ~Edges();
        void set_source_image(cv::Mat query);
        const cv::Mat get_source_image() const;
        void set_result_image(cv::Mat result_image);
        const cv::Mat get_result_image() const;
        void set_lower_threshold(const int& set_lower);
        int get_lower_threshold() const;
        void set_upper_threshold(const int& set_upper);
        int get_upper_threshold() const;
        cv::Mat applyCanny();
        cv::Mat applyCanny(cv::Mat query,cv::Mat result_image,const int& set_lower,const int& set_upper);
    };
}


