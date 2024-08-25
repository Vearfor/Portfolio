/*------------------------------------------------------------------------*\
|* sWindow.cpp
\*------------------------------------------------------------------------*/

#include "sWindow.h"
#include "sOpenGL.h"
#include "../tool/cTool.h"
#include "../tool/cLog.h"
#include "../tool/consola/cConsola.h"


//--------------------------------------------------------------------------
// Statics
//--------------------------------------------------------------------------
sItem sWindow::m_vTipoEstilosVentana[] =
{
    { static_cast<int>(eEstiloW::eMain)     , "eMain"       , NULL },		// Ventana principal: su estilo es popup.
    { static_cast<int>(eEstiloW::eControl)  , "eControl"    , NULL },		// Estilo 'Pop-up', pero en un control.
    { static_cast<int>(eEstiloW::ePopup)    , "ePopUp"      , NULL },		// Estilo 'Pop-up'.
    { static_cast<int>(eEstiloW::eCaption)  , "eCaption"    , NULL },		// Con nombre en el caption.
    { static_cast<int>(eEstiloW::eSystem)   , "eSystem"     , NULL },		// Con boton de cierre de sistema.
    { static_cast<int>(eEstiloW::eWindow)   , "eWindow"     , NULL },		// Con todo.
    { static_cast<int>(eEstiloW::eMainV)    , "eMainV"      , NULL },		// Ventana principal: Virtual
    { static_cast<int>(eEstiloW::eControlV) , "eControlV"   , NULL },		// Estilo 'Pop-up', Virtual
};
//--------------------------------------------------------------------------
sWindow* sWindow::m_pThis = nullptr;
WNDPROC sWindow::m_pFuncion = (WNDPROC) sWindow::windowProc;
int sWindow::m_iNumWindows = 0;                                 // Numero de ventanas
HGLRC sWindow::m_hRcmain;                                       // Hd. contexto de generacion del hDC: principal para toda la aplicacion.
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Resultados cambio dinamico de la pantalla
//--------------------------------------------------------------------------
sItem vtChangeDisplaySettingsEx[]
{
    { DISP_CHANGE_SUCCESSFUL    , "El cambio de configuración fue exitoso."                         , nullptr },
    { DISP_CHANGE_BADDUALVIEW   , "El sistema es compatible con DualView."                          , nullptr },
    { DISP_CHANGE_BADFLAGS      , "Se pasó un conjunto de banderas no válido."                      , nullptr },
    { DISP_CHANGE_BADMODE       , "El modo de gráficos no es compatible."                           , nullptr },
    { DISP_CHANGE_BADPARAM      , "Se pasó un parámetro no válido."                                 , nullptr },
    { DISP_CHANGE_FAILED        , "El controlador de pantalla falló en el modo de gráficos dado."   , nullptr },
    { DISP_CHANGE_NOTUPDATED    , "No se puede escribir la configuración en el registro."           , nullptr },
    { DISP_CHANGE_RESTART       , "Debe reiniciar el ordenador para que funcione el nuevo modo."    , nullptr },
};
//--------------------------------------------------------------------------


/*------------------------------------------------------------------------*/
sWindow::sWindow()
{
    m_pThis = this;
}

sWindow::~sWindow()
{
}
/*------------------------------------------------------------------------*/


int sWindow::crea(
    int     iLeftP,
    int	    iTopP,
    int	    iAnchoP,
    int	    iAltoP,
    double dCercano,
    double dLejano,
    double dFov,
    int iBitsColor,
    int iBitsDepth,
    bool bFull,
    eEstiloW estilo,
    const char* pcWindowName,
    const char* pcClassName,
    WNDPROC	pFuncion)
{
    if (m_hWindow)
        return 0;

    strcpy_s(m_tVentana.vcWindowName, sizeof(m_tVentana.vcWindowName), pcWindowName);
    strcpy_s(m_tVentana.vcClassName, sizeof(m_tVentana.vcClassName), pcClassName);

    m_iBitsColor = iBitsColor;
    m_iBitsDepth = iBitsDepth;
    m_dFov = dFov;
    m_dPCercano = dCercano;
    m_dPLejano = dLejano;

    m_tVentana.hInstancia = cConsola::getInstance();
    m_tVentana.eType = estilo;
    m_tVentana.dwEstilo = 0;
    m_tVentana.iAncho = iAnchoP;
    m_tVentana.iAlto = iAltoP;

    m_tVentana.bFull = bFull;
    if (m_tVentana.eType == eEstiloW::eMain || m_tVentana.eType == eEstiloW::eMainV)
    {
        m_tVentana.bFull = true;
    }

    int iMonitor;
    int iTop, iLeft;
    int iAnchoW, iAltoW;

    iMonitor = ((m_tVentana.eType == eEstiloW::eMainV || m_tVentana.eType == eEstiloW::eControlV) && cMonitor::getNumMonitors() > 1) ? 1 : 0;
    cMonitor* poMonitor = (cMonitor*) cMonitor::getMonitor(iMonitor);
    miNulo(poMonitor);

    iAnchoW = poMonitor->m_oRect.getAncho();
    iAltoW = poMonitor->m_oRect.getAlto();
    iTop = poMonitor->m_oRect.top;
    iLeft = poMonitor->m_oRect.left;

    if (m_tVentana.eType == eEstiloW::eControl || m_tVentana.eType == eEstiloW::eControlV)
    {
        switch (iLeftP)
        {
            case (int) ePosControl::eRIGHT:     iLeft += iAnchoW - iAnchoP;       break;
            case (int) ePosControl::eCENTER:	iLeft += (iAnchoW - iAnchoP) / 2; break;
            case (int) ePosControl::eLEFT:		iLeft += 0;                       break;
        }
        switch (iTopP)
        {
            case (int) ePosControl::eTOP:		iTop += 0;                      break;
            case (int) ePosControl::eCENTER:	iTop += (iAltoW - iAltoP) / 2;  break;
            case (int) ePosControl::eBOT:		iTop += iAltoW - iAltoP;        break;
        }
    }

    cRect   tRect;
    tRect.top = iTop;
    tRect.left = iLeft;
    tRect.bottom = iAltoP + iTop;
    tRect.right = iAnchoP + iLeft;
    m_tVentana.tRect = tRect;

    if (pFuncion)
        m_pFuncion = pFuncion;

    miError(registra(m_tVentana.hInstancia, m_tVentana.vcClassName, m_pFuncion, m_tVentana.eType) || crea(&m_tVentana));

    return 0;
}


//--------------------------------------------------------------------------
// Registro de ventanas: Lo que nosotros pondremos como minimo
//--------------------------------------------------------------------------
int sWindow::registra(HINSTANCE hInstancia, const char* pcClassName, WNDPROC pFuncion, eEstiloW eEstilo)
{
    if (!m_bRegistrada)
    {
        if (!pFuncion)									// Intentamos registrar la clase
        {
            cLog::error(" Error: sWindow::registra. No hay funcion de mensajes: WNDPROC\n");
            return -1;
        }

        WNDCLASS	wc;						// Estructura de la clase ventana (Windows Class Structure)
        WCHAR wcClassName[LON_BUFF / 8];    // 1024/8 = 128: suponemos espacio suficiente:

        cTool::copiaMultibyteToUnicode(std::string(pcClassName), wcClassName, sizeof(wcClassName));

        if (!(::GetClassInfo(hInstancia, wcClassName, &wc)))
        {
            DWORD dwStyle;

            //------------------------------------------------------------------
            dwStyle = CS_OWNDC;
            if (eEstilo != eEstiloW::eControl && eEstilo != eEstiloW::eControlV)
            {
                dwStyle |= CS_HREDRAW | CS_VREDRAW;     // Redraw On Size, And Own DC For Window.
            }
            //------------------------------------------------------------------
            // Creacion/Registro de la clase de ventana
            //------------------------------------------------------------------
            wc.style = dwStyle;
            wc.lpfnWndProc = pFuncion;                          // Manejador de mensajes WndProc
            wc.cbClsExtra = 0;									// No Extra Class Data
            wc.cbWndExtra = 0;									// No Extra Window Data
            wc.hInstance = hInstancia;						    // Establece la instancia
            wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			    // Carga del Icono por defecto
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Carga del puntero de flecha
            wc.hbrBackground = NULL;							// No hace falta un fondo para GL
            wc.lpszMenuName = NULL;								// No queremos un menú
            wc.lpszClassName = wcClassName;                     // Establece el nombre de la clase
            //----------------------------------------------------------------------

            if (!RegisterClass(&wc))							// Intentamos registrar la clase
            {
                cLog::error(" Error: sWindow::registra. Al registrar con 'RegisterClass' la clase ventana de la aplicacion\n");
                return -1;
            }
        }
        else
        {
            cLog::traza( cLog::eTraza::nor, " Clase registrada: [%s]\n", pcClassName);
        }

        m_bRegistrada = true;
    }

    return 0;
}


//--------------------------------------------------------------------------
// Funciones privadas:
// - parametros de la ventana.
// Establece los valores de posiciones segun los dados.
//--------------------------------------------------------------------------
int sWindow::setWindowSettings(sCreaVentana* ptVentana, cRect* ptWindowRect)
{
    //------------------------------------------------------------------
    setNombre(ptVentana->vcWindowName);
    //------------------------------------------------------------------
    *ptWindowRect = ptVentana->tRect;
    ptVentana->iAlto = ptWindowRect->getAlto();
    ptVentana->iAncho = ptWindowRect->getAncho();
    ptVentana->x = ptWindowRect->left;                  // Posiciones iniciales     // x = CW_USEDEFAULT;
    ptVentana->y = ptWindowRect->top;					// Posiciones iniciales     // y = CW_USEDEFAULT;
    //------------------------------------------------------------------
    ptVentana->dwExEstilo = WS_OVERLAPPEDWINDOW;        // Para el cambio de estilo en la siguiente.
    //------------------------------------------------------------------
    return 0;
}


int	sWindow::setWindowStyles(sCreaVentana* ptVentana, cRect* ptWindowRect)
{
    //------------------------------------------------------------------
    eEstiloW type = ptVentana->eType;
    switch (type)
    {
        case eEstiloW::eCaption:
            ptVentana->dwExEstilo = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
            ptVentana->dwEstilo = WS_OVERLAPPED | WS_CAPTION;
            break;

        case eEstiloW::eSystem:
            ptVentana->dwExEstilo = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
            ptVentana->dwEstilo = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
            break;

        case eEstiloW::eWindow:
            ptVentana->dwExEstilo = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
            ptVentana->dwEstilo = WS_POPUP | WS_OVERLAPPEDWINDOW;
            break;

        case eEstiloW::eControl:
        case eEstiloW::eControlV:
            ptVentana->dwExEstilo = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
            ptVentana->dwEstilo = WS_POPUP;
            break;

        case eEstiloW::eMain:
        case eEstiloW::eMainV:
        case eEstiloW::ePopup:
            {
                ptVentana->dwExEstilo = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
                ptVentana->dwEstilo = WS_POPUP;

                int iMonitor = (ptVentana->eType == eEstiloW::eMainV && cMonitor::getNumMonitors() > 1) ? 1 : 0;
                cMonitor* poMonitor = (cMonitor*) cMonitor::getMonitor(iMonitor);
                miNulo(poMonitor);

                int iAncho = poMonitor->m_oRect.getAncho();
                int iAlto = poMonitor->m_oRect.getAlto();

                ptVentana->x = poMonitor->m_oRect.left + (iAncho / 2) - (ptVentana->iAncho / 2);
                ptVentana->y = poMonitor->m_oRect.top + (iAlto / 2) - (ptVentana->iAlto / 2);
            }
            break;

        default:
            break;
    }
    //------------------------------------------------------------------
    if (type != eEstiloW::eControl)
    {
        int		iError;
        // Ajustamos la ventana al tamaño solicitado
        if (!AdjustWindowRectEx(ptWindowRect, ptVentana->dwEstilo, false, ptVentana->dwExEstilo))
        {
            iError = GetLastError();
            if (iError && iError != 2)
            {
                // ToDo oError
                // return oError.mensaje(dERR, "Ajuste de la ventana.");
            }
            cLog::error(" Error: sWindow::setWindowStyles: ajuste de la ventana: AdjustWindowRectEx: iError: %d\n", iError);
            return -1;
        }
    }

    //------------------------------------------------------------------
    // Investigamos lo de las ventanas transparentes ::
    //------------------------------------------------------------------
    //------------------------------------------------------------------

    return 0;
}


//--------------------------------------------------------------------------
// Creacion de ventanas
//--------------------------------------------------------------------------
int sWindow::crea(void* pEstructuraVentana)
{
    sCreaVentana* pCreaVentana = static_cast<sCreaVentana*>(pEstructuraVentana);

    cRect tWindowRect;

    miError(
        setWindowSettings(pCreaVentana, &tWindowRect) ||
        setWindowStyles(pCreaVentana, &tWindowRect)
    );

    WCHAR wcClassName[LON_BUFF / 8];
    WCHAR wcWindowName[LON_BUFF / 8];

    cTool::copiaMultibyteToUnicode(pCreaVentana->vcClassName, wcClassName, sizeof(wcClassName));
    cTool::copiaMultibyteToUnicode(pCreaVentana->vcWindowName, wcWindowName, sizeof(wcWindowName));

    m_hWindow = ::CreateWindow(
        wcClassName,
        wcWindowName,
        pCreaVentana->dwEstilo | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        pCreaVentana->x,
        pCreaVentana->y,				// Posición de la ventana
        pCreaVentana->iAncho,			// Ancho de la ventana
        pCreaVentana->iAlto,			// Alto de la ventana
        pCreaVentana->hWndPadre,
        pCreaVentana->hMenu,
        pCreaVentana->hInstancia,
        pCreaVentana->ptCreate);

    if (!m_hWindow)
    {
        cLog::error(" Error: sWindow::crea: CreateWindowXX No ha creado la Ventana\n");
        return -1;
    }

    GetWindowRect(m_hWindow, &m_wCurrentRect);

    enPantallaCompleta();

    return 0;
}


/*------------------------------------------------------------------------*\
|* Metodos de tareas concretas a realizar
\*------------------------------------------------------------------------*/
int sWindow::creaDeviceContext(HWND hWindow)
{
    if (hWindow)
    {
        m_hDC = GetDC(hWindow);
        if (m_hDC == NULL)
        {
            cLog::error(" Error: sWindow::creaDeviceContext: no hay Device Context\n");
            return -1;
        }
    }

    return 0;
}


int sWindow::destruyeDeviceContext()
{
    if (m_hWindow && m_hDC)
    {
        if (!ReleaseDC(m_hWindow, m_hDC))
        {
            cLog::error(" Error: sWindow::destruyeDeviceContext: error en ReleaseDC\n");
            return -1;
        }
        m_hDC = NULL;
    }

    return 0;
}


//--------------------------------------------------------------------------
// Establece el pixel format
//--------------------------------------------------------------------------
int sWindow::creaPixelFormat(HDC hdc)
{
    miNulo(hdc);

    uint iPixelFormat;
    //----------------------------------------------------------------------
    // PixelFormatDescriptor
    //----------------------------------------------------------------------
    // Esto es antiguo !!! y no queda por aprender ... !!!
    //----------------------------------------------------------------------
    m_pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);  // Tamaño del descriptor del formato de pixel
    m_pfd.nVersion = 1;                     // Numero de version
    m_pfd.dwFlags =
        PFD_DRAW_TO_WINDOW |                // Debe soportar ventanas
        PFD_SUPPORT_OPENGL |                // Debe soportar OpenGL
        PFD_DOUBLEBUFFER;                   // Soporta doble buffer
    m_pfd.iPixelType = PFD_TYPE_RGBA;		// Formato de color RGBA
    m_pfd.cColorBits = m_iBitsColor;        // Profundidad de color
    m_pfd.cRedBits = 0;				        // 
    m_pfd.cRedShift = 0;                    //
    m_pfd.cGreenBits = 0;				    //
    m_pfd.cGreenShift = 0;				    //
    m_pfd.cBlueBits = 0;				    //
    m_pfd.cBlueShift = 0;				    //
    m_pfd.cAlphaBits = 0;				    // No queremos buffer alpha (ya veremos)
    m_pfd.cAlphaShift = 0;				    // Ignoramos Shift Bit
    m_pfd.cAccumBits = 0;				    // No queremos buffer de acumulacion (ya veremos)
    m_pfd.cAccumRedBits = 0;				//
    m_pfd.cAccumGreenBits = 0;				//	Por lo que ignoramos estos bits
    m_pfd.cAccumBlueBits = 0;				//
    m_pfd.cAccumAlphaBits = 0;				//
    m_pfd.cDepthBits = m_iBitsDepth;		// Buffer de profundidad de 16 bits
    m_pfd.cStencilBits = 0;				    // No usamos el buffer de pincel ??? (estarcido)
    m_pfd.cAuxBuffers = 0;				    // No usamos buffers auxiliares
    m_pfd.iLayerType = PFD_MAIN_PLANE;      // Dibuja en el plano principal ???
    m_pfd.bReserved = 0;				    // Reservado
    m_pfd.dwLayerMask = 0;				    // Se ignoran niveles de mascara ???
    m_pfd.dwVisibleMask = 0;
    m_pfd.dwDamageMask = 0;
    //----------------------------------------------------------------------
    iPixelFormat = ChoosePixelFormat(hdc, &m_pfd);
    //----------------------------------------------------------------------
    if (!iPixelFormat)	// Hemos obtenido el formato de pixel?
    {
        cLog::error(" Error: sWindow::creaPixelFormat:\n");
        cLog::error("        ChoosePixelFormat: No podemos encontrar un formato de pixel aceptable.\n");
        cLog::error("                           Hay que cambiar los parametros.\n");
        return -1;
    }
    //----------------------------------------------------------------------
    // ¿Podemos establecer el formato de pixel?
    //----------------------------------------------------------------------
    if (!SetPixelFormat(hdc, iPixelFormat, &m_pfd))
    {
        cLog::error(" Error: sWindow::creaPixelFormat:\n");
        cLog::error("        SetPixelFormat: No se puede establacer el formato de Pixel: %d.\n", iPixelFormat);
        return -1;
    }
    //----------------------------------------------------------------------
    iPixelFormat = GetPixelFormat(hdc);
    if (!iPixelFormat)
    {
        cLog::error(" Error: sWindow::creaPixelFormat:\n");
        cLog::error("        GetPixelFormat: No podemos obtener el formato de pixel del Contexto.\n");
        return -1;
    }
    m_iPixelFormat = iPixelFormat;
    iPixelFormat = DescribePixelFormat(hdc, m_iPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &m_pfd);
    if (!iPixelFormat)
    {
        cLog::error(" Error: sWindow::creaPixelFormat:\n");
        cLog::error("        DescribePixelFormat: No podemos obtener la informacion del formato de pixel\n");
        return -1;
    }
    //----------------------------------------------------------------------
    if (cLog::siTraza(cLog::eTraza::nor))
    {
        cLog::print("   * Pixelformat: %d\n", m_iPixelFormat);
        cLog::print("     cColorBits: %d\n", m_pfd.cColorBits);
        cLog::print("     cDepthBits: %d\n", m_pfd.cDepthBits);
        cLog::print("     Maximo Pixelformat: %d\n", iPixelFormat);
    }
    //----------------------------------------------------------------------

    return 0;
}


int sWindow::creaRenderContext(HDC hdc)
{
    if (hdc)
    {
        //------------------------------------------------------------------
        if (!m_hRcmain)
        {
            // El principal, que luego se comparte ...
            m_hRcmain = wglCreateContext(hdc);
            if (!m_hRcmain)			// Podemos obtener un contexto de renderización?
            {
                cLog::error(" Error: sWindow::creaRenderContext:\n");
                cLog::error("        wglCreateContext: No se puede crear el GL Rendering Context Principal.\n");
                return -1;
            }
        }
        //------------------------------------------------------------------
        m_hRc = wglCreateContext(hdc);
        if (!m_hRc)			// Podemos obtener un contexto de renderización?
        {
            cLog::error(" Error: sWindow::creaRenderContext:\n");
            cLog::error("        wglCreateContext: No se puede crear un GL Rendering Context. (no principal)\n");
            return -1;
        }
        //------------------------------------------------------------------
        // El contexto de renderizacion principal se comparte con el resto
        // de contexto de renderizacion de las ventanas siguientes:
        // - Con esto conseguimos que las listas y las texturas solo se
        //   cargan una vez por ventana.
        //------------------------------------------------------------------
        if (m_hRcmain)
        {
            if (!wglShareLists(m_hRcmain, m_hRc))
            {
                cLog::error(" Error: sWindow::creaRenderContext:\n");
                cLog::error("        wglCreateContext: No se puede compartir el GL Rendering Context.\n");
                return -1;
            }
            m_iNumWindows++;	// Con contexto de renderizacion compartido.
        }
        //------------------------------------------------------------------
    }
    return 0;
}


//--------------------------------------------------------------------------
// Para un dispositivo dado establece cual es el que esta en curso.
//--------------------------------------------------------------------------
int sWindow::setRenderContext(bool bSinContexto)
{
    if (bSinContexto)
        return 0;
    return setRenderContext(m_hDC, m_hRc);
}


//--------------------------------------------------------------------------
// Quita el render del contexto actual
//--------------------------------------------------------------------------
int sWindow::unSetRenderContext(bool bSinContexto)
{
    if (bSinContexto)
        return 0;
    return setRenderContext(m_hDC, NULL);
}


int sWindow::setRenderContext(HDC hDC, HGLRC hRC)
{
    //----------------------------------------------------------------------
    if (hDC)
    {
        if (!wglMakeCurrent(hDC, hRC))
        {
            cLog::error(" Error: sWindow::SetRenderContext: wglMakeCurrent: No se puede activar / desactivar el GL Render Context.\n");
            return -1;
        }
    }
    //----------------------------------------------------------------------
    return 0;
}


int	sWindow::destruyeRenderContext(void)
{
    if (m_hWindow)
    {
        if (m_hRc)							// ¿Hay un Rendering Context?
        {
            HGLRC	hRc;

            hRc = wglGetCurrentContext();
            if (m_hRc == hRc)				// Deberia ser el mismo contexto que tenemos, pero puede que no.
            {
                cLog::traza(cLog::eTraza::nor, " Tenemos el contexto actual\n");
            }

            if (m_hDC)
            {
                if (!wglMakeCurrent(m_hDC, NULL))	// ¿Podemos eliminar el DC Context y el RC Context?
                {
                    cLog::error(" Error: sWindow::destruyeRenderContext: wglMakeCurrent: al quitar el current RC del DC.\n");
                    // sera un error pero no salimos, sin return.
                }
            }

            if (!wglDeleteContext(m_hRc))	// ¿Podemos eliminar el RC?
            {
                cLog::error(" Error: sWindow::destruyeRenderContext: wglDeleteContext: al eliminar el RC.\n");
                // sera un error pero no salimos, sin return.
            }
            m_hRc = NULL;

            m_iNumWindows--;	// Con error o sin error esto se disminuye.
            if (m_iNumWindows == 0 && m_hRcmain)		// Si es el ultimo...
            {
                if (!wglDeleteContext(m_hRcmain))
                {
                    cLog::error(" Error: sWindow::destruyeRenderContext: wglDeleteContext: al eliminar el RC principal.\n");
                    // sera un error pero no salimos, sin return.
                }
                m_hRcmain = NULL;
            }
        }
    }
    return 0;
}


//--------------------------------------------------------------------------
// Si es en pantalla completa los estilos estan muy claros.
//--------------------------------------------------------------------------
bool sWindow::enPantallaCompleta()
{
    // Estamos en modo pantalla completa ?
    if (m_tVentana.bFull && m_tVentana.eType != eEstiloW::eControl && m_tVentana.eType != eEstiloW::eControlV)
    {
        memset(&m_dmScreenSettings, 0, sizeof(m_dmScreenSettings));

        setFullDesktop();
        m_tVentana.dwExEstilo |= WS_EX_APPWINDOW;						// Estilo extendido de ventana (Window Extended Style)
        m_tVentana.dwEstilo |= WS_POPUP;								// Estilo de ventana

        return false;	// Si es false ya no continua el ciclo.	(Esto hay que probarlo)
    }

    return true;		//	Devuelve true, porque no es pantalla completa y debe continuar el 'ciclo'.
}


//--------------------------------------------------------------------------
//  Funcion que establece la resolución de la pantalla
//--------------------------------------------------------------------------
int	sWindow::setFullDesktop(void)
{
    if (!m_bFullDesktop)
    {
        //------------------------------------------------------------------
        // Guarda los valores iniciales de la ventana en m_wInitRect.
        // Sobre el m_hWindow de la ventana que ha recibido el evento
        // de FullDeskTop
        //------------------------------------------------------------------
        GetWindowRect(m_hWindow, &m_wInitRect);
        //------------------------------------------------------------------
        int iMonitor = (m_tVentana.eType == eEstiloW::eMainV && (cMonitor::getNumMonitors() > 1)) ? 1 : 0;

        cMonitor* poMonitor = (cMonitor*) cMonitor::getMonitor(iMonitor);
        if (poMonitor)
        {
            int iTop = 0;
            int iLeft = 0;
            int iFullAncho = m_tVentana.iAncho;		    // Ancho de la pantalla
            int iFullAlto = m_tVentana.iAlto;			// Altura de la pantalla

            if (cMonitor::esFullMonitor())
            {
                iFullAncho = poMonitor->m_oRect.getAncho();
                iFullAlto = poMonitor->m_oRect.getAlto();
                iTop = poMonitor->m_oRect.top;
                iLeft = poMonitor->m_oRect.left;
            }

            memset(&m_dmScreenSettings, 0, sizeof(m_dmScreenSettings));	// Nos aseguramos de la memoria
            m_dmScreenSettings.dmSize = sizeof(m_dmScreenSettings);		// Tamaño de la estructura Devmode
            m_dmScreenSettings.dmPelsWidth = iFullAncho;
            m_dmScreenSettings.dmPelsHeight = iFullAlto;
            m_dmScreenSettings.dmBitsPerPel = m_iBitsColor;				// Bits por pixel seleccionados
            m_dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

            // Intentamos cargar el modo seleccionado y obtener los resultados.
            // NOTA: CDS_FULLSCREEN es usado para eliminar la barra de inicio
            // if (ChangeDisplaySettings(&m_dmScreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
            cLog::traza(cLog::eTraza::min, "\n");
            cLog::traza(cLog::eTraza::min, " Cambiamos el monitor: [%s] a pantalla completa.\n", poMonitor->m_sDisplayName.c_str());
            cLog::traza(cLog::eTraza::min, " - Ancho: %d\n", iFullAncho);
            cLog::traza(cLog::eTraza::min, " -  Alto: %d\n", iFullAlto);
            cLog::traza(cLog::eTraza::min, " +   Top: %d\n", iTop);
            cLog::traza(cLog::eTraza::min, " +  Left: %d\n", iLeft);
            WCHAR wcDisplayName[LON_BUFF / 8];
            cTool::copiaMultibyteToUnicode(poMonitor->m_sDisplayName, wcDisplayName, sizeof(wcDisplayName));
            long result = ChangeDisplaySettingsEx(wcDisplayName, &m_dmScreenSettings, NULL, CDS_FULLSCREEN, NULL);
            if (result == DISP_CHANGE_SUCCESSFUL)
            {
                setFullWindow(iLeft, iTop, iFullAncho, iFullAlto);
            }
            else
            {
                // if (buscaDevMode(poMonitor, m_dmScreenSettings))
                if (poMonitor->buscaDevMode(&m_dmScreenSettings))
                {
                    // Volvemos a probar con el m_dmScreenSettings, encontrado y modificado.
                    result = ChangeDisplaySettingsEx(wcDisplayName, &m_dmScreenSettings, NULL, CDS_FULLSCREEN, NULL);
                    if (result == DISP_CHANGE_SUCCESSFUL)
                    {
                        setFullWindow(iLeft, iTop, iFullAncho, iFullAlto);
                    }
                    else
                    {
                        mostrarErrorFullWindow(result, iFullAncho, iFullAlto);
                    }
                }
                else
                {
                    mostrarErrorFullWindow(result, iFullAncho, iFullAlto);
                }
            }
        }
        //------------------------------------------------------------------
        GetWindowRect(m_hWindow, &m_wCurrentRect);
    }
    return 0;
}


void sWindow::setFullWindow(int iLeft, int iTop, int iAncho, int iAlto)
{
    m_bFullDesktop = true;
    if (m_hWindow)
    {
        if (!MoveWindow(m_hWindow, iLeft, iTop, iAncho, iAlto, true))
        {
            cLog::error(" Error: sWindow::setFullWindow: MoveWindow m_bFullDesktop: %s\n", msTrue(m_bFullDesktop));
        }
    }
}


void sWindow::mostrarErrorFullWindow(long result, int iAncho, int iAlto)
{
    const char* pcError = errorPorChangeDisplaySettings(result);
    bool bDisplay = cTool::getDisplay();
    cTool::Display();
    cTool::displayMensaje(
        NULL, MB_OK, m_tVentana.vcWindowName,
        "El modo pantalla completa [%d:%d] no se puede establecer.\n[%s]", iAncho, iAlto, (pcError) ? pcError : "Razon desconocida");
    cTool::setDisplay(bDisplay);
}


const char* sWindow::errorPorChangeDisplaySettings(long result)
{
    const char* pcError = nullptr;

    int iNumArray = mSizeArray(vtChangeDisplaySettingsEx);

    for (int i = 0; i < iNumArray && !pcError; i++)
    {
        if (vtChangeDisplaySettingsEx[i].iIndex == result)
        {
            pcError = vtChangeDisplaySettingsEx[i].vcNombre;
        }
    }

    return pcError;
}


//--------------------------------------------------------------------------
// Funcion que reestablece la resolución de la pantalla
//--------------------------------------------------------------------------
int	sWindow::recuperaDesktop(bool bDestruirVentana)
{
    if (m_bFullDesktop)
    {
        int iMonitor = ( m_tVentana.eType == eEstiloW::eMainV && (cMonitor::getNumMonitors() > 1)) ? 1 : 0;

        cMonitor* poMonitor = (cMonitor*) cMonitor::getMonitor(iMonitor);
        if (poMonitor)
        {
            WCHAR wcDisplayName[LON_BUFF / 8];
            cTool::copiaMultibyteToUnicode(poMonitor->m_sDisplayName, wcDisplayName, sizeof(wcDisplayName));
            ChangeDisplaySettingsEx(wcDisplayName, NULL, NULL, 0, NULL);		// Devuelve la antigua configuración
            ShowCursor(TRUE);
            m_bFullDesktop = false;

            if (m_hWindow && !bDestruirVentana)
            {
                MoveWindow(
                    m_hWindow,
                    m_wInitRect.left, m_wInitRect.top,
                    m_wInitRect.right - m_wInitRect.left,
                    m_wInitRect.bottom - m_wInitRect.top,
                    true);
                GetWindowRect(m_hWindow, &m_wCurrentRect);
            }
        }
    }
    return 0;
}


//--------------------------------------------------------------------------
// Usando las funciones anteriores, va cambiando el modo de pantalla
// De completa a normal, y de normal a completa.
//--------------------------------------------------------------------------
int	sWindow::cambiaDesktop(void)
{
    if (m_bFullDesktop)
        recuperaDesktop();
    else
        setFullDesktop();

    return 0;
}


//--------------------------------------------------------------------------
// Llama a la funcion ShowWindow para mostrar la ventana
//--------------------------------------------------------------------------
int sWindow::muestraVentana()
{
    if (!m_bVisible)
    {
        //------------------------------------------------------------------
        ShowWindow(m_hWindow, SW_SHOW);					// Mostrar la ventana
        m_bVisible = true;
        //------------------------------------------------------------------
    }

    return 0;
}


//--------------------------------------------------------------------------
// Llama a la funcion ShowWindow para ocultar la ventana
//--------------------------------------------------------------------------
int sWindow::ocultaVentana()
{
    if (m_bVisible)
    {
        //------------------------------------------------------------------
        ShowWindow(m_hWindow, SW_HIDE);					// Oculta la ventana
        m_bVisible = false;
        //------------------------------------------------------------------
    }

    return 0;
}


//--------------------------------------------------------------------------
// Clean de los buffers inicio de escena
//--------------------------------------------------------------------------
void sWindow::clean(void)
{
    setRenderContext();

    // cOpenGL::clearColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


//--------------------------------------------------------------------------
// El Swap de los buffers
//--------------------------------------------------------------------------
void sWindow::swap()
{
    if ( sOpenGL::m_bGlFinish )    // Por defecto a false
    {
        glFinish();     // obliga pintar. espera a que el Hw termine
    }
    else                //
    {                   // Por defecto estamos en este ...
        glFlush();      // obliga a pintar, no espera a que el Hw termine.
    }

    if (m_hDC)
    {
        if (!SwapBuffers(m_hDC))
        {
            cLog::error(" Error: sWindow::swap: SwapBuffers\n");
        }
    }
}


//--------------------------------------------------------------------------
// Destruye la ventana
//--------------------------------------------------------------------------
int sWindow::destruyeVentana(void)
{
    if (m_hWindow)
    {
        if (!DestroyWindow(m_hWindow))
        {
            cLog::error(" Error: sWindow::destruyeVentana: DestroyWindow\n");
            return -1;
        }
        PostQuitMessage(0);

        m_hWindow = NULL;
    }

    return 0;
}


/*------------------------------------------------------------------------*\
|* Metodos de Respuesta a los mensajes de ventana
|*------------------------------------------------------------------------
|* Minetras me acuerde de ir registrando los metodos ya registrados aqui
|*  - OnCreate
|*  - OnDestroy
|*  - OnSize
\*------------------------------------------------------------------------*/
long sWindow::OnCreate(HWND hwnd)
{
    miError(
        creaDeviceContext(hwnd) ||
        creaPixelFormat(m_hDC) ||
        creaRenderContext(m_hDC) ||
        setRenderContext() ||
        initWindow() ||
        unSetRenderContext()
    );

    return 0;
}


long sWindow::OnDestroy()
{
    miError(
        recuperaDesktop(true)   ||
        destruyeRenderContext() ||
        destruyeDeviceContext()
    );

    return 0;
}


long sWindow::OnSize(int newWidth, int newHeight)
{
    const char* _metodo = __FUNCTION__;

    setAnchoOnSize(newWidth);
    setAltoOnSize(newHeight);

    m_dAspectX = (double)getAnchoOnSize() / (double)getScreenX();
    m_dAspectY = (double)getAltoOnSize() / (double)getScreenY();

    //cLog::traza(cLog::eTraza::nor, " - iXwin (AnchoOnSize): %4d  %4d\n", newWidth, getAnchoOnSize());
    //cLog::traza(cLog::eTraza::nor, " - iYwin (AltoOnSize) : %4d  %4d\n", newHeight, getAltoOnSize());
    //cLog::traza(cLog::eTraza::nor, " -            ScreenX : %6.2f\n", (double)getScreenX());
    //cLog::traza(cLog::eTraza::nor, " -            ScreenY : %6.2f\n", (double)getScreenY());
    //cLog::traza(cLog::eTraza::nor, " -            AspectX : %6.2f\n", m_dAspectX);
    //cLog::traza(cLog::eTraza::nor, " -            AspectY : %6.2f\n", m_dAspectY);

    glViewport(0, 0, getAnchoOnSize(), getAltoOnSize());

    return 0;
}


long sWindow::OnKeyDown(long lTecla)
{
    short sTecla = (short)lTecla;
    return 0;
}


long sWindow::OnKeyUp(long lTecla)
{
    short sTecla = (short)lTecla;
    return 0;
}


/*------------------------------------------------------------------------*\
|* windowProc
\*------------------------------------------------------------------------*/
long sWindow::windowProc(HWND hwnd, uint uiMsg, WPARAM wparam, LPARAM lparam)
{
    return m_pThis->commonWindowProc(hwnd, uiMsg, wparam, lparam);
}
/*------------------------------------------------------------------------*/
long sWindow::myWindowProc(HWND hwnd, uint uiMsg, WPARAM wparam, LPARAM lparam)
{
    const char* _metodo = __FUNCTION__;

    switch (uiMsg)
    {
        case WM_CREATE:
            OnCreate(hwnd);
            break;

        case WM_DESTROY:
            OnDestroy();
            break;

        case WM_SIZE:
            OnSize(LOWORD(lparam), HIWORD(lparam));
            break;

        case WM_KEYDOWN:
            OnKeyDown((long)wparam);
            break;

        case WM_KEYUP:
            OnKeyUp((long)wparam);
            break;

        default:
            return (long) ::DefWindowProc(hwnd, uiMsg, wparam, lparam);
    }

    //return (long) ::DefWindowProc(hwnd, uiMsg, wparam, lparam);
    return 0;
}


/*------------------------------------------------------------------------*\
|* Fin de sWindow.h
\*------------------------------------------------------------------------*/
