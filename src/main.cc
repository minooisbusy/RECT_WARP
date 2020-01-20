#include "Sides.h"
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
        std::cout<<"Usage: ./callbacktest <image_name>"<<std::endl;
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
    
    char key=0;
    print("test Show plot");
    cv::imshow("test Show", frm[0].m_im_rgb);
    
    // 4. Sort Points
    // 5. Warp image
    // 6. Fusion image
    
    cv::setMouseCallback("test Show", RECT_WARP::CallBackProc, &frm[0]);
    print("Mouse callback start");
    while(1)
    {
        for(int i=0; i<frm[0].m_limit_iter;i++)
        cv::circle(frm[0].m_im_rgb, frm[0].m_vP[i], 1, cv::Scalar(0,255,0),-1);
        cv::imshow("test Show",frm[0].m_im_rgb);
        key = cv::waitKey(30);
        if(key=='q'||frm[0].m_limit_iter>=4)
        {
            print("CallBack loop end!");
            break;
        }
    }
    std::cout<<"Original Point:\n"<<frm[0].m_vP<<std::endl;
    // 4. Side specification
    // Allocate Personal Point class to OpenCV Point class.
    cv::imshow("test Show",frm[0].m_im_rgb);
    cv::waitKey(0);

    // Find Top-Left, Top-Right, Bottom-Left, Bottom-Right
    for(int i=0; i<frm[0].m_max_points;i++)
        cv::circle(frm[0].m_im_g, frm[0].m_vP[i], 1, cv::Scalar(0,255,0),-1);
    // Sort the vectors to { LT, LB, RT, RB }
    frm[0].SideSort();
    std::cout<<"Original Point Sorted:\n"<<frm[0].m_vP<<std::endl;
    frm[0].SideModify();

    
    // Show arrow for correction
    unsigned int idx=0;
    std::cout<<"Modified point:\n"<<frm[0].m_vP_mod<<std::endl;
    for(int i=0; i<frm[0].m_vP.size();i++)
    {
        cv::arrowedLine(frm[0].m_im_rgb, frm[0].m_vP[i],frm[0].m_vP_mod[i],cv::Scalar(255,255,0));
        cv::circle(frm[0].m_im_rgb, frm[0].m_vP[i], 1, cv::Scalar(0,0,255),-1);
        idx++;
    }
    std::cout<<"Point position show!"<<std::endl;
    cv::imshow("test Show", frm[0].m_im_rgb);
    cv::waitKey(0); 
    // 5. key Points define and init
    const cv::Point2f kpts1[4] = {frm[0].m_vP[0], frm[0].m_vP[1], frm[0].m_vP[2], frm[0].m_vP[3]};
    const cv::Point2f kpts2[4] = {frm[0].m_vP_mod[0], frm[0].m_vP_mod[1], frm[0].m_vP_mod[2], frm[0].m_vP_mod[3]};


    std::cout<<"----------------------------"<<std::endl;
    cv::Mat H = cv::getPerspectiveTransform(kpts1, kpts2);
    cv::Mat img_result;
    cv::Size sz = cv::Size(frm[0].m_vP_mod[2].x, frm[0].m_vP_mod[2].y);
    
    cv::warpPerspective(frm[0].m_im_g, img_result, H,sz);
    std::cout<<H<<std::endl;
    std::cout<<sz<<std::endl;
    cv::cvtColor(img_result,img_result,cv::COLOR_GRAY2BGR);
//    for(int i=0; i<4; i++)
//   {
//        cv::circle(img_result, vP2[i],1,cv::Scalar(0,0,255),-1);
//    }

    // Crop Region?
    cv::imshow("Result", img_result);
    std::cout<<sz<<std::endl;
    cv::waitKey(0);
return 0;
}
