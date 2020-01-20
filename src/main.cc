#include "Sides.h"
#define RESIZE_FACTOR 4

void print(char* prm)
{
    std::cout<<prm<<std::endl;
}
int main(int argc, char* argv[])
{
    // 0. Argment Parsing
    for(int i=0; i<argc; i++)
        std::cout<<argv[i]<<std::endl;
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

    // 3. Collect Points
    // 4. Sort Points
    // 5. Warp image
    // 6. Fusion image
    cv::setMouseCallback("test Show", RECT_WARP::CallBackProc, &frm->);
    while(1)
    {
        for(int i=0; i<max_point;i++)
        cv::circle(img_draw, ps.vP[i], 1, cv::Scalar(0,255,0),-1);
        imshow("test Show",img_draw);
        key = cv::waitKey(30);
        if(key=='q'||ps.n_limit_iter>=4)
            break;
    }
    
    // 4. Side specification
    // Allocate Personal Point class to OpenCV Point class.
    std::vector<cv::Point> vP1(ps.vP.begin(), ps.vP.end());

    std::vector<cv::Point> vP1_resz = RECT_WARP::PointZoomOut(vP1, RESIZE_FACTOR);

    std::vector<cv::Point> vP2, vP2_resz;
    for(unsigned int i=0; i<vP1.size(); i++)
        std::cout<<vP1[i]<<std::endl;
    // Find Top-Left, Top-Right, Bottom-Left, Bottom-Right
    for(int i=0; i<max_point;i++)
        cv::circle(img_draw, ps.vP[i], 1, cv::Scalar(0,255,0),-1);
    // Sort the vectors to { LT, LB, RT, RB }
    vP1 = RECT_WARP::SideSort(vP1);
    vP1_resz=RECT_WARP::SideSort(vP1_resz);

    //TODO: Makes Image Size points!!!!
    vP2=RECT_WARP::SideModify(vP1);

    vP2_resz = RECT_WARP::PointZoomOut(vP2, RESIZE_FACTOR);
    
    // Show arrow for correction
    std::vector<cv::Point>::iterator iter = vP2.begin();
    unsigned int idx=0;
    for(iter=vP2.begin(); iter!=vP2.end();++iter)
    {
        cv::arrowedLine(img_draw, vP1[idx], *iter, cv::Scalar(255,255,0));
        cv::circle(img_draw, *iter, 1, cv::Scalar(0,0,255),-1);
        idx++;
    }
    
    // 5. key Points define and init
    imshow("test Show",img_draw);
    cv::waitKey(0);
    const cv::Point2f kpts1[4] = {vP1[0], vP1[1], vP1[2], vP1[3]};
    const cv::Point2f kpts2[4] = {vP2[0], vP2[1], vP2[2], vP2[3]};


    std::cout<<"----------------------------"<<std::endl;
    cv::Mat H = cv::getPerspectiveTransform(kpts1, kpts2);
    cv::Mat img_result;
    //TODO: sz must follow container stadard ratio
    sz = cv::Size(vP2[2].x, vP2[2].y);
    
    cv::warpPerspective(img_g, img_result, H,sz);
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
