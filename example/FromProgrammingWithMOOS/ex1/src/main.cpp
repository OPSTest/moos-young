#include "SimpleApp.h"

int main(int argc, char* argv[])
{
    const char* sMissionFile = "Mission.moos";
    
    const char* sMOOSName = "MyMOOSApp";

    switch(argc)
    {
    case 3:
        sMOOSName = argv[2];

    case 2:
	sMissionFile = argv[1];

    }

    CSimpleApp TheApp;

    TheApp.Run(sMOOSName, sMissionFile);

    return 0;
}
