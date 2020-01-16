#ifndef SIDES_H
#define SIDES_H

#include<opencv2/core/types.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include<iostream>
#include<algorithm>

namespace RECT_WARP
{
#define MAX_POINTS 4
void CallBackProc(int event, int x, int y, int flags, void * point);
std::vector<cv::Point> CollectPoints(cv::Mat img);
std::vector<cv::Point> SideSort(std::vector<cv::Point> vP);
std::vector<cv::Point> SideModify(std::vector<cv::Point> vP);
class CBPoints
{
    public:
        int n_max_points;
        int n_limit_iter;
        std::vector<cv::Point> vP;
        CBPoints();
        CBPoints(int _n);
};
}
#endif
