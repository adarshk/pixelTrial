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
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <ImageMagick-6/Magick++.h>

#include "load_image.h"
#include "find_edges.h"
#include "detect_contours.h"
#include "watershed_marker.h"

using namespace std;
using namespace cv;
using namespace tesseract;
using namespace Magick;

namespace ppc {
    class Components{
        
    public:
        Components();
        Components(std::string& dir_path);
        ~Components();
        void init();
        void ocr();
        void mini_ocr(cv::Mat& ocr_image,int j);
        void mini_mini_ocr();
        void read_ocr();
        void do_image_magick();
        void ocr_image_magick();
        void test_method();
        void mini_watershed_for_thresholding();
        void find_test() throw(cv::Exception);
        void chamfer_matching(string src_path,string matching_image) throw(cv::Exception);
        void basic_thresholding_method() throw(cv::Exception);
        void find() throw(cv::Exception);
        void find_watershed() throw(cv::Exception);
        void find_grabcut() throw(cv::Exception);
        void squares_method(cv::Mat& gray0,cv::Mat& gray,std::vector<std::vector<cv::Point>>& contours) throw(cv::Exception);
        void print_values();
        void show_image_windows();
        void save_image(string dir);
        void save_images();
        void set_output_path(string dir);
        static bool mysortfunction(Rect r1,Rect r2);
        Point2f find_intersection(Vec4i a,Vec4i b);
        void sort_corners(vector<Point2f>& corners, Point2f center);
        void rotate_image(Mat& input, double angle,Mat& rotated_image);
        
    private:
        std::string path,output_path;
        
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
        
        //Tesseract
        char *output_text;
        
    };
}