//
//  find_components.cpp
//  PaperPixelCode
//
//  Created by Adarsh Kosuru on 6/30/14.
//  Copyright (c) 2014 Frog. All rights reserved.
//

#ifndef NDEBUG
#define Check(x) assert(x)
#define CheckWithMessage(str,x) assert((str,x))
#endif

#include "find_components.h"

namespace ppc {
    Components::Components():path(""){}
    Components::Components(std::string& dir_path):
    
        path(dir_path),
        no_of_threshold_levels(10),
        lower_thresh(0),
        upper_thresh(255),
        counter(0),
        extracted_images_counter(0),
        area(500),
        width(500),
        height(500),
        edge_image(lower_thresh,upper_thresh),
        window_size(CV_WINDOW_AUTOSIZE),
        display_extracted_images(false),
        contours_image(CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE)
    {}
    
    Components::~Components(){}
    
    double Components::angle( Point pt1, Point pt2, Point pt0 )
    {
        double dx1 = pt1.x - pt0.x;
        double dy1 = pt1.y - pt0.y;
        double dx2 = pt2.x - pt0.x;
        double dy2 = pt2.y - pt0.y;
        return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
    }
    
    void Components::print_values(){
        cout << "lower thresh - " << lower_thresh << "upper thresh - " << upper_thresh << endl;
        cout << "path - " << path << endl;
    }
    
    void Components::find() throw(cv::Exception){
        CheckWithMessage(std::string("Set path during initialization before finding components"), path != "");
        LoadImage load_source_image(this->path,"SquaresImage");
        source_image = load_source_image.get_image();
        CheckWithMessage(std::string("Image loaded incorrectly"), source_image.data);
        cv::resize(source_image, source_image_resized, cv::Size(0,0), 0.50,0.50 );
        
        load_source_image.set_image(source_image_resized);
        
        load_source_image.show();
        source_image_resized.copyTo(source_image_output);
        source_image_resized.copyTo(source_image_hull);
        
        cv::Mat gray0(source_image_resized.size(), CV_8U),gray;
        
        cv::medianBlur(source_image_resized, source_image_blurred, 0);
        //    cv::GaussianBlur(source_image_resized,blurred_image,Size(1,1),0);
//        cv::namedWindow("Blurred",CV_WINDOW_AUTOSIZE);
//        cv::imshow("Blurred", source_image_blurred);
        std::vector<std::vector<cv::Point>> contours;
        
        squares.clear();
        
        //    namedWindows();
        
        //    createTrackbar( "Threshold","ContourResult", &thresh,maxThreshold, changeThresh);
        
        
        squares_method(gray0,gray,contours);
        
//        cv::waitKey(0);
    }
    
    void Components::squares_method(cv::Mat& gray0,cv::Mat& gray,std::vector<std::vector<cv::Point>>& contours) throw(cv::Exception){
        
        vector<cv::Vec4i> parent_hierarchy;
        vector<int> indexes;
        
        for (int channels=0;channels < 3; channels++) {
            int mixChannelsArray[] = {channels,0};
            
            mixChannels(&source_image_blurred, 1, &gray0, 1, mixChannelsArray, 1);
            
            for (int threshold_level =0 ; threshold_level<no_of_threshold_levels; threshold_level++) {
                if (threshold_level ==0) {
                    gray = edge_image.set_source_image(gray0).set_aperture_size(5).applyCanny().get_result_image();
                    dilate(gray, gray, Mat(),Point(-1,-1));
                }
                else{
                    gray = gray0 >= (threshold_level + 1) * 255 / no_of_threshold_levels;
                }
                
                contours.clear();
                squares_hierarchy.clear();
                parent_hierarchy.clear();
                indexes.clear();
                needed_contours.clear();
                contours = contours_image.set_source_image(gray).find().get_contours();
                squares_hierarchy = contours_image.get_hierarchy();
                
                
                //            cout << "contour size - " << contours.size() << endl;
                //            cout << "hierarchy size - " << squares_hierarchy.size() << endl;
                
                for (vector<vector<Point>>::iterator itr = contours.begin(); itr!=contours.end(); ++itr) {
                    //cout << "contours - " << *itr << endl;
                }
                
                for (vector<cv::Vec4i>::iterator itr = squares_hierarchy.begin(); itr!=squares_hierarchy.end(); ++itr) {
                    //                cout << "square_hierarchy - " << threshold_level << " - "<<*itr << endl;
                    counter++;
                }
                
                //            cout << "counter - " << counter << endl;
                //            counter = 0;
                
                
                for (int i=0; i<squares_hierarchy.size(); i++) {
                    //        std::cout  << hierarchy[i] << std::endl;
                    if (squares_hierarchy[i][3] == -1) {
                        parent_hierarchy.push_back(squares_hierarchy[i]);
                        indexes.push_back(i);
                        //                    needed_contours.push_back(contours[indexes.size()-1]);
                        //            cout << "parent here - " << endl;
                        //            std::cout  << hierarchy[i] << std::endl;
                    }
                }
                
                
                for (vector<int>::iterator itt = indexes.begin();itt!=indexes.end();++itt) {
                    needed_contours.push_back(contours[*itt]);
                }
                
                
                //            cout << "indexes size - " << indexes.size() << endl;
                //            cout << "parent hierarchy size - " << parent_hierarchy.size() << endl;
                
                for (int i=0; i<parent_hierarchy.size(); i++) {
                    //         std::cout  << parent_hierarchy[i] << "index - " << indexes[i] <<std::endl;
                }
                
                for (vector<int>::iterator itt = indexes.begin();itt!=indexes.end();++itt) {
                    
                    //                cout << "selected contours - " << *itt<<  " - "<<contours[*itt] << endl;
                    //                needed_contours.push_back(contours[*itt]);
                    cv::Rect r = cv::boundingRect(cv::Mat(contours[*itt]));
                    //        cout << "rect width & height - " << itr - indexes.begin() << r.width << " , " << r.height << endl;
                    //        cout << "rect area - " << itr - indexes.begin() << r.area()  << endl;
                    
                    if (r.area() > area || r.width > width || r.height > height) {
                        all_rectangles.push_back(r);
                        //                    rectangle(source_image_output, r, cv::Scalar(255), 2);
                        
                        //if (flag == 0) {
                        Mat cop(source_image_resized,r);
                        cop.copyTo(copied);
                        extracted_images.push_back(copied);
                        //cout << "r.x - " << r.tl().x << ",  r.y - " << r.tl().y << ",  r.width - " << r.width << ",  r.height - " << r.height << endl;
                        //source_image_resized.copyTo(copied(Rect(r.tl().x,r.tl().y,r.width,r.height)));
                        
                        //    flag =1;
                        //}
                    }
                    
                }
                
                
                // Hull Method
                
                vector<vector<Point> >hull( needed_contours.size() );
                for( int i = 0; i < needed_contours.size(); i++ )
                {  convexHull( Mat(needed_contours[i]), hull[i], false ); }
                
                /// Draw contours + hull results
                Mat drawing = Mat::zeros( gray.size(), CV_8UC3 );
                for( int i = 0; i< needed_contours.size(); i++ )
                {
                    
                    Scalar color(0,255,0);
                    //Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
                    drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
                }
                
                for (int i=0; i<hull.size(); i++) {
                    //        cout << "hull - " << i << " - " << hull[i] << endl;
                }
                
                Mat rect_hull = cv::Mat::zeros(gray.size(), CV_8UC3);
                for (vector<vector<Point>>::iterator it=hull.begin(); it!=hull.end(); ++it) {
                    Rect rh = boundingRect(Mat(*it));
                    hull_rectangles.push_back(rh);
                    
                    //                Mat cop(source_image_resized,rh);
                    //                cop.copyTo(copied);
                    //                extracted_images.push_back(copied);
                    //                rectangle(rect_hull, rh, cv::Scalar(255), 2);
                    
                }
                
                /// Show in a window
                //            namedWindow( "Hull Contours", CV_WINDOW_AUTOSIZE );
                //            imshow( "Hull Contours", drawing );
                //            namedWindow( "HullRect", CV_WINDOW_AUTOSIZE );
                //            imshow( "HullRect", rect_hull );
                
                //            namedWindow("RectOutput",CV_WINDOW_AUTOSIZE);
                //            imshow("RectOutput", source_image_output);
                
                
                
                //End of Hull
                
                
                vector<Point> approx;
                for (vector<int>::iterator itt = indexes.begin();itt!=indexes.end();++itt) {
                    //            for (size_t i=0; i < contours.size(); i++) {
                    //                approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);
                    approxPolyDP(Mat(contours[*itt]), approx, arcLength(Mat(contours[*itt]), true) * 0.02, true);
                    
                    
                    if (approx.size() == 4 && fabs(contourArea(Mat(approx))) > 100 && isContourConvex(Mat(approx))) {
                        double maxCosine = 0;
                        
                        for (int j=2; j<5; j++) {
                            double cosine = fabs(angle(approx[j%4],approx[j-2],approx[j-1]));
                            maxCosine = MAX(maxCosine, cosine);
                            
                            if (maxCosine < 0.3) {
                                squares.push_back(approx);
                            }
                        }
                    }
                    
                }
                
                
            }
            
            //        namedWindow("Gray",CV_WINDOW_AUTOSIZE);
            //        imshow("Gray", gray);
        }
        
        for (size_t i=0; i<squares.size(); i++) {
            const Point* p = &squares[i][0];
            int n = (int)squares[i].size();
            polylines(source_image_resized, &p, &n, 1, true, Scalar(0,255,0),3,CV_AA);
        }
        
        namedWindow("ParentContours",CV_WINDOW_AUTOSIZE);
        imshow("ParentContours", source_image_resized);
        
        
        
//        cout << "all rectangles size - " << all_rectangles.size()<<endl;
        
        for (vector<Rect>::iterator rec=all_rectangles.begin(); rec!=all_rectangles.end(); ++rec) {
            rectangle(source_image_output, *rec, cv::Scalar(255), 2);
        }
        
        for (vector<Rect>::iterator rec=hull_rectangles.begin(); rec!=hull_rectangles.end(); ++rec) {
            rectangle(source_image_hull, *rec, cv::Scalar(255), 2);
        }
        
        
        if (display_extracted_images) {
            
            
            for (vector<Mat>::iterator exim=extracted_images.begin(); exim!=extracted_images.end(); ++exim) {
                string image_name = "Image"+to_string(extracted_images_counter);
                namedWindow(image_name,CV_WINDOW_AUTOSIZE);
                imshow(image_name, *exim);
                extracted_images_counter++;
            }
        }
        
        show_image_windows();
        
        /*
        namedWindow("AllRectangles",CV_WINDOW_AUTOSIZE);
        imshow("AllRectangles", source_image_output);
        namedWindow("HullRectangles",CV_WINDOW_AUTOSIZE);
        imshow("HullRectangles", source_image_hull);
         */
        
        //    namedWindow("Copied",CV_WINDOW_AUTOSIZE);
        //    imshow("Copied", copied);
        
    }
    
    
    void Components::show_image_windows(){
        
        namedWindow("ParentContours",CV_WINDOW_AUTOSIZE);
        imshow("ParentContours", source_image_resized);
        
        namedWindow("AllRectangles",CV_WINDOW_AUTOSIZE);
        imshow("AllRectangles", source_image_output);
        namedWindow("HullRectangles",CV_WINDOW_AUTOSIZE);
        imshow("HullRectangles", source_image_hull);
    }
    
    void Components::save_image(string dir){
        imwrite( dir + "/image_result.jpg", source_image_output);
    }
    
    
    void Components::find_grabcut() throw(cv::Exception){
        
    }
    
    void Components::find_watershed() throw(cv::Exception){
        CheckWithMessage(std::string("Set path during initialization before finding components"), path != "");
        LoadImage load_source_image(this->path,"SquaresImage");
        source_image = load_source_image.get_image();
        CheckWithMessage(std::string("Image loaded incorrectly"), source_image.data);
        cv::resize(source_image, source_image_resized, cv::Size(0,0), 0.50,0.50 );
        
        load_source_image.set_image(source_image_resized);
        source_image_resized = load_source_image.get_image();
        
        load_source_image.show();
        
        Mat binary;
        cvtColor(source_image_resized, binary, CV_BGR2GRAY);
        threshold(binary, binary, 100, 255, CV_THRESH_BINARY);
        imshow("binary", binary);
        
        Mat fg,bg;
        erode(binary, fg, Mat(),Point(-1,-1),3);
        imshow("fg", fg);
        
        dilate(binary, bg, Mat(),Point(-1,-1),3);
        threshold(bg, bg, 1, 128, CV_THRESH_BINARY_INV);
        imshow("bg", bg);
        
        Mat markers(binary.size(),CV_8U,Scalar(0));
        markers = fg + bg;
        imshow("markers", markers);
        
        WatershedMarker wm;
        wm.set_markers(markers);
        
        Mat res,temp_res;
        res = wm.apply_watershed(source_image_resized);
        res.convertTo(res, CV_8U);
        imshow("res", res);
//        imwrite("/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/res.jpg",res);
        
        
        Mat res_canny;
        Canny(res, res_canny, 0, 255);
        imshow("resCanny", res_canny);
        
        
        
        
//        res.convertTo(res,CV_32S);
//        Rect br = boundingRect(res);
//        Mat biggest_rect(res,br);
        
        Mat image_contours;
        std::vector<std::vector<cv::Point>> main_contours;
        vector<cv::Vec4i> hierar;
        vector<int> indexes;
        Scalar color(0,255,0);
        findContours(res, main_contours, squares_hierarchy, 0, CV_CHAIN_APPROX_SIMPLE);
        drawContours(res, main_contours, -1, color);
        
        imshow("contour", res);
        
        //main_contours = contours_image.set_source_image(res).find().get_contours();
        //squares_hierarchy = contours_image.get_hierarchy();
        
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
        
        int area = 0;
        Rect main_rect;
        for (vector<int>::iterator itt = indexes.begin();itt!=indexes.end();++itt) {
            Rect r = boundingRect(Mat(main_contours[*itt]));
            if (r.area() > area) {
                area = r.area();
                main_rect = r;
            }
        }
        
        all_rectangles.push_back(main_rect);
        
        Mat cop(res,main_rect);
        imshow("cop", cop);
        
        
//        cv::Mat gray0(source_image_resized.size(), CV_8U),gray;
//        cv::medianBlur(source_image_resized, source_image_blurred, 0);
//        std::vector<std::vector<cv::Point>> contours;
    }
}