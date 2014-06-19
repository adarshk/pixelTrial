//
//  FindContours.h
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/10/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

namespace ppc{
    
    class Contours{
        
    private:
        cv::Mat source,result;
        int mode,approximation;
        std::vector<cv::Vec4i> hierarchy;
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::RotatedRect> min_rectangles;
        void form_rectangles();
        

        
    public:
        Contours();
        Contours(cv::Mat query,int set_mode,int set_aproximation);
        Contours(int set_mode,int set_aproximation);
        ~Contours();
        
//        void set_contours(std::vector<std::vector<cv::Point>> set_contours);
        const std::vector<std::vector<cv::Point>> get_contours() const;
//        void set_hierarchy(std::vector<cv::Vec4i> set_hierarchy);
        const std::vector<cv::Vec4i> get_hierarchy() const;
        Contours& set_source_image(cv::Mat query);
        const cv::Mat get_source_image() const;
        const cv::Mat get_result_image() const;
        void set_contour_mode(int set_mode);
        void set_approximaiton_type(int set_approximation);
        Contours& find();
        Contours& find_without_hierarchy();
        Contours& draw();
        Contours& draw_without_hierarchy();
        Contours& draw_rotated_rectangles();
        Contours& draw_rectangles();
    };
}

