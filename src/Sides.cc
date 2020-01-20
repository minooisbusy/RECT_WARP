#include"Sides.h"
//#include<opencv2/opencv.hpp>
//#include<iostream>

namespace RECT_WARP
{
void CallBackProc(int event, int x, int y, int flags, void *point)
{
    CBPoints *p = (CBPoints*)point;
    
    if(event == cv::EVENT_LBUTTONDOWN&&p->n_limit_iter<p->n_max_points)
    {
        p->vP[p->n_limit_iter].x = x;
        p->vP[p->n_limit_iter].y = y;
        p->n_limit_iter++;
    }
}
std::vector<cv::Point> SideSort(std::vector<cv::Point> vP)
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

std::vector<cv::Point> SideModify(std::vector<cv::Point> vP)
{
    double arrX[MAX_POINTS] = {vP[0].x, vP[1].x, vP[2].x, vP[3].x};
    double arrY[MAX_POINTS] = {vP[0].y, vP[1].y, vP[2].y, vP[3].y};

    const auto [minX, maxX] = std::minmax_element(&arrX[0], &arrX[4]);
    const auto [minY, maxY] = std::minmax_element(&arrY[0], &arrY[4]);
    
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
//    cv::Point tmp(*minX,*minY);
//    res.push_back(tmp); // Left-Top
//    res.push_back(cv::Point_(*maxX, *minY)); // Right-Top
//    res.push_back(cv::Point_(*maxX, *maxY)); // Left-Bottom
//    res.push_back(cv::Point_(*minX, *maxY)); // Right-Bottom

    return res;
}
std::vector<cv::Point> PointZoomOut(std::vector<cv::Point> vP, double sz)
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
CBPoints::CBPoints(int _n)
{
    n_max_points = _n;
    n_limit_iter = 0;
    for(int i=0; i<n_max_points;i++)
        vP.push_back(cv::Point(0,0));
}
}
