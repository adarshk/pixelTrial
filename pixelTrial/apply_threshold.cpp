//
//  apply_threshold.cpp
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/18/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//


#ifndef NDEBUG
#define Check(x) assert(x)
#define CheckWithMessage(str,x) assert((str,x))
#endif

#include "apply_threshold.h"

namespace ppc {
    Threshold::Threshold() : minimum_threshold(0),maximum_threshold(255),type(0){
        
    }
    
    Threshold::Threshold(const int& set_min,const int& set_max,const int& set_type) : minimum_threshold(set_min),maximum_threshold(set_max),type(set_type){
        
    }
    
    Threshold::Threshold(cv::Mat query, cv::Mat thresholded,const int& set_min,const int& set_max,const int& set_type) : source(query),thresholded_image(thresholded),minimum_threshold(set_min),maximum_threshold(set_max),type(set_type){
        
    }
    
    Threshold::~Threshold(){
        
    }
    
    void Threshold::set_minimum_threshold(const int& set_min){
        Check(set_min>=0);
        Check(set_min<=255);
        minimum_threshold = set_min;
    }
    
    void Threshold::set_maximum_threshold(const int& set_max){
        Check(set_max<=255);
        Check(set_max>=0);
        maximum_threshold = set_max;
    }
    
    cv::Mat Threshold::apply(cv::Mat thresholded){
        Check(source.data);
        cv::threshold(source, thresholded, minimum_threshold, maximum_threshold, type);
        return thresholded;
    }
    
    cv::Mat Threshold::apply(){
        CheckWithMessage(std::string("Source image data not available"), source.data);
        //        assert(("Image data not available", !source.data ));
        cv::threshold(source, thresholded_image, minimum_threshold, maximum_threshold, type);
        return thresholded_image;
    }
    
    cv::Mat Threshold::apply(cv::Mat query, cv::Mat thresholded,int set_min,int set_max,int set_type){
        CheckWithMessage(std::string("Source image data not available"), source.data);
        thresholded_image = thresholded;
        minimum_threshold = set_min;
        maximum_threshold = set_max;
        type = set_type;
        cv::threshold(source, thresholded_image, minimum_threshold, maximum_threshold, type);
        return thresholded_image;
    }
    
    int Threshold::get_minimum_threshold() const{
        return minimum_threshold;
    }
    
    int Threshold::get_maximum_threshold() const{
        return maximum_threshold;
    }
    void Threshold::set_image(cv::Mat query){
        source = query;
    }
    
    cv::Mat Threshold::get_image() const{
        return source;
    }
    
    void Threshold::set_threshold_image(cv::Mat thresholded){
        thresholded_image = thresholded;
    }
    
    cv::Mat Threshold::get_thresholded_image() const{
        return thresholded_image;
    }
    
    void Threshold::set_threshold_type(int set_type){
        type = set_type;
    }
    int Threshold::get_threshold_type() const{
        return type;
    }
    
    void Threshold::set_min_max(const int& set_min,const int& set_max){
        CheckWithMessage(std::string("Threshold should be >=0 or =< 255"), set_min>=0);
        CheckWithMessage(std::string("Threshold should be >=0 or =< 255"), set_min<=255);
        CheckWithMessage(std::string("Threshold should be >=0 or =< 255"), set_max>=0);
        CheckWithMessage(std::string("Threshold should be >=0 or =< 255"), set_max<=255);
        
        minimum_threshold = set_min;
        maximum_threshold = set_max;
    }
    
}