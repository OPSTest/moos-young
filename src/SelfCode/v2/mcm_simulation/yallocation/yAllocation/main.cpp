#include "Allocation.h"

int main(int argc, char* argv[])
{
    const char* sMissionFile = "yAllocation.moos";

    const char* sMOOSName = "Allocation";

    switch(argc)
    {
    case 3:
        sMOOSName = argv[2];

    case 2:
        sMissionFile = argv[1];

    }

    CAllocation TheApp;
    TheApp.Run(sMOOSName, sMissionFile);

    return 0;
}
