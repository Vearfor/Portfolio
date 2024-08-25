/*========================================================================*\
|* cMonitor.h
\*========================================================================*/


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "cVecModosGraficos.h"
#include "../tool/cItem.h"
#include <string>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Ampliamos  RECT
//--------------------------------------------------------------------------
class cRect : public RECT
{
public:

    cRect(void);
    ~cRect(void);

    long getAlto(void);
    long getAncho(void);
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// cMonitor
//--------------------------------------------------------------------------
class cMonitor : public cItem
{
    static  std::vector<cMonitor>* m_pvMonitor;
    static  int     m_iNumMonitors;
    static  bool    m_bFullMonitor;

    std::vector<cModoGrafico> m_vModo;

public:

    cMonitor(void);
    ~cMonitor(void);

    DISPLAY_DEVICE m_tDisplayDevice;
    cRect m_oRect;
    std::string m_sDisplayName;
    std::string m_sDeviceString;

    void resetDisplayDevice(void);
    void insertarModo(cModoGrafico modo);
    bool buscaDevMode(DEVMODE  * lpDevSettings);

    //----------------------------------------------------------------------
    static  int     getNumMonitors(void);
    static  void    setFullMonitor(bool bFull);
    static  bool    esFullMonitor(void);
    static  void    getInfoMonitors(void);
    static  void    delInfoMonitors(void);
    static const cMonitor* getMonitor(int iMonitor);
    static  void    release();
    //----------------------------------------------------------------------
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cMonitor.h
\*========================================================================*/

