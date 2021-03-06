#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>
//#include "highgui.h"
#include <iostream>
#include <string>
#include <sys/stat.h>

using namespace cv;
using namespace std;

struct Img {
    char name[];
};

extern "C" void print_int(int x) {
    cout << "int: " << x << endl;
}

extern "C" void print_str(char x[]) {
    cout << "str: " << x << endl;
}


extern "C" struct Img* emptyInitImg() {
    cout << "EmptyImg inited" << endl;
    return NULL;
}

extern "C" struct Img* load(char imageName[])
{
    //make a folder
    //go inside folder and make temp variable name
    mkdir("./temp_directory", 0777);

    string path = string("temp_directory/") + string(imageName);

    Mat img;
    img = imread(imageName, CV_LOAD_IMAGE_COLOR);

    //check if img exists
    if (img.empty()) {
        cout << "Error: Image does not exist" << endl;
        exit(1);
    }

    imwrite(path, img);
    struct Img* output = (struct Img*) malloc(sizeof(struct Img));
    strcpy(output->name, imageName);  // Saves imageName without 'tempDir/'
    return output;
}

extern "C" int save(char location[], struct Img* input)
{
    string path = string("temp_directory/") + string(input->name);
    Mat img = imread(path, CV_LOAD_IMAGE_COLOR);
    imwrite(location, img);
    return 0;
}

extern "C" int display(struct Img* input) {
    string path = string("temp_directory/") + string(input->name);
    Mat img = imread(path, CV_LOAD_IMAGE_COLOR);
    namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
    imshow( "Display window", img ); // Show our image inside it.
    waitKey(0); 
    return 0;
}

extern "C" int cleanup(struct Img* input)
{
    free(input);
    return 0;
}

extern "C" struct Img* brighten(struct Img* input, int value) {
    //read in temp image
    string path = string("temp_directory/") + string(input->name);
    Mat img = imread(path, CV_LOAD_IMAGE_COLOR);

    //modifications
    img = img + value;

    //output temp image
    imwrite(path, img);
    return input;
}

extern "C" struct Img* copy(struct Img* input) {
    string path = string("temp_directory/") + string(input->name);
    Mat img = imread("temp_directory/test_fish.png", CV_LOAD_IMAGE_COLOR);

    string new_img = string("copy_") + string(input->name);
    string new_path = string("temp_directory/") + new_img;

    imwrite(new_path, img);

    struct Img* output = (struct Img*) malloc(sizeof(struct Img));
    strcpy(output->name, new_img.c_str());  // Saves imageName without 'tempDir/'
    return output;
}

extern "C" struct Img* dilation(struct Img* input, int size, int shape) {
    //read in temp image
    string path = string("temp_directory/") + string(input->name);
    Mat img = imread(path, CV_LOAD_IMAGE_COLOR);
    //Mat out = img;

    //modifications
    int seShape;
    if(shape == 1) {
        seShape = MORPH_ELLIPSE;
    }
    else if( shape == 2) {
        seShape = MORPH_CROSS;
    }
    else {
        seShape = MORPH_RECT;
    }
    int seSize = size;

    Mat se = getStructuringElement(seShape, Size(2*seSize+1, 2*seSize+1), Point(seSize, seSize));

    //dilate(img, out, se);
    dilate(img,img,se);

    //output temp image
    imwrite(path, img);
    return input;
}

extern "C" struct Img* sobel(struct Img* input) {
    //read in temp image
    string path = string("temp_directory/") + string(input->name);
    Mat img = imread(path, CV_LOAD_IMAGE_COLOR);
    Mat out;

    //modifications
    cvtColor(img, out, CV_BGR2GRAY);
    Mat gx, gy;
    Mat absgx, absgy;

    Sobel(out, gx, CV_16S,1,0,3,1,0, BORDER_DEFAULT);
    Sobel(out, gy, CV_16S,0,1,3,1,0, BORDER_DEFAULT);

    convertScaleAbs(gx, absgx);
    convertScaleAbs(gy, absgy);

    addWeighted(absgx, 0.5, absgy, 0.5, 0, out);

    //output temp image
    imwrite(path, out);
    return input;
}

extern "C" struct Img* threshold(struct Img* input, int val) {
    //read in temp image
    string path = string("temp_directory/") + string(input->name);
    Mat img = imread(path, CV_LOAD_IMAGE_COLOR);
    Mat out;

    //modifications
    cvtColor(img, out, CV_BGR2GRAY);

    threshold(out, out, val, 255, 0);

    //output temp image
    imwrite(path, out);
    return input;
}

extern "C" struct Img* gaussian(struct Img* input, int val) {
    //read in temp image
    string path = string("temp_directory/") + string(input->name);
    Mat img = imread(path, CV_LOAD_IMAGE_COLOR);
    Mat out;

    //modifications
    GaussianBlur(img, out, Size(val,val),0,0);

    //output temp image
    imwrite(path, out);
    return input;
}

extern "C" struct Img* color(struct Img* input, int val) {
    //read in temp image
    string path = string("temp_directory/") + string(input->name);
    Mat img = imread(path, CV_LOAD_IMAGE_COLOR);
    Mat out;

    //modifications
    applyColorMap(img, out, val);

    //output temp image
    imwrite(path, out);
    return input;
}

extern "C" struct Img* sharpen(struct Img* input, int val) {
    //read in temp image
    string path = string("temp_directory/") + string(input->name);
    Mat img = imread(path, CV_LOAD_IMAGE_COLOR);
    Mat out;
    Mat laplac;
    Mat grey;

    //modifications
    cvtColor(img, grey, CV_BGR2GRAY);
    Laplacian(grey, laplac, CV_8U, 3, 1, 0, BORDER_DEFAULT);
    addWeighted(grey, 1, laplac, val/100, 0, out);

    //output temp image
    imwrite(path, out);
    return input;
}

extern "C" struct Img* median(struct Img* input, int val) {
    //read in temp image
    string path = string("temp_directory/") + string(input->name);
    Mat img = imread(path, CV_LOAD_IMAGE_COLOR);
    Mat out;

    //modifications
    medianBlur(img, out, val);

    //output temp image
    imwrite(path, out);
    return input;
}