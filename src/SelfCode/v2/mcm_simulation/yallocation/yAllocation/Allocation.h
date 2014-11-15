/*****************************************************************/
/*author : younghz                                               */
/*date   : 14.5.25                                               */
/*project: moos-young/deallocation                               */
/*brief  : 首先通过计算PointAndCost中点的rel_dist执行子区域的划分  */
/*         然后根据计算的cost值执行路径点的排序（但是对于这里，得到 */
/*         的结果不）是很理想，具具体分析见父文件夹下README.md中    */
/*         14.6.5部分的分析。对于解决方式可按照TSP方式（曾做过的按照*/
/*         模拟退火解决TSP问题可以用到这里），因为之前已经执行了子区域*/
/*         划分，所以这时就相当于单航行器的TSP问题。                 */
/******************************************************************/


#ifndef ALLOCATIONH
#define ALLOCATIONH

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "VehiclePointRealTime.h"

#include <vector>
#include <map>
#include <string>


//新结构
struct PointAndCost
{
    double pointx;
    double pointy;

    //rel_dist划分区域使用
    double rel_dist;

    //dist在计算中会多次用到
    double dist;

    //决定子区域路径点访问顺序
    double cost;
};

//自定义排序函数，在根据cost排序时会用到
bool sortByCost(const PointAndCost &pac1, const PointAndCost &pac2);

class CAllocation : public AppCastingMOOSApp
{
public:
    CAllocation();
    virtual  ~CAllocation();

    void registerVariables();

    bool OnNewMail(MOOSMSG_LIST& NewMail);
    bool OnConnectToServer();
    bool OnStartUp();
    bool Iterate();

private:
    VehiclePointVector *instace_vehicle_point_vector;

    //航行器最新经过的点的位置
    PointYoughz yang_current_point, betty_current_point, archie_current_point;
    //区域中相距最远的两个点
    double max_dist_in_child_regin;
    PointYoughz max_dist_point1_in_child_regin,max_dist_point2_in_child_regin;

    //计算cost因子
    float alpha;

    //装有路径点和路径点cost的vector
    std::vector<PointAndCost> vector_point_and_cost_yang;
    std::vector<PointAndCost> vector_point_and_cost_betty;
    std::vector<PointAndCost> vector_point_and_cost_archie;

    //根据三个vector_points_*的最新值划分子区域
    void childRegion( std::vector<PointYoughz> *origin_points_yang,
                      std::vector<PointYoughz> *origin_points_betty,
                      std::vector<PointYoughz> *origin_points_archie);

    //将最后得到的路径发送
    std::string notifyString( std::vector<PointAndCost> *vector_vehicle_final_path );
    std::string convertDouble(double value);

protected:

};

#endif
