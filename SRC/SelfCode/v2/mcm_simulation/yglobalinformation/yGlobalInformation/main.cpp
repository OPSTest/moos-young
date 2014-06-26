#include "GlobalInformation.h"

int main(int argc, char* argv[])
{
    const char* sMissionFile = "yGlobalInformation.moos";
    
    const char* sMOOSName = "GlobalInformation";

    switch(argc)
    {
    case 3:
        sMOOSName = argv[2];

    case 2:
	sMissionFile = argv[1];

    }

    CGlobalInformation TheApp;

    TheApp.Run(sMOOSName, sMissionFile);

    return 0;
}
