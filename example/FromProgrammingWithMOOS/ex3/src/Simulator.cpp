#include "Simulator.h"
#include <math.h>

CSimulator::CSimulator()
{
}

CSimulator::~CSimulator()
{
}

bool CSimulator::OnNewMail(MOOSMSG_LIST &newmail)
{
    return true;
}

bool CSimulator::OnConnectToServer()
{
    return true;
}

bool CSimulator::Iterate()
{
    static int k = 0;
    if( k++ % 10 == 0 )
    {
	static double dfHeading = 0;
	dfHeading += MOOSWhiteNoise(0.1);
	
	//publish data
	m_Comms.Notify("Heading", dfHeading, MOOSTime());
    }

    if( k % 35 ==0  )
    {
	static double dfVolts = 100;
	dfVolts -= fabs(MOOSWhiteNoise(0.1));
	std::string sStatus = MOOSFormat("Status = %s, BatteryVoltage = %.2f, Bilge = %s", 
	dfVolts>50.0 ? "Good": "Bad",
	dfVolts,
	k%100? "On": "Off");
    

        m_Comms.Notify("VehicleStatus", sStatus, MOOSTime());
    }

    return true;

}

bool CSimulator::OnStartUp()
{
    return true;
}
