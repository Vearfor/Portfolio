/*========================================================================*\
|* cfGestorMensajesVentana.cpp
\*========================================================================*/


#include "cVentana.h"
#include "../Files/cLog.h"


#ifdef _MINGW_
#ifndef _In_
#define _In_
#endif
#endif


//--------------------------------------------------------------------------
// Statics
//--------------------------------------------------------------------------
std::vector<cMonitor> * cVentana::m_pvMonitor = nullptr;
int cVentana::m_iNumMonitors = GetSystemMetrics(SM_CMONITORS);
bool cVentana::m_bFullMonitor = false;
//--------------------------------------------------------------------------


/*========================================================================*\
|* Info MONITORES presentes
\*========================================================================*/
BOOL CALLBACK MonitorEnumProc(
    _In_ HMONITOR hMonitor,
    _In_ HDC      hdcMonitor,
    _In_ LPRECT   lprcMonitor,
    _In_ LPARAM   dwData
);
/*========================================================================*/
void cVentana::getInfoMonitors(void)
{
    // Anchura y Altura del monitor principal
    mDelete(m_pvMonitor);
    m_pvMonitor = new std::vector<cMonitor>();

    cLog::print("\n");
    cLog::print(" Info Monitores Disponibles\n");
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)m_pvMonitor);
}


//--------------------------------------------------------------------------
// Get info monintors, of the system
//--------------------------------------------------------------------------
BOOL CALLBACK MonitorEnumProc(
    _In_ HMONITOR hMonitor,
    _In_ HDC      hdcMonitor,
    _In_ LPRECT   lprcMonitor,
    _In_ LPARAM   dwData
)
{
    cMonitor oMonitor;

    oMonitor.m_oRect.top = lprcMonitor->top;
    oMonitor.m_oRect.left = lprcMonitor->left;
    oMonitor.m_oRect.bottom = lprcMonitor->bottom;
    oMonitor.m_oRect.right = lprcMonitor->right;

    std::vector<cMonitor> * pvMonitor = (std::vector<cMonitor> *) dwData;

    oMonitor.resetDisplayDevice();
    if (EnumDisplayDevices(NULL, (DWORD)pvMonitor->size(), &oMonitor.m_tDisplayDevice, 0))
    {
        oMonitor.m_sDisplayName = oMonitor.m_tDisplayDevice.DeviceName;

        cLog::print("\n");
        cLog::print(" + monitor %2d:  top: %d  left: %d\n",
            pvMonitor->size(),
            oMonitor.m_oRect.top, oMonitor.m_oRect.left);
        cLog::print("   Device Name   : %s\n", oMonitor.m_sDisplayName.Cad());
        cLog::print("   Device String : %s\n", oMonitor.m_tDisplayDevice.DeviceString);
        cLog::print("            alto : %d\n", oMonitor.m_oRect.getAlto());
        cLog::print("            ancho: %d\n", oMonitor.m_oRect.getAncho());

        oMonitor.resetDisplayDevice();

        if (EnumDisplayDevices(oMonitor.m_sDisplayName.Cad(), 0, &oMonitor.m_tDisplayDevice, 0))
        {
            oMonitor.setNombre(oMonitor.m_tDisplayDevice.DeviceName);

            cLog::print("   Monitor Name  : %s\n", oMonitor.getNombre());
            cLog::print("   Monitor String: %s\n", oMonitor.m_tDisplayDevice.DeviceString);
        }

        pvMonitor->push_back(oMonitor);
    }

    return TRUE;
}


/*========================================================================*/
const cMonitor * cVentana::getMonitor(int iMonitor)
{
    if (m_pvMonitor == nullptr)
    {
        getInfoMonitors();
    }

    const cMonitor * poMonitor = NULL;
    if (m_pvMonitor && m_pvMonitor->size()>0)
    {
        poMonitor = (const cMonitor *)& m_pvMonitor->at(iMonitor);
    }
    return poMonitor;
}


void cVentana::delInfoMonitors(void)
{
    mDelete(m_pvMonitor);
}


int cVentana::getNumMonitors(void)
{
    return m_iNumMonitors;
}


//--------------------------------------------------------------------------
// Establecer si se utiliza la resolucion completa del monitor
// si nos ponemos en pantalla completa
//--------------------------------------------------------------------------
void cVentana::setFullMonitor (bool bFull)
{
    m_bFullMonitor = bFull;
}


bool cVentana::esFullMonitor (void)
{
    return m_bFullMonitor;
}


