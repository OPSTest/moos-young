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
    //首先在moos文件中得到航行器名字
    m_sVehicleName = "UnNamed";
    if( !m_MissionReader.GetConfigurationParam("VehicleName", m_sVehicleName) )
    {
	MOOSTrace("Warning parameter \"VehicleName\" not specified. Using default of \"%s\" \n", m_sVehicleName.c_str());
    }
    else
    {
        MOOSTrace("Found vehivcle name in ex4.moos,it is -- %s\n", m_sVehicleName.c_str());
    }

    //在moos文件中得到位置信息
    std::vector<double> vInitialLocation(3, 0.0);
    int nRows = vInitialLocation.size();
    int nCols = 1;
    if( !m_MissionReader.GetConfigurationParam("InitialLocation", vInitialLocation, nRows, nCols) )
    {
	MOOSTrace("Warning parameter \"InitialLocation\" not specified. Using default of \"%s\"\n", DoubleVector2String(vInitialLocation).c_str() );
    }

    std::string sComplex;
    if( m_MissionReader.GetConfigurationParam("InitialConditions", sComplex))
    {
	m_sBilge = "off";
	MOOSValFromString( m_sBilge, sComplex, "Bilge");
	
	m_dfBatteryVoltage = 100.0;
	MOOSValFromString(m_dfBatteryVoltage, sComplex, "BatteryVoltage");

	m_dfHeading = 0;
	MOOSValFromString(m_dfHeading, sComplex, "Heading");
    }
    else
    {
	return MOOSFail("no \"InitialConditions\" specified in mission file (compulsory)\n");
    }
    
    MOOSTrace("Verbose Summary:\n");
    MOOSTrace("\tVehicle is called : %s\n", m_sVehicleName.c_str());
    MOOSTrace("\tInitial Location is : %s\n", DoubleVector2String(vInitialLocation).c_str());
    MOOSTrace("\tHeading is : %f\n", m_dfHeading);
    MOOSTrace("\tBatteryVoltage is : %f\n", m_dfBatteryVoltage);   
    return true;
}
