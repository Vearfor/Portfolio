/*========================================================================*\
|* cMonitor.cpp
\*========================================================================*/


#include "cMonitor.h"
#include "../../tool/cLog.h"
#include "../../tool/cTool.h"


//--------------------------------------------------------------------------
// Statics
//--------------------------------------------------------------------------
std::vector<cMonitor>* cMonitor::m_pvMonitor = nullptr;
int cMonitor::m_iNumMonitors = GetSystemMetrics(SM_CMONITORS);
bool cMonitor::m_bFullMonitor = false;
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cMonitor::cMonitor(void)
    : m_vModo()
{
    resetDisplayDevice();
}


cMonitor::~cMonitor(void)
{
}


//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
void cMonitor::release()
{
    if (m_pvMonitor)
    {
        m_pvMonitor->clear();
        delete m_pvMonitor;
        m_pvMonitor = nullptr;
    }
}


void cMonitor::resetDisplayDevice(void)
{
    memset(&m_tDisplayDevice, 0, sizeof(DISPLAY_DEVICE));
    m_tDisplayDevice.cb = sizeof(DISPLAY_DEVICE);
}



void cMonitor::insertarModo(cModoGrafico modo)
{
    m_vModo.push_back(modo);
}


bool cMonitor::buscaDevMode(DEVMODE  * lpDevSettings)
{
    int iBpp = lpDevSettings->dmBitsPerPel;
    int iWidth = lpDevSettings->dmPelsWidth;
    int iHeight = lpDevSettings->dmPelsHeight;

    bool bFound = false;
    int iNum = (int) m_vModo.size();
    for (int i=0; i<iNum && !bFound; i++)
    {
        cModoGrafico modo = m_vModo[i];
        if (modo.m_tDevMode.dmBitsPerPel == iBpp && modo.m_tDevMode.dmPelsWidth == iWidth && modo.m_tDevMode.dmPelsHeight == iHeight)
        {
            *lpDevSettings = modo.m_tDevMode;
            bFound = true;
        }
    }

    return bFound;
}


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
void cMonitor::getInfoMonitors(void)
{
    // Anchura y Altura del monitor principal
    // mDelete(m_pvMonitor);
    if (!m_pvMonitor)
    {
        m_pvMonitor = new std::vector<cMonitor>();
        cLog::print("\n");
        cLog::print(" Info Monitores Disponibles\n");
        EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)m_pvMonitor);
    }
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

    std::vector<cMonitor>* pvMonitor = (std::vector<cMonitor> *) dwData;

    oMonitor.resetDisplayDevice();
    if (EnumDisplayDevices(NULL, (DWORD)pvMonitor->size(), &oMonitor.m_tDisplayDevice, 0))
    {
        cTool::copiaUnicodeToMultibyte(oMonitor.m_tDisplayDevice.DeviceName, oMonitor.m_sDisplayName);
        cTool::copiaUnicodeToMultibyte(oMonitor.m_tDisplayDevice.DeviceString, oMonitor.m_sDeviceString);

        cLog::print("\n");
        cLog::print(" + monitor %2d:  top: %d  left: %d\n",
            pvMonitor->size(),
            oMonitor.m_oRect.top, oMonitor.m_oRect.left);
        cLog::print("   Device Name   : %s\n", oMonitor.m_sDisplayName.c_str());
        cLog::print("   Device String : %s\n", oMonitor.m_sDeviceString.c_str());
        cLog::print("            alto : %d\n", oMonitor.m_oRect.getAlto());
        cLog::print("            ancho: %d\n", oMonitor.m_oRect.getAncho());

        oMonitor.resetDisplayDevice();

        if (EnumDisplayDevices((LPCWSTR) oMonitor.m_sDisplayName.c_str(), 0, &oMonitor.m_tDisplayDevice, 0))
        {
            oMonitor.setNombre(oMonitor.m_sDisplayName.c_str());

            cLog::print("   Monitor Name  : %s\n", oMonitor.getNombre());
            cLog::print("   Monitor String: %s\n", oMonitor.m_sDeviceString.c_str());
        }

        pvMonitor->push_back(oMonitor);
    }

    return TRUE;
}


/*========================================================================*/
const cMonitor* cMonitor::getMonitor(int iMonitor)
{
    if (m_pvMonitor == nullptr)
    {
        getInfoMonitors();
    }

    const cMonitor* poMonitor = NULL;
    if (m_pvMonitor && m_pvMonitor->size() > 0)
    {
        poMonitor = (const cMonitor*)&m_pvMonitor->at(iMonitor);
    }
    return poMonitor;
}


void cMonitor::delInfoMonitors(void)
{
    mDelete(m_pvMonitor);
}


int cMonitor::getNumMonitors(void)
{
    return m_iNumMonitors;
}


//--------------------------------------------------------------------------
// Establecer si se utiliza la resolucion completa del monitor
// si nos ponemos en pantalla completa
//--------------------------------------------------------------------------
void cMonitor::setFullMonitor(bool bFull)
{
    m_bFullMonitor = bFull;
}


bool cMonitor::esFullMonitor(void)
{
    return m_bFullMonitor;
}


/*========================================================================*\
|* Fin de cMonitor.cpp
\*========================================================================*/
