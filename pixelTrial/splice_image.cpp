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

#define SHELLSCRIPT "\
#/bin/bash \n\
echo \"hello\" \n\
/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/convert -type grayscale -normalize -negate -lat 15x15+5% \
-negate /Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved.jpg \
/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_run_script.jpg \n\
"

#include "splice_image.h"

namespace ppc{
    
    Splice::Splice():path(""),thresh(50),N(1),counter(0),rng(12345){
        
    }
    
    Splice::Splice(const string& dir_path):thresh(50),N(1),counter(0),rng(12345){
        path = dir_path;
    }
    
    Splice::~Splice(){
        
    }
    
    void Splice::init(){

       // Call only watershed. No Need to call others. Watershed calls the rest.
        
        this->init_dictionary();
        
        this->watershed();
//        this->text_cleaner();
//        this->squares_method();
    }
    
    void Splice::watershed(){
        
        bool show_windows = false;
        
        Mat source_image_resized,binary,image_for_saving;
        
        CheckWithMessage(std::string("Set path during initialization before finding components"), path != "");
        LoadImage load_source_image(this->path,"SquaresImage");
        Mat source_image = load_source_image.get_image();
        CheckWithMessage(std::string("Image loaded incorrectly"), source_image.data);
        
//        source_image.copyTo(source_image_resized);
        //load_source_image.set_image(source_image_resized);
        //source_image_resized = load_source_image.get_image();
        cv::resize(source_image, source_image_resized, cv::Size(0,0), 0.5,0.5 );
        source_image_resized.copyTo(image_for_saving);
        
        cvtColor(source_image_resized, binary, CV_BGR2GRAY);
        threshold(binary, binary, 100, 255, CV_THRESH_BINARY);
        
        if (show_windows) {
            imshow("binary", binary);
        }
        
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
        std::vector<std::vector<cv::Point>> main_contours;
        std::vector<std::vector<cv::Point>> needed_contours;
        vector<cv::Vec4i> squares_hierarchy;
        vector<cv::Vec4i> hierar;
        vector<int> indexes;
        Scalar color(0,255,0);
        findContours(res_canny, main_contours, squares_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
        res_output = Mat::zeros(res_canny.size(), CV_8UC3);
        
        //        cout << "Num of watershed contours - " << main_contours.size() << endl;
        
        drawContours(res_output, main_contours, -1, color,2,8,hierar,0,Point());
        
//        imshow("resOutput", res_output);
        
        for (int i=0; i<squares_hierarchy.size(); i++) {
            //        std::cout  << hierarchy[i] << std::endl;
            if (squares_hierarchy[i][3] == -1) {
                hierar.push_back(squares_hierarchy[i]);
                indexes.push_back(i);
            }
        }
        
        for (vector<int>::iterator itt = indexes.begin();itt!=indexes.end();++itt) {
            needed_contours.push_back(main_contours[*itt]);
        }
        
        drawContours(image_for_saving, main_contours, -1, color,2,8,squares_hierarchy,0,Point());
        
        imwrite("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_watershed.jpg", image_for_saving);
        
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
        
        for (vector<Rect>::iterator rect_iterator = all_rectangles.begin(); rect_iterator!=all_rectangles.end(); ++rect_iterator) {
            
            if (inserted_areas.size()==0) {
                inserted_areas.push_back(rect_iterator->area());
                sorted_rectangles.push_back(*rect_iterator);
            }
            
            else{
                
                
                for (int sr=0; sr<sorted_rectangles.size(); sr++) {
                    if (rect_iterator->area() > sorted_rectangles[sr].area() && sr!=sorted_rectangles.size()-1) {
                        continue;
                    }
                    
                    else{
                        sorted_rectangles.insert(sorted_rectangles.begin() + sr, *rect_iterator);
                        break;
                    }
                }
                
            }
            
        }
        
        sort(areas.begin(), areas.end());
        sort(all_rectangles.begin(),all_rectangles.end(),mysortfunction);
        
        
        for (vector<Rect>::iterator sr=sorted_rectangles.begin(); sr!=sorted_rectangles.end(); ++sr) {
            cout << "sorted_rectangles_area[" << sr-sorted_rectangles.begin() <<"] - " <<sr->area() <<endl;
        }
        
        for (vector<Rect>::iterator ar=all_rectangles.begin(); ar!=all_rectangles.end(); ++ar) {
            cout << "all_rectangles_area[" << ar-all_rectangles.begin() <<"] - " <<ar->area() <<endl;
        }
        
        for (vector<int>::iterator areas_vector = areas.begin(); areas_vector!=areas.end(); ++areas_vector) {
            cout << "areas[" << areas_vector-areas.begin() <<"]- " << *areas_vector <<endl;
        }
        
        cout << "all_rectangles_size - " <<all_rectangles.size()<<endl;
        Mat cop = source_image_resized(second_highest_area_rect);
        cop.copyTo(saved);
        
        this->save_image("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/");
        this->image_magick();
        this->thresholding_image();
//        this->text_cleaner();
//        this->run_script();
        
        this->squares_method(saved,"/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_result.jpg","/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/comps/","/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_contours.jpg");
        
        LoadImage load_saved_run_script("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_run_script.jpg","SquaresImage");
        Mat saved_run_script = load_saved_run_script.get_image();
        CheckWithMessage(std::string("Image loaded incorrectly"), saved_run_script.data);
        
        imwrite("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_run_script2.jpg",saved_run_script);
        
        this->squares_method2(saved_run_script,"/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_run_script2.jpg","/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/compsFromImageMagick/","/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_run_contours.jpg");
        
        squares_method2(saved,"/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_run_script2.jpg","/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/compsFromImageMagick_originalImage/","/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_run_contours_original_image.jpg");
        
        
        this->image_magick2();
        this->tesseract();
        
        this->image_magick3();
        this->tesseract2();
        
        this->image_magick4();
        this->tesseract3();
//        this->keypoint_matching();
    }
    
    void Splice::thresholding_image(){
        
        Mat src_gray,adaptive_thresholded_image,noise;
        
        Mat src = imread("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved.jpg", 1 );
        
        cvtColor( src, src_gray, CV_RGB2GRAY );
        
        fastNlMeansDenoising(src_gray, noise);
        
        adaptiveThreshold(noise, adaptive_thresholded_image, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 7, 5);
        
        imwrite("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_run_script.jpg", adaptive_thresholded_image);
    }
    
    void Splice::save_image(const string& save_dir){
        imwrite(save_dir + "/saved.jpg", saved);
    }
    
    
    bool Splice::mysortfunction(Rect r1, Rect r2){
        
        
        return r1.area()<r2.area();
    }
    
    void Splice::text_cleaner(){
        
        system(SHELLSCRIPT);
        
        /*
        char command[50];
        string inputFile="/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved.jpg";
        string outputFile="/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/fred_result.jpg";
        

        
        system("chmod 777 /Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/textcleaner.sh");
        system("chmod 777 /Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/Components/cop.jpg");
        system("chmod 777 /Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/Components/");
        system("chmod 777 /Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/convert");
        system("chmod 777 /Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved.jpg");

            sprintf(command,"/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/textcleaner.sh -g -e normalize %s %s",inputFile.c_str(),outputFile.c_str());
        
        system(command);
        
//        system("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/textcleaner.sh -g -e normalize /Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/Components/0.jpg /Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/Components/0_result.jpg");
        
               
               
               
//                /Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/cop.jpg /Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/cop_result.jpg");
        
        cout << "Done" <<endl;
         
         */
    }
    
    void Splice::image_magick(){
        
        InitializeMagick("");
        
        Magick::Image image;
        string images_path = string("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved.jpg");
        
        try{
            
            image.read(images_path);
            image.type(GrayscaleType);
            image.normalize();
            image.write("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_result.jpg");
            
        }
        catch(Magick::Exception &err){
            cout << "Exception" << err.what() << endl;
        }
        
    }
    
    void Splice::run_script(){
        
        InitializeMagick("");
        
        Magick::Image image;
        string images_path = string("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved.jpg");
        
        try{
            
            image.read(images_path);
            image.type(GrayscaleType);
            image.normalize();
            image.negate();
            image.adaptiveThreshold(40, 40,5);
//            image.contrastStretch(0, 0);
//            image.compose(CopyOpacityCompositeOp);
            image.negate(true);
            image.write("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_run_script.jpg");
            
        }
        catch(Magick::Exception &err){
            cout << "Exception" << err.what() << endl;
        }
    }
    
    void Splice::squares_method(const Mat& original_image,string image_pth,string components_path,string contours_path){
//        static const char* names[] = {"/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_result.jpg",0 };
        static const char* names[] = {image_pth.c_str(),0 };
        
        vector<vector<Point> > squares;
        
        for( int i = 0; names[i] != 0; i++ )
        {
            Mat image = imread(names[i], 1);
            if( image.empty() )
            {
                cout << "Couldn't load " << names[i] << endl;
                continue;
            }
            
            //threshold(image, thresholded_image, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
            
            //resize(image, image, Size(0,0),0.5,0.5);
            
            findSquares(original_image,image, squares,components_path,contours_path);
            //drawSquares(image, squares);
            //imshow(wndname, image);
            
            //imwrite("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/cop_squares.jpg", image);
            
        }
    }
    
    void Splice::findSquares( const Mat& original_image,const Mat& image, vector<vector<Point> >& squares,string components_path,string contours_path )
    {
        squares.clear();
        counter=0;
        Mat image_for_saving;
        image.copyTo(image_for_saving);
        
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
                // hack: use Canny instead of zero threshold level.
                // Canny helps to catch squares with gradient shading
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
                        
                        
                        /*
                        Mat im(original_image,boundRect[i]);
                        //                    imwrite("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/comps/" + to_string((int)c) +"-"+ to_string((int)l) +"-"+ to_string((int)i) + ".jpg", im);
                        imwrite("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/comps/" +to_string(c) +"-"+ to_string(counter) + ".jpg", im);
                        counter++;
                         */
                        // if cosines of all angles are small
                        // (all angles are ~90 degree) then write quandrange
                        // vertices to resultant sequence
                        if( maxCosine < 0.3 )
                        squares.push_back(approx);
                    }
                }
            }
        }
      
        for( size_t i = 0; i < squares.size(); i++ )
        {
            Rect rt = boundingRect(Mat(squares[i]));
            
            Size s = image.size();
//            cout << "img_area - " << s.area() << " rt.area - " << rt.area()<<endl;
            if (rt.area() < 3*s.area() / 4) {
                
                drawContours(image_for_saving, squares, (int)i, Scalar(0,255,0),2,8, vector<Vec4i>(), 0, Point());
                
                Mat im(original_image,rt);
            imwrite(components_path + to_string(counter) + ".jpg", im);
//                imwrite("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/comps/" + to_string(counter) + ".jpg", im);
                counter++;
            }
            

        }
  
            imwrite(contours_path, image_for_saving);
        
//        imwrite("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_contours.jpg", image_for_saving);
        //imshow("drawing", drawing);
    }
    
    
    
    void Splice::squares_method2(const Mat& original_image,string image_pth,string components_path,string contours_path){
        //        static const char* names[] = {"/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_result.jpg",0 };
        static const char* names[] = {"/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_run_script2.jpg",0 };
        
        vector<vector<Point> > squares;
        
        for( int i = 0; names[i] != 0; i++ )
        {
            Mat image = imread(names[i], 1);
            if( image.empty() )
            {
                cout << "Couldn't load " << names[i] << endl;
                continue;
            }
            
            //threshold(image, thresholded_image, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
            
            //resize(image, image, Size(0,0),0.5,0.5);
            
            findSquares2(original_image,image, squares,components_path,contours_path);
            //drawSquares(image, squares);
            //imshow(wndname, image);
            
            //imwrite("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/cop_squares.jpg", image);
            
        }
    }
    
    void Splice::findSquares2( const Mat& original_image,const Mat& image, vector<vector<Point> >& squares,string components_path,string contours_path )
    {
        squares.clear();
        counter = 0;
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
                // hack: use Canny instead of zero threshold level.
                // Canny helps to catch squares with gradient shading
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
                        
                        
                        /*
                         Mat im(original_image,boundRect[i]);
                         //                    imwrite("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/comps/" + to_string((int)c) +"-"+ to_string((int)l) +"-"+ to_string((int)i) + ".jpg", im);
                         imwrite("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/comps/" +to_string(c) +"-"+ to_string(counter) + ".jpg", im);
                         counter++;
                         */
                        // if cosines of all angles are small
                        // (all angles are ~90 degree) then write quandrange
                        // vertices to resultant sequence
                        if( maxCosine < 0.3 )
                        squares.push_back(approx);
                    }
                }
            }
        }
        
        for( size_t i = 0; i < squares.size(); i++ )
        {
            Rect rt = boundingRect(Mat(squares[i]));
            
            Size s = image.size();
            //            cout << "img_area - " << s.area() << " rt.area - " << rt.area()<<endl;
            if (rt.area() < 3*s.area() / 4 && rt.width < 1000 && rt.height<1000) {
                
                drawContours(image_for_saving, squares, (int)i, Scalar(0,255,0),2,8, vector<Vec4i>(), 0, Point());
                
                Mat im(original_image,rt);
                imwrite(components_path + to_string(counter) + ".jpg", im);
                //                imwrite("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/comps/" + to_string(counter) + ".jpg", im);
                counter++;
            }
            
            
        }
        
        imwrite(contours_path, image_for_saving);
        
        //        imwrite("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved_contours.jpg", image_for_saving);
        //imshow("drawing", drawing);
    }
    
    
    double Splice::angle( Point pt1, Point pt2, Point pt0 )
    {
        double dx1 = pt1.x - pt0.x;
        double dy1 = pt1.y - pt0.y;
        double dx2 = pt2.x - pt0.x;
        double dy2 = pt2.y - pt0.y;
        return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
    }
    
    void Splice::image_magick2(){
        
        InitializeMagick("");
        
//        for (int i=0; i<counter; i++) {
                for (int i=0; i<30; i++) {
        
            Magick::Image image;
            string images_path = string("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/comps/") + to_string(i) + ".jpg";
            
            try{
                
                image.read(images_path);
                image.type(GrayscaleType);
                image.normalize();
//                image.sharpen();
                image.resize("300%");
                image.write("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/comps/" + to_string(i) + "_gray.jpg") ;
                
            }
            catch(Magick::Exception &err){
                cout << "Exception" << err.what() << endl;
            }
            
        }
    }
    
    
    void Splice::tesseract(){
        
    cout << "-------------------- Tesseract on original image --------------------" << endl;

    for (int i=0; i<30; i++) {
//        for (int i=0; i<counter; i++) {
            TessBaseAPI api;
            char *result;
            
            if (api.Init(NULL, "eng")) {
                fprintf(stderr, "Tesseract API not initialised");
                exit(1);
            }
            
            string read_path = "/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/comps/" + to_string(i) + "_gray.jpg";
            Pix *image = pixRead(read_path.c_str());
            api.SetImage(image);
            
            result = api.GetUTF8Text();
            cout << i << " - " << result << endl;
            
            api.End();
            delete [] result;
            pixDestroy(&image);
        }
    }
    
    
    void Splice::image_magick3(){
        
        InitializeMagick("");
        
        //        for (int i=0; i<counter; i++) {
        for (int i=0; i<30; i++) {
            
            Magick::Image image;
            string images_path = string("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/compsFromImageMagick/") + to_string(i) + ".jpg";
            
            try{
                
                image.read(images_path);
                image.type(GrayscaleType);
                image.normalize();
                //                image.sharpen();
                image.resize("300%");
                image.write("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/compsFromImageMagick/" + to_string(i) + "_gray.jpg") ;
                
            }
            catch(Magick::Exception &err){
                cout << "Exception" << err.what() << endl;
            }
            
        }
    }
    
    
    void Splice::tesseract2(){
        
        
        cout << "-------------------- Tesseract after running script --------------------" << endl;
        
        for (int i=0; i<30; i++) {
            //        for (int i=0; i<counter; i++) {
            TessBaseAPI api;
            char *result;
            
            if (api.Init(NULL, "eng")) {
                fprintf(stderr, "Tesseract API not initialised");
                exit(1);
            }
            
            string read_path = "/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/compsFromImageMagick/" + to_string(i) + "_gray.jpg";
            Pix *image = pixRead(read_path.c_str());
            api.SetImage(image);
            
            result = api.GetUTF8Text();
            cout << i << " - " << result << endl;
            
            api.End();
            delete [] result;
            pixDestroy(&image);
        }
    }
    
    void Splice::image_magick4(){
        
        InitializeMagick("");
        
        //        for (int i=0; i<counter; i++) {
        for (int i=0; i<30; i++) {
            
            Magick::Image image;
            string images_path = string("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/compsFromImageMagick_originalImage/") + to_string(i) + ".jpg";
            
            try{
                
                image.read(images_path);
                image.type(GrayscaleType);
                image.normalize();
                //                image.sharpen();
                image.resize("300%");
                image.write("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/compsFromImageMagick_originalImage/" + to_string(i) + "_gray.jpg") ;
                
            }
            catch(Magick::Exception &err){
                cout << "Exception" << err.what() << endl;
            }
            
        }
    }
    
    
    void Splice::tesseract3(){
        
        
        cout << "-------------------- Tesseract after running script on original image--------------------" << endl;
        
        string res;
        
        for (int i=0; i<30; i++) {
            //        for (int i=0; i<counter; i++) {
            TessBaseAPI *api = new TessBaseAPI();
            
            
            if (api->Init(NULL, "eng")) {
                fprintf(stderr, "Tesseract API not initialised");
                exit(1);
            }
            
            string read_path = "/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/compsFromImageMagick_originalImage/" + to_string(i) + "_gray.jpg";
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
//            dictionary_comparision(st);
            
            
//            for (vector<string>::iterator i= all_words.begin(); i!=all_words.end();i++) {
        
//            if (strcmp(result, i->c_str()) ==0) {
            
            cout << "compare - " << strcmp(result, "search") << " ,result - "<< result<< endl;
              if (strcmp(result, "Text Input") ==0) {
                cout << "yes - " <<endl;
//            }
                
        }
            
            if(find(all_words.begin(), all_words.end(), res) != all_words.end()){
                
                cout << res << " found" <<endl;
                
            }
            else{
                cout << res << " not found" <<endl;
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
        
            cout << i << " - " << result << endl;
            
            api->End();
            delete [] result;
            pixDestroy(&image);
        }
    }
    
    bool Splice::choose_char(char c){
        
        if (c != ' ' || c!= '/' || c != '~' || c != '.' || c != ',' || c != ';' || c != '"') {
            return false;
        }
        else {
            return true;
        }
        
    }
    
    void Splice::parse_word(string &word){
        
        regex r("\\b(Nav)([^ ]*)");
        smatch sm;
        
        regex_search(word,sm,r);
        
        for(auto x:sm){
            cout << x << " regex found" << endl;
        }
    }
    
    void Splice::init_dictionary(){
        
//        tearsheet_words[0] = "Navigation";
        
        
        /*
        tearsheet_words.insert(make_pair("Navigation", 0));
        tearsheet_words.insert(make_pair("Panel", 1));
        
        tearsheet_words.insert(make_pair("BUTTON", 2));
        tearsheet_words.insert(make_pair("PRIMARY BUTTON", 3));
        tearsheet_words.insert(make_pair("DISABLED BUTTON", 4));
        
        tearsheet_words.insert(make_pair("LINK 01", 5));
        tearsheet_words.insert(make_pair("LINK 02", 6));
        tearsheet_words.insert(make_pair("LINK 03", 7));
        tearsheet_words.insert(make_pair("LINK 04", 8));
        tearsheet_words.insert(make_pair("LINK 05", 9));
        tearsheet_words.insert(make_pair("LINK 06", 10));
        
        tearsheet_words.insert(make_pair("search", 11));
        tearsheet_words.insert(make_pair("Text Input", 12));
        tearsheet_words.insert(make_pair("Select", 13));
        tearsheet_words.insert(make_pair("World", 14));

        
        dictionary_comparision("Select");
        */
         
//        map<string,int>::iterator it;
//        it = tearsheet_words.find("Select");
//        
//        if (it!=tearsheet_words.end()) {
//            int m = it->second++;
//            cout << "m - " << m << endl;
//        }
        
        
        string t_words[] = {"Navigation","Panel","BUTTON","PRIMARY BUTTON","DISABLED BUTTON","LINK 01","LINK 02","LINK 03","LINK 04","LINK 05","LINK 06","search","Text Input","Select","World"};
        
        
        vector<string> words;
        
        for (int w=0; w<sizeof(t_words)/sizeof(*t_words); w++) {
//            cout << "word pushback - " << t_words[w] <<endl;
            words.push_back(t_words[w]);
        }
        
        
        all_words.insert(all_words.end(), words.begin(),words.end());
        
        for (vector<string>::iterator i= all_words.begin(); i!=all_words.end();i++) {
//            cout << "all words - " << *i << endl;
         }
        
    }
    
    void Splice::dictionary_comparision(string word){
        
        for (vector<string>::iterator i= all_words.begin(); i!=all_words.end();i++) {
                        cout << "all words - " << *i << endl;
        }
        
        
        if(find(all_words.begin(), all_words.end(), word) != all_words.end()){
            
            cout << word << " found" <<endl;
            
        }
        /*
        else{
            cout << word << " not found" <<endl ;
        }
        
       */
        
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
    
    void Splice::keypoint_matching(){
        
        
        
        
        for (int hm = 0; hm<1; hm++) {
            
        
        
        Mat img_object = imread( "/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/basic_thresholding/comps/" + to_string(hm) + ".jpg", CV_LOAD_IMAGE_GRAYSCALE );
        Mat img_scene = imread( "/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/saved.jpg", CV_LOAD_IMAGE_GRAYSCALE );
        
//        resize(img_scene,img_scene,Size(0,0),0.8,0.8);
        
        if( !img_object.data || !img_scene.data )
        { std::cout<< " --(!) Error reading images " << std::endl; }
        
        //-- Step 1: Detect the keypoints using SURF Detector
        int minHessian = 400;
        
        SurfFeatureDetector detector( minHessian );
        
        std::vector<KeyPoint> keypoints_object, keypoints_scene;
        
        detector.detect( img_object, keypoints_object );
        detector.detect( img_scene, keypoints_scene );
        
        //-- Step 2: Calculate descriptors (feature vectors)
        SurfDescriptorExtractor extractor;
        
        Mat descriptors_object, descriptors_scene;
        
        extractor.compute( img_object, keypoints_object, descriptors_object );
        extractor.compute( img_scene, keypoints_scene, descriptors_scene );
        
        //-- Step 3: Matching descriptor vectors using FLANN matcher
        FlannBasedMatcher matcher;
        std::vector< DMatch > matches;
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
        std::vector< DMatch > good_matches;
        
        for( int i = 0; i < descriptors_object.rows; i++ )
        { if( matches[i].distance < 3*min_dist )
            { good_matches.push_back( matches[i]); }
        }
        
        Mat img_matches;
        drawMatches( img_object, keypoints_object, img_scene, keypoints_scene,
                    good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                    vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
        
        
        //-- Localize the object from img_1 in img_2
        std::vector<Point2f> obj;
        std::vector<Point2f> scene;
        
        for( size_t i = 0; i < good_matches.size(); i++ )
        {
            //-- Get the keypoints from the good matches
            obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
            scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
        }
        
        Mat H = findHomography( obj, scene, CV_RANSAC );
        
        //-- Get the corners from the image_1 ( the object to be "detected" )
        std::vector<Point2f> obj_corners(4);
        obj_corners[0] = Point(0,0); obj_corners[1] = Point( img_object.cols, 0 );
        obj_corners[2] = Point( img_object.cols, img_object.rows ); obj_corners[3] = Point( 0, img_object.rows );
        std::vector<Point2f> scene_corners(4);
        
        perspectiveTransform( obj_corners, scene_corners, H);
        
        
        //-- Draw lines between the corners (the mapped object in the scene - image_2 )
        Point2f offset( (float)img_object.cols, 0);
        line( img_matches, scene_corners[0] + offset, scene_corners[1] + offset, Scalar(0, 255, 0), 4 );
        line( img_matches, scene_corners[1] + offset, scene_corners[2] + offset, Scalar( 0, 255, 0), 4 );
        line( img_matches, scene_corners[2] + offset, scene_corners[3] + offset, Scalar( 0, 255, 0), 4 );
        line( img_matches, scene_corners[3] + offset, scene_corners[0] + offset, Scalar( 0, 255, 0), 4 );
        
            
            imwrite("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/keypoints/" + to_string(hm) + ".jpg", img_matches);
        //-- Show detected matches
        imshow( "Good Matches & Object detection", img_matches );
      }
    }

}