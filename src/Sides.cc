#include"Sides.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
//#include<opencv2/opencv.hpp>
//#include<iostream>

namespace RECT_WARP
{

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
    std::vector<cv::Point> res(MAX_POINTS), cand_left, cand_right;
    
    for(int i=0; i<MAX_POINTS; i++)
    {
        y_res = cy-m_vP[i].y;
        x_res = cx-m_vP[i].x;

        sign_y[i] = y_res<0 ? false : true;
        sign_x[i] = x_res<0 ? false : true;
        std::cout<<"Original Point["<<i<<"]="<<m_vP[i]<<std::endl;
        if( sign_x[i] == true)
        {
            cand_left.push_back(m_vP[i]);
        }
        else
        {
            cand_right.push_back(m_vP[i]);
        }
    }
    std::cout<<"candidate specify ON!"<<std::endl;
    std::cout<<"right cand size: "<<cand_right.size()<<std::endl;
    std::cout<<"left cand side:  "<<cand_left.size()<<std::endl;
        if(cand_right[0].y > cand_right[1].y)
        {
            res[1] = cand_right[1];
            res[2] = cand_right[0];
        }
        else
        {
            res[1] = cand_right[0];
            res[2] = cand_right[1];
        }
        
        if(cand_left[0].y > cand_left[1].y)
        {
            res[0] = cand_left[1];
            res[3] = cand_left[0];
        }
        else
        {
            res[0] = cand_left[0];
            res[3] = cand_left[1];
        }

    m_vP = std::vector<cv::Point>(res.begin(), res.end());
}
double norm(cv::Point p, cv::Point q)
{
    return cv::sqrt(cv::pow((p.x-q.x),2)+cv::pow(p.y-q.y,2));
}
void Frame::SideModify()
{
    // Images are caputred at left or right side.
    // we can modify width with real container size ratio
    // Compute Left-side width
    double left_height;// = cv::abs( m_vP[0].y - m_vP[3].y );
    left_height = norm(m_vP[0],m_vP[3]);
    double right_height;// = cv::abs( m_vP[1].y - m_vP[2].y );
    right_height = norm(m_vP[1],m_vP[2]);
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
void Frame::PointZoomOut()
{
    std::cout<<"PointZoomOut start"<<std::endl;
    if(!m_vP_zoomOut.empty() || !m_vP_mod_zoomOut.empty())
    {
        m_vP_zoomOut = std::vector<cv::Point>();
        m_vP_mod_zoomOut = std::vector<cv::Point>();
    }
    for(unsigned int i=0; i<m_factor_resize; i++)
    {
        m_vP_zoomOut.push_back(m_vP[i] * m_factor_resize);
        m_vP_mod_zoomOut.push_back(m_vP_mod[i] * m_factor_resize);
    }
}

void CBProc(int event, int x, int y, int flags, void *pointer)
{
     RECT_WARP::Frame*p = (RECT_WARP::Frame*)pointer;
    if(event == cv::EVENT_LBUTTONDOWN && p->m_limit_iter < p->m_max_points)
    {

        std::cout<<"Call back HIT!!"<<std::endl;
        p->m_vP[p->m_limit_iter].x = x;
        p->m_vP[p->m_limit_iter].y = y;
        p->m_limit_iter++;
    }
}

cv::Mat Frame::warpProcess()
{
    char winName[] = "Temporary: Select image points";
    char key = 0;
    std::vector<cv::Point> result(4);
    std::cout<<winName<<std::endl;
    cv::imshow(winName,m_im_g);
    cv::setMouseCallback(winName, CBProc, this);

    std::cout<<"Mouse Callback is registed"<<std::endl;
    while(true)
    {
        for(int i=0; i<m_limit_iter;i++)
            cv::circle(m_im_rgb, m_vP[i], 1, cv::Scalar(0, 255, 0), -1);
        cv::imshow(winName, m_im_rgb);
        key = cv::waitKeyEx(30);
        if( key == 'q' || m_limit_iter >= 4)
        {
            std::cout<<" Call-back routine end!"<<std::endl;
            break;
        }
    }

    for(int i=0; i<m_limit_iter;i++)
        cv::circle(m_im_rgb, m_vP[i], 1, cv::Scalar(0, 255, 0), -1);
    cv::imshow(winName, m_im_rgb); 
    cv::waitKey(10);
    
    SideSort();
    SideModify();

    // Show Arrowline
    for(int i=0; i<m_vP.size(); i++)
    {
        cv::arrowedLine(m_im_rgb, m_vP[i], m_vP_mod[i], cv::Scalar(255,255,0));
        cv::circle(m_im_rgb,m_vP[i], 1, cv::Scalar(0,0,255), -1);
    }

    cv::imshow(winName, m_im_rgb);
    cv::waitKey(0);
    // Original image processing
    PointZoomOut();

    const cv::Point2f kpts1[4] = {m_vP_zoomOut[0], m_vP_zoomOut[1], m_vP_zoomOut[2], m_vP_zoomOut[3]};
    const cv::Point2f kpts2[4] = {m_vP_mod_zoomOut[0], m_vP_mod_zoomOut[1], m_vP_mod_zoomOut[2], m_vP_mod_zoomOut[3]};

    cv::Mat H = cv::getPerspectiveTransform(kpts1, kpts2);
    cv::Mat res;
    cv::Size sz = cv::Size(m_vP_mod_zoomOut[2].x, m_vP_mod_zoomOut[2].y);

    cv::warpPerspective(m_im, res, H,sz );
    sz = cv::Size(m_vP_mod[2].x,m_vP_mod[2].y);
    cv::resize(res, res, sz);
    cv::imshow("Result", res);
    cv::waitKey(0);
    

    return res;
}
}
