//
//  find_components.h
//  PaperPixelCode
//
//  Created by Adarsh Kosuru on 6/30/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#pragma once

#include <iostream>
#include <map>
#include <opencv2/opencv.hpp>
#include "load_image.h"
#include "find_edges.h"
#include "detect_contours.h"
#include "watershed_marker.h"

using namespace std;
using namespace cv;

namespace ppc {
    class Components{
        
    public:
        Components();
        Components(std::string& dir_path);
        ~Components();
        void find() throw(cv::Exception);
        void find_watershed() throw(cv::Exception);
        void find_grabcut() throw(cv::Exception);
        void squares_method(cv::Mat& gray0,cv::Mat& gray,std::vector<std::vector<cv::Point>>& contours) throw(cv::Exception);
        void print_values();
        void show_image_windows();
        void save_image(string dir);
        static bool mysortfunction(Rect r1,Rect r2);
        Point2f find_intersection(Vec4i a,Vec4i b);
        void sort_corners(vector<Point2f>& corners, Point2f center);
        
    private:
        std::string path;
        
        std::vector<std::vector<cv::Point>> squares;
        vector<cv::Vec4i> squares_hierarchy;
        vector<vector<Point>> needed_contours;
        vector<Rect> all_rectangles,hull_rectangles;
        
        cv::Mat source_image, source_image_resized,source_image_output,source_image_hull,source_image_blurred;
        cv::Mat copied;
        vector<Mat>extracted_images;
        
        int lower_thresh,upper_thresh;
        int counter,extracted_images_counter;
        int no_of_threshold_levels;
        int area,width,height;
        //int area(0),width(0),height(0);
        int window_size;
        
        bool display_extracted_images;
        
        Edges edge_image;
        Contours contours_image;
        
        static double angle(Point pt1, Point pt2, Point pt0);
        
    };
}