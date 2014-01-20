#ifndef SIMPLEAPPH
#define SIMPLEAPPH

#include "MOOS/libMOOS/App/MOOSApp.h"

class CSimpleApp : public CMOOSApp
{
public:
    CSimpleApp();
    virtual ~CSimpleApp();

protected:
    bool OnNewMail( MOOSMSG_LIST &NewMail );
    bool Iterate();
    bool OnConnectToServer();
    bool OnStartUp();


    // state your instrested in variables
    void DoRegistrations();

    // we will call this if/when we receive vehicle status message
    bool OnVehicleStatus(CMOOSMsg& msg);

    // we will call this if/when we receive heading message
    bool OnHeading(CMOOSMsg& msg);
    

};

#endif
