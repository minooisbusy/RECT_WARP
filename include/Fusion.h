#ifndef FUSION_H
#define FUSION_H
#include<opencv2/opencv.hpp>
namespace RECT_WARP
{
cv::Mat Fusion(cv::Mat right_img, cv::Mat left_img);
cv::Mat RectangleWarp(cv::Mat img, cv::Point2f kpts1, cv::Point2f kpts2=cv::Point2f(-1,-1));
}
#endif
