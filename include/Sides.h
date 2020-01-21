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
#define CONTAINER_20ft_RATIO 2.3255813953488373
void CallBackProc(int event, int x, int y, int flags, void * Frm);
class Frame
{
    public:
        cv::Mat m_im;
        cv::Mat m_im_g;
        cv::Mat m_im_rgb;
    public:
        std::vector<cv::Point> m_vP;
        std::vector<cv::Point> m_vP_mod;
        //Callback Points Class
        int m_limit_iter;
        unsigned int m_max_points;
        double m_factor_resize;
    public:
        Frame();
        Frame(char* filename, char* factor_resize);
        unsigned char* validation();
        std::vector<cv::Point> CollectPoints(cv::Mat img);
        void SideSort();
        void SideModify();
        std::vector<cv::Point> PointZoomOut(std::vector<cv::Point> vP, double sz);
        std::vector<cv::Point> PointZoomIn(std::vector<cv::Point> vP, double sz);
        std::vector<cv::Point> warpProcess();
};
}
#endif
