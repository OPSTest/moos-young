/*
 * younghz 2014.1.11 from moos-ivp doc MOOS-V10-Tutorial
 */

// @brief 增加相应的头文件，头文件的存储位置在doc下已做分析
#include "MOOS/libMOOS/Comms/MOOSAsyncCommClient.h"
#include "MOOS/libMOOS/Utils/CommandLineParser.h"
#include "MOOS/libMOOS/Utils/ConsoleColours.h"
#include "MOOS/libMOOS/Utils/ThreadPrint.h"

MOOS::ThreadPrint gPrinter(std::cout);

// @brief 一般在此函数中注册想要接收的信息
bool OnConnect(void* pParam)
{
    CMOOSCommClient* pC = reinterpret_cast<CMOOSCommClient*> (pParam);
    pC->Register("X", 0.0);
    pC->Register("Y", 0.0);
    pC->Register("Z", 0.0);

    return true;
    
}

// @brief 在此函数中对接收到mail的处理
bool OnMail(void* pParam)
{
    CMOOSCommClient* pC = reinterpret_cast<CMOOSCommClient*> (pParam);
    
    //MOOSMSG_LIST相当于std::list<string>
    MOOSMSG_LIST M;
    //获取信息并存储于M中
    pC->Fetch(M);
    
    MOOSMSG_LIST::iterator q;//处理信息
    for(q=M.begin();q!=M.end();q++)
    {
        gPrinter.SimplyPrintTimeAndMessage("mail:"+q->GetSource());
    }
    return true;
}

bool funcX(CMOOSMsg &M, void* XCallback)
{
    std::cout<<"processing X...\n";
    gPrinter.SimplyPrintTimeAndMessage("call back for X");
    return true;
}

bool funcY(CMOOSMsg& M, void* YCallback)
{
    std::cout<<"processing Y...\n";
    gPrinter.SimplyPrintTimeAndMessage("call back for Y");
    return true;
}

// @brief 主函数
int main(int argc, char* argv[])
{
    //understand the command line
    MOOS::CommandLineParser P(argc, argv);
 
    std::string db_host = "localhost";
    P.GetVariable("--moos_host", db_host);

    int db_port = 9000;
    P.GetVariable("--moos_port", db_port);

    std::string my_name = "ex40";
    P.GetVariable("--moos_name", my_name);

    //配置comms;
    MOOS::MOOSAsyncCommClient Comms;
    Comms.SetOnMailCallBack(OnMail, &Comms);
    Comms.SetOnConnectCallBack(OnConnect, &Comms);

    // @brief 使用此函数，当有mail到来时启动线程处理
    // @param callback名字
    // @param mail 名字
    // @param mail到来调用函数
    Comms.AddMessageCallback("callback_X", "X", funcX, NULL);
    Comms.AddMessageCallback("callback_Y", "Y", funcY, NULL);

    // start comms running
    Comms.Run(db_host, db_port, my_name);

    //一直发送数据
    std::vector<unsigned char> X(100);
    for(;;)
    {
	MOOSPause(1000);
	//发送名字是X，数据是vector X的mail
	//	std::cout<<"send X,Y,Z\n";
        Comms.Notify("X", X);
	Comms.Notify("Y", "This is Y");
  	Comms.Notify("Z", 7.0);
    }
    return 0;
}
