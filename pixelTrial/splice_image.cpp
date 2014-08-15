//
//  splice_image.cpp
//  pixelTrial
//
//  Created by Adarsh Kosuru on 7/31/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#ifndef NDEBUG
#define Check(x) assert(x)
#define CheckWithMessage(str,x) assert((str,x))
#endif

#define IMAGEMAGICKBASH "\
#/bin/bash \n\
echo \"Bash says hello\" \n\
/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/convert -type grayscale -normalize -negate -lat 15x15+5% \
-negate /Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved.jpg \
/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_run_script.jpg \n\
"



    /*
      ____                           ____  _           __   ______          __
     / __ \____ _____  ___  _____   / __ \(_)  _____  / /  / ____/___  ____/ /__
    / /_/ / __ `/ __ \/ _ \/ ___/  / /_/ / / |/_/ _ \/ /  / /   / __ \/ __  / _ \
   / ____/ /_/ / /_/ /  __/ /     / ____/ />  </  __/ /  / /___/ /_/ / /_/ /  __/
  /_/    \__,_/ .___/\___/_/     /_/   /_/_/|_|\___/_/   \____/\____/\__,_/\___/
             /_/
 */


#include "splice_image.h"

namespace ppc{
    
    Splice::Splice():thresh(50),N(1),rng(12345),write_output(true),cwd(""),show_windows(false),no_of_components(0){
        
    }
    
//    Splice::Splice(const string& dir_path):thresh(50),N(1),rng(12345),write_output(true),cwd(""),show_windows(false),no_of_components(0){
//        path = dir_path;
//    }
    
    Splice::~Splice(){
        
    }
    
    
    /* CORE FUNCTIONS
      ____                   _____                 _   _
     / ___|___  _ __ ___    |  ___|   _ _ __   ___| |_(_) ___  _ __  ___
    | |   / _ \| '__/ _ \   | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
    | |__| (_) | | |  __/   |  _|| |_| | | | | (__| |_| | (_) | | | \__ \
     \____\___/|_|  \___|   |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
     
     1. init() - The method to be called after object initialization
     2. is_background_present() - Checks for background
     3. watershed() - removes background if present
     4. image_cleaner() - cleans up and binarizes the image(black and white)
     5. find_squares() - Finds bounding boxes around the text boxes - calls imagemagick and tesseract and writes a JSON file to disk
     */
    
    
    
    /*!
     To be called after object initialization
     1. Initializes the dictionary from file
     2. Calls all core functions
     */
    void Splice::init(string working_dir,string img_name,string word_list_file_name){
        
        
        string split_name;
        if(working_dir == ""){
        
        CheckWithMessage(string("Set image name in init"), img_name != "");
        
        dir = getcwd(NULL, 0);
        cwd = string(dir);
        split_name = img_name;
        }
        else if(last4chars(working_dir) == ".jpg" ||  last4chars(working_dir) == ".JPG" || last4chars(working_dir) == ".png"){
            size_t position = working_dir.find_last_of("/");
            
            if(position != working_dir.length()){
                split_name = working_dir.substr(position);
            }
            
            cwd = working_dir.substr(0,position);
        }
        else{
        CheckWithMessage(string("Set image name in init"), img_name != "");
        cwd = working_dir;
        split_name = img_name;
        }
        
//        cout << "Working dir - " <<cwd << ", image name - " << split_name << endl;
        
        
        CheckWithMessage(string("Set word list name in init"), word_list_file_name != "");
        init_dictionary(word_list_file_name);
        //        this->opencv2imagemagick_test(cwd + "/red2.jpg");
        
        
        bool val = is_background_present(split_name);
        if(val){
            
            watershed();
            
            //Bash script for cleaning up the text
            image_cleaner(); //Input - saved , output - saved_run_script.jpg

        }
        else{
            
            save_image(cwd,"saved.jpg",src_image);

            //Bash script for cleaning up the text
            image_cleaner(); //Input - saved , output - saved_run_script.jpg
        }
        
        

        
        
        // Three methods for replicating results of the bash script using opencv, iamgemagick C/c++ API
        // Currently none of them are being used as they dont give the same results
        
        /*
        this->thresholding_image(); //output - saved_run_script_opencv.jpg
        this->image_magick_cpp_api(); //ImageMagick C++ API, bash version gives better resutls. Output - saved_run_script_api.jpg
        this->magiccore(); //Imagemagick C API, Does not work & does not write to output currently
        */
        
        
        Mat saved_run_script = load_image(cwd + string("/saved_run_script.jpg"),"SquaresImage");
        
        json_init();
        this->find_squares(saved_run_script,"/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_run_script.jpg","/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/compsFromImageMagick/","/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_run_contours.jpg");
        
        
        //        this->image_magick_cleanup("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/compsFromImageMagick/");
        //        this->tesseract("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/compsFromImageMagick/");
        
        json_write_to_file();
        
        
        
        json_init();
        this->find_squares(saved,"/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_run_script.jpg","/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/compsFromImageMagick_originalImage/","/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_run_contours_original_image.jpg");
        
        
        //        this->image_magick2("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/comps/");
        
        //        this->tesseract("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/comps/");
        
        
        //        this->image_magick_cleanup("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/compsFromImageMagick_originalImage/");
        //        this->tesseract_with_dictionary("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/compsFromImageMagick_originalImage/");
        
        
        json_write_to_file();
        
        
        //        this->keypoint_matching();
         
        
        
    }
    
    
    
    
    
    
    
    
    
    
    
    /*!
     Uses bounding box technique to check if the image has background.
     Returns true if image has background, else returns false.
     
     For more information refer to
     http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/bounding_rects_circles/bounding_rects_circles.html
     */
     bool Splice::is_background_present(const string& img_name){
        
        Mat src_image_copy,grayscale_image, denoised_image, thresholded_image,contours_image;
        vector<vector<Point> > contours;
        vector<cv::Vec4i> hierarchy;
        int highest_area = 0;
        int bounding_rect_index = -1;
        
        
        //Loads image from path given during object initialization
         string full_path;
         full_path = join_path(cwd, img_name);
        src_image = load_image(full_path,"src_image");
    
        src_image.copyTo(src_image_copy);
        
        
        /*
         1. Converts to grayscale
         2. Denoise the image
         3. Finds contours to check for the biggest contour
         */
        
        cvtColor(src_image_copy, grayscale_image, CV_RGB2GRAY );
        blur(grayscale_image,grayscale_image,Size(3,3));
        
        fastNlMeansDenoising(grayscale_image, denoised_image);
        
        //        adaptiveThreshold(noise, thresholded_image, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 7, 5);
        threshold(denoised_image, thresholded_image, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
        
        findContours(thresholded_image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,Point(0,0));
        
        if(contours.size() == 0){
            cerr << "No contours found on the image" << endl;
        }
        
        vector<vector<Point> > contours_poly(contours.size());
        vector<Rect> boundRect(contours.size());
        vector<Point2f> center(contours.size());
        
        
        //Forms an approximate polygon(rectangle) around each contour and finds the biggest contour
        
        for(int i=0;i<contours.size();i++){
            approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
            boundRect[i] = boundingRect(Mat(contours_poly[i]));
            
            if(boundRect[i].area() > highest_area){
                highest_area = boundRect[i].area();
                bounding_rect_index = i;
            }
        }
        
        Size s = src_image_copy.size();
        int image_area = s.width * s.height;
        //        cout << "Image area - " << image_area <<  endl;
        //        cout << "highest area - " << highest_area <<  endl;
        
        //        cout << "x - " << boundRect[bounding_rect_index].tl().x << endl;
        //        cout << "x + height - " << boundRect[bounding_rect_index].br().x - boundRect[bounding_rect_index].width<< endl;
        
        
        CheckWithMessage(string("No bounding boxes found"), bounding_rect_index != -1);
        
        
        /* Performs basic filtering on the image to see if the area of the biggest contour is approximately the area of the source image
         or if the contour stretches along the full width of the source image
         
         If the contour has one of the above properties, then most likely it doesnt have a background - so it returns false
         */
         
        if(image_area % highest_area < 0.05 * image_area ||
           (boundRect[bounding_rect_index].width >= s.width && boundRect[bounding_rect_index].height >= s.height) ||
           ((boundRect[bounding_rect_index].tl().x < 5 && boundRect[bounding_rect_index].br().x - boundRect[bounding_rect_index].width < 5) &&
            (boundRect[bounding_rect_index].tl().x + boundRect[bounding_rect_index].width > s.width - 5 && boundRect[bounding_rect_index].br().x > s.width - 5) ))
        {
            return false;
        }
        
        
        //Uncomment for debugging
        /*
         Mat drawing = Mat::zeros(thresholded_image.size(),CV_8UC3);
         
         //        for(int i=0;i<contours.size();i++){
         Scalar color(0,255,0);
         
         drawContours(src_image_copy, contours_poly, bounding_rect_index, color);
         rectangle(src_image_copy, boundRect[bounding_rect_index].tl(), boundRect[bounding_rect_index].br(), color);
         
         //        }
         
         imwrite("/Users/adarsh.kosuru/Desktop/Tearsheets/differentBackground/result.jpg", source_image_copy);
         */
        
        return true;
    }
    
    
    
    
    
    
    
    
    
    /*!
     Removes background from image using watershed technique
     For more info on watershed, check any/all of these
     1. An example in the opnecv source code folder in opencv_source_code/samples/cpp/watershed.cpp
     2. Chapter 5 - OpenCV 2 Computer Vision Application Programming Cookbook by Robert Laganiere
     3. http://docs.opencv.org/trunk/doc/py_tutorials/py_imgproc/py_watershed/py_watershed.html
     4. OpenCV docs
     */
    void Splice::watershed(){
        
        
        
        Mat source_image_resized,binary,image_for_saving;
        
        cv::resize(src_image, source_image_resized, cv::Size(0,0), 0.5,0.5 );
        source_image_resized.copyTo(image_for_saving);
        
        cvtColor(source_image_resized, binary, CV_BGR2GRAY);
        threshold(binary, binary, 100, 255, CV_THRESH_BINARY);
        
        if (show_windows) {
            imshow("binary", binary);
        }
        
        //Separates foreground and background by eroding and dilating the image
        Mat fg,bg;
        erode(binary, fg, Mat(),Point(-1,-1),3);
        if (show_windows) {
            imshow("fg", fg);
        }
        
        dilate(binary, bg, Mat(),Point(-1,-1),3);
        threshold(bg, bg, 1, 128, CV_THRESH_BINARY_INV);
        if (show_windows) {
            imshow("bg", bg);
        }
        
        
        Mat markers(binary.size(),CV_8U,Scalar(0));
        markers = fg + bg;
        if (show_windows) {
            imshow("markers", markers);
        }
        
        WatershedMarker wm;
        wm.set_markers(markers);
        
        Mat res;
        res = wm.apply_watershed(source_image_resized);
        res.convertTo(res, CV_8U);
        
        Mat res_canny,res_output;
        Canny(res, res_canny, 100, 255);
        
        Mat image_contours;
        vector<vector<Point> > main_contours;
        vector<vector<Point> > needed_contours;
        vector<Vec4i> squares_hierarchy;
        vector<Vec4i> hierarchy;
        vector<int> indexes;
        Scalar color(0,255,0);
        findContours(res_canny, main_contours, squares_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
        res_output = Mat::zeros(res_canny.size(), CV_8UC3);
        
        //        cout << "Num of watershed contours - " << main_contours.size() << endl;
        
        drawContours(res_output, main_contours, -1, color,2,8,hierarchy,0,Point());
        
        if (show_windows) {
            imshow("resOutput", res_output);
        }
        
        //Finds only the parent contours and filters out the rest
        for (int i=0; i<squares_hierarchy.size(); i++) {
            //        cout  << hierarchy[i] << endl;
            if (squares_hierarchy[i][3] == -1) {
                hierarchy.push_back(squares_hierarchy[i]);
                indexes.push_back(i);
            }
        }
        
        for (vector<int>::iterator itt = indexes.begin();itt!=indexes.end();++itt) {
            needed_contours.push_back(main_contours[*itt]);
        }
        
        drawContours(image_for_saving, main_contours, -1, color,2,8,squares_hierarchy,0,Point());
        
        if(write_output){
            //            imwrite(cwd + "/saved_watershed.jpg", image_for_saving);
            save_image(cwd, "saved_watershed.jpg", image_for_saving);
        }
        
        
        /*
         Finds the contours with the biggest areas to get only the image and not the background
         The contour with the second biggest area is taken because the one with the biggest area is the whole image
         */
        int highest_area = 0,second_highest_area=0;
        Rect main_rect,second_highest_area_rect;
        vector<Rect> all_rectangles;
        vector<int> areas;
        vector<int> inserted_areas;
        vector<Rect> sorted_rectangles;
        for (vector<int>::iterator itt = indexes.begin();itt!=indexes.end();++itt) {
            Rect r = boundingRect(Mat(main_contours[*itt]));
            //            cout << "area[" << *itt << "] - "<<r.area() <<endl;
            areas.push_back(r.area());
            all_rectangles.push_back(r);
            
            
            if (r.area() > highest_area) {
                second_highest_area=highest_area;
                highest_area = r.area();
                second_highest_area_rect = main_rect;
                main_rect = r;
            }
            
        }
        
        
        //For debugging
        
        /*
         for (vector<Rect>::iterator sr=sorted_rectangles.begin(); sr!=sorted_rectangles.end(); ++sr) {
         cout << "sorted_rectangles_area[" << sr-sorted_rectangles.begin() <<"] - " <<sr->area() <<endl;
         }
         
         for (vector<Rect>::iterator ar=all_rectangles.begin(); ar!=all_rectangles.end(); ++ar) {
         cout << "all_rectangles_area[" << ar-all_rectangles.begin() <<"] - " <<ar->area() <<endl;
         }
         
         for (vector<int>::iterator areas_vector = areas.begin(); areas_vector!=areas.end(); ++areas_vector) {
         cout << "areas[" << areas_vector-areas.begin() <<"]- " << *areas_vector <<endl;
         }
         
         cout << "Watershed rectangles - " <<all_rectangles.size()<<endl;
         */
        
        //Copies the contour(rectangle) into another image
        
        Mat cop = source_image_resized(second_highest_area_rect);
        cop.copyTo(saved);
        
        
        save_image(cwd,"saved.jpg",saved);

        
        
    }
    
    
    
    
    
    
    
    
    
    
    
    /*!
     Reads image, calls ImageMagick bash script to enhance (binarize) the image and writes the image to disk
     */
    void Splice::image_cleaner(){
        system(IMAGEMAGICKBASH);
    }
    
    void Splice::thresholding_image(){
        
        Mat src_gray,adaptive_thresholded_image,noise,eroded,dilated;
        Mat pyr_down,pyr_up;
//        int erosion_type = MORPH_RECT;
//        Mat element = getStructuringElement(erosion_type, Size(2*1 + 1,2*1 + 1),Point(1,1));
        
        Mat src = imread(cwd + "/saved.jpg", 1 );
        
        cvtColor( src, src_gray, CV_RGB2GRAY );
        
        fastNlMeansDenoising(src_gray, noise);
        
        adaptiveThreshold(noise, adaptive_thresholded_image, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 7, 5);
//        threshold(noise, adaptive_thresholded_image, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
        
//        pyrDown(adaptive_thresholded_image, pyr_down, Size(adaptive_thresholded_image.cols/2, adaptive_thresholded_image.rows/2));
//        pyrUp(pyr_down, pyr_up, adaptive_thresholded_image.size());
        
//        dilate(adaptive_thresholded_image, dilated, Mat(),Point(-1,-1),1);
//        dilate(eroded, dilated, Mat(),Point(-1,-1),1);
//        erode(adaptive_thresholded_image, eroded, Mat(),Point(-1,-1));
//        erode(adaptive_thresholded_image, eroded, element);
        
        int morph_size = 2;
        Mat element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
        
        morphologyEx(adaptive_thresholded_image, eroded, MORPH_OPEN, element);
        
        Splice::save_image(cwd, "saved_run_script_opencv.jpg", eroded);
    }
    

    void Splice::image_magick_cpp_api(){
        
        Magick::InitializeMagick("");
        
        Magick::Image image;
        string images_path = string("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved.jpg");
        
        try{
            
            image.read(images_path);
            image.type(GrayscaleType);
            image.normalize();
            image.negate();
            image.adaptiveThreshold(15, 15,5);
//            image.contrastStretch(0, 0);
//            image.compose(CopyOpacityCompositeOp);
            image.negate();
            image.write("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_run_script_api.jpg");
            
        }
        catch(Magick::Exception &err){
            cout << "Exception" << err.what() << endl;
        }
    }
    
    void Splice::magiccore(){
        
        
        MagickCore::Image *image,*images,*write_image;
        MagickCore::ImageInfo *image_info,*write_info;
        
        ExceptionInfo *exception;
        
        MagickCoreGenesis("", MagickTrue);
        exception = AcquireExceptionInfo();
        image_info = CloneImageInfo((ImageInfo *) NULL);
        write_info = CloneImageInfo((ImageInfo *) NULL);
        
        (void) strcpy(image_info->filename,"/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved.jpg");
        images = ReadImage(image_info, exception);
        
        if(exception->severity != UndefinedException)
            CatchException(exception);
        
        if(images == (MagickCore::Image *)NULL){
            printf("Image null\n");
            exit(1);
        }
        
        
        write_image = NewImageList();
        
        
        while ((image=RemoveFirstImageFromList(&images)) != (MagickCore::Image *) NULL)
        {
            
            GrayscaleImage(image, AveragePixelIntensityMethod);
            (void) AppendImageToList(&write_image, image);
            DestroyImage(image);
        }
        
        
        (void ) strcpy(write_info->filename, "/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/");
        
//        MagickCore::WriteImage(write_info, write_image);
     
        
//        write_image = DestroyImageList(write_image);
        image_info = DestroyImageInfo(image_info);
        write_info = DestroyImageInfo(write_info);
        exception = DestroyExceptionInfo(exception);
        MagickCoreTerminus();
        
        
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    /*!
     original_image - the source image, used at the end of the function to copy parts of the image based on found bounding boxes.
     image_path -  Image used to find the bounding boxes. This binary image is cleaned up version from the bash script or imagemagick/opencv
     components_path - Optional. Default value is "". Path to save the individual components on disk
     contours_path - Optional. Default value is "". Path to save the original image with bounding boxes drawn on top. Saves to disk.
     
     For more information refer to Squares based approach in the opencv source code folder, in opencv_source_code/samples/cpp/squares.cpp
     
     1. Finds all the contours i.e. rectangle bounding boxes in the image
     2. Checks if the rectangles intersect and if they do, only considers one of them
     3. Filters unwanted rectangles based on area, width and height
     4. For each rectangle (i.e. component), imagemagick is called which enhances the image and then OCR is performed on this enhanced image using tesseract
     */
    void Splice::find_squares( const Mat& original_image,const string& image_path,string components_path,string contours_path )
    {
        
        vector<vector<Point> > squares;
        
        Mat image = load_image(image_path);
        
        squares.clear();
        all_rects.clear();
        coordinates.clear();
        no_of_components = 0;
        Mat image_for_saving;
        original_image.copyTo(image_for_saving);
        
        Mat pyr, timg, gray0(image.size(), CV_8U), gray;
        
        // down-scale and upscale the image to filter out the noise
        pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
        pyrUp(pyr, timg, image.size());
        vector<vector<Point> > contours;
        Mat drawing = Mat::zeros( image.size(), CV_8UC3 );
        
        
        // find squares in every color plane of the image
        for( int c = 0; c < 3; c++ )
        {
            int ch[] = {c, 0};
            mixChannels(&timg, 1, &gray0, 1, ch, 1);
            
            // try several threshold levels
            for( int l = 0; l < N; l++ )
            {
                
                if( l == 0 )
                {
                    // apply Canny. Take the upper threshold from slider
                    // and set the lower to 0 (which forces edges merging)
                    Canny(gray0, gray, 0, thresh, 5);
                    // dilate canny output to remove potential
                    // holes between edge segments
                    dilate(gray, gray, Mat(), Point(-1,-1));
                }
                else
                {
                    // apply threshold if l!=0:
                    //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                    gray = gray0 >= (l+1)*255/N;
                }
                
                // find contours and store them all as a list
                findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
                
                
                cout << "contours size - " << contours.size() << endl;
                
                vector<Point> approx;
                vector<Rect> boundRect( contours.size() );
                
                
                // test each contour
                for( size_t i = 0; i < contours.size(); i++ )
                {
                    // approximate contour with accuracy proportional
                    // to the contour perimeter
                    approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
                    boundRect[i] = boundingRect( Mat(approx) );
                    
                    // square contours should have 4 vertices after approximation
                    // relatively large area (to filter out noisy contours)
                    // and be convex.
                    // Note: absolute value of an area is used because
                    // area may be positive or negative - in accordance with the
                    // contour orientation
                    if( approx.size() == 4 &&
                       fabs(contourArea(Mat(approx))) > 1000 &&
                       isContourConvex(Mat(approx)) )
                    {
                        double maxCosine = 0;
                        
                        for( int j = 2; j < 5; j++ )
                        {
                            // find the maximum cosine of the angle between joint edges
                            double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                            maxCosine = MAX(maxCosine, cosine);
                        }
                        
                        drawContours(drawing, contours, (int)i, Scalar(0,255,0),2,8, vector<Vec4i>(), 0, Point());
                        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
                        rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color);
                        
                        
                        // if cosines of all angles are small
                        // (all angles are ~90 degree) then write quandrange
                        // vertices to resultant sequence
                        if( maxCosine < 0.3 )
                            squares.push_back(approx);
                    }
                }
            }
        }
        
        
        
        //Checks if rectangles intersect and if they do then only considers one of the rectangles
        for( size_t i = 0; i < squares.size(); i++ )
        {
            Rect rt = boundingRect(Mat(squares[i]));
            bool intersect = false;
            
            for (vector<Rect>::iterator rect_iterator=all_rects.begin(); rect_iterator!=all_rects.end(); ++rect_iterator) {
                
                intersect =   rect_intersect(rt, *rect_iterator);
                
                if (intersect) {
                    break;
                }
                else{
                    continue;
                }
                
            }
            
            if (intersect == false) {
                all_rects.push_back(rt);
            }
            else{
                continue;
            }
            
            
            Size s = image.size();
            //            cout << "img_area - " << s.area() << " rt.area - " << rt.area()<<endl;
            //            if (rt.area() < 3*s.area() / 4) {
            
            
            
            //Filters rectangles based on area, width and height
            if (rt.area() < 3*s.area() / 4 && rt.width < (s.width/2) && rt.height<(s.height/2)) {
                
                drawContours(image_for_saving, squares, (int)i, Scalar(0,255,0),2,8, vector<Vec4i>(), 0, Point());
                
                Mat im(original_image,rt);
                if(write_output){
                    imwrite(components_path + to_string(no_of_components) + ".jpg", im);
                }
                
                coordinates.push_back(make_pair(rt.x, rt.y));
//                cout << "Json x - " << rt.x << ", Json y - " << rt.y <<endl;
//                Mat ml;
                Mat ml = opencv2imagemagick(im,components_path + to_string(no_of_components) + "_gray.jpg");
                string matched_word = opencv2tesseract(ml);
                pair<int, int> p(rt.x,rt.y);
                form_json(matched_word, p);
                
                counter_inc();
            }
            
            
        }
        
        if(write_output){
        imwrite(contours_path, image_for_saving);
        }
        
        
    }
    

    
    
    
    
    /* UTILITY FUNCTIONS
     _   _ _   _ _ _ _             _____                 _   _
    | | | | |_(_) (_) |_ _   _    |  ___|   _ _ __   ___| |_(_) ___  _ __  ___
    | | | | __| | | | __| | | |   | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
    | |_| | |_| | | | |_| |_| |   |  _|| |_| | | | | (__| |_| | (_) | | | \__ \
     \___/ \__|_|_|_|\__|\__, |   |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
                         |___/
     1. load_image() - Loads image and check if the image is valid
     2. save_image() - Saves image to disk
     3. init_dictionary - Initializes  the dictioanry of words
     4. levenshtein_distace() - Calculates the differce between 2 strings
     5. dictionary_comparision() - matches strings to see if the words are in the dictionary
     */
    
    
    /*!
     Load image and check if the image is valid i.e. assert if the image has any data using image.data
     image_path - image load path directory
     window_name - name of the window when the image is displayed. No need to explicitly define. Default is "Image".
     */
    Mat Splice::load_image(string image_path,string window_name){
        
        LoadImage l(image_path,window_name);
        Mat m = l.get_image();
        CheckWithMessage(string("Image loaded incorrectly"), m.data);
        
        return m;
    }
    
    /*!
     Saves image to disk
     save_dir - The directory path the image will be written to
     save_name - Name of the image
     saved_img - Image to be saved
     */
    void Splice::save_image(const string& save_dir,string save_name,Mat& saved_img){
        
        char last_slash = save_dir.back();
        char first_slash = save_name.at(0);
        
        if (last_slash == '/' || first_slash=='/') {
            imwrite(save_dir + save_name, saved_img);
        }
        else{
            imwrite(save_dir + "/" + save_name, saved_img);
        }
    }
    
    /*!
     Init_dictioanry - initialises the dictionary from a file with the words from the file
     */
    void Splice::init_dictionary(string word_list){
        
//        string file_path = join_path(cwd, word_list);
        
//        cout << "file path - " << word_list << endl;
        
        ifstream file(word_list);
        
        string word;
        
        if(file.good()){
        
        while(getline(file,word)){
//            cout << "word - " << word<<endl;
            all_words.push_back(word);
        }
        }
        else{
            cerr << "File not found" << endl;
        }
        
        if(all_words.size() == 0 ){
            cerr << "No words found in file" << endl;
        }
        
//        string dictionary_words[] = {"Navigation","Panel","BUTTON","PRIMARY BUTTON","DISABLED BUTTON","LINK 01","LINK 02","LINK 03","LINK 04","LINK 05","LINK 06","search","Text Input","Select","World"};
        
        
//        int dictionary_keys[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
        
        //Enable for debugging
        /*
        map<string,int>::iterator it;
        it = tearsheet_words.find("Select");
        
        if (it!=tearsheet_words.end()) {
            int m = it->second++;
            cout << "m - " << m << endl;
        }
         
        vector<string> words;
        vector<int> keys;
        
        for (int w=0; w<sizeof(dictionary_words)/sizeof(*dictionary_words); w++) {
//            cout << "word pushback - " << t_words[w] <<endl;
            words.push_back(dictionary_words[w]);
            keys.push_back(dictionary_keys[w]);
        }
        
        for (int k=0;k<sizeof(dictionary_keys)/sizeof(*dictionary_keys);k++) {
//            keys.push_back(t_keys[k]);
        }
        */
    
        
    }
    
    /*!
     levenshtein_distace - Calculates the distance between 2 strings. For example the distance between 'Select' and 'Select 3' is 2(space and number 3).
     */
    size_t Splice::levenshtein_distace(const string &original, const string &from_tesseract){
        
        const size_t m(original.size());
        const size_t n(from_tesseract.size());
        
        if( m==0 ) return n;
        if( n==0 ) return m;
        
        size_t *costs = new size_t[n + 1];
        
        for( size_t k=0; k<=n; k++ ) costs[k] = k;
        
        size_t i = 0;
        for ( string::const_iterator it1 = original.begin(); it1 != original.end(); ++it1, ++i )
        {
            costs[0] = i+1;
            size_t corner = i;
            
            size_t j = 0;
            for ( string::const_iterator it2 = from_tesseract.begin(); it2 != from_tesseract.end(); ++it2, ++j )
            {
                size_t upper = costs[j+1];
                if( *it1 == *it2 )
                {
                    costs[j+1] = corner;
                }
                else
                {
                    size_t t(upper<corner?upper:corner);
                    costs[j+1] = (costs[j]<t?costs[j]:t)+1;
                }
                
                corner = upper;
            }
        }
        
        size_t result = costs[n];
        delete [] costs;
        
        return result;
        
    }
    
    
    /*!
     Compares the words in the dictionary
     */
    string Splice::dictionary_comparision(string word){
        
        
        string rt;
        //        for (vector<string>::iterator i= all_words.begin(); i!=all_words.end();i++) {
        
        //            if (strcmp(result, i->c_str()) ==0) {
        
        //            cout << "compare - " << strcmp(result, "search") << " ,result - "<< result<< endl;
        
        
        string delimiter = "\n";
        
        string delimited_string = word.substr(0,word.find(delimiter));
        
        
        if(find(all_words.begin(), all_words.end(), delimited_string) != all_words.end()){
            
            return delimited_string;
            
        }
        
        else{
            
            return "Not found";
        }
        
        
        //              if (strcmp(result, "Text Input") ==0) {
        //                cout << "yes - " <<endl;
        //            }
        
        //        }
        
        //        return "Not found";
        
        // vector method
        
        /*
         
         for (vector<string>::iterator i= all_words.begin(); i!=all_words.end();i++) {
         cout << "all words - " << *i << endl;
         }
         
         
         if(find(all_words.begin(), all_words.end(), word) != all_words.end()){
         
         cout << word << " found" <<endl;
         
         }
         
         */
        /*
         else{
         cout << word << " not found" <<endl ;
         }
         
         */
        
        
        // Map method
        
        
        /*
         cout << "word passed - " << word << endl;
         
         map<string,int>::iterator it;
         it = tearsheet_words.find(word);
         
         if (tearsheet_words.count(word)) {
         cout << "word found - " <<word << endl;
         }
         
         
         if (it!=tearsheet_words.end()) {
         int m = it->second;
         //            cout << word << " in dictionary" << endl;
         cout << "m - " << m << endl;
         }
         
         //        else{
         //            cout << word << " not in dictionary" << endl;
         //        }
         */
    }
    
    
    /*!
     Clears and initializes JSON string
     */
    void Splice::json_init(){
        json_string.clear();
        json_string = string("{");
    }
    
    /*!
     Forms JSON. Appends strings to json_string
     word - word to be added to JSON
     pos - position of the word on the image
     */
    void Splice::form_json(string word, pair<int, int> pos){
        
        //        cout << "pos x - " << pos.first << "," << pos.second <<endl;
        
        if(no_of_components == 0){
            json_string += string("\"Component\":") + "\"" + word + "\"";
        }
        else if(no_of_components > 0){
            json_string += string(",\"Component\":") + "\"" + word + "\"";
        }
        json_string += string(",\"x\":") + "\"" + to_string(pos.first) + "\"";
        json_string += string(",\"y\":") + "\"" + to_string(pos.second) + "\"";
    }
    
    /*!
     Writes JSON to file
     */
    void Splice::json_write_to_file(){
        
        json_string += string("}");
        
        char json[json_string.size() + 1];
        strcpy(json, json_string.c_str());
        cout << "JSON - " <<json << endl;
        document.Parse(json);
        if (document.Parse(json).HasParseError()) {
            cerr << "JSON parsing error" << endl;
        }
        
        
        FILE * output_file = fopen(string(cwd + "/output.json").c_str(), "w");
        char writeBuffer[65536];
        
    rapidjson:FileWriteStream os(output_file,writeBuffer,sizeof(writeBuffer));
        Writer<FileWriteStream> writer(os);
        document.Accept(writer);
        fclose(output_file);
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    /* HELPER FUNCTIONS
      _   _      _                     _____                 _   _
     | | | | ___| |_ __   ___ _ __    |  ___|   _ _ __   ___| |_(_) ___  _ __  ___
     | |_| |/ _ \ | '_ \ / _ \ '__|   | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
     |  _  |  __/ | |_) |  __/ |      |  _|| |_| | | | | (__| |_| | (_) | | | \__ \
     |_| |_|\___|_| .__/ \___|_|      |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
                  |_|
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
    
    
    /*!
     im - image to enhance
     save_dir - Directory to save the image in (Use opencv2imagemagick(Mat& im) if theres no need to save to dir)
     1. Converts opencv image to ImageMagick image format
     2. Enhances the ImageMagick image by cconverting to grayscale,normalizing and upscaling by 300%.
     3. Converts it back again to opencv
     */
    Mat Splice::opencv2imagemagick(Mat& im, string save_dir){
        
        
        Mat om;
        im.copyTo(om);
        
        char* block;
        size_t width,height;
        
        Magick::InitializeMagick("");
        
        Magick::Image image(om.cols,om.rows,"BGR",CharPixel,(char *)om.data);
        
        try{
            
            image.type(GrayscaleType);
            image.normalize();
            //        image.sharpen();
            image.resize("300%");
            
            width = image.columns();
            height = image.rows();
            
            block = (char*)malloc(width*height*3);
            image.write(0,0, width, height, "BGR", CharPixel, block);
        }
        
        catch(Magick::Exception &err){
            cout << "Exception - " <<err.what()<<endl;
        }
        
        
        
        //        Mat cv_im(int(height),int(width),CV_8U,3);
        Mat cv_im(int(image.rows()),int(image.columns()),CV_8UC3);
        memcpy(cv_im.data, block, width*height*3 );
        
        if(write_output){
            imwrite(save_dir, cv_im);
        }
        return cv_im;
        
    }
    
    /*!
     im - image to enhance
     Use opencv2imagemagick(Mat& im,string save_dir) if theres a need to save to dir
     
     1. Converts opencv image to ImageMagick image format
     2. Enhances the ImageMagick image by cconverting to grayscale,normalizing and upscaling by 300%.
     3. Converts it back again to opencv
     */
    Mat Splice::opencv2imagemagick(Mat& im){
        
        
        Mat om;
        im.copyTo(om);
        
        char* block;
        size_t width,height;
        
        Magick::InitializeMagick("");
        
        Magick::Image image(om.cols,om.rows,"BGR",CharPixel,(char *)om.data);
        
        try{
            
            width = image.columns();
            height = image.rows();
            
            block = (char*)malloc(width*height*3);
            image.write(0,0, width, height, "BGR", CharPixel, block);
        }
        
        catch(Magick::Exception &err){
            cout << "Exception - " <<err.what()<<endl;
        }
        
        
        
        //        Mat cv_im(int(height),int(width),CV_8U,3);
        Mat cv_im(int(image.rows()),int(image.columns()),CV_8UC3);
        memcpy(cv_im.data, block, width*height*3 );
        
        return cv_im;
        
    }
    
    
    /*!
     im - Image to perform OCR on
     Helper function to convert opencv image format to tesseract image format and perform OCR on that image.
     */
    
    string Splice::opencv2tesseract(cv::Mat &tm){
        
        
        cout << "-------------------- Tesseract on the fly--------------------" << endl;
        
        string res;
        
        TessBaseAPI *api = new TessBaseAPI();
        
        
        if (api->Init(NULL, "eng")) {
            cerr << "Tesseract API not initialised" << endl;
            exit(1);
        }
        
        //            Pix *image = pixRead(read_path.c_str());
        api->SetImage((uchar*)tm.data,int(tm.size().width),int(tm.size().height),tm.channels(),int(tm.step1()));
        
        const char *result = api->GetUTF8Text();
        res = string(result);
        
        string returned_string = dictionary_comparision(res);
        
        string delimiter = "\n";
        
        string delimited_string = res.substr(0,res.find(delimiter));
        vector<size_t> temp_distances;
        size_t lowest_distance = 100;
        string matched_string;
        
        for (vector<string>::iterator aw_it=all_words.begin();aw_it != all_words.end();++aw_it){
            
            size_t dist = levenshtein_distace(*aw_it, delimited_string);
            
            if(dist <= 5){
                if(dist < lowest_distance){
                    lowest_distance = dist;
                    matched_string = *aw_it;
                }
            }
            
            temp_distances.push_back(dist);
            //                cout << "Levenshtein dist - " << dist << ", Original - " << *aw_it << ", From_tesseract - "<< res<< endl;
            
        }
        
        
        if(!matched_string.empty()){
            cout << "Distance - " << lowest_distance << ", Matched word - " << matched_string << endl;
        }
        else{
            cerr << "No matched string" << endl;
        }
        
        
        /*
         map<string,int>::iterator it = tearsheet_words.find(res);
         
         if (tearsheet_words.count(res)) {
         cout << "word found - " <<res << endl;
         }
         
         
         //            assert(it!=tearsheet_words.end());
         //            cout << "Key - " << it->first << " value - " << it->second << endl;
         if (it!=tearsheet_words.end()) {
         
         cout << "map contains key" << endl;
         cout << "Key - " << it->first << " value - " << it->second << endl;
         }
         
         */
        
        cout << "Dictionary" << " - " << returned_string << ", Input - " << result << endl;
        
        
        api->End();
        delete [] result;
        
        return matched_string;
        
    }
    
    string Splice::last4chars(string& dir_path){
        return dir_path.substr(dir_path.size() - 4);
    }
    
    string Splice::join_path(const string& path1,const string& path2){
        string full_path;
        char last_slash = path1.back();
        char first_slash = path2.at(0);
        
        if (last_slash == '/' || first_slash=='/') {
            full_path = path1 + path2;
        }
        else{
            full_path = path1 + "/" + path2;
        }
        
        return full_path;
    }
    
    /*!
     opencv2imagemagick_test - Test function to convert opencv image to imagemagick image format. Use opencv2imagemagick() instead. This function is only for testing.
     */
    void Splice::opencv2imagemagick_test(string p){
        
        
        Mat om = imread(p);
        Size s = om.size();
        
        
        Magick::InitializeMagick("");
        
        Magick::Image image(s.width,s.height,"BGR",CharPixel,(char *)om.data);
        
        try{
            
            image.write(cwd + "/opencv2imagemagick.jpg") ;
            
        }
        catch(Magick::Exception &err){
            cout << "Exception" << err.what() << endl;
        }
        
    }
    
    
    
    /*!
     Helper function to test imageMagick C++ api enhance features. Use opencv2imagemagick instead.
     */
    void Splice::image_magick_cleanup(string image_path){
        
        Magick::InitializeMagick("");
        
        for (int i=0; i<no_of_components; i++) {
            //                for (int i=0; i<30; i++) {
            
            Magick::Image image;
            string images_path = image_path + to_string(i) + ".jpg";
            
            try{
                
                image.read(images_path);
                image.type(GrayscaleType);
                image.normalize();
                //                image.sharpen();
                image.resize("300%");
                image.write(cwd + to_string(i) + "_gray.jpg") ;
                
            }
            catch(Magick::Exception &err){
                cout << "Exception" << err.what() << endl;
            }
            
        }
    }
    
    /*!
     Helper function to test tesseract. Use opencv2tesseract instead for converting image from opencv to tesseract
     */
    
    void Splice::tesseract_test(string image_path){
        
        cout << "-------------------- Tesseract on original image --------------------" << endl;
        
        //    for (int i=0; i<30; i++) {
        for (int i=0; i<no_of_components; i++) {
            TessBaseAPI api;
            char *result;
            
            if (api.Init(NULL, "eng")) {
                fprintf(stderr, "Tesseract API not initialised");
                exit(1);
            }
            
            string read_path = image_path + to_string(i) + "_gray.jpg";
            Pix *image = pixRead(read_path.c_str());
            api.SetImage(image);
            
            result = api.GetUTF8Text();
            cout << i << " - " << result << endl;
            
            api.End();
            delete [] result;
            pixDestroy(&image);
        }
    }
    
    /*!
     Tesseract test helper funciton to read image from disk and perform dictionary matching.
     Use opencv2tesseract instead.
     */
    
    void Splice::tesseract_with_dictionary(string image_path){
        
        
        cout << "-------------------- Tesseract after running script on original image--------------------" << endl;
        
        string res;
        
        //        for (int i=0; i<30; i++) {
        for (int i=0; i<no_of_components; i++) {
            TessBaseAPI *api = new TessBaseAPI();
            
            
            if (api->Init(NULL, "eng")) {
                fprintf(stderr, "Tesseract API not initialised");
                exit(1);
            }
            
            string read_path = image_path + to_string(i) + "_gray.jpg";
            Pix *image = pixRead(read_path.c_str());
            api->SetImage(image);
            
            const char *result = api->GetUTF8Text();
            size_t size = strlen(result) + 1;
            wchar_t w_result[size];
            mbstowcs(w_result, result, size);
            //            printf("Wide char - %ls",w_result);
            //            res.clear();
            res = string(result);
            //            res.erase(remove_if(res.begin(), res.end(), this->choose_char), res.end());
            string st("Text Input");
            //            printf("tess result - %s",result);
            //            cout << "tesseract result - " << res << endl;
            //            parse_word(res);
            string returned_string = dictionary_comparision(res);
            
            
            
            /*
             map<string,int>::iterator it = tearsheet_words.find(res);
             
             if (tearsheet_words.count(res)) {
             cout << "word found - " <<res << endl;
             }
             
             
             //            assert(it!=tearsheet_words.end());
             //            cout << "Key - " << it->first << " value - " << it->second << endl;
             if (it!=tearsheet_words.end()) {
             
             cout << "map contains key" << endl;
             cout << "Key - " << it->first << " value - " << it->second << endl;
             }
             
             */
            
            cout << "Dictionary" << " - " << returned_string << ", Input - "<< i << " - " << result << endl;
            
            
            api->End();
            delete [] result;
            pixDestroy(&image);
        }
    }
    
    
    /*!
     Checks if 2 rectangles intersect
     */
    bool Splice::rect_intersect(Rect r1, Rect r2){
        
        bool x_overlap = overlap(r1.x, r2.x, r2.x+ r2.width)  || overlap(r2.x, r1.x, r1.x+ r1.width);
        bool y_overlap = overlap(r1.y, r2.y, r2.y+ r2.height) || overlap(r2.y, r1.y, r1.y+ r1.height);
        
        return x_overlap && y_overlap;
    }
    
    /*!
     Checks to see if a value is greater than minimum and less than maximum. Used by rect_intersect to check if rectangles intersect
     */
    bool Splice::overlap(int value, int min, int max){
        
        return (value >= min) && (value <= max);
    }
    
    /*!
     Finds the angle between the points
     */
    double Splice::angle( Point pt1, Point pt2, Point pt0 )
    {
        double dx1 = pt1.x - pt0.x;
        double dy1 = pt1.y - pt0.y;
        double dx2 = pt2.x - pt0.x;
        double dy2 = pt2.y - pt0.y;
        return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
    }
    
    
    bool Splice::choose_char(char c){
        
        if (c != ' ' || c!= '/' || c != '~' || c != '.' || c != ',' || c != ';' || c != '"') {
            return false;
        }
        else {
            return true;
        }
        
    }
    
    /*!
     Parse word using regex
     */
    void Splice::parse_word(string &word){
        
        regex r("\\b(Nav)([^ ]*)");
        smatch sm;
        
        regex_search(word,sm,r);
        
        for(auto x:sm){
            cout << x << " regex found" << endl;
        }
    }
    
    
    void Splice::counter_inc(){
        no_of_components++;
    }
    
    
    
    
    
    
    /*!
     Keypoint matching using homography
     Refer to homography example in opencv source code in opencv_source_code/samples/cpp/tutorial_code/features2D/SURF_Homography.cpp
     */
    void Splice::keypoint_matching(){
        
        
        
        
        for (int hm = 0; hm<1; hm++) {
            
        
        
        Mat img_object = imread( "/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/comps/" + to_string(hm) + ".jpg", CV_LOAD_IMAGE_GRAYSCALE );
        Mat img_scene = imread( "/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved.jpg", CV_LOAD_IMAGE_GRAYSCALE );
        
//        resize(img_scene,img_scene,Size(0,0),0.8,0.8);
        
        if( !img_object.data || !img_scene.data )
        { cout<< " --(!) Error reading images " << endl; }
        
        //-- Step 1: Detect the keypoints using SURF Detector
        int minHessian = 400;
        
        SurfFeatureDetector detector( minHessian );
        
        vector<KeyPoint> keypoints_object, keypoints_scene;
        
        detector.detect( img_object, keypoints_object );
        detector.detect( img_scene, keypoints_scene );
        
        //-- Step 2: Calculate descriptors (feature vectors)
        SurfDescriptorExtractor extractor;
        
        Mat descriptors_object, descriptors_scene;
        
        extractor.compute( img_object, keypoints_object, descriptors_object );
        extractor.compute( img_scene, keypoints_scene, descriptors_scene );
        
        //-- Step 3: Matching descriptor vectors using FLANN matcher
        FlannBasedMatcher matcher;
        vector< DMatch > matches;
        matcher.match( descriptors_object, descriptors_scene, matches );
        
        double max_dist = 0; double min_dist = 100;
        
        //-- Quick calculation of max and min distances between keypoints
        for( int i = 0; i < descriptors_object.rows; i++ )
        { double dist = matches[i].distance;
            if( dist < min_dist ) min_dist = dist;
            if( dist > max_dist ) max_dist = dist;
        }
        
        printf("-- Max dist : %f \n", max_dist );
        printf("-- Min dist : %f \n", min_dist );
        
        //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
        vector< DMatch > good_matches;
        
        for( int i = 0; i < descriptors_object.rows; i++ )
        { if( matches[i].distance < 3*min_dist )
            { good_matches.push_back( matches[i]); }
        }
        
        Mat img_matches;
        drawMatches( img_object, keypoints_object, img_scene, keypoints_scene,
                    good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                    vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
        
        
        //-- Localize the object from img_1 in img_2
        vector<Point2f> obj;
        vector<Point2f> scene;
        
        for( size_t i = 0; i < good_matches.size(); i++ )
        {
            //-- Get the keypoints from the good matches
            obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
            scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
        }
        
        Mat H = findHomography( obj, scene, CV_RANSAC );
        
        //-- Get the corners from the image_1 ( the object to be "detected" )
        vector<Point2f> obj_corners(4);
        obj_corners[0] = Point(0,0); obj_corners[1] = Point( img_object.cols, 0 );
        obj_corners[2] = Point( img_object.cols, img_object.rows ); obj_corners[3] = Point( 0, img_object.rows );
        vector<Point2f> scene_corners(4);
        
        perspectiveTransform( obj_corners, scene_corners, H);
        
        
        //-- Draw lines between the corners (the mapped object in the scene - image_2 )
        Point2f offset( (float)img_object.cols, 0);
        line( img_matches, scene_corners[0] + offset, scene_corners[1] + offset, Scalar(0, 255, 0), 4 );
        line( img_matches, scene_corners[1] + offset, scene_corners[2] + offset, Scalar( 0, 255, 0), 4 );
        line( img_matches, scene_corners[2] + offset, scene_corners[3] + offset, Scalar( 0, 255, 0), 4 );
        line( img_matches, scene_corners[3] + offset, scene_corners[0] + offset, Scalar( 0, 255, 0), 4 );
        
            if(write_output){
            imwrite(cwd + "/keypoints/" + to_string(hm) + ".jpg", img_matches);
            }
        //-- Show detected matches
            if(show_windows){
//        imshow( "Good Matches & Object detection", img_matches );
            }
      }
    }

}