/*

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

using namespace cv;
using namespace std;

int main(int argc, const char * argv[])
{
    
    if (argc != 3) {
        cout << "Image arguments missing" << endl;
        return -1;
    }
    
    Mat img1 = imread(argv[1],CV_LOAD_IMAGE_ANYCOLOR);
    Mat img2 = imread(argv[2],CV_LOAD_IMAGE_ANYCOLOR);
    
    if(!img1.data || !img2.data ){
        cout << "Error -  images loaded" << endl;
        return -1;
    }
    
    
    // Orb(4000 or 5000), Orb, BruteForce
    //Surf (400), Surf, Flann
    // Fast (40), Freak, Brute
    
    
    //SurfFeatureDetector detector(400);
    OrbFeatureDetector detector(5000);
    vector<KeyPoint>keypoints1, keypoints2;
    
    detector.detect(img1, keypoints1);
    detector.detect(img2, keypoints2);
    
    
    //SurfDescriptorExtractor extractor;
    OrbDescriptorExtractor extractor;
    Mat descriptors1,descriptors2;
    extractor.compute(img1, keypoints1, descriptors1);
    extractor.compute(img2, keypoints2, descriptors2);
    
    //cout << "Descriptors - " << descriptors1 << endl;
    
    
    //BruteForceMatcher<Hamming> matcher;
    BFMatcher matcher(NORM_L2);
    //FlannBasedMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);
    
    
    double maxDist = 0,  minDist = 100;
    
    for (int i=0; i<descriptors1.rows; i++) {
        double dist = matches[i].distance;
        if(dist< minDist) minDist = dist;
        if (dist > maxDist) maxDist = dist;
        
    }
    
    cout << "Min Dist - " << minDist<< endl;
    cout << "Max Dist - " << maxDist<< endl;
    
    vector<DMatch> good_matches;
    static const vector<DMatch> gm2;
    
    for (int i=0; i<descriptors1.rows; i++) {
        if (matches[i].distance < max (2*minDist,0.02 )) {
            good_matches.push_back(matches[i]);
            //gm2.push_back(DMatch(0, 0, 0));
        }
    }
    
    //namedWindow("matches",CV_WINDOW_KEEPRATIO);
    Mat imgMatches;
    //    drawMatches(img1, keypoints1, img2, keypoints2, matches, imgMatches);
    
    //With Feature vectors
    drawMatches(img1, keypoints1, img2, keypoints2, good_matches, imgMatches,Scalar::all(-1), Scalar::all(-1),vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    
    //Without feature vectors
    //    drawMatches(img1, keypoints1, img2, keypoints2, gm2, imgMatches,Scalar::all(-1), Scalar::all(-1),vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    
    //    drawMatches(img3, keypoints3, img2, keypoints2, matches2, imgMatches,Scalar::all(-1), Scalar::all(-1),vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    
    
    vector<Point2f>obj;
    vector<Point2f>scene;
    
    for (size_t i=0; i<good_matches.size(); i++) {
        obj.push_back(keypoints1[good_matches[i].queryIdx].pt);
        scene.push_back(keypoints2[good_matches[i].trainIdx].pt);
    }
    
    
    
    Mat H = findHomography(obj, scene, RANSAC);
    
    vector<Point2f> objCorners(4);
    objCorners[0] = cvPoint(0,0);
    objCorners[1] = cvPoint(img1.cols,0);
    objCorners[2] = cvPoint(img1.cols,img1.rows);
    objCorners[3] = cvPoint(0,img1.rows);
    
    vector<Point2f>sceneCorners(4);
    
    perspectiveTransform(objCorners, sceneCorners, H);
    Point2f offset( (float)img1.cols, 0);
    //    line(imgMatches, sceneCorners[0]+Point2f(img1.cols,0), sceneCorners[1]+Point2f(img1.cols,0), Scalar(0,255,0),4);
    //        line(imgMatches, sceneCorners[1]+Point2f(img1.cols,0), sceneCorners[2]+Point2f(img1.cols,0), Scalar(0,255,0),4);
    //        line(imgMatches, sceneCorners[2]+Point2f(img1.cols,0), sceneCorners[3]+Point2f(img1.cols,0), Scalar(0,255,0),4);
    //        line(imgMatches, sceneCorners[3]+Point2f(img1.cols,0), sceneCorners[0]+Point2f(img1.cols,0), Scalar(0,255,0),4);
    
    line( imgMatches, sceneCorners[0] + offset, sceneCorners[1] + offset, Scalar(0, 255, 0), 4 );
    line( imgMatches, sceneCorners[1] + offset, sceneCorners[2] + offset, Scalar( 0, 255, 0), 4 );
    line( imgMatches, sceneCorners[2] + offset, sceneCorners[3] + offset, Scalar( 0, 255, 0), 4 );
    line( imgMatches, sceneCorners[3] + offset, sceneCorners[0] + offset, Scalar( 0, 255, 0), 4 );
    
    
    //imwrite("newImg.jpeg", imgMatches);
    imshow("matches", imgMatches);
    waitKey(0);
    
    std::cout << "Success!\n";
    
    
    return 0;
}

*/