/**************************************************************/
/*author : younghz                                            */
/*date   : 14.5.20                                            */
/*project: moos-young/deallocation                            */
/*brief  : 实现航行器与shoreside之间以及航行器与航行器之间的通信 */
/*         具体原理可参见父文件夹下的README文件。               */
/*         每个航行器都向shoreside发送vehiclename_WPT_INDEX数据,*/
/*         然后shoreside将数据内容转发，数据名字变为            */
/*         vehiclename_WPT_INDEX_vehiclename,以实现航行器和航行*/
/*         器之间的通信。                                      */
/**************************************************************/

#ifndef GLOBALINFORMATIONH
#define GLOBALINFORMATIONH

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <vector>
#include <map>
#include <string>

class CGlobalInformation : public AppCastingMOOSApp
{
public:
    CGlobalInformation();
    virtual  ~CGlobalInformation();

    void registerVariables();

    bool OnNewMail(MOOSMSG_LIST& NewMail);
    bool OnConnectToServer();
    bool OnStartUp();
    bool Iterate();

private:
    std::string m_sVehicleName, m_sBilge;
    double m_dfHeading, m_dfBatteryVoltage;

protected:

};

#endif
