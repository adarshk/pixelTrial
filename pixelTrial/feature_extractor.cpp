//
//  feature_extractor.cpp
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/20/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#ifndef NDEBUG
#define Check(x) assert(x)
#define CheckWithMessage(str,x) assert((str,x))
#endif

#include "feature_extractor.h"

namespace ppc {
    
    FeatureExtractor::FeatureExtractor():detector_points(5000){}
    FeatureExtractor::FeatureExtractor(cv::Mat query,int no_of_detector_points):source(query),detector_points(no_of_detector_points){}
    FeatureExtractor::~FeatureExtractor(){}
    
    void FeatureExtractor::set_no_of_detector_points(int no_of_detector_points){
        detector_points = no_of_detector_points;
    }
    
    int FeatureExtractor::get_no_of_detector_points() const{
        return detector_points;
    }
    
    void FeatureExtractor::set_source_image(cv::Mat query){
        source = query;
    }
    
    std::vector<cv::KeyPoint> FeatureExtractor::get_keypoints(){
        return keypoints;
    }
    
    cv::Mat FeatureExtractor::get_descriptors(){
        return descriptors;
    }
    
    void FeatureExtractor::detect_orb(){
        CheckWithMessage(std::string("Set a source image using set_source_image before detecting features"), source.data);
        cv::OrbFeatureDetector detector(detector_points);
        detector.detect(source, keypoints);
    }
    
    void FeatureExtractor::extract_orb(){
        CheckWithMessage(std::string("Set a source image using set_source_image before extracting features"), source.data);
        cv::OrbDescriptorExtractor extractor;
        extractor.compute(source, keypoints, descriptors);
    }
    
}