//
//  display_window.h
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/16/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#ifndef __pixelTrial__display_window__
#define __pixelTrial__display_window__

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#endif /* defined(__pixelTrial__display_window__) */

namespace ppc {
    class DisplayWindow{
        
    private:
        std::string window_name;
        int window_size;
        cv::Mat query_img;
        
        //Need to implement drag_bar  - createTrackbar
        void drag_bar();
    public:
        
        DisplayWindow(const std::string& name,cv::Mat image_to_show,int flags CV_DEFAULT(CV_WINDOW_AUTOSIZE)) : window_name(name),window_size(flags),query_img(image_to_show){
        }
        ~DisplayWindow();
        
        void set_window_name(const std::string& swn);
        std::string get_window_name() const;
        void show();

    };
}