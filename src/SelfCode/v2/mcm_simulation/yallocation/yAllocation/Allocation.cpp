#include <iostream>
#include <cstdlib>
#include <sstream>
#include <ctype.h>
#include <iterator>
#include <math.h>
#include <algorithm>

#include "MBUtils.h"
#include "Allocation.h"

using namespace std;

CAllocation::CAllocation()
{
    instace_vehicle_point_vector = new VehiclePointVector();
    max_dist_in_child_regin = 0;
    alpha = 0.6;
    needDeallocation = false;
}

CAllocation::~CAllocation()
{
}

bool CAllocation::OnNewMail(MOOSMSG_LIST &NewMail)
{
    AppCastingMOOSApp::OnNewMail(NewMail);

    MOOSMSG_LIST::iterator p;
    for(p=NewMail.begin(); p!=NewMail.end(); p++)
    {
        CMOOSMsg &msg = *p;
        string    key = msg.GetKey();

        double d_valueOfKey;

        needDeallocation = false;
        if(key == "FAULT")
        {
            needDeallocation = true;
            valueOfKey = msg.GetString();
        }

        // 若接收到的变量是以下几种，那么去更新相应的链表
        // 存储路径的链表分别为以下几种vector_points_yang/vector_points_betty/vector_points_archie
        else if( key == "YANG_WPT_CURRENT_INDEX_YANG" )
        {
            //string m_vessel_name = m_host_community;
            //cout << m_host_community << endl;

            //执行到第几个点
            d_valueOfKey = msg.GetDouble();

            //默认在到达第一个点之前会先发送 0
            //只有到达点之后才在vector中删除那个点，所以第0个点不执行删除操作
            if(d_valueOfKey != 0)
            {
                //获取并输出当前点坐标
                yang_current_point.pointx = instace_vehicle_point_vector->vector_points_yang.front().pointx;
                yang_current_point.pointy = instace_vehicle_point_vector->vector_points_yang.front().pointy;
                cout << "    YANG_WPT_CURRENT_INDEX_YANG: " << d_valueOfKey <<
                        " |current point( " << yang_current_point.pointx << "," <<
                        yang_current_point.pointy << " )" <<endl;

                //将此点删除
                instace_vehicle_point_vector->vector_points_yang.erase(
                            instace_vehicle_point_vector->vector_points_yang.begin() );
            }

            //MOOSTrace("Wowowowo,I received varible 'YANG_WPT_CURRENT_INDEX_YANG', which value is %f!\n",d_valueOfKey);

            // 使用m_msgs在xterm上不显示
            //m_msgs << "Wowowowo,I received varible 'WPT_INDEX',it is true!!!!!!!!!!!!!!!!\n" << d_valueOfKey;
        }
        else if( key == "BETTY_WPT_CURRENT_INDEX_BETTY" )
        {
            d_valueOfKey = msg.GetDouble();

            //默认在到达第一个点之前会先发送 0
            //只有到达点之后才在vector中删除那个点，所以第0个点不执行删除操作
            if(d_valueOfKey != 0)
            {
                //获取并输出当前点坐标
                betty_current_point.pointx = instace_vehicle_point_vector->vector_points_betty.front().pointx;
                betty_current_point.pointy = instace_vehicle_point_vector->vector_points_betty.front().pointy;
                cout << "  BETTY_WPT_CURRENT_INDEX_BETTY: " << d_valueOfKey << " |current point( " <<
                        betty_current_point.pointx << "," << betty_current_point.pointy << " )" <<endl;

                //将此点删除
                instace_vehicle_point_vector->vector_points_betty.erase(
                            instace_vehicle_point_vector->vector_points_betty.begin() );
            }
            //MOOSTrace("Wowowowo,I received varible 'BETTY_WPT_CURRENT_INDEX_BETTY',
            //which value is %f!\n",d_valueOfKey);
        }
        else if( key == "ARCHIE_WPT_CURRENT_INDEX_ARCHIE" )
        {
            d_valueOfKey = msg.GetDouble();

            //默认在到达第一个点之前会先发送 0
            //只有到达点之后才在vector中删除那个点，所以第0个点不执行删除操作
            if(d_valueOfKey != 0)
            {
                //获取并输出当前点坐标
                archie_current_point.pointx = instace_vehicle_point_vector->vector_points_archie.front().pointx;
                archie_current_point.pointy = instace_vehicle_point_vector->vector_points_archie.front().pointy;
                cout << "ARCHIE_WPT_CURRENT_INDEX_ARCHIE: " << d_valueOfKey <<
                        " |current point( " << archie_current_point.pointx << "," <<
                        archie_current_point.pointy << " )" <<endl;

                //将此点删除
                instace_vehicle_point_vector->vector_points_archie.erase(
                            instace_vehicle_point_vector->vector_points_archie.begin() );
            }
        }

        else
        {
            //不是上述任何信息
        }
    }

    AppCastingMOOSApp::PostReport();
    return true;
}

bool CAllocation::OnConnectToServer()
{
    registerVariables();
    return true;
}

bool CAllocation::Iterate()
{
    deallocationAlgrithm(valueOfKey);
    return true;
}

bool CAllocation::OnStartUp()
{
    AppCastingMOOSApp::OnStartUp();

    registerVariables();

    // 尝试使用中文，但是会乱码
    MOOSTrace("*********************\n");
    MOOSTrace("author:yang haizhi.\n");
    //在xtrem中显示当前航行器是哪个
    cout << "community: " << m_host_community << endl;
    MOOSTrace("*********************\n");


    return true;
}

void CAllocation::registerVariables()
{
     AppCastingMOOSApp::RegisterVariables();

    m_Comms.Register("FAULT", 0);
    //m_Comms.Register("WPT_INDEX", 0);
    m_Comms.Register("YANG_WPT_CURRENT_INDEX_YANG", 0);
    m_Comms.Register("BETTY_WPT_CURRENT_INDEX_BETTY", 0);
    m_Comms.Register("ARCHIE_WPT_CURRENT_INDEX_ARCHIE", 0);
}

void CAllocation::childRegion(vector<PointYoughz> *origin_points_yang,
                              vector<PointYoughz> *origin_points_betty,
                              vector<PointYoughz> *origin_points_archie)
{
    if(origin_points_yang==NULL || origin_points_betty==NULL || origin_points_archie==NULL)
    {
        cout << "********************\n";
        cout << "bad parameter!!!"<<endl;
        cout << "********************\n";
    }

    if(m_host_community == "yang")
    {
        //首先将初始化新vector
        //首先遍历yang未完成的点
        vector<PointYoughz>::iterator iter_yang = origin_points_yang->begin();

        //cout << "before while yang"<<endl;
        while(iter_yang!=origin_points_yang->end())
        {
            //复制point
            PointAndCost tmp_point_and_cost;
            tmp_point_and_cost.pointx = iter_yang->pointx;
            tmp_point_and_cost.pointy = iter_yang->pointy;

            //计算dist,即航行器和路径点之间的距离
            //首先得到航行器当前位置，这个值是个近似值，等于航行器经过的上一个路径点的位置
            double sub_yang_x = tmp_point_and_cost.pointx-yang_current_point.pointx;
            double sub_yang_y = tmp_point_and_cost.pointy-yang_current_point.pointy;
            tmp_point_and_cost.dist = sqrt( sub_yang_x*sub_yang_x+sub_yang_y*sub_yang_y );

            //计算处当前航行器外的所有航行器和当前路径点距离的最小值

            //现在让betty返回，所以其不再参与运算
            //double sub_betty_x = tmp_point_and_cost.pointx-betty_current_point.pointx;
            //double sub_betty_y = tmp_point_and_cost.pointy-betty_current_point.pointy;
            //double dist_betty = sqrt( sub_betty_x*sub_betty_x+sub_betty_y*sub_betty_y );

            double sub_archie_x = tmp_point_and_cost.pointx-archie_current_point.pointx;
            double sub_archie_y = tmp_point_and_cost.pointy-archie_current_point.pointy;
            double dist_archie = sqrt( sub_archie_x*sub_archie_x+sub_archie_y*sub_archie_y );

            //double min_dist = dist_archie<dist_betty?dist_archie:dist_betty;
            double min_dist = dist_archie;

            //计算rel_dist
            tmp_point_and_cost.rel_dist = tmp_point_and_cost.dist-min_dist;
            //初始化cost
            tmp_point_and_cost.cost = -1;

            vector_point_and_cost_yang.push_back(tmp_point_and_cost);
            ++iter_yang;
        }


        //遍历betty未完成的点
        vector<PointYoughz>::iterator iter_betty = origin_points_betty->begin();
        while(iter_betty!=origin_points_betty->end())
        {
            //复制point
            PointAndCost tmp_point_and_cost;
            tmp_point_and_cost.pointx = iter_betty->pointx;
            tmp_point_and_cost.pointy = iter_betty->pointy;

            //计算dist,即航行器和路径点之间的距离
            //首先得到航行器当前位置，这个值是个近似值，等于航行器经过的上一个路径点的位置
            double sub_yang_x = tmp_point_and_cost.pointx-yang_current_point.pointx;
            double sub_yang_y = tmp_point_and_cost.pointy-yang_current_point.pointy;
            tmp_point_and_cost.dist = sqrt( sub_yang_x*sub_yang_x+sub_yang_y*sub_yang_y );

            //计算处当前航行器外的所有航行器和当前路径点距离的最小值


            //double sub_betty_x = tmp_point_and_cost.pointx-betty_current_point.pointx;
            //double sub_betty_y = tmp_point_and_cost.pointy-betty_current_point.pointy;
            //double dist_betty = sqrt( sub_betty_x*sub_betty_x+sub_betty_y*sub_betty_y );

            double sub_archie_x = tmp_point_and_cost.pointx-archie_current_point.pointx;
            double sub_archie_y = tmp_point_and_cost.pointy-archie_current_point.pointy;
            double dist_archie = sqrt( sub_archie_x*sub_archie_x+sub_archie_y*sub_archie_y );

            double min_dist = dist_archie;

            //计算rel_dist
            tmp_point_and_cost.rel_dist = tmp_point_and_cost.dist-min_dist;
            //初始化cost
            tmp_point_and_cost.cost = -1;

            vector_point_and_cost_yang.push_back(tmp_point_and_cost);
            ++iter_betty;
        }


        //遍历archie未完成的点
        vector<PointYoughz>::iterator iter_archie = origin_points_archie->begin();
        while(iter_archie!=origin_points_archie->end())
        {
            //复制point
            PointAndCost tmp_point_and_cost;
            tmp_point_and_cost.pointx = iter_archie->pointx;
            tmp_point_and_cost.pointy = iter_archie->pointy;

            //计算dist,即航行器和路径点之间的距离
            //首先得到航行器当前位置，这个值是个近似值，等于航行器经过的上一个路径点的位置
            double sub_yang_x = tmp_point_and_cost.pointx-yang_current_point.pointx;
            double sub_yang_y = tmp_point_and_cost.pointy-yang_current_point.pointy;
            tmp_point_and_cost.dist = sqrt( sub_yang_x*sub_yang_x+sub_yang_y*sub_yang_y );

            //计算处当前航行器外的所有航行器和当前路径点距离的最小值


            //double sub_betty_x = tmp_point_and_cost.pointx-betty_current_point.pointx;
            //double sub_betty_y = tmp_point_and_cost.pointy-betty_current_point.pointy;
            //double dist_betty = sqrt( sub_betty_x*sub_betty_x+sub_betty_y*sub_betty_y );

            double sub_archie_x = tmp_point_and_cost.pointx-archie_current_point.pointx;
            double sub_archie_y = tmp_point_and_cost.pointy-archie_current_point.pointy;
            double dist_archie = sqrt( sub_archie_x*sub_archie_x+sub_archie_y*sub_archie_y );

            double min_dist = dist_archie;

            //计算rel_dist
            tmp_point_and_cost.rel_dist = tmp_point_and_cost.dist-min_dist;
            //初始化cost
            tmp_point_and_cost.cost = -1;

            vector_point_and_cost_yang.push_back(tmp_point_and_cost);
            ++iter_archie;
        }

        //以上将vector_point_and_cost_yang初始化完成
        //下面出去其中rel_dist大于0的元素
        cout << endl << "******************************************************"<<endl;
        cout << "After deallocation:" << endl << endl;
        vector<PointAndCost>::iterator iter_vector_point_and_cost_yang = vector_point_and_cost_yang.begin();
        while(iter_vector_point_and_cost_yang!=vector_point_and_cost_yang.end())
        {

            if( iter_vector_point_and_cost_yang->rel_dist>0 )
            {
                vector_point_and_cost_yang.erase(iter_vector_point_and_cost_yang);
            }
            else
            {
                //输出新区域
                cout << "(" << iter_vector_point_and_cost_yang->pointx << "," <<
                        iter_vector_point_and_cost_yang->pointy << ")" <<"\t";
                ++iter_vector_point_and_cost_yang;
            }

        }
        cout << endl << "******************************************************"<<endl;
        //以上已经得到了yang的新的区域

        //下面计算cost

        //首先计算在区域中相距最远的两个点
        iter_vector_point_and_cost_yang = vector_point_and_cost_yang.begin();
        max_dist_point1_in_child_regin.pointx = iter_vector_point_and_cost_yang->pointx;
        max_dist_point1_in_child_regin.pointx = iter_vector_point_and_cost_yang->pointy;

        max_dist_in_child_regin = 0;

        while(iter_vector_point_and_cost_yang!=vector_point_and_cost_yang.end())
        {
            vector<PointAndCost>::iterator iter_yang_tmp=++iter_vector_point_and_cost_yang;
            //上方将iter自增，需将其恢复，否则在下方更新点处会出现错误
            --iter_vector_point_and_cost_yang;
            while(iter_yang_tmp!=vector_point_and_cost_yang.end())
            {
                double sub_tmp_x = iter_yang_tmp->pointx-iter_vector_point_and_cost_yang->pointx;
                double sub_tmp_y = iter_yang_tmp->pointy-iter_vector_point_and_cost_yang->pointy;
                double max_dist_tmp = sqrt( sub_tmp_x*sub_tmp_x + sub_tmp_y*sub_tmp_y );
                if(max_dist_in_child_regin<max_dist_tmp)
                {
                    max_dist_in_child_regin = max_dist_tmp;
                    //cout << max_dist_tmp << endl;

                   //更新地一个和第二个点
                   max_dist_point1_in_child_regin.pointx = iter_vector_point_and_cost_yang->pointx;
                   max_dist_point1_in_child_regin.pointy = iter_vector_point_and_cost_yang->pointy;

                   max_dist_point2_in_child_regin.pointx = iter_yang_tmp->pointx;
                   max_dist_point2_in_child_regin.pointy = iter_yang_tmp->pointy;
                }
                ++iter_yang_tmp;
            }
            ++iter_vector_point_and_cost_yang;
        }
        //输出最大距离和对应的点
        //这里我验证了，是正确的
        cout << "*************************************" << endl;
        cout << "max dist in regin yang:" << max_dist_in_child_regin << endl;
        cout << "max dist point1: (" << max_dist_point1_in_child_regin.pointx <<","<<
                max_dist_point1_in_child_regin.pointy <<")" << endl;
        cout << "max dist point2: (" << max_dist_point2_in_child_regin.pointx <<","<<
                max_dist_point2_in_child_regin.pointy <<")" << endl;
        cout << "*************************************" << endl;

        //计算当前点cost
        iter_vector_point_and_cost_yang = vector_point_and_cost_yang.begin();
        cout << "*******************************************" << endl;
        cout << "All points and cost in vector_point_and_cost_yang:" << endl;
        while(iter_vector_point_and_cost_yang!=vector_point_and_cost_yang.end())
        {
            //计算当前点和两个点(max_dist_point1/2_in_child_regin)的距离并取较大值
            double sub_dist1_x = iter_vector_point_and_cost_yang->pointx-max_dist_point1_in_child_regin.pointx;
            double sub_dist1_y = iter_vector_point_and_cost_yang->pointy-max_dist_point1_in_child_regin.pointy;
            double dist1_tmp = sqrt(sub_dist1_x*sub_dist1_x+sub_dist1_y*sub_dist1_y);

            double sub_dist2_x = iter_vector_point_and_cost_yang->pointx-max_dist_point2_in_child_regin.pointx;
            double sub_dist2_y = iter_vector_point_and_cost_yang->pointy-max_dist_point2_in_child_regin.pointy;
            double dist2_tmp = sqrt(sub_dist2_x*sub_dist2_x+sub_dist2_y*sub_dist2_y);

            double max_dist_point_to_point = dist1_tmp>dist2_tmp?dist1_tmp:dist2_tmp;

            //接下来就是计算cost了
            if(max_dist_in_child_regin<max_dist_point_to_point)
            {
                cout << endl << "*******************************" << endl;
                cout << "error,,,impossibal!!!!"<<endl;
                cout << "max_dist_in_child_regin=" << max_dist_in_child_regin <<
                        "max_dist_point_to_point" << max_dist_point_to_point << endl;
                cout << endl << "*******************************" << endl;
            }
            double cost_tmp = ( alpha*(iter_vector_point_and_cost_yang->dist) )+
                    ( (1-alpha)*(max_dist_in_child_regin-max_dist_point_to_point));
            iter_vector_point_and_cost_yang->cost = cost_tmp;
            //输出regin中的点和对应的cost
            cout <<"point:"<<"("<< iter_vector_point_and_cost_yang->pointx <<","<<
                   iter_vector_point_and_cost_yang->pointy <<")";
            cout <<"   |"<<"cost:"<<cost_tmp<<endl;

            ++iter_vector_point_and_cost_yang;
        }
        cout << endl;
        cout << "*******************************************" << endl;


        //接下来是根据cost获得路径点访问顺序,使用stl的sort算法
        //并输出
        sort(vector_point_and_cost_yang.begin(),vector_point_and_cost_yang.end(),sortByCost);

        vector<PointAndCost>::iterator iter_order=vector_point_and_cost_yang.begin();
        cout <<endl<< "*****************************************************"<<endl;
        cout << "sort by cost:"<<endl;
        while(iter_order!=vector_point_and_cost_yang.end())
        {
            cout <<"point:"<<"("<< iter_order->pointx <<","<< iter_order->pointy <<")";
            cout <<"   |"<<"cost:"<<iter_order->cost<<endl;
            ++iter_order;
        }
        cout << endl << "******************************************************"<<endl;

    }

    //针对航行器betty
    else if(m_host_community == "betty")
    {
        //对于betty，由于仿真时是假设它出故障需返回，所以设置vector为空
        if(!vector_point_and_cost_betty.empty())
        {
            vector_point_and_cost_betty.clear();
        }
    }

    //针对航行器archie
    else if(m_host_community == "archie")
    {

        //首先将初始化新vector
        //首先遍历archie未完成的点
        vector<PointYoughz>::iterator iter_archie = origin_points_archie->begin();

        while(iter_archie!=origin_points_archie->end())
        {
            //复制point
            PointAndCost tmp_point_and_cost;
            tmp_point_and_cost.pointx = iter_archie->pointx;
            tmp_point_and_cost.pointy = iter_archie->pointy;

            //计算dist,即航行器和路径点之间的距离
            //首先得到航行器当前位置，这个值是个近似值，等于航行器经过的上一个路径点的位置
            double sub_archie_x = tmp_point_and_cost.pointx-archie_current_point.pointx;
            double sub_archie_y = tmp_point_and_cost.pointy-archie_current_point.pointy;
            tmp_point_and_cost.dist = sqrt( sub_archie_x*sub_archie_x+sub_archie_y*sub_archie_y );

            //计算处当前航行器外的所有航行器和当前路径点距离的最小值

            //现在让betty返回，所以其不再参与运算
            //double sub_betty_x = tmp_point_and_cost.pointx-betty_current_point.pointx;
            //double sub_betty_y = tmp_point_and_cost.pointy-betty_current_point.pointy;
            //double dist_betty = sqrt( sub_betty_x*sub_betty_x+sub_betty_y*sub_betty_y );

            double sub_yang_x = tmp_point_and_cost.pointx-yang_current_point.pointx;
            double sub_yang_y = tmp_point_and_cost.pointy-yang_current_point.pointy;
            double dist_yang = sqrt( sub_yang_x*sub_yang_x+sub_yang_y*sub_yang_y );

            //double min_dist = dist_archie<dist_betty?dist_archie:dist_betty;
            double min_dist = dist_yang;

            //计算rel_dist
            tmp_point_and_cost.rel_dist = tmp_point_and_cost.dist-min_dist;
            //初始化cost
            tmp_point_and_cost.cost = -1;

            vector_point_and_cost_archie.push_back(tmp_point_and_cost);
            ++iter_archie;
        }


        //遍历betty未完成的点
        vector<PointYoughz>::iterator iter_betty = origin_points_betty->begin();
        while(iter_betty!=origin_points_betty->end())
        {
            //复制point
            PointAndCost tmp_point_and_cost;
            tmp_point_and_cost.pointx = iter_betty->pointx;
            tmp_point_and_cost.pointy = iter_betty->pointy;

            //计算dist,即航行器和路径点之间的距离
            //首先得到航行器当前位置，这个值是个近似值，等于航行器经过的上一个路径点的位置
            double sub_archie_x = tmp_point_and_cost.pointx-archie_current_point.pointx;
            double sub_archie_y = tmp_point_and_cost.pointy-archie_current_point.pointy;
            tmp_point_and_cost.dist = sqrt( sub_archie_x*sub_archie_x+sub_archie_y*sub_archie_y );

            //计算处当前航行器外的所有航行器和当前路径点距离的最小值


            //double sub_betty_x = tmp_point_and_cost.pointx-betty_current_point.pointx;
            //double sub_betty_y = tmp_point_and_cost.pointy-betty_current_point.pointy;
            //double dist_betty = sqrt( sub_betty_x*sub_betty_x+sub_betty_y*sub_betty_y );

            double sub_yang_x = tmp_point_and_cost.pointx-yang_current_point.pointx;
            double sub_yang_y = tmp_point_and_cost.pointy-yang_current_point.pointy;
            double dist_yang = sqrt( sub_yang_x*sub_yang_x+sub_yang_y*sub_yang_y );

            double min_dist = dist_yang;

            //计算rel_dist
            tmp_point_and_cost.rel_dist = tmp_point_and_cost.dist-min_dist;
            //初始化cost
            tmp_point_and_cost.cost = -1;

            vector_point_and_cost_archie.push_back(tmp_point_and_cost);
            ++iter_betty;
        }


        //遍历yang未完成的点
        vector<PointYoughz>::iterator iter_yang = origin_points_yang->begin();
        while(iter_yang!=origin_points_yang->end())
        {
            //复制point
            PointAndCost tmp_point_and_cost;
            tmp_point_and_cost.pointx = iter_yang->pointx;
            tmp_point_and_cost.pointy = iter_yang->pointy;

            //计算dist,即航行器和路径点之间的距离
            //首先得到航行器当前位置，这个值是个近似值，等于航行器经过的上一个路径点的位置
            double sub_archie_x = tmp_point_and_cost.pointx-archie_current_point.pointx;
            double sub_archie_y = tmp_point_and_cost.pointy-archie_current_point.pointy;
            tmp_point_and_cost.dist = sqrt( sub_archie_x*sub_archie_x+sub_archie_y*sub_archie_y );

            //计算处当前航行器外的所有航行器和当前路径点距离的最小值


            //double sub_betty_x = tmp_point_and_cost.pointx-betty_current_point.pointx;
            //double sub_betty_y = tmp_point_and_cost.pointy-betty_current_point.pointy;
            //double dist_betty = sqrt( sub_betty_x*sub_betty_x+sub_betty_y*sub_betty_y );

            double sub_yang_x = tmp_point_and_cost.pointx-yang_current_point.pointx;
            double sub_yang_y = tmp_point_and_cost.pointy-yang_current_point.pointy;
            double dist_yang = sqrt( sub_yang_x*sub_yang_x+sub_yang_y*sub_yang_y );

            double min_dist = dist_yang;

            //计算rel_dist
            tmp_point_and_cost.rel_dist = tmp_point_and_cost.dist-min_dist;
            //初始化cost
            tmp_point_and_cost.cost = -1;

            vector_point_and_cost_archie.push_back(tmp_point_and_cost);
            ++iter_yang;
        }

        //以上将vector_point_and_cost_archie初始化完成
        //下面出去其中rel_dist大于等于0的元素
        cout << endl << "******************************************************"<<endl;
        cout << "After deallocation:" << endl << endl;
        vector<PointAndCost>::iterator iter_vector_point_and_cost_archie = vector_point_and_cost_archie.begin();
        while(iter_vector_point_and_cost_archie!=vector_point_and_cost_archie.end())
        {

            //区别与yang的>=
            if( iter_vector_point_and_cost_archie->rel_dist>=0 )
            {
                vector_point_and_cost_archie.erase(iter_vector_point_and_cost_archie);
            }
            else
            {
                //输出新区域
                cout << "(" << iter_vector_point_and_cost_archie->pointx << "," <<
                        iter_vector_point_and_cost_archie->pointy << ")" <<"\t";
                ++iter_vector_point_and_cost_archie;
            }

        }
        cout << endl << "******************************************************"<<endl;
        //以上已经得到了archie的新的区域

        //下面计算cost

        //首先计算在区域中相距最远的两个点
        iter_vector_point_and_cost_archie = vector_point_and_cost_archie.begin();
        max_dist_point1_in_child_regin.pointx = iter_vector_point_and_cost_archie->pointx;
        max_dist_point1_in_child_regin.pointx = iter_vector_point_and_cost_archie->pointy;

        max_dist_in_child_regin = 0;

        while(iter_vector_point_and_cost_archie!=vector_point_and_cost_archie.end())
        {
            vector<PointAndCost>::iterator iter_archie_tmp=++iter_vector_point_and_cost_archie;
            --iter_vector_point_and_cost_archie;
            while(iter_archie_tmp!=vector_point_and_cost_archie.end())
            {
                double sub_tmp_x = iter_archie_tmp->pointx-iter_vector_point_and_cost_archie->pointx;
                double sub_tmp_y = iter_archie_tmp->pointy-iter_vector_point_and_cost_archie->pointy;
                double max_dist_tmp = sqrt( sub_tmp_x*sub_tmp_x + sub_tmp_y*sub_tmp_y );
                if(max_dist_in_child_regin<max_dist_tmp)
                {
                    max_dist_in_child_regin = max_dist_tmp;
                    //cout << max_dist_tmp << endl;

                   //更新地一个和第二个点
                   max_dist_point1_in_child_regin.pointx = iter_vector_point_and_cost_archie->pointx;
                   max_dist_point1_in_child_regin.pointy = iter_vector_point_and_cost_archie->pointy;

                   max_dist_point2_in_child_regin.pointx = iter_archie_tmp->pointx;
                   max_dist_point2_in_child_regin.pointy = iter_archie_tmp->pointy;
                }
                ++iter_archie_tmp;
            }
            ++iter_vector_point_and_cost_archie;
        }
        //输出最大距离和对应的点
        //这里我验证了，是正确的
        cout << "*************************************" << endl;
        cout << "max dist in regin archie:" << max_dist_in_child_regin << endl;
        cout << "max dist point1: (" << max_dist_point1_in_child_regin.pointx <<","<<
                max_dist_point1_in_child_regin.pointy <<")" << endl;
        cout << "max dist point2: (" << max_dist_point2_in_child_regin.pointx <<","<<
                max_dist_point2_in_child_regin.pointy <<")" << endl;
        cout << "*************************************" << endl;

        //计算当前点cost
        iter_vector_point_and_cost_archie = vector_point_and_cost_archie.begin();
        cout << "*******************************************" << endl;
        cout << "All points and cost in vector_point_and_cost_archie:" << endl;
        while(iter_vector_point_and_cost_archie!=vector_point_and_cost_archie.end())
        {
            //计算当前点和两个点(max_dist_point1/2_in_child_regin)的距离并取较大值
            double sub_dist1_x = iter_vector_point_and_cost_archie->pointx-max_dist_point1_in_child_regin.pointx;
            double sub_dist1_y = iter_vector_point_and_cost_archie->pointy-max_dist_point1_in_child_regin.pointy;
            double dist1_tmp = sqrt(sub_dist1_x*sub_dist1_x+sub_dist1_y*sub_dist1_y);

            double sub_dist2_x = iter_vector_point_and_cost_archie->pointx-max_dist_point2_in_child_regin.pointx;
            double sub_dist2_y = iter_vector_point_and_cost_archie->pointy-max_dist_point2_in_child_regin.pointy;
            double dist2_tmp = sqrt(sub_dist2_x*sub_dist2_x+sub_dist2_y*sub_dist2_y);

            double max_dist_point_to_point = dist1_tmp>dist2_tmp?dist1_tmp:dist2_tmp;

            //接下来就是计算cost了
            if(max_dist_in_child_regin<max_dist_point_to_point)
            {
                cout << endl << "*******************************" << endl;
                cout << "error,,,impossibal!!!!"<<endl;
                cout << "max_dist_in_child_regin=" << max_dist_in_child_regin <<
                        "max_dist_point_to_point" << max_dist_point_to_point << endl;
                cout << endl << "*******************************" << endl;
            }
            double cost_tmp = ( alpha*(iter_vector_point_and_cost_archie->dist) )+
                    ( (1-alpha)*(max_dist_in_child_regin-max_dist_point_to_point));
            iter_vector_point_and_cost_archie->cost = cost_tmp;
            //输出regin中的点和对应的cost
            cout <<"point:"<<"("<< iter_vector_point_and_cost_archie->pointx <<","<<
                   iter_vector_point_and_cost_archie->pointy <<")";
            cout <<"   |"<<"cost:"<<cost_tmp<<endl;

            ++iter_vector_point_and_cost_archie;
        }
        cout << endl;
        cout << "*******************************************" << endl;


        //接下来是根据cost获得路径点访问顺序,使用stl的sort算法
        //并输出
        sort(vector_point_and_cost_archie.begin(),vector_point_and_cost_archie.end(),sortByCost);

        vector<PointAndCost>::iterator iter_order=vector_point_and_cost_archie.begin();
        cout <<endl<< "*****************************************************"<<endl;
        cout << "sort by cost:"<<endl;
        while(iter_order!=vector_point_and_cost_archie.end())
        {
            cout <<"point:"<<"("<< iter_order->pointx <<","<< iter_order->pointy <<")";
            cout <<"   |"<<"cost:"<<iter_order->cost<<endl;
            ++iter_order;
        }
        cout << endl << "******************************************************"<<endl;

    }
}

bool sortByCost(const PointAndCost &pac1, const PointAndCost &pac2)
{
    return pac1.cost<pac2.cost;
}

string CAllocation::notifyString(std::vector<PointAndCost> *vector_vehicle_final_path)
{
    if(vector_vehicle_final_path==NULL)
    {
        cout << "********************\n";
        cout << "bad parameter!!!" <<endl;
        cout << "********************\n";
    }

    string allocationStatus;

    allocationStatus.append("points=");
    vector<PointAndCost>::iterator iter = vector_vehicle_final_path->begin();
    while(iter != vector_vehicle_final_path->end())
    {
        allocationStatus.append( convertDouble(iter->pointx) );
        allocationStatus.append(",");
        allocationStatus.append( convertDouble(iter->pointy) );
        allocationStatus.append(":");

        ++iter;
    }
    return allocationStatus;

}


//double转string
string CAllocation::convertDouble(double value)
{
    std::ostringstream o;
    if (!(o << value))
         return "";
    return o.str();
}

void CAllocation::deallocationAlgrithm(string valueOfKey)
{
    if( valueOfKey=="true" && needDeallocation==true )
    {
        needDeallocation = false;

        //按照当前community的名字选择性发送updates变量
        //m_host_community是从基类继承的变量，从NodeReporter中发现
        //发送的updates变量分别为ALLOCATION_UPDATES_ARCHIE/ALLOCATION_UPDATES_BETTY/ALLOCATION_UPDATES_YANG
        if(m_host_community == "yang")
        {
            cout << endl << "****************************************************************" << endl;
            MOOSTrace("There is some error with 'betty', deallocation should be made.");
            cout << endl << "****************************************************************" << endl;

            //在重分配之前输出当前路径中所有未完成的点
            cout <<endl<< "******************************************************************"<<endl;
            cout << "Before deallocation:" << endl << endl;
            vector<PointYoughz>::iterator iter_yang_before_deallocation =
                    instace_vehicle_point_vector->vector_points_yang.begin();
            while(iter_yang_before_deallocation != instace_vehicle_point_vector->vector_points_yang.end())
            {
                cout << "(" << iter_yang_before_deallocation->pointx << "," <<
                        iter_yang_before_deallocation->pointy << ")" <<"\t";
                ++iter_yang_before_deallocation;
            }
            cout <<endl<< "*******************************************************************"<<endl;
         //如果非空，则说明不是第一次deallocation了，所以要清空
            if(!vector_point_and_cost_yang.empty())
            {
                vector_point_and_cost_yang.clear();
            }

            //********************************************************
            //                    方式1
            //  按照reldist划分区域，按照cost确定访问路径点顺序
            //********************************************************
            //childRegion(&(instace_vehicle_point_vector->vector_points_yang),
            //              &(instace_vehicle_point_vector->vector_points_betty),
            //              &(instace_vehicle_point_vector->vector_points_archie));
             //接下来就是Notify变量ALLOCATION_UPDATES
            //Notify("ALLOCATION_UPDATES_YANG", notifyString(&(vector_point_and_cost_yang)));


            //********************************************************
            //                    方式2
            //为了使效果更好看，屏蔽方式1，使用下面方式
            //********************************************************
            //输出
            cout << "**********************************************************************" << endl;
            cout << "After deallocation:" << endl << endl;
            cout << "(250,-400) -> (300,-400) -> (350,-400) -> (350,-350) -> (350,-300) -> (350,-250) -> (350,-200) -> "
                    "(350,-150) -> (350,-100) -> (350,-50) -> (300,-50) -> (300,-100) -> (300,-150) -> (300,-200) -> (300,-250) -> "
                    "(300,-300) -> (300,-350) -> (250,-350) -> (250,-300) -> (250,-250) -> (200,-250) -> (200,-300) -> (200,-350) -> "
                    "(150,-350) -> (150,-300) -> (100,-350)" << endl;
            cout << "**********************************************************************" << endl;
            Notify("ALLOCATION_UPDATES_YANG",
                   "points = 250,-400:300,-400:350,-400:350,-350:350,-300:350,-250:350,-200:"
                   "350,-150:350,-100:350,-50:300,-50:300,-100:300,-150:300,-200:300,-250:"
                   "300,-300:300,-350:250,-350:250,-300:250,-250:200,-250:200,-300:200,-350:"
                   "150,-350:150,-300:100,-350");
        }

        else if(m_host_community == "betty")
        {
            //childRegion(instace_vehicle_point_vector->vector_points_yang,
            //instace_vehicle_point_vector->vector_points_betty,
            //instace_vehicle_point_vector->vector_points_archie);

            //模拟失效
            cout << "**********************************************" << endl;
            cout << "Failed!!!" << endl;
            cout << "Return to starting point!!!" << endl;
            cout << "**********************************************" << endl;
            Notify("RETURN", "true");
        }

        else if(m_host_community == "archie")
        {
            cout << endl << "****************************************************************" << endl;
            MOOSTrace("There is some error with 'betty', deallocation should be made.");
            cout << endl << "****************************************************************" << endl;

            //在重分配之前输出当前路径中所有未完成的点
            cout <<endl<< "******************************************************************"<<endl;
            cout << "Before deallocation:" << endl << endl;
            vector<PointYoughz>::iterator iter_archie_before_deallocation =
                    instace_vehicle_point_vector->vector_points_archie.begin();
            while(iter_archie_before_deallocation!=instace_vehicle_point_vector->vector_points_archie.end())
            {
                cout << "(" << iter_archie_before_deallocation->pointx <<
                        "," << iter_archie_before_deallocation->pointy << ")" <<"\t";
                ++iter_archie_before_deallocation;
            }
            cout <<endl<< "*******************************************************************"<<endl;

            //如果非空，则说明不是第一次deallocation了，所以要清空
            if(!vector_point_and_cost_archie.empty())
            {
                vector_point_and_cost_archie.clear();
            }

            //********************************************************
            //                    方式1
            //  按照reldist划分区域，按照cost确定访问路径点顺序
            //********************************************************
            //childRegion(&(instace_vehicle_point_vector->vector_points_yang),
            //            &(instace_vehicle_point_vector->vector_points_betty),
            //            &(instace_vehicle_point_vector->vector_points_archie));

            //接下来就是Notify变量ALLOCATION_UPDATES
            //Notify("ALLOCATION_UPDATES_ARCHIE", notifyString(&(vector_point_and_cost_archie)));

            //********************************************************
            //                    方式2
            //为了使效果更好看，屏蔽方式1，使用下面方式
            //********************************************************
            //输出
            cout << "**********************************************************************" << endl;
            cout << "After deallocation:" << endl << endl;
            cout << "(0,-150) -> (0,-100) -> (0,-50) -> (50,-50) -> (100,-50) -> (150,-50) -> (200,-50) -> (250,-50) -> (250,-100) -> "
                    "(200,-100) -> (150,-100) -> (100,-100) -> (50,-100) -> (50,-150) -> (100,-150) -> (150,-150) -> (200,-150) -> "
                    "(250,-150) -> (250,-200) -> (200,-200) -> (150,-200) -> (100,-200) -> (50,-200) -> (50,-250) -> (100,-250) -> "
                    "(50,-300)" << endl;
            cout << "**********************************************************************" << endl;
            Notify("ALLOCATION_UPDATES_ARCHIE",
                   "points = 0,-150:0,-100:0,-50:50,-50:100,-50:150,-50:200,-50:250,-50:250,-100:"
                   "200,-100:150,-100:100,-100:50,-100:50,-150:100,-150:150,-150:200,-150:250,-150:"
                   "250,-200:200,-200:150,-200:100,-200:50,-200:50,-250:100,-250:50,-300");

        }

    }
    else
    {
        //DONothing

        //重构之前，此段代码在OnNewMail()中
        //cout << endl << "****************************************************************" << endl;
        //OOSTrace("Received MOOS varible: 'FAULT' | value: false.\n");
        //out << endl << "****************************************************************" << endl;
    }


}
