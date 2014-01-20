#ifndef SIMULATORH
#define SIMULATORH

#include "MOOS/libMOOS/App/MOOSApp.h"

class CSimulator : public CMOOSApp
{

public:
    CSimulator();
    virtual  ~CSimulator();

protected:
    bool OnNewMail(MOOSMSG_LIST& newmail);
    bool OnConnectToServer();
    bool OnStartUp();
    bool Iterate();
};

#endif 
