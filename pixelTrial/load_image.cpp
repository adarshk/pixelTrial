//
//  load_image.cpp
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/17/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#include "load_image.h"

namespace ppc {
    
    LoadImage::LoadImage():directory_path(""),image_name(""){
        
    }
    LoadImage::LoadImage(const std::string& query_path,const std::string& name) : directory_path(query_path),image_name(name){
        source = cv::imread(directory_path);
    }
    LoadImage::LoadImage(cv::Mat query_image,const std::string& name) : source(query_image),image_name(name){
        
    }
    
    LoadImage::~LoadImage(){
        
    }
    
    const cv::Mat LoadImage::get_image() const{
        return source;
    }
    
    const std::string LoadImage::get_path() const{
        return directory_path;
    }
    
    void LoadImage::set_path(const std::string& query_path){
        directory_path = query_path;
    }
    
    void LoadImage::set_path_and_load_image(const std::string &query_path){
        directory_path = query_path;
        source = cv::imread(directory_path);
    }
    
    void LoadImage::set_image(cv::Mat query_image){
        source = query_image;
    }
    
    void LoadImage::load_image_from_path(){
        source = cv::imread(directory_path);
    }
    
    void LoadImage::show(const std::string& name,int window_size){
        cv::namedWindow(name,window_size);
        cv::imshow(name, source);
    }
    
    void LoadImage::show(int window_size){
        
        assert(image_name!="" && "Set window name before displaying image");
        cv::namedWindow(image_name,window_size);
        cv::imshow(image_name, source);
    }
    
    void LoadImage::set_image_name(const std::string& name){
        image_name = name;
    }
    
    void LoadImage::set_window_size(int window_size){
        current_window_size = window_size;
    }
    
    std::string LoadImage::get_image_name() const{
        return image_name;
    }
    
}