#include <iostream>
#include <string>
#include <chrono>
#include <parlay/primitives.h>
#include <parlay/sequence.h>
#include <parlay/internal/get_time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <fstream>

#include "flood-opencv.h"
#include "helper/graph_utils.h"

int main(int argc, char* argv[]) {
    char frame_name[256];
    int imreadflags = cv::IMREAD_COLOR; 
    sprintf(frame_name,"prova1.png");
    ////////////////////////////////////////////////////
    cv::Mat image = cv::imread(frame_name, imreadflags);
    cv::Mat subSample(image.rows, image.cols, image.type());
    parlay::sequence<parlay::sequence<Pixel>> matrice;
    //display image
    cv::namedWindow("original image", cv::WINDOW_NORMAL);
    cv::imshow("original image", image);
    for(int v =0;v<image.rows;v++)
    {
        parlay::sequence<Pixel> tmp;
        for(int u=0;u<image.cols;u++)
        {
            int b = image.data[ (u + v*image.cols)*3];
            int g = image.data[ (u + v*image.cols)*3 + 1];
            int r = image.data[ (u + v*image.cols)*3 + 2];
            tmp.push_back({u,v,{b,g,r}});
        }
        matrice.push_back(tmp);
    }
    ////////////////////////////////////////////////////
    Pixel start = {1,9};
    Color replacement = {0,0,0};
    auto startC = std::chrono::steady_clock::now();
    //for(int i=0; i<1000; i++){
    FloodFill(start, matrice, replacement);
    //}
    std::cout << std::endl;
    auto endC = std::chrono::steady_clock::now();std::chrono::duration<double> elapsed_seconds = endC-startC;
    std::cout << std::endl << "Result" << std::endl;
    for(int v =0;v<subSample.rows;v++)
    {
        for(int u=0;u<subSample.cols;u++)
        {
            subSample.data[ (u + v*image.cols)*3] = matrice[v][u].color.b;
            subSample.data[ (u + v*image.cols)*3 + 1] = matrice[v][u].color.g;
            subSample.data[ (u + v*image.cols)*3 + 2] = matrice[v][u].color.r;
        }
    }
    cv::namedWindow("subSample", cv::WINDOW_NORMAL);
    cv::imshow("subSample", subSample);
    std::cout << "elapsed time: " << elapsed_seconds.count() * 1000 << "ms\n";
    if(image.empty())
    {
      std::cout<<"Unable to open "<<frame_name<<std::endl;
    }
    unsigned char key = cv::waitKey(0);
}
