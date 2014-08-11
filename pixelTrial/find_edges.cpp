//
//  Edges.cpp
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/10/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#ifndef NDEBUG
#define Check(x) assert(x)
#define CheckWithMessage(str,x) assert((str,x))
#endif

#include "find_edges.h"

namespace ppc {
    
    Edges::Edges() : lower_threshold(0),upper_threshold(255),aperture_size(3){}
    Edges::Edges(const int& set_lower,const int& set_upper) : lower_threshold(set_lower),upper_threshold(set_upper),aperture_size(3){
        
    }
    Edges::Edges(cv::Mat& query,cv::Mat& result_image,const int& set_lower,const int& set_upper):source(query),result(result_image),lower_threshold(set_lower),upper_threshold(set_upper){
        
    }
    Edges::~Edges(){}
    
    Edges& Edges::set_source_image(cv::Mat query){
        source = query;
        return *this;
    }
    const cv::Mat Edges::get_source_image() const{
        return source;
    }
    const cv::Mat Edges::get_result_image() const{
        return result;
    }
    Edges& Edges::set_lower_threshold(const int& set_lower){
        lower_threshold = set_lower;
        return *this;
    }
    int Edges::get_lower_threshold() const{
        return lower_threshold;
    }
    Edges& Edges::set_upper_threshold(const int& set_upper){
        upper_threshold = set_upper;
        return *this;
    }
    int Edges::get_upper_threshold() const{
        return upper_threshold;
    }
    Edges& Edges::set_aperture_size(int aperture){
        aperture_size = aperture;
        return *this;
    }
    
    Edges& Edges::applyCanny(){
        CheckWithMessage(std::string("No image data available. Set source image using set_source_image before finding edges"), source.data);
        
        cv::Canny(source, result, lower_threshold, upper_threshold,this->aperture_size);
        return *this;
    }
    
    Edges& Edges::applyCanny(cv::Mat& query,cv::Mat& result_image,const int& set_lower,const int& set_upper){
        
        source = query;
        result = result_image;
        lower_threshold = set_lower;
        upper_threshold = set_upper;
        CheckWithMessage(std::string("No image data available. Set source image using set_source_image before finding edges"), source.data);
        cv::Canny(source, result, lower_threshold, upper_threshold);
        return *this;
    }
    
}