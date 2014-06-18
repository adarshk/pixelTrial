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
    
    Edges::Edges() : lower_threshold(0),upper_threshold(255){}
    Edges::Edges(const int& set_lower,const int& set_upper) : lower_threshold(set_lower),upper_threshold(set_upper){
        
    }
    Edges::Edges(cv::Mat query,cv::Mat result_image,const int& set_lower,const int& set_upper):source(query),result(result_image),lower_threshold(set_lower),upper_threshold(set_upper){
        
    }
    Edges::~Edges(){}
    
    void Edges::set_source_image(cv::Mat query){
        source = query;
    }
    cv::Mat Edges::get_source_image() const{
        return source;
    }
    void Edges::set_result_image(cv::Mat result_image){
        result = result_image;
    }
    cv::Mat Edges::get_result_image() const{
        return result;
    }
    void Edges::set_lower_threshold(const int& set_lower){
        lower_threshold = set_lower;
    }
    int Edges::get_lower_threshold() const{
        return lower_threshold;
    }
    void Edges::set_upper_threshold(const int& set_upper){
        upper_threshold = set_upper;
    }
    int Edges::get_upper_threshold() const{
        return upper_threshold;
    }
    
    cv::Mat Edges::applyCanny(){
        CheckWithMessage(std::string("No image data available"), source.data);
        
        cv::Canny(source, result, lower_threshold, upper_threshold);
        return result;
    }
    
    cv::Mat Edges::applyCanny(cv::Mat query,cv::Mat result_image,const int& set_lower,const int& set_upper){
        
        source = query;
        result = result_image;
        lower_threshold = set_lower;
        upper_threshold = set_upper;
        CheckWithMessage(std::string("No image data available. Set source image using set_source_image before finding edges"), source.data);
        cv::Canny(source, result, lower_threshold, upper_threshold);
        return result;
    }
    
}