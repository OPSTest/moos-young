#include "GlobalInformation.h"
#include <iostream>
#include <cstdlib>
#include <ctype.h>
#include "/home/haizhi/moos-ivp/include/ivp/ColorParse.h"
#include <iterator>
#include "/home/haizhi/moos-ivp/include/ivp/MBUtils.h"
#include "/home/haizhi/moos-ivp/include/ivp/ACTable.h"

using namespace std;

CGlobalInformation::CGlobalInformation()
{
}

CGlobalInformation::~CGlobalInformation()
{
}

bool CGlobalInformation::OnNewMail(MOOSMSG_LIST &NewMail)
{
    AppCastingMOOSApp::OnNewMail(NewMail);

    MOOSMSG_LIST::iterator p;
    for(p=NewMail.begin(); p!=NewMail.end(); p++)
    {
        CMOOSMsg &msg = *p;
        string    key = msg.GetKey();

        //string valueOfKey;
        double d_valueOfKey;

        if(key == "YANG_WPT_INDEX")
        {
            d_valueOfKey = msg.GetDouble();

            MOOSTrace("vechile: yang  |MOOS varible: 'YANG_WPT_INDEX'  |value: %f.\n", d_valueOfKey);
            Notify("YANG_WPT_CURRENT_INDEX", d_valueOfKey);
        }
        else if(key == "BETTY_WPT_INDEX")
        {
            d_valueOfKey = msg.GetDouble();

            MOOSTrace("vechile: betty |MOOS varible: 'BETTY_WPT_INDEX' |value: %f.\n", d_valueOfKey);
            Notify("BETTY_WPT_CURRENT_INDEX", d_valueOfKey);
        }
        else if(key == "ARCHIE_WPT_INDEX")
        {
            d_valueOfKey = msg.GetDouble();

            MOOSTrace("vechile: archie|MOOS varible: 'ARCHIE_WPT_INDEX'|value: %f.\n", d_valueOfKey);
            Notify("ARCHIE_WPT_CURRENT_INDEX", d_valueOfKey);
        }
    }

    AppCastingMOOSApp::PostReport();
    return true;
}

bool CGlobalInformation::OnConnectToServer()
{
    registerVariables();
    return true;
}

bool CGlobalInformation::Iterate()
{
    return true;
}

bool CGlobalInformation::OnStartUp()
{
    AppCastingMOOSApp::OnStartUp();

    registerVariables();

    // 尝试使用中文，但是会乱码
    MOOSTrace("********************\n");
    MOOSTrace("GloableInfomation application. By yang haizhi.\n");
    cout << "community: " << m_host_community << endl;
    MOOSTrace("********************\n");

    return true;
}

void CGlobalInformation::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();

  m_Comms.Register("FAULT", 0);
  //m_Comms.Register("WPT_INDEX", 0);
  m_Comms.Register("YANG_WPT_INDEX", 0);
  m_Comms.Register("BETTY_WPT_INDEX", 0);
  m_Comms.Register("ARCHIE_WPT_INDEX", 0);
}
