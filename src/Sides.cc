#include"Sides.h"
//#include<opencv2/opencv.hpp>
//#include<iostream>

namespace RECT_WARP
{
void CallBackProc(int event, int x, int y, int flags, void *point)
{
    Frame *p = (Frame*)point;
    
    if(event == cv::EVENT_LBUTTONDOWN&&p->m_limit_iter<p->m_max_points)
    {
        p->m_vP[p->m_limit_iter].x = x;
        p->m_vP[p->m_limit_iter].y = y;
        p->m_limit_iter++;
    }
}
Frame::Frame(){}
Frame::Frame(char* filename, char* factor_resize)
{
    m_im = cv::imread(filename, 0);
    cv::cvtColor(m_im,m_im_g, cv::COLOR_RGB2GRAY);
    m_max_points = 4;
    m_factor_resize = atoi(factor_resize);
    m_limit_iter = 0;
}
unsigned char* Frame::validation()
{
    return m_im.data;
}
std::vector<cv::Point> Frame::SideSort(std::vector<cv::Point> vP)
{
    bool sign_y[MAX_POINTS] = {false};
    bool sign_x[MAX_POINTS] = {false};
    double cx(0.0), cy(0.0);

    for(int i=0; i<MAX_POINTS; i++)
    {
        cx+=vP[i].x;
        cy+=vP[i].y;
    }
    cx /= (double)MAX_POINTS;
    cy /= (double)MAX_POINTS;
    
    double y_res, x_res;
    // Clock-wise: LT RT RB LB
    std::vector<cv::Point> res(MAX_POINTS);
    
    for(int i=0; i<MAX_POINTS; i++)
    {
        y_res = cy-vP[i].y;
        x_res = cx-vP[i].x;

        sign_y[i] = y_res<0 ? false : true;
        sign_x[i] = x_res<0 ? false : true;
        
        if( sign_x[i] == false && sign_y[i] == false)
        {
            res[2] = vP[i];
        }
        else if( sign_x[i] == true && sign_y[i] == false)
        {
            res[3] = vP[i];
        }
        else if( sign_x[i] == false && sign_y[i] == true)
        {
            res[1] = vP[i];
        }
        else 
        {
            res[0] = vP[i];
        }
    }
        return res;       
}

std::vector<cv::Point> Frame::SideModify(std::vector<cv::Point> vP)
{
    // Images are caputred at left or right side.
    // we can modify width with real container size ratio
    // Compute Left-side width
    double left_height = cv::abs( vP[0].y - vP[3].y );
    double right_height = cv::abs( vP[1].y - vP[2].y );
    double width(0.0);
    double height(0.0);
    if(left_height > right_height)
    {
        height = left_height;
        width = CONTAINER_20ft_RATIO*left_height;
    }
    else 
    {
        height = right_height;
        width = CONTAINER_20ft_RATIO*right_height;
    }
    // Compute Right-Side width
    
    
    std::vector<cv::Point> res;
    res.push_back(cv::Point(0,0));
    res.push_back(cv::Point(width, 0));
    res.push_back(cv::Point(width, height));
    res.push_back(cv::Point(0, height));

    return res;
}
std::vector<cv::Point> Frame::PointZoomOut(std::vector<cv::Point> vP, double sz)
{
    std::cout<<"PointZoomOut start"<<std::endl;
    std::vector<cv::Point> res;
    
    std::vector<cv::Point>::iterator iter;
     
    for(iter = vP.begin(); iter!=vP.end(); ++iter)
    {
        res.push_back(cv::Point(iter->x*sz, iter->y*sz));
    }
    return res;
}
}
