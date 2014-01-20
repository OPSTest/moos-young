#include "Simulator.h"

int main(int argc, char* argv[])
{
    const char* sMissionFile = "Mission.moos";
    
    const char* sMOOSName = "simulatorapp";

    switch(argc)
    {
    case 3:
        sMOOSName = argv[2];

    case 2:
	sMissionFile = argv[1];

    }

    CSimulator TheApp;

    TheApp.Run(sMOOSName, sMissionFile);

    return 0;
}
