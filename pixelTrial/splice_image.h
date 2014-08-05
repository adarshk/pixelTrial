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
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/photo/photo.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <ImageMagick-6/Magick++.h>
#include <stdlib.h>
#include <regex>

#include "load_image.h"
#include "find_edges.h"
#include "detect_contours.h"
#include "watershed_marker.h"

using namespace std;
using namespace cv;
using namespace tesseract;
using namespace Magick;


namespace ppc {
    class Splice{
        
    public:
        Splice();
        Splice(const std::string& dir_path);
        ~Splice();
        void init();
        void save_image(const std::string& save_dir);
        
    private:
        void watershed();
        void text_cleaner();
        void image_magick();
        void image_magick2();
        void image_magick3();
        void image_magick4();
        void run_script();
        void squares_method(const Mat& original_image,string image_pth,string components_path,string contours_path);
        void squares_method2(const Mat& original_image,string image_pth,string components_path,string contours_path);
        static bool mysortfunction(Rect r1,Rect r2);
        static double angle(Point pt1,Point pt2,Point pt0);
        void findSquares( const Mat& original_image,const Mat& image, vector<vector<Point> >& squares,string components_path,string contours_path );
        void findSquares2( const Mat& original_image,const Mat& image, vector<vector<Point> >& squares,string components_path,string contours_path );
        void tesseract();
        void tesseract2();
        void tesseract3();
        void keypoint_matching();
        void thresholding_image();
        void init_dictionary();
        void dictionary_comparision(string word);
        void parse_word(string &word);
        static bool choose_char(char c);
        
        std::string path;
        Mat saved;
        int thresh, N;
        int counter;
        RNG rng;
        std::map<int,string> tearsheet_words;
        vector<string> all_words;
        
    };
}