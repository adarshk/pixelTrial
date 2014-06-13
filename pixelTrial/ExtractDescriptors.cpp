//
//  ExtractDescriptors.cpp
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/11/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#include "ExtractDescriptors.h"

namespace ppc {
    cv::Mat ExtractDescriptors::detectFeatures(cv::Mat src,std::string featureDetector,int noOfPoints){
        
        
        return src;
    }
    
    std::vector<cv::KeyPoint> ExtractDescriptors::detectFeatures(Features f,int noOfPoints){
        
        if (!query.data) {
            std::cout<< "No Image data" << std::endl;
            return query;
        }
        
        switch(f){
                
            case ORB_F:
                OrbFeatures(noOfPoints);
                break;
                
            case FAST:
                break;
                
            case SURF_F:
                break;
                
            default:
                std::cout << "Feature Detector not supported";
                break;
                
        }
        
        return query;
        
    }
    
    void ExtractDescriptors::OrbFeatures(int noOfPoints){
        
        cv::OrbFeatureDetector detector(noOfPoints);
        detector.detect(query, keypoints);
        
        
    }
    
    
    cv::Mat ExtractDescriptors::identifyDescriptors(Descriptors d){
        
        if (!query.data) {
            std::cout<< "No Image data" << std::endl;
            return query;
        }
        
        switch(d){
                
            case ORB_D:
                DescExc();
                break;
                
            case FREAK:
                break;
                
            case SURF_D:
                break;
                
            default:
                std::cout << "Descriptor not supported";
                break;
                
        }
        
        return descriptors;
    }
    
    cv::Mat ExtractDescriptors::DescExc(){
        
        cv::OrbDescriptorExtractor extractor;
        extractor.compute(query, keypoints, descriptors);
        
        return descriptors;
        
    }
    
    std::vector<cv::DMatch> ExtractDescriptors::returnMatches(cv::Mat compareDesc){
        
        cv::BFMatcher matcher(cv::NORM_L2);
        matcher.match(descriptors, compareDesc, matches);
        findDist();
        return matches;
        
    }
    
    void ExtractDescriptors::findDist(){
        
        for (int i=0; i<descriptors.rows; i++) {
            double dist = matches[i].distance;
            if(dist< minDist) minDist = dist;
            if (dist > maxDist) maxDist = dist;
            
        }
        
    }
    
    
    std::vector<cv::DMatch> ExtractDescriptors::returnGoodMatches(cv::Mat compareDesc){
        for (int i=0; i<descriptors.rows; i++) {
            if (matches[i].distance <= std::max(2*minDist , 0.05)) {
                goodMatches.push_back(matches[i]);
            }
        }
        
        return goodMatches;
    }
    
    cv::Mat ExtractDescriptors::drawMatches(cv::Mat img2,std::vector<cv::KeyPoint>keypoints2){
        
        cv::drawMatches(query, keypoints, img2, keypoints2, goodMatches, result,cv::Scalar::all(-1), cv::Scalar::all(-1),std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
        return result;
    }
}