#include "Sides.h"
#include "Fusion.h"
#include <opencv2/highgui.hpp>
#define RESIZE_FACTOR 4
//TODO: Per picture processing must be refactorized
void print(char* prm)
{
    std::cout<<prm<<std::endl;
}
int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout<<"Usage: ./execute_file_name <left_image> <right_image> <#ofPoints>"<<std::endl;
        return 0;
    }

    // 1. Image load
    RECT_WARP::Frame frm[2];
    frm[0] = RECT_WARP::Frame(argv[1],argv[3]);
    frm[1] = RECT_WARP::Frame(argv[2],argv[3]);

    for(int i=0; i<2;i++)
    {
       if(!frm[i].validation())
        {
        std::cout<<"Image data is Not loaded"<<std::endl;
        return 0;
        }
    }
    cv::Mat res_left = frm[0].warpProcess();
    cv::Mat res_right =  frm[1].warpProcess();
    
    RECT_WARP::Fusion fus(frm[0].m_im,frm[1].m_im, frm[0].m_vP_zoomOut, frm[1].m_vP_zoomOut);

    cv::Mat result = fus.fusionProcess();   
    cv::imwrite("../..result/result3.jpg",result);
    //cv::imwrite("left_result.jpg", res_left);
    //cv::imwrite("right_result.jpg", res_right);

    //RECT_WARP::Fusion();
    // 4. Sort Points
    // 5. Warp image
    // 6. Fusion image
    
return 0;
}
