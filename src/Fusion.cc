#include "Fusion.h"
using namespace cv;
namespace RECT_WARP 
{
// Constructor
Fusion::Fusion(){}
Fusion::Fusion(cv::Mat left, cv::Mat right, std::vector<cv::Point> kpts1, std::vector<cv::Point> kpts2)
{
    im_left = left;// Daringly NOT use Deep copy (clone)
    im_right = right;
    kpts_left = kpts1;
    kpts_right = kpts2;
    
}
// Functiom
cv::Mat Fusion::histMatch(const Mat &reference, Mat &target, Mat &result)
{
    const float HISTMATCH = 0.000001;
    double min, max;
    std::vector<cv::Mat> ref_channels;
    cv::split(reference, ref_channels);
    std::vector<cv::Mat> tgt_channels;
    cv::split(target, tgt_channels);

    int histSize = 256;
    float range[] = {0, 256};
    const float *histRange = { range };
    bool uniform = true;
    
    for(int i = 0; i<3; i++)
    {
        cv::Mat ref_hist, tgt_hist;
        cv::Mat ref_hist_accum, tgt_hist_accum;
        cv::calcHist(&ref_channels[i], 1, 0, cv::Mat(), ref_hist, 1, &histSize,&histRange,uniform); 
         cv::calcHist(&tgt_channels[i], 1, 0, cv::Mat(), tgt_hist, 1, &histSize, &histRange,uniform);

        cv::minMaxLoc(ref_hist, &min, &max);
        if( max == 0 )
        {
            std::cout<<"Error: max is 0 in ref_hist" << std::endl;
        }
        cv::normalize(ref_hist, ref_hist, min / max, 1.0, cv::NORM_MINMAX);

        cv::minMaxLoc(tgt_hist, &min, &max);
        if( max == 0 )
        {
            std::cout<<"Error: max is 0 in ref_hist" << std::endl;
        }
        cv::normalize(ref_hist, ref_hist, min / max, 1.0, cv::NORM_MINMAX);

        ref_hist.copyTo(ref_hist_accum);
        ref_hist.copyTo(tgt_hist_accum);

        float *src_cdf_data = ref_hist_accum.ptr<float>();
        float *dst_cdf_data = tgt_hist_accum.ptr<float>();

        for(int j = 0; j < 256; j++)
        {
            src_cdf_data[j] += src_cdf_data[j - 1];
            dst_cdf_data[j] += dst_cdf_data[j - 1];
        }

        cv::minMaxLoc(ref_hist_accum, &min, &max);
        cv::normalize(ref_hist_accum, ref_hist_accum, min/max, 1.0, cv::NORM_MINMAX);

        cv::minMaxLoc(tgt_hist_accum, &min, &max);
        cv::normalize(tgt_hist_accum, tgt_hist_accum, min/max, 1.0, cv::NORM_MINMAX);

        cv::Mat lut(1, 256, CV_8UC1);
        unsigned char *M = lut.ptr<unsigned char>();
        unsigned char last = 0;

        for(int j = 0; j < tgt_hist_accum.rows; j++)
        {
            float F1 = dst_cdf_data[j];

            for(unsigned char k =last; k < ref_hist_accum.rows; k++)
            {
                float F2 = src_cdf_data[k];
                if(cv::abs(F2 - F1) < HISTMATCH || F2 > F1)
                {
                    M[j] = k;
                    last = k;
                    break;
                }
            }
        }

        LUT(tgt_channels[i], lut, tgt_channels[i]);
    }
    cv::merge(tgt_channels, result);
    return result;
}

double Fusion::norm(cv::Point p, cv::Point q)
{
    return cv::sqrt(cv::pow(p.x-q.x,2.0)+cv::pow(p.y-q.y,2.0));
}

void Fusion::SideModifyByRef()
{
    // { LT, RT, RB, LB }
    double left_max = norm(kpts_left[0], kpts_left[3]);
    double right_max = norm(kpts_right[1], kpts_right[2]);
    double height(0), width(0);

    if(left_max > right_max)
    {
        height = left_max;
        width = CONTATINER_20ft_ratio*left_max;
    }
    else
    {
        height = right_max;
        width = CONTATINER_20ft_ratio*right_max;
    }

    kpts_tgt.push_back(cv::Point(0,0));
    kpts_tgt.push_back(cv::Point(width, 0));
    kpts_tgt.push_back(cv::Point(width, height));
    kpts_tgt.push_back(cv::Point(0, height));
}
cv::Mat Fusion::makePerspective(std::vector<cv::Point> ref_vec)
{
    const cv::Point2f kpts1[4]={ref_vec[0], ref_vec[1], ref_vec[2], ref_vec[3]};
    const cv::Point2f kpts2[4]={kpts_tgt[0], kpts_tgt[1], kpts_tgt[2], kpts_tgt[3]};

    
    return cv::getPerspectiveTransform(kpts1,kpts2);
}

cv::Mat Fusion::fusionProcess()
{
    SideModifyByRef();
    cv::Mat H_left = makePerspective(kpts_left);
    cv::Mat H_right = makePerspective(kpts_right);
    cv::Mat res_left, res_right, sub_right_hist;
    
    cv::Size sz = cv::Size(kpts_tgt[2].x, kpts_tgt[2].y);
    cv::warpPerspective(im_left, res_left, H_left, sz);
    cv::warpPerspective(im_right, res_right, H_right, sz);

    
   std::cout<<"left result size:"<<"["<<res_left.rows<<", "<<res_left.cols<<"]"<<std::endl;
   std::cout<<"left result size:"<<"["<<res_right.rows<<","<<res_right.cols<<"]"<<std::endl;
    cv::Size sz_shrink = sz/4;
    cv::resize(res_left, res_left, sz/4);
    cv::resize(res_right,res_right,sz/4);
    
    cv::Mat result = Mat::zeros(sz.width/4, sz.height/4, CV_8UC3);
    cv::Rect rect_left(0,0,sz_shrink.width/2, sz_shrink.height);
    cv::Rect rect_right(sz_shrink.width/2,0,sz_shrink.width/2,sz_shrink.height);
    Mat sub_left = res_left(rect_left);
    Mat sub_right = res_right(rect_right);
    std::cout<<"hist matching start"<<std::endl;
    histMatch(sub_left,sub_right,sub_right_hist);
    std::cout<<"hist matching end"<<std::endl;
    cv::hconcat(sub_left, sub_right_hist, result);
    
    Mat tmp;
    cv::vconcat(sub_right, sub_right_hist, tmp);
    imshow("temp", tmp);
    imshow("Result", result);
    
    waitKey(0);
    return result;
}
}

