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
#include <fstream>

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
        Splice(string working_dir,string img_name,string word_list_file_name, string settings_file);
//        Splice(const std::string& dir_path);
        ~Splice();
        
        //Core Functions
        void init();
        
        //Utility Functions
        void save_image(const std::string& save_dir,string save_name,Mat& saved_img);
        string get_json(const string& file_path="");
        
        //Variables
        bool write_output;
        bool show_windows;
        
    private:
        
        //Core Functions
        /*
         1. init() - The method to be called after object initialization
         2. is_background_present() - Checks for background
         3. watershed() - removes background if present
         4. image_cleaner() - cleans up and binarizes the image(black and white)
         5. find_squares() - Finds bounding boxes around the text boxes - calls imagemagick and tesseract and writes a JSON file to disk
         */
        
        bool is_background_present(const string& img_name);
        Mat watershed();
        void image_cleaner();
        
        Mat opencv_cleanup(Mat& img_load);
        void image_magick_cpp_api();
        void magiccore();
        void find_squares( const Mat& original_image,const Mat& run_script, string components_path="",string contours_path="" );
//        void find_squares( const Mat& original_image,const string& image_path, string components_path="",string contours_path="" );
        
        
        
        
        
        //Utility Functions
        /*
         1. load_image() - Loads image and check if the image is valid
         2. save_image() - Saves image to disk
         3. init_dictionary - Initializes  the dictioanry of words
         4. levenshtein_distace() - Calculates the differce between 2 strings
         5. dictionary_comparision() - matches strings to see if the words are in the dictionary
         6. json_init() - Clears and initializes JSON string
         7. form_json() - Forms JSON. Appends strings to json_string
         8. json_write_to_file() - Writes JSON to file
         */
        Mat load_image(string image_path,string window_name="image");
        void init_dictionary(string word_list);
        size_t levenshtein_distace(const string& original_word, const string& from_tesseract);
        string dictionary_comparision(string word);
        void json_init();
        void form_json(string word, pair<int, int> pos);
        void json_write_to_file();
        void load_settings(string file_path);
        void return_settings(const string& parm,const string& value);
        bool return_true_or_false(const string& value);
        
        template<typename T>
        T convert_string_to_number(const string& stn){
            stringstream input_text(stn);
            T converted_number;
            
            return input_text >> converted_number ? converted_number : -1;
        }
        
        
        
        
        
        //Helper Functions
        /*
         1. opencv2imagemagick(Mat, string) - converts image from opencv to imagegick, enhances the image and converts it back to opencv
         2. opencv2imagemagick(Mat) - converts image from opencv to imagegick, enhances the image and converts it back to opencv
         3. opencv2tesseract() - Converts image from opnecv to tesseract and performs OCR on it
         4. opencv2imagemagick_test - Test function to convert opencv image to imagemagick image format. Use opnecv2imagemagick() instead
         5. image_magick_cleanup() - Helper function to test imageMagick C++ api enhance features. Use opencv2imagemagick() instead.
         6. tesseract_test() - Helper function to test tesseract. Use opencv2tesseract instead for converting image from opencv to tesseract
         7. tesseract_with_dictionary() - tesseract test helper funciton to read image from disk and perform dictionary matching.
         Use opencv2tesseract() instead.
         8. rect_intersect() - Checks if 2 rectangles intersect
         9. overlap() - Checks to see if a value is greater than minimum and less than maximum. Used by rect_intersect to check if rectangles intersect
         10. angle() - Finds the angle between the points
         */
        Mat opencv2imagemagick(Mat& im,string save_dir);
        Mat opencv2imagemagick(Mat& im);
        string opencv2tesseract(Mat& tm);
        bool rect_intersect(cv::Rect r1,cv::Rect r2);
        bool overlap(int value,int min, int max);
        static double angle(Point pt1,Point pt2,Point pt0);
        void counter_inc();
        string last4chars(string& dir_path);
        string join_path(const string& path1,const string& path2);
        
        // Helper Functions - not being used, only for testing
        void opencv2imagemagick_test(string p);
        void image_magick_cleanup(string image_path);
        void tesseract_test(string image_path);
        void tesseract_with_dictionary(string image_path);

        void parse_word(string &word);
        static bool choose_char(char c);
        
        int get_no_of_components();
        void reset_no_of_components();
        
        //Other
        void keypoint_matching();


        //Variables
//        std::string path;
        Mat src_image,saved,adaptive_thresholded;
        int thresh;
        int no_of_components;
        RNG rng;
        std::map<int,string> tearsheet_words;
        vector<string> all_words;
        char * dir;
        string cwd,json_string;
        string working_dir, img_name, word_list_file_name,settings_file;
        vector<cv::Rect> all_rects;
        vector<std::pair<int, int>> coordinates;
        
        
        //Settings file
        bool use_bash;
        int Num_of_thresholds,background_threshold,watershed_lower_threshold,watershed_rectangle_filter;
        int adaptive_threshold_blockSize, adaptive_threshold_lower_limit, adaptive_threshold_upper_limit,word_distance;
        
        rapidjson::Document document;
        
    };
}