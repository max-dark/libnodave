
#include "config.h"

#ifdef HAVE_S7ONLINE

#include "openS7online.h"
#include "log2.h"
#ifdef OS_WINDOWS
#include <windows.h>
#include <stdio.h>
#endif

/*
    Access S7onlinx.dll and load pointers to the functions. 
    We load them using GetProcAddress on their names because:
    1. We have no .lib file for them.
    2. We don't want to link with a particular version.
    3. Libnodave shall remain useable without Siemens .dlls. So it shall not try to access them
       unless the user chooses the daveProtoS7online.
*/

typedef int (CALL_SPEC * _setHWnd) (int, HWND);

EXPORTSPEC HANDLE CALL_SPEC openS7online(const char * accessPoint, HWND handle) {
    HMODULE hmod;
    int h,en;
	_setHWnd SetSinecHWnd;
	int daveDebug;

	daveDebug = daveGetDebug();

    hmod=LoadLibrary("S7onlinx.dll");
    if (daveDebug & daveDebugOpen)
	LOG2("LoadLibrary(S7onlinx.dll) returned %p)\n",hmod);

    SCP_open=GetProcAddress(hmod,"SCP_open");
    if (daveDebug & daveDebugOpen)
    	LOG2("GetProcAddress returned %p)\n",SCP_open);

    SCP_close=GetProcAddress(hmod,"SCP_close");
    if (daveDebug & daveDebugOpen)
	LOG2("GetProcAddress returned %p)\n",SCP_close);

    SCP_get_errno=GetProcAddress(hmod,"SCP_get_errno");
    if (daveDebug & daveDebugOpen)
	LOG2("GetProcAddress returned %p)\n",SCP_get_errno);

    SCP_send=GetProcAddress(hmod,"SCP_send");
    if (daveDebug & daveDebugOpen)
	LOG2("GetProcAddress returned %p)\n",SCP_send);

    SCP_receive=GetProcAddress(hmod,"SCP_receive");
    if (daveDebug & daveDebugOpen)
	LOG2("GetProcAddress returned %p)\n",SCP_receive);
    
    SetSinecHWnd=GetProcAddress(hmod,"SetSinecHWnd");
    if (daveDebug & daveDebugOpen)
	LOG2("GetProcAddress returned %p)\n",SetSinecHWnd);

    en=SCP_get_errno();
    h=SCP_open(accessPoint);
    en=SCP_get_errno();
    LOG3("handle: %d  error:%d\n", h, en);
	SetSinecHWnd(h, handle);
    return h;
};
    
EXPORTSPEC HANDLE CALL_SPEC closeS7online(int h) {
    return SCP_close(h);
}

#endif // HAVE_S7ONLINE
/*
    01/09/07  Used Axel Kinting's version.

	2018-02-16	extern int daveDebug -> daveGetDebug()
*/
