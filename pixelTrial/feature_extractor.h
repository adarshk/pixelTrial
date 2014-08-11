//
//  feature_extractor.h
//  pixelTrial
//
//  Created by Adarsh Kosuru on 6/20/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

namespace ppc {
    class FeatureExtractor{
    public:
        FeatureExtractor();
        FeatureExtractor(cv::Mat& query,int no_of_detector_points);
        ~FeatureExtractor();
        void detect_orb();
        void extract_orb();
        std::vector<cv::KeyPoint> get_keypoints();
        cv::Mat get_descriptors();
        void set_source_image(cv::Mat& query);
        void set_no_of_detector_points(int no_of_detector_points);
        int get_no_of_detector_points() const;
    private:
        int detector_points;
        std::vector<cv::KeyPoint> keypoints;
        cv::Mat descriptors,source;
    };
}