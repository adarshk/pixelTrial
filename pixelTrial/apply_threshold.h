//
//  apply_threshold.h
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/18/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

namespace ppc {
    class Threshold{
        
    public:
        Threshold();
        Threshold(cv::Mat query, cv::Mat thresholded,const int& set_min,const int& set_max,const int& set_type);
        Threshold(const int& set_min,const int& set_max,const int& set_type);
        ~Threshold();
        void set_minimum_threshold(const int& set_min);
        int  get_minimum_threshold() const;
        void set_maximum_threshold(const int& set_max);
        int  get_maximum_threshold() const;
        void set_min_max(const int& set_min,const int& set_max);
        void set_image(cv::Mat query);
        void set_threshold_image(cv::Mat thresholded);
        void set_threshold_type(int set_type);
        int  get_threshold_type() const;
        cv::Mat get_thresholded_image() const;
        cv::Mat get_image() const;
        cv::Mat apply(cv::Mat thresholded);
        cv::Mat apply();
        cv::Mat apply(cv::Mat query, cv::Mat thresholded,int set_min,int set_max,int set_type);
        
    private:
        int minimum_threshold,maximum_threshold,type;
        cv::Mat source,thresholded_image;
    };
}
