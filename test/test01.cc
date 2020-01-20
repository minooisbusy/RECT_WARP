#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
int main()
{
    cv::Mat img = Mat::zeros(cv::Size(1200,1200), CV_8UC3);
    circle(img, Point(414,52), 1, Scalar(255,255,0),-1);
    circle(img, Point(365, 733), 1, Scalar(0,255,0),-1);
    circle(img, Point(1006, 693), 1, Scalar(0,255,0),-1);
    circle(img, Point(975, 514), 1, Scalar(0,255,0),-1);
    Point p[4]={Point(414,52), Point(365, 733), Point(1006, 693), Point(975, 514)};
    Point center(0,0);
    for(int i=0; i<4 ;i++)
        center+=p[i];

    center/=4.0;
    cv::circle(img, center,1, Scalar(255,0,0), -1);
    cv::imshow("test", img);
    waitKey(0);
}
