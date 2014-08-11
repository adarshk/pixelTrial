//
//  display_window.h
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/16/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#pragma once

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>


namespace ppc {
    class DisplayWindow{
        
    private:
        std::string window_name;
        int window_size;
        cv::Mat query_img;
        
        //Need to implement drag_bar  - createTrackbar
        void drag_bar();
   
    public:
        DisplayWindow();
        DisplayWindow(cv::Mat& image_to_show);
        DisplayWindow(const std::string& name,cv::Mat& image_to_show,int flags CV_DEFAULT(CV_WINDOW_AUTOSIZE));
        ~DisplayWindow();
        
        void set_window_name(const std::string& swn);
        std::string get_window_name() const;
        void show();
        void show_with_random_name();
        void set_image(cv::Mat& q_img);

    };
}