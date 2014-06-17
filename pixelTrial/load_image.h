//
//  load_image.h
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/17/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#pragma once

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

namespace ppc {
    class LoadImage{
        
    public:
        LoadImage(const std::string& query_path,const std::string& name);
        LoadImage(cv::Mat query_image,const std::string& name);
        LoadImage();
        ~LoadImage();
        const cv::Mat get_image() const;
        const std::string get_path() const;
        void set_path(const std::string& query_path);
        void set_path_and_load_image(const std::string& query_path);
        void load_image_from_path();
        void set_image(cv::Mat query_image);
        void show(const std::string& name,int window_size = cv::WINDOW_AUTOSIZE);
        void show(int window_size = cv::WINDOW_AUTOSIZE);
        void set_image_name(const std::string& name);
        std::string get_image_name() const;
        
    private:
        cv::Mat source;
        std::string directory_path;
        std::string image_name;
    };
}


