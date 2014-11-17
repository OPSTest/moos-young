/*****************************************************************
author : younghz
date   : 14.5.30
project: moos-young/deallocation
brief  : 以STL中vector存放三个航行器的路径点，vector中内容是航行器的未
         完成的点，点的顺序就是航行器遍历的顺序，每完成一个路径点，就
         在相应的航行器vector中将相应的点（vector中的第一个元素）删除
         通过vector内容的改变体现航行器任务的完成程度。
         每个航行器community中都存在这三个vector，通过这三个vector体现
         全局路径点的完成情况，以便在重分配时使用。
******************************************************************/

#ifndef VEHICLEPOINTREALTIME_H
#define VEHICLEPOINTREALTIME_H

#include <string>
#include <vector>

//路径点结构
struct PointYoughz
{
    double pointx;
    double pointy;
};

class VehiclePointVector
{
public:
    VehiclePointVector();
    virtual ~VehiclePointVector();

    //三个航行器路径点vector
    std::vector<PointYoughz> vector_points_yang;
    std::vector<PointYoughz> vector_points_betty;
    std::vector<PointYoughz> vector_points_archie;

private:
    //上面的三个路径点vector初始化程序
    void initializeVehicleYang();
    void initializeVehicleBetty();
    void initializeVehicleArchie();
};

#endif

