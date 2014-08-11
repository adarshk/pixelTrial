//
//  display_window.cpp
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/16/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#ifndef NDEBUG
#define Check(x) assert(x)
#define CheckWithMessage(str,x) assert((str,x))
#endif

#include "display_window.h"

namespace ppc {
    
    DisplayWindow::DisplayWindow():window_size(CV_WINDOW_AUTOSIZE),window_name(""){
        
    }
    
    DisplayWindow::DisplayWindow(cv::Mat& image_to_show):query_img(image_to_show),window_size(CV_WINDOW_AUTOSIZE),window_name(""){
        
    }
    
    DisplayWindow::DisplayWindow(const std::string& name,cv::Mat& image_to_show,int flags) : window_name(name),window_size(flags),query_img(image_to_show){
    }
    
    DisplayWindow::~DisplayWindow(){
        
    }
    
    void DisplayWindow::show(){
        
        CheckWithMessage(std::string("No image data available. Set image using 'set_image' before calling show"), query_img.data);
        CheckWithMessage(std::string("Set window name using 'set_window_name' before displaying image"), window_name!="");
        
        cv::namedWindow(window_name,window_size);
        cv::imshow(window_name, query_img);
    }
    
    void DisplayWindow::show_with_random_name(){
        
        int r = rand();
        window_name = std::to_string(r);
        
        CheckWithMessage(std::string("No image data available. Set image using 'set_image' before calling show"), query_img.data);
        CheckWithMessage(std::string("Set window name using 'set_window_name' before displaying image"), window_name!="");
        
        cv::namedWindow(window_name,window_size);
        cv::imshow(window_name, query_img);
    }
    
    void DisplayWindow::set_window_name(const std::string& swn){
        window_name = swn;
    }
    
    std::string DisplayWindow::get_window_name() const{
        return window_name;
    }
    
    void DisplayWindow::set_image(cv::Mat &q_img){
        query_img = q_img;
    }
    
    
}