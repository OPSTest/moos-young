#ifndef SIMULATORH
#define SIMULATORH

#include "MOOS/libMOOS/App/MOOSApp.h"

class CSimulator : public CMOOSApp
{

public:
    CSimulator();
    virtual  ~CSimulator();

private:
    std::string m_sVehicleName, m_sBilge;
    double m_dfHeading, m_dfBatteryVoltage;
    
protected:
    bool OnNewMail(MOOSMSG_LIST& newmail);
    bool OnConnectToServer();
    bool OnStartUp();
    bool Iterate();
};

#endif 
