
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <map>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include "find_edges.h"
#include "detect_contours.h"
#include "display_window.h"
#include "load_image.h"
#include "apply_threshold.h"
#include "feature_extractor.h"
#include "find_components.h"




using namespace cv;
using namespace std;
using namespace ppc;
using namespace tesseract;

Mat query,queryGreyscale,thresholdResult,testImg,testImgGrayscale;
RNG rng(12345);
int thresh(20);
int maxThreshold(200);

Mat src_image_temp,src_image,src_image_grayscale,src_image_threshold,src_image_edges,src_image_contours,src_image_adaptive_threshold,src_image_grayscale2;
const bool showImg = true;


void detectRegions(int,void*);
void ApplyThreshold(int,void*);
void SquaresMethod(Mat gray0,Mat gray,vector<vector<Point>> contours);
void changeThresh(int,void*);
void ShowImage(Mat im);
void HoughTransform(Mat dst);
void allImshows();
void namedWindows();

Threshold th(thresh,maxThreshold,1);
//Threshold th;
Edges fe;
Contours ct(CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE),sit(CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
Mat contour_test,contour_result;
std::vector<cv::RotatedRect> min_rectangles;
FeatureExtractor features;
int flag=0;
Mat copied;
int area(500),width(500),height(500);
//int area(0),width(0),height(0);
int window_size = CV_WINDOW_AUTOSIZE;









//Squares method


Mat source_img,source_image_resized,blurred_image,source_image_output,source_image_hull;;
int no_of_threshold_levels = 2;
int lower_thresh = 0,upper_thresh=255;
Edges edge_image(lower_thresh,upper_thresh);
Contours contours_image(CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE);
vector<vector<Point>> squares;
vector<vector<Point>> needed_contours;
vector<Rect> all_rectangles,hull_rectangles;
vector<cv::Vec4i> squares_hierarchy;
int counter = 0,extracted_images_counter=0;
vector<Mat>extracted_images;
bool display_extracted_images = false;

int main(int argc,char** argv){
    
    
    /*
//    string path = argv[2] + string("/tearsheet/") + "Tearsheet.png";
    string path = argv[2] + string("/tearsheet/") + "printedTearsheetVertical.JPG";
    
    cout << "dir - " << path << endl;
    
    
    LoadImage load_source_image(path,"SquaresImage");
    source_img = load_source_image.get_image();
    resize(source_img, source_image_resized, Size(0,0), 0.50,0.50 );
    //    src_image = imread(dir);
    if(!source_image_resized.data){
        cout << "No image data" <<endl;
    }
    load_source_image.set_image(source_image_resized);
    
    load_source_image.show();
    
    Mat gray0(source_image_resized.size(), CV_8U),gray;
    
    medianBlur(source_image_resized, blurred_image, 3);
    vector<vector<Point>> contours;
    
    squares.clear();
    
    for(int channels=0;channels < 3; channels++){
        
        int mixChannelsArray[] = {channels,0};
        
        mixChannels(&blurred_image, 1, &gray0, 1, mixChannelsArray, 1);
        
        for (int threshold_level =0 ; threshold_level<no_of_threshold_levels; threshold_level++) {
            if (threshold_level ==0 ) {
                gray = edge_image.set_source_image(gray0).set_aperture_size(5).applyCanny().get_result_image();
                dilate(gray, gray, Mat(),Point(-1,-1));
            }
            else{
                gray = gray0 >= (threshold_level + 1) * 255 / no_of_threshold_levels;
            }
            
            namedWindow("Gray",CV_WINDOW_AUTOSIZE);
            imshow("Gray", gray);
            
            contours = contours_image.set_source_image(gray).find().get_contours();
            squares_hierarchy = contours_image.get_hierarchy();
            
            
            for (vector<vector<Point>>::iterator itr = contours.begin(); itr!=contours.end(); ++itr) {
                //cout << "contours - " << *itr << endl;
            }
            
            for (vector<cv::Vec4i>::iterator itr = squares_hierarchy.begin(); itr!=squares_hierarchy.end(); ++itr) {
                cout << "square_hierarchy - " << threshold_level << " - "<<*itr << endl;
                counter++;
            }
            
            cout << "counter - " << counter << endl;
            counter = 0;
            
            vector<Point> approx;
            
            for (size_t i=0; i < contours.size(); i++) {
                approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);
                
                
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
    }

    
    namedWindow("Grayscale",CV_WINDOW_AUTOSIZE);
    imshow("Grayscale", gray);
    
    
    
    for (size_t i=0; i<squares.size(); i++) {
        const Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        polylines(source_image_resized, &p, &n, 1, true, Scalar(0,255,0),3,CV_AA);
    }
    
    namedWindow("WithLines",CV_WINDOW_AUTOSIZE);
    imshow("WithLines", source_image_resized);
    waitKey(0);
    */
    
    
    /*
    //string dir = argv[2] + string("/layout4/") + "layout4.png";
//    string dir = argv[2] + string("/bootstrap1/") + "bootstrap1.png";
//    string dir = argv[2] + string("/bootstrap1/") + "bootstrap1_2.png";
    string dir = argv[2] + string("/tearsheet/") + "printedTearsheetSeat.JPG";
    cout << "dir - " << dir << endl;
    
    
    LoadImage li(dir,"Layout1");
    src_image_temp = li.get_image();
    resize(src_image_temp, src_image, Size(0,0), 0.50,0.50 );
    //    src_image = imread(dir);
    if(!src_image.data){
        cout << "No image data" <<endl;
    }
    li.set_image(src_image);

    li.show();
    

    //DisplayWindow dw("Layout1", src_image,CV_WINDOW_AUTOSIZE);
    //dw.show();
    
    cvtColor(src_image, src_image_grayscale, COLOR_BGR2GRAY);
    //blur(src_image_grayscale, src_image_grayscale, Size(3,3));
    GaussianBlur(src_image_grayscale,src_image_grayscale,Size(3,3),0);
    
    
    th.set_source_image(src_image_grayscale);

    namedWindows();
    
    createTrackbar( "Threshold","ContourResult", &thresh,maxThreshold, ApplyThreshold );
    ApplyThreshold(0, 0);
    
    waitKey(0);
    cout<<"success"<<endl;
    */
    
    
    
    
    
    
    // Method 3 - Hybrid
    
    /*
    char * dir = getcwd(NULL, 0);
    //    printf("Current dir: %s", dir);
//    string path =  string(dir) + "/Tearsheet.png";
    string path =  string(dir) + "/red1.JPG";
    Components com(path);
    com.find_watershed();
    //com.find();
    //com.save_image(string(dir));
    waitKey(0);
    cout<<"success"<<endl;
    */
    
    
    //Test
    /*
    Mat markerMask(300,300,CV_8U,Scalar::all(0));
    namedWindow("Marker",CV_WINDOW_AUTOSIZE);
    imshow("Marker", markerMask);
    waitKey(0);
    */
    
    
    
    // Testing tesseract
    
    
    char *outText;
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    
    if (api->Init(NULL,"eng")) {
        fprintf(stderr, "Coult not initialize tesseract. \n");
        exit(1);
    }
    
    
    Pix *image = pixRead("/Users/adarsh.kosuru/Desktop/tesseracttest/tearsheetBroken/buttons.png");
    api->SetImage(image);
    outText = api->GetUTF8Text();
    printf("Output - %s",outText);
    
    api->End();
    delete [] outText;
    pixDestroy(&image);
    return 0;
    
}

void changeThresh(int,void*){
    
}






void ApplyThreshold(int,void*){
    
    cout << "thresh - " << thresh << endl;
    
    
//    th.set_minimum_threshold(thresh);
//    th.apply();
//    src_image_threshold = th.get_thresholded_image();
    
    adaptiveThreshold(src_image_grayscale, src_image_adaptive_threshold, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 3, 5);
    
    double otsu = threshold(src_image_grayscale, src_image_threshold, thresh, maxThreshold, CV_THRESH_BINARY_INV);
    
    cout << "otsu - " << otsu << endl;
    
//    fe.set_lower_threshold(0.5 * otsu);
//    fe.set_upper_threshold(otsu);
    fe.set_lower_threshold(thresh);
    fe.set_upper_threshold(thresh*3);
    fe.set_source_image(src_image_grayscale);
     fe.applyCanny();
    src_image_edges = fe.get_result_image();
    
    //HoughTransform(src_image_edges);
    
    src_image_grayscale.copyTo(src_image_grayscale2);
    
    contour_test = ct.set_source_image(src_image_edges).find().draw_rotated_rectangles().get_result_image();
    
    vector<cv::Vec4i> hierarchy;
    vector<cv::Vec4i> parent_hierarchy;
    vector<int> indexes;
    vector<vector<cv::Point>> contours;
    contours = ct.get_contours();
    hierarchy = ct.get_hierarchy();
    contour_result = cv::Mat::zeros(src_image_edges.size(), CV_8UC3);
    min_rectangles.resize(contours.size());
    
    vector<vector<Point> >hull( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    {  convexHull( Mat(contours[i]), hull[i], false ); }
    
    /// Draw contours + hull results
    Mat drawing = Mat::zeros( src_image_edges.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        
        Scalar color(0,255,0);
        //Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
    }
    
    for (int i=0; i<hull.size(); i++) {
//        cout << "hull - " << i << " - " << hull[i] << endl;
    }
    
    Mat rect_hull = cv::Mat::zeros(src_image_edges.size(), CV_8UC3);
    for (vector<vector<Point>>::iterator it=hull.begin(); it!=hull.end(); ++it) {
        Rect rh = boundingRect(Mat(*it));
        rectangle(rect_hull, rh, cv::Scalar(255), 2);
        
    }
    
    /// Show in a window
    namedWindow( "Hull demo", CV_WINDOW_AUTOSIZE );
    imshow( "Hull demo", drawing );
    namedWindow( "HullRect", CV_WINDOW_AUTOSIZE );
    imshow( "HullRect", rect_hull );

    
    for (int i=0; i<hierarchy.size(); i++) {
//        std::cout  << hierarchy[i] << std::endl;
        if (hierarchy[i][3] == -1) {
            parent_hierarchy.push_back(hierarchy[i]);
            indexes.push_back(i);
//            cout << "parent here - " << endl;
//            std::cout  << hierarchy[i] << std::endl;
        }
    }
    
        for (int i=0; i<parent_hierarchy.size(); i++) {
//         std::cout  << parent_hierarchy[i] << "index - " << indexes[i] <<std::endl;
        }
    
    for (vector<int>::iterator itr =indexes.begin(); itr!=indexes.end();++itr) {
        cv::Rect r = cv::boundingRect(cv::Mat(contours[*itr]));
//        cout << "rect width & height - " << itr - indexes.begin() << r.width << " , " << r.height << endl;
//        cout << "rect area - " << itr - indexes.begin() << r.area()  << endl;
        
        if (r.area() > area || r.width > width || r.height > height) {
            rectangle(contour_result, r, cv::Scalar(255), 2);
            
            if (flag == 0) {
//                src_image.copyTo(copied(Rect(r.tl().x,r.tl().y,r.width,r.height)));
                
                flag =1;
            }
        }
        
    }
    
    
//    namedWindow("copiedImage",CV_WINDOW_AUTOSIZE);
//    imshow("copiedImage", copied);
    
//    for (std::vector<cv::Vec4i>::iterator itr = hierarchy.begin(); itr!=hierarchy.end(); ++itr) {
//            std::cout  << *itr << std::endl;
//        }
    
    //contour_result = ct.draw_rectangles().get_result_image();
    
    
    
//    for (size_t i=0; i<contours.size(); i++) {
//        min_rectangles[i]  = cv::minAreaRect(cv::Mat(contours[i]));
//        Rect r = min_rectangles[i].boundingRect();
//        rectangle(contour_result, r, cv::Scalar(255), 2);
//    }
    
    
    cout << "no of contours - " << contours.size() << endl;
  
    /*
    for (std::vector<std::vector<cv::Point>>::iterator itr = contours.begin(); itr!=contours.end(); ++itr) {
//        cout << "itr num - " << itr - contours.begin() << endl;
//        min_rectangles[itr - contours.begin()]  = cv::minAreaRect(cv::Mat(*itr));
        cv::Rect r = cv::boundingRect(cv::Mat(*itr));
        rectangle(contour_result, r, cv::Scalar(255), 2);
        
    }
    */
    

    
    
    
//    for (std::vector<vector<cv::Point>>::iterator itr = contours.begin(); itr!=contours.end(); ++itr) {
//        std::cout  << *itr << std::endl;
//        
//    }
    
    
//    hierarchy = ct.get_hierarchy();
//    for (std::vector<cv::Vec4i>::iterator itr = hierarchy.begin(); itr!=hierarchy.end(); ++itr) {
//        std::cout  << *itr << std::endl;
//        
//    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    //src_image_edges = sit.set_source_image(src_image_edges).find_without_hierarchy().draw_rectangles().get_result_image();


    allImshows();
  
}

void namedWindows(){
    namedWindow("Edges",window_size);
    namedWindow("contourTest",window_size);
    namedWindow("ContourResult",window_size);
    namedWindow("AdaptiveThresh",window_size);
    namedWindow("Thresh",window_size);
}

void allImshows(){
    
    imshow("ContourResult", contour_result);
    
    imshow("contourTest", contour_test);
    imshow("Edges", src_image_edges);
    imshow("AdaptiveThresh", src_image_adaptive_threshold);
    
//    imshow("Thresh", src_image_threshold);
    
}

void HoughTransform(Mat dst){
    
    Mat cdst;
    cvtColor(dst, cdst, CV_GRAY2BGR);
    
    vector<Vec2f> lines;
    // detect lines
    HoughLines(dst, lines, 1, CV_PI/180, 150, 0, 0 );
    
    // draw lines
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
    }
    
    namedWindow("HoughLines",window_size);
    imshow("HoughLines", cdst);
}



void setup(){
    
}







void ShowImage(Mat im){
    
    if (showImg) {
        imshow("Detected Contours", im);
    }
    
}

void detectRegions(int,void*){
    
    
    
    // hierarchy
    
    
    //     for (vector<Vec4i>::iterator itr = hierarchy.begin(); itr!=hierarchy.end(); ++itr) {
    //            cout  << *itr << endl;
    //     }
    
    //Warping image -- still work to do
    
    /*
     
     Point2f perspectiveOutput[4];
     Mat pOut = getPerspectiveTransform(rectPoints, perspectiveOutput);
     Mat unWarped;
     warpPerspective(queryGreyscale, unWarped, pOut, cannyResult.size());
     
     // namedWindow("Contours",CV_WINDOW_AUTOSIZE);
     // imshow("Contours", drawBoundries);
     //
     //     namedWindow("unWarp",CV_WINDOW_AUTOSIZE);
     //     imshow("unWarp", unWarped);
     */
    
}
