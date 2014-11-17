#include "VehiclePointRealTime.h"

#include <stdlib.h>

using namespace std;

VehiclePointVector::VehiclePointVector()
{
    initializeVehicleYang();
    initializeVehicleBetty();
    initializeVehicleArchie();
}

VehiclePointVector::~VehiclePointVector()
{

}

void VehiclePointVector::initializeVehicleYang()
{
    //yang航行器的初始点字符串
    string str_points_yang = "50,-400:100,-400:150,-400:200,-400:250,-400:300,-400:350,-400:"
            "350,-350:350,-300:350,-250:350,-200:300,-250:300,-300:300,-350:250,-350:250,-300:"
            "250,-250:200,-300:200,-350:150,-350:150,-300:100,-350:";

    string::iterator iter = str_points_yang.begin();
    string::iterator iter_begin = iter;

    PointYoughz point_yang;

    //从str_points_*中解析出路径点
    while(iter != str_points_yang.end())
    {
        int cnt =0;
        while( *iter != ',')
        {
            ++iter;
            ++cnt;
        }
        if ( *iter == ',' && iter != str_points_yang.end() )
        {
            string str_single_point_x(iter_begin,iter_begin+cnt);

            point_yang.pointx = atof(str_single_point_x.c_str());

            //,后的那个元素
            iter_begin = ++iter;
        }

        cnt = 0;
        while( *iter != ':')
        {
            ++iter;
            ++cnt;
        }
        if( *iter == ':' && iter != str_points_yang.end() )
        {
            string str_single_point_y(iter_begin,iter_begin+cnt);

            point_yang.pointy = atof(str_single_point_y.c_str());

            //:后的元素
            iter_begin = ++iter;
        }

        //将得到的点放到vector中
        vector_points_yang.push_back(point_yang);
    }
}

void VehiclePointVector::initializeVehicleBetty()
{
    //betty 初始点：
    //  points = 0,-400:50,-350:100,-300:150,-250:200,-250:250,-200:300,-200:350,-150:350,-100:350,-50:
    //300,-50:300,-100:300,-150:250,-150:250,-100:250,-50:200,-100:200,-150:200,-200:150,-200:100,-250


    //betty航行器的初始点字符串
    string str_points_betty = "0,-400:50,-350:100,-300:150,-250:200,-250:250,-200:300,-200:350,-150:"
            "350,-100:350,-50:300,-50:300,-100:300,-150:250,-150:250,-100:250,-50:200,-100:200,-150:"
            "200,-200:150,-200:100,-250:";

    string::iterator iter =str_points_betty.begin();
    string::iterator iter_begin = iter;
    //string::iterator iter_end = iter_begin;

    PointYoughz point_betty;

    while(iter != str_points_betty.end())
    {
        int cnt =0;
        while( *iter != ',')
        {
            ++iter;
            ++cnt;
        }
        if ( *iter == ',' && iter != str_points_betty.end() )
        {
            string str_single_point_x(iter_begin,iter_begin+cnt);

            //cout << str_single_point_x;

            point_betty.pointx = atof(str_single_point_x.c_str());

            //++iter;

            //,后的那个元素
            iter_begin = ++iter;

        }

        cnt = 0;

        while( *iter != ':')
        {
            ++iter;
            ++cnt;
        }
        if( *iter == ':' && iter != str_points_betty.end() )
        {
            string str_single_point_y(iter_begin,iter_begin+cnt);

            //cout << str_single_point_y;

            point_betty.pointy = atof(str_single_point_y.c_str());

            //++iter;

            //:后的元素
            iter_begin = ++iter;
        }

        //将得到的点放到vector中
        vector_points_betty.push_back(point_betty);
    }
}

void VehiclePointVector::initializeVehicleArchie()
{
    //archie 初始点：
    // points = 0,-350:0,-300:0,-250:0,-200:0,-150:0,-100:0,-50:50,-50:100,-50:150,-50:200,-50:150,-100:
    //100,-100:50,-100:50,-150:100,-150:150,-150:100,-200:50,-200:50,-250:50,-300

    //archie航行器的初始点字符串
    string str_points_archie = "0,-350:0,-300:0,-250:0,-200:0,-150:0,-100:0,-50:50,-50:100,-50:150,-50:"
            "200,-50:150,-100:100,-100:50,-100:50,-150:100,-150:150,-150:100,-200:50,-200:50,-250:50,-300:";

    string::iterator iter =str_points_archie.begin();
    string::iterator iter_begin = iter;
    //string::iterator iter_end = iter_begin;

    PointYoughz point_archie;

    while(iter != str_points_archie.end())
    {
        int cnt =0;
        while( *iter != ',')
        {
            ++iter;
            ++cnt;
        }
        if ( *iter == ',' && iter != str_points_archie.end() )
        {
            string str_single_point_x(iter_begin,iter_begin+cnt);

            //cout << str_single_point_x;

            point_archie.pointx = atof(str_single_point_x.c_str());

            //++iter;

            //,后的那个元素
            iter_begin = ++iter;

        }

        cnt = 0;

        while( *iter != ':')
        {
            ++iter;
            ++cnt;
        }
        if( *iter == ':' && iter != str_points_archie.end() )
        {
            string str_single_point_y(iter_begin,iter_begin+cnt);

            //cout << str_single_point_y;

            point_archie.pointy = atof(str_single_point_y.c_str());

            //++iter;

            //:后的元素
            iter_begin = ++iter;
        }

        //将得到的点放到vector中
        vector_points_archie.push_back(point_archie);
    }
}
