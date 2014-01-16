#include "SimpleApp.h"

CSimpleApp::CSimpleApp()
{

}

CSimpleApp::~CSimpleApp()
{
}

bool CSimpleApp::OnNewMail(MOOSMSG_LIST &NewMail)
{
    return true;
}

bool CSimpleApp::OnConnectToServer()
{
    return true;
}

bool CSimpleApp::Iterate()
{
    return true;
}

bool CSimpleApp::OnStartUp()
{
    return true;
}
