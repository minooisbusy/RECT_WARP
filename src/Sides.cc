#include"Sides.h"
//#include<opencv2/opencv.hpp>
//#include<iostream>

namespace RECTWARP
{
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
    float arrX[MAX_POINTS] = {vP[0].x, vP[1].x, vP[2].x, vP[3].x};
    float arrY[MAX_POINTS] = {vP[0].y, vP[1].y, vP[2].y, vP[3].y};

    const auto [minX, maxX] = std::minmax_element(&arrX[0], &arrX[4]);
    const auto [minY, maxY] = std::minmax_element(&arrY[0], &arrY[4]);
    
    
    std::vector<cv::Point> res;
    cv::Point tmp(*minX,*minY);
    res.push_back(tmp); // Left-Top
    res.push_back(cv::Point_(*maxX, *minY)); // Right-Top
    res.push_back(cv::Point_(*maxX, *maxY)); // Left-Bottom
    res.push_back(cv::Point_(*minX, *maxY)); // Right-Bottom

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
