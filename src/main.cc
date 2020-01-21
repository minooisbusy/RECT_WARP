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
    frm[0].warpProcess();
    char key=0;
    print("test Show plot");
    cv::imshow("test Show", frm[0].m_im_rgb);
    
    // 4. Sort Points
    // 5. Warp image
    // 6. Fusion image
    
return 0;
}
