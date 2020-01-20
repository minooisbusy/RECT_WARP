#include"Sides.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
//#include<opencv2/opencv.hpp>
//#include<iostream>

namespace RECT_WARP
{
void CallBackProc(int event, int x, int y, int flags, void *Frm)
{
    Frame *p = (Frame*)Frm;
    
    if(event == cv::EVENT_LBUTTONDOWN&&p->m_limit_iter<p->m_max_points)
    {
        std::cout<<"Call back HIT!!"<<std::endl;
        p->m_vP[p->m_limit_iter].x = x;
        p->m_vP[p->m_limit_iter].y = y;
        p->m_limit_iter++;
    }
}

Frame::Frame(){}

Frame::Frame(char* filename, char* factor_resize)
{
    std::cout<<"Image load: "<<filename<<std::endl;
    m_im = cv::imread(filename, cv::IMREAD_COLOR);
    cv::cvtColor(m_im,m_im_g, cv::COLOR_RGB2GRAY);
    double factor = atoi(factor_resize);
    cv::Size sz(m_im.cols/factor,m_im.rows/factor);
    std::cout<<sz<<std::endl;
    cv::resize(m_im_g, m_im_g, sz);
    cv::cvtColor(m_im_g,m_im_rgb,cv::COLOR_GRAY2RGB);
    m_max_points = 4;
    m_factor_resize = atoi(factor_resize);
    m_limit_iter = 0;
    for(int i=0; i<m_max_points;i++)
        m_vP.push_back(cv::Point(0,0));
}

unsigned char* Frame::validation()
{
    return m_im.data;
}

void Frame::imshow(char* filename)
{
  // cv::imshow(filename, m_im_g);
    
}

void Frame::SideSort()
{
    bool sign_y[MAX_POINTS] = {false};
    bool sign_x[MAX_POINTS] = {false};
    double cx(0.0), cy(0.0);

    for(int i=0; i<MAX_POINTS; i++)
    {
        cx+=m_vP[i].x;
        cy+=m_vP[i].y;
    }
    cx /= (double)MAX_POINTS;
    cy /= (double)MAX_POINTS;
    
    double y_res, x_res;
    // Clock-wise: LT RT RB LB
    std::vector<cv::Point> res(MAX_POINTS);
    
    for(int i=0; i<MAX_POINTS; i++)
    {
        y_res = cy-m_vP[i].y;
        x_res = cx-m_vP[i].x;

        sign_y[i] = y_res<0 ? false : true;
        sign_x[i] = x_res<0 ? false : true;
        std::cout<<"Original Point["<<i<<"]="<<m_vP[i]<<std::endl;
        if( sign_x[i] == false && sign_y[i] == false)
        {
            res[2] = m_vP[i];
            std::cout<<"res[2]="<<res[2]<<std::endl;
        }
        else if( sign_x[i] == true && sign_y[i] == false)
        {
            res[3] = m_vP[i];
            std::cout<<"res[3]="<<res[3]<<std::endl;
        }
        else if( sign_x[i] == false && sign_y[i] == true)
        {
            res[1] = m_vP[i];
            std::cout<<"res[1]="<<res[1]<<std::endl;

        }
        else 
        {
            res[0] = m_vP[i];
            std::cout<<"res[0]="<<res[0]<<std::endl;

        }
    }
    m_vP = std::vector<cv::Point>(res.begin(), res.end());
}

void Frame::SideModify()
{
    // Images are caputred at left or right side.
    // we can modify width with real container size ratio
    // Compute Left-side width
    double left_height = cv::abs( m_vP[0].y - m_vP[3].y );
    double right_height = cv::abs( m_vP[1].y - m_vP[2].y );
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
    
    
    m_vP_mod.push_back(cv::Point(0,0));
    m_vP_mod.push_back(cv::Point(width, 0));
    m_vP_mod.push_back(cv::Point(width, height));
    m_vP_mod.push_back(cv::Point(0, height));
    

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
