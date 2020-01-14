#include<Sides.h>


void Callbackproc(int event, int x, int y, int flags, void* point)
{
    RECTWARP::CBPoints *p= (RECTWARP::CBPoints*)point;
    if(event==cv::EVENT_LBUTTONDOWN&&p->n_limit_iter<4)
    {
        std::cout<<"Event occured!!"<<std::endl;
        p->vP[p->n_limit_iter].x = x; // width
        p->vP[p->n_limit_iter].y = y; // height
        p->n_limit_iter++;
    }

}
int main(int argc, char* argv[])
{
    for(int i=0; i<argc; i++)
        std::cout<<argv[i]<<std::endl;
    if(argc < 2)
    {
        std::cout<<"Usage: ./callbacktest <image_name>"<<std::endl;
        return 0;
    }
    int max_point = atoi(argv[2]);
    cv::Mat img = cv::imread(argv[1]);
    cv::Mat img_g, img_draw;
    //cv::Point p;
    RECTWARP::CBPoints ps(4);
    char key=0;
    if(!img.data)
    {
        std::cout<<"Image data is Not loaded"<<std::endl;
        return 0;
    }
    cv::cvtColor(img, img_g, cv::COLOR_RGB2GRAY);
    cv::Size sz(img_g.cols, img_g.rows);
    sz=sz/4;
    cv::resize(img_g,img_g,sz);
    cv::imshow("test Show", img_g);
    cv::cvtColor(img_g, img_draw, cv::COLOR_GRAY2BGR);
    //cv::waitKey(0);
    cv::setMouseCallback("test Show", Callbackproc, &ps);
    while(1)
    {
        for(int i=0; i<max_point;i++)
        cv::circle(img_draw, ps.vP[i], 1, cv::Scalar(0,255,0),-1);
        imshow("test Show",img_draw);
        key = cv::waitKey(30);
        if(key=='q'||ps.n_limit_iter>=4)
            break;
    }
    std::vector<cv::Point> vP1(ps.vP.begin(), ps.vP.end());
    std::vector<cv::Point> vP2;
    for(unsigned int i=0; i<vP1.size(); i++)
        std::cout<<vP1[i]<<std::endl;
    // Find Top-Left, Top-Right, Bottom-Left, Bottom-Right
    for(int i=0; i<max_point;i++)
        cv::circle(img_draw, ps.vP[i], 1, cv::Scalar(0,255,0),-1);
    // Sort the vectors to { LT, LB, RT, RB }
    vP1=RECTWARP::SideSort(vP1);
    vP2=RECTWARP::SideModify(vP1);
   
    // max x, y. min x,y
    
    std::vector<cv::Point>::iterator iter = vP2.begin();
    unsigned int idx=0;
    for(iter=vP2.begin(); iter!=vP2.end();++iter)
    {
        cv::arrowedLine(img_draw, vP1[idx], *iter, cv::Scalar(255,255,0));
        cv::circle(img_draw, *iter, 1, cv::Scalar(0,0,255),-1);
        idx++;
    }
    std::cout<<vP2<<std::endl;
    imshow("test Show",img_draw);
    cv::waitKey(0);
    const cv::Point2f kpts1[4] = {vP1[0], vP1[1], vP1[2], vP1[3]};
    const cv::Point2f kpts2[4] = {vP2[0], vP2[1], vP2[2], vP2[3]};
    std::cout<<"----------------------------"<<std::endl;
    cv::Mat H = cv::getPerspectiveTransform(kpts1, kpts2);
    cv::Mat img_result;
    cv::warpPerspective(img_g, img_result, H,sz);
    cv::cvtColor(img_result,img_result,cv::COLOR_GRAY2BGR);
    for(int i=0; i<4; i++)
    {
        cv::circle(img_result, vP2[i],1,cv::Scalar(0,0,255),-1);
    }
    cv::imshow("Result", img_result);
    std::cout<<sz<<std::endl;
    cv::waitKey(0);
return 0;
}
