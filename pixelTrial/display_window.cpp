//
//  display_window.cpp
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/16/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#include "display_window.h"

namespace ppc {
    
    void DisplayWindow::show(){
        cv::namedWindow(window_name,window_size);
        cv::imshow(window_name, query_img);
    }
    
    DisplayWindow::~DisplayWindow(){
        
    }
    
    void DisplayWindow::set_window_name(const std::string& swn){
        window_name = swn;
    }
    
    std::string DisplayWindow::get_window_name() const{
        return window_name;
    }
    
}