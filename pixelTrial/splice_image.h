//
//  splice_image.h
//  pixelTrial
//
//  Created by Adarsh Kosuru on 7/31/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#pragma once

#include <iostream>
#include <map>
#include <algorithm>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <regex>
#include <unistd.h>
#include <utility>
#include <stdio.h>
#include <cstdio>

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/photo/photo.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <ImageMagick-6/Magick++.h>
#include <magick/MagickCore.h>
#include <rapidjson/document.h>
#include <rapidjson/filestream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>

#include "load_image.h"
#include "find_edges.h"
#include "detect_contours.h"
#include "watershed_marker.h"

using namespace std;
using namespace cv;
using namespace tesseract;
using namespace Magick;
using namespace rapidjson;
using namespace MagickCore;

namespace ppc {
    class Splice{
        
    public:
        Splice();
        Splice(const std::string& dir_path);
        ~Splice();
        void init();
        void save_image(const std::string& save_dir,string save_name,Mat& image_name);
        bool write_output;
        
    private:
        void watershed();
        void text_cleaner();
        void image_magick();
        void image_magick_cleanup(string path);
        void run_script();
        void squares_method2(const Mat& original_image,string image_pth,string components_path,string contours_path);
        static bool mysortfunction(Rect r1,Rect r2);
        static double angle(Point pt1,Point pt2,Point pt0);
        void find_squares( const Mat& original_image,const string& image_path, string components_path,string contours_path );
        void tesseract(string path);
        void tesseract_with_dictionary(string path);
        void keypoint_matching();
        void thresholding_image();
        void init_dictionary();
        string dictionary_comparision(string word);
        void parse_word(string &word);
        static bool choose_char(char c);
        bool rect_intersect(cv::Rect r1,cv::Rect r2);
        bool overlap(int value,int min, int max);
        void counter_inc();
        Mat opencv2imagemagick(Mat& im,string save_dir);
        Mat opencv2imagemagick(Mat& im);
        size_t levenshtein_distace(const string& original_word, const string& from_tesseract);
        
        void opencv2imagemagick_test(string p);
        void tesseract_direct(Mat& tm);
        void magiccore();
        bool is_background_present();
        
        std::string path;
        Mat saved;
        int thresh, N;
        int counter;
        int no_of_components;
        RNG rng;
        std::map<int,string> tearsheet_words;
        vector<string> all_words;
        char * dir;
        string cwd;
        vector<cv::Rect> all_rects;
        vector<std::pair<int, int>> coordinates;
        
    };
}