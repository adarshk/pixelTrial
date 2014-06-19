//
//  FindContours.cpp
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/10/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#ifndef NDEBUG
#define Check(x) assert(x)
#define CheckWithMessage(str,x) assert((str,x))
#endif

#include "detect_contours.h"


namespace ppc {
    
    Contours::Contours():mode(0),approximation(0){}
    Contours::Contours(cv::Mat query,int set_mode,int set_aproximation):source(query),mode(0),approximation(0){}
    Contours::Contours(int set_mode,int set_aproximation) : mode(set_mode),approximation(set_aproximation){}
    Contours::~Contours(){}
    
    Contours& Contours::find(){
        CheckWithMessage(std::string("Set a source image using set_source_image method before finding contours"), source.data);
        cv::findContours(source, contours, hierarchy, mode, approximation,cv::Point(0,0));
        return *this;
    }
    
    Contours& Contours::find_without_hierarchy(){
        CheckWithMessage(std::string("Set a source image using set_source_image method before finding contours"), source.data);
        cv::findContours(source, contours, mode, approximation);
        return *this;
    }
    
    Contours& Contours::draw(){
        CheckWithMessage(std::string("Set a source image using set_source_image method before finding contours"), source.data);
        result = cv::Mat::zeros(source.size(), CV_8UC3);
        for (size_t i=0; i<contours.size(); i++) {
            cv::drawContours(result, contours, (int)i, cv::Scalar(255),1,8,hierarchy,0,cv::Point());
        }
        return *this;
    }
    
    Contours& Contours::draw_without_hierarchy(){
        CheckWithMessage(std::string("Set a source image using set_source_image method before finding contours"), source.data);
        result = cv::Mat::zeros(source.size(), CV_8UC3);
        for (size_t i=0; i<contours.size(); i++) {
            cv::drawContours(result, contours, (int)i, cv::Scalar(255),CV_FILLED);
        }
        return *this;
    }
    
    Contours& Contours::draw_rotated_rectangles(){
        CheckWithMessage(std::string("Set a source image using set_source_image method before finding contours"), source.data);
        
        Contours::form_rectangles();
        
        result = cv::Mat::zeros(source.size(), CV_8UC3);
        cv::Point2f rect_points[4];
        
        for (size_t i=0; i<contours.size(); i++) {
            //cv::drawContours(result, contours, (int)i, cv::Scalar(255),2,8,hierarchy,0,cv::Point());
            
            min_rectangles[i].points(rect_points);
            
            for (int j=0; j<4; j++) {
                cv::line(result, rect_points[j], rect_points[(j+1)%4], cv::Scalar(0,0,255),1,8);
            }
        }
        
        return *this;
    }
    
    Contours& Contours::draw_rectangles(){
        CheckWithMessage(std::string("Set a source image using set_source_image method before finding contours"), source.data);
        result = cv::Mat::zeros(source.size(), CV_8UC3);
        
        for (std::vector<std::vector<cv::Point>>::iterator itr = contours.begin(); itr!=contours.end(); ++itr) {
            cv::Rect r = cv::boundingRect(cv::Mat(*itr));
            rectangle(result, r, cv::Scalar(255), 2);
        }
        
        return *this;
    }
    
    void Contours::form_rectangles(){
        
        
        min_rectangles.resize(contours.size());
        
        for (size_t i=0; i<contours.size(); i++) {
            min_rectangles[i]  = cv::minAreaRect(cv::Mat(contours[i]));
        }
    }
    
    
    const std::vector<std::vector<cv::Point>> Contours::get_contours() const{
        return contours;
    }
    const std::vector<cv::Vec4i> Contours::get_hierarchy() const{
        return hierarchy;
    }
    Contours& Contours::set_source_image(cv::Mat query){
        source = query;
        return *this;
    }
    const cv::Mat Contours::get_source_image() const{
        return source;
    }
    const cv::Mat Contours::get_result_image() const{
        return result;
    }
    void Contours::set_contour_mode(int set_mode){
        mode = set_mode;
    }
    void Contours::set_approximaiton_type(int set_approximation){
        approximation = set_approximation;
    }
    
    
}
