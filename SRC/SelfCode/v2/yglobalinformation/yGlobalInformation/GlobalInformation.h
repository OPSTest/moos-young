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
