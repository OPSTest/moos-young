#include "Simulator.h"

int main(int argc, char* argv[])
{
    const char* sMissionFile = "ex4.moos";
    
    const char* sMOOSName = "Simulator";

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
