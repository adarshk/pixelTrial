
#include <iostream>
#include <string.h>
#include <unistd.h>

#include "find_components.h"
#include "splice_image.h"




using namespace cv;
using namespace std;
using namespace ppc;
using namespace tesseract;











int main(int argc,char** argv){
    
    
    
    //Splice method
    
//    char * dir = getcwd(NULL, 0);
//    string path =  string(dir) + "/red2.JPG";
//    string path = "/Users/adarsh.kosuru/Desktop/canonphotos/IMG_1515.jpg";
    string path = "/Users/adarsh.kosuru/Desktop/Tearsheets/differentBackground/IMG_1938.JPG";
//    string path = "/Users/adarsh.kosuru/Desktop/Tearsheets/differentBackground/photo1.JPG";
//    string path = "/Users/adarsh.kosuru/Desktop/Tearsheets/differentBackground/Tearsheet_black1.JPG";
//    string path = "/Users/adarsh.kosuru/Desktop/pixelTrial/DerivedData/pixelTrial/Build/Products/Debug/printedTearsheet_mac.JPG";
    Splice s(path);
    s.init();
//    s.save_image(string(dir));
    
//    waitKey(0);
    
    
    
    
    
    
    /*
    char * dir = getcwd(NULL, 0);
    string path = "/Users/adarsh.kosuru/Desktop/canonphotos/IMG_1515.JPG";
    Components com(path);
    com.mini_watershed_for_thresholding();
    com.save_image(string(dir));
    */
    
    
    
    
    
    /*
    
    //Componenets Method
    
    // Method 3 - Hybrid
    
  
    char * dir = getcwd(NULL, 0);
    //    printf("Current dir: %s", dir);
//      string path =  string(dir) + "/Tearsheet.png";
//    string path =  string(dir) + "/Tearsheet_nav_textboxes.png";
//    string path =  string(dir) + "/Tearsheet_no_blue.png";
//    string path =  string(dir) + "/Tearsheet_search_box.png";
//      string path =  string(dir) + "/half.JPG";
//    string path =  string(dir) + "/Tearsheet_world.png";
//    string path =  string(dir) + "/Tearsheet_new.png";
//    string path =  string(dir) + "/red2.JPG";
//    string path =  string(dir) + "/printedTearsheet_mac.JPG";
//    string path =  string(dir) + "/printedTearsheet_mac_cropped.png";
    
    
    
    
      string path = "/Users/adarsh.kosuru/Desktop/canonphotos/IMG_1515.JPG";
//    string path = "/Users/adarsh.kosuru/Desktop/Tearsheets/differentBackground/Tearsheet_green2.jpg";
    Components com(path);
//    com.set_output_path(string(dir)+"/Components");
//    com.set_output_path(string(dir)+"/photoResults");
    com.set_output_path(string(dir)+"/basic_thresholding");
//    com.basic_thresholding_method();
  
    
    // -------------   Use mini_watershed + save_images or mini_mini_ocr ---------
    
    com.mini_mini_ocr();
    //com.mini_watershed_for_thresholding();
//        com.find_watershed();
//        com.find();
    //    com.set_output_path(string(dir)+"/Components");
//    com.set_output_path(string(dir)+"/photoResults");
//    com.init();
    //com.save_image(string(dir));
    
    
    
    
//    waitKey(0);
    //    cout<<"success"<<endl;
    //    return 0;
    
*/
    
    
    
  
    
    
}
