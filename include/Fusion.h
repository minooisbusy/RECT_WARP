#ifndef FUSION_H
#define FUSION_H
#include<opencv2/opencv.hpp>
namespace RECT_WARP
{
class Fusion
{
#define CONTATINER_20ft_ratio 2.3255813953488373
    public:
        cv::Mat im_left;
        cv::Mat im_right;
        std::vector<cv::Point> kpts_left;
        std::vector<cv::Point> kpts_right;
        std::vector<cv::Point> kpts_tgt;

        bool flg_bigger;
    // Constructor
        Fusion();
        Fusion(cv::Mat left, cv::Mat right,std::vector<cv::Point> kpts1, std::vector<cv::Point> kpts2);
    // Function
        cv::Mat histMatch(const cv::Mat &reference, cv::Mat &target, cv::Mat &result);
        double norm(cv::Point p, cv::Point q);
        void SideModifyByRef(); // Return kpts_target;
        cv::Mat makePerspective(std::vector<cv::Point> ref_vec);
        cv::Mat fusionProcess();

};
}
#endif
