#include<homography.h>

namespace MY_HOMO
{
Warpper::Wapper(){};
Warpper::Wapper(const cv::Mat _img)
{
    try
    {
        cv::cvtColor(_img, m_src, CV_RGB2_GRAY);
    }
    catch(cv:Exception& e)
    {
        cerr << "Exception occured: "<< e.err <<endl;
    }
}
Warpper::selectKeyPoints(int n_points=4, cv::CallBackFunc, NULL ):q
                                                                  
{
    
}

}
