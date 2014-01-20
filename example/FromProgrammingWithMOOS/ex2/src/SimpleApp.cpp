#include "SimpleApp.h"

CSimpleApp::CSimpleApp()
{

}

CSimpleApp::~CSimpleApp()
{

}

/**
当有mail到达时由基类调用这个函数
你应该将你对mail的处理函数放在这个函数体中
**/
bool CSimpleApp::OnNewMail(MOOSMSG_LIST &NewMail)
{
    MOOSMSG_LIST::iterator p;

    for(p=NewMail.begin(); p!=NewMail.end(); p++)
    {
        CMOOSMsg& rMsg = *p;

        if( MOOSStrCmp( rMsg.GetKey(), "VehicleStatus" ) )
        {
            OnVehicleStatus(rMsg);
        }
        else if( MOOSStrCmp(rMsg.GetKey(), "Heading") )
        {
            OnHeading(rMsg);
        }
    }

    return true;
}

/**
当app与MOOSDB建立连接并打开一个channel时由基类调用这个函数
你应该在这里注册你想要接收的数据
**/
bool CSimpleApp::OnConnectToServer()
{
    DoRegistrations();

    return true;
}

/**
基类以一定频率调用这个函数
此函数的内容应是在这个app中你想要做的事情
**/

bool CSimpleApp::Iterate()
{

    return true;
}

/**
最好是在这个函数和OnConnectToServer中都进行想要接收数据的注册
这个函数在第一次执行Iterate之前被调用
**/
bool CSimpleApp::OnStartUp()
{
    DoRegistrations();

    return true;
}

bool CSimpleApp::OnVehicleStatus(CMOOSMsg &msg)
{
    MOOSTrace("I (%s) received a notification about \"%s\"the details are:\n",
              GetAppName().c_str(),
              msg.GetKey().c_str());

    //如果你想看所有输出信息的细节，
    //Msg.Trace();

    if( !msg.IsString() )
    {
        return MOOSFail("ouch - I was promised \"VehicleStatus\" would be a string");
    }

    //这里假设某个app发送信息格式如下的email：
    //Status = [],BatteryVoltage = <double>, Bilge = []
    //下面做的是在email中得到自己想要的信息
    std::string sStatus = "Unknown";
    if( !MOOSValFromString(sStatus, msg.GetString(), "Status") );
    {
        MOOSTrace("warning field \"Status\" not found in vehiclestatus string %s",
                  MOOSHERE);
    }

    double dfBatteryVoltage = -1;
    if( !MOOSValFromString(dfBatteryVoltage, msg.GetString(), "BatteryVoltage") )
    {
        MOOSTrace("warning field \"BatteryVoltage\" not found in vehiclestatus string %s",
                  MOOSHERE);
    }

    MOOSTrace("Status is \"%s\" and BatteryVoltage is %.2fV\n",
              sStatus.c_str(), dfBatteryVoltage);

    return true;
}

bool CSimpleApp::OnHeading(CMOOSMsg &msg)
{
    MOOSTrace("I (%s) have received a notification about \"%s\" the details are:\n",
              GetAppName().c_str(),  //获得这个app的名字
              msg.GetKey().c_str()); //得到变量的名字

    if( !msg.IsDouble() )
    {
        return MOOSFail("ouch - I was promised \"Heading\" would be a double %s",
                        MOOSHERE);
    }

    double dfHeading = msg.GetDouble();
    double dfTime = msg.GetTime();
    MOOSTrace("The heading (from process %s), at time %f (%f since appstart) is %f\n",
              msg.GetSource().c_str(),  //who wrote it
              dfTime,			//when
              dfTime-GetAppStartTime(), //运行时间
              dfHeading);		//the actual heading

    return true;
}

void CSimpleApp::DoRegistrations()
{

    //第二个参数代表频率
    //m_Comms在这里可以直接使用
    m_Comms.Register("VehicleStatus", 0);

    m_Comms.Register("Heading", 0.25);

    return;
}
