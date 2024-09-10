/*------------------------------------------------------------------------*\
|* sWindow.cpp
\*------------------------------------------------------------------------*/

#include "sWindow.h"
//#include "sEscena.h"
#include "../sOpenGL.h"
#include "../../tool/cTool.h"
#include "../../tool/cLog.h"
#include "../../tool/consola/cConsola.h"
#include <glm/glm.hpp>


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


/*------------------------------------------------------------------------*\
|* Constructor & Destructor
\*------------------------------------------------------------------------*/
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
    float fCercano,
    float fLejano,
    float fFov,
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
    m_fFov = fFov;
    m_fPCercano = fCercano;
    m_fPLejano = fLejano;

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

    cRect<long> tRect;
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
int sWindow::setWindowSettings(sCreaVentana* ptVentana, cRect<long>* ptWindowRect)
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


int	sWindow::setWindowStyles(sCreaVentana* ptVentana, cRect<long>* ptWindowRect)
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
        RECT rect{ ptWindowRect->left, ptWindowRect->top, ptWindowRect->right, ptWindowRect->bottom };
        if (!AdjustWindowRectEx(&rect, ptVentana->dwEstilo, false, ptVentana->dwExEstilo))
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

    cRect<long> tWindowRect;

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

    sWindow::GetWindowRect(m_hWindow, &m_wCurrentRect);
    cLog::print("\n");
    cLog::print(" [%s][%s]\n", pCreaVentana->vcWindowName, pCreaVentana->vcClassName);
    cLog::print(" Left  : %4d\n", m_wCurrentRect.left);
    cLog::print(" Top   : %4d\n", m_wCurrentRect.top);
    cLog::print(" Right : %4d\n", m_wCurrentRect.right);
    cLog::print(" Bottom: %4d\n", m_wCurrentRect.bottom);
    cLog::print(" Ancho : %4d\n", m_wCurrentRect.getAncho());
    cLog::print(" Alto  : %4d\n", m_wCurrentRect.getAlto());
    cLog::print("\n");

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
            //if (m_hRc == hRc)				// Deberia ser el mismo contexto que tenemos, pero puede que no.
            //{
            //    cLog::traza(cLog::eTraza::nor, " Eliminamos el contexto de render actual\n");
            //}

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
        sWindow::GetWindowRect(m_hWindow, &m_wInitRect);
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
        sWindow::GetWindowRect(m_hWindow, &m_wCurrentRect);
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
// Hacemos el glViewPort con los valores de onSize
//--------------------------------------------------------------------------
int sWindow::setViewPort()
{
    glViewport(0, 0, getAnchoOnSize(), getAltoOnSize());
    return 0;
}


int sWindow::setViewPort(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
    return 0;
}


//--------------------------------------------------------------------------
// Aspect: los OnSize provocaran los cambios en estos valores:
// - Se estan calculando en los Viewport de las escenas
// - Quizas debieramos traernos algo de eso a la ventana ya que
//   los Aspect a que nos referimos no son los de la relacion entre
//   Ancho/Alto sino
//   Resolucion Absoluta y Resolucion Relativa.
//
//--------------------------------------------------------------------------
// La de la X
//--------------------------------------------------------------------------
float  sWindow::getAspectX(void)
{
    return m_fAspectX;
}


//--------------------------------------------------------------------------
// La de la Y
//--------------------------------------------------------------------------
float  sWindow::getAspectY(void)
{
    return m_fAspectY;
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
    sOpenGL::syncro(eSync::Finish);

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
|*  - OnClose
|*  - OnKeyDown: sin completar
|*  - OnKeyUp: sin completar
\*------------------------------------------------------------------------*/
long sWindow::OnCreate(HWND hwnd)
{
    miError(
        creaDeviceContext(hwnd) ||
        creaPixelFormat(m_hDC) ||
        creaRenderContext(m_hDC) ||
        setRenderContext() ||           // El Render Context esta: se podrian poner sentencias openGL.
        initWindow()
        // unSetRenderContext()
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

    m_fAspectX = (getScreenX() > 0) ? (float)getAnchoOnSize() / (float)getScreenX() : 1.0f;
    m_fAspectY = (getScreenY() > 0) ? (float)getAltoOnSize() / (float)getScreenY() : 1.0f;

    //----------------------------------------------------------------------
    // Jugamos con que no ponemos Viewport por defecto ?
    // Al maximizar no ocupa toda la pantalla, el viewport necesario
    // en este caso.
    //----------------------------------------------------------------------
    glViewport(0, 0, getAnchoOnSize(), getAltoOnSize());
    //----------------------------------------------------------------------

    generaMatrices((float)getAnchoOnSize(), (float)getAltoOnSize());

    return 0;
}


long sWindow::OnClose()
{
    destruyeVentana();
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

        case WM_CLOSE:
            OnClose();
            break;

        case WM_KEYDOWN:
            OnKeyDown((long)wparam);
            break;

        case WM_KEYUP:
            OnKeyUp((long)wparam);
            break;

        case WM_ACTIVATE:
            setActiva(!HIWORD(wparam));
            break;

        default:
            return (long) ::DefWindowProc(hwnd, uiMsg, wparam, lparam);
    }

    return 0;
}


/*------------------------------------------------------------------------*\
|* Funciones Static
\*------------------------------------------------------------------------*/
cstatic bool sWindow::GetWindowRect(HWND hWindow, cRect<long>* pRectRes)
{
    bool valor = true;
    if (hWindow && pRectRes)
    {
        RECT rect;
        bool valor = static_cast<bool>(::GetWindowRect(hWindow, &rect));
        if (valor)
        {
            pRectRes->left = rect.left;
            pRectRes->top = rect.top;
            pRectRes->right = rect.right;
            pRectRes->bottom = rect.bottom;
        }
        else
        {
            cLog::error(" Error: sWindow::GetWindowRect:  ::GetWindowRect");
        }
    }
    return valor;
}


cstatic bool sWindow::GetClientRect(HWND hWindow, cRect<long>* pClientRes)
{
    bool valor = true;
    if (hWindow && pClientRes)
    {
        RECT rect;
        bool valor = static_cast<bool>(::GetClientRect(hWindow, &rect));
        if (valor)
        {
            pClientRes->left = rect.left;
            pClientRes->top = rect.top;
            pClientRes->right = rect.right;
            pClientRes->bottom = rect.bottom;
        }
        else
        {
            cLog::error(" Error: sWindow::GetClientRect:  ::GetClientRect");
        }
    }
    return valor;
}


void sWindow::setAnchoOnSize(int iAncho)
{
    m_iAnchoOnSize = iAncho;
}

void sWindow::setAltoOnSize(int iAlto)
{
    m_iAltoOnSize = iAlto;
}

int sWindow::getAnchoOnSize(void)
{
    return m_iAnchoOnSize;
}

int sWindow::getAltoOnSize(void)
{
    return m_iAltoOnSize;
}


/*------------------------------------------------------------------------*\
|* Funciones Public
\*------------------------------------------------------------------------*/
glm::mat4 sWindow::getPerspProjection()
{
    return m_perspective;
}


glm::mat4 sWindow::getOrthoProjection()
{
    return m_ortho;
}


GLfloat* sWindow::getOrthoProjectionPtr()
{
    return (GLfloat *) &m_vfProyOrtogonal;
}


//--------------------------------------------------------------------------
// Genera Matrices de proyeccion
//--------------------------------------------------------------------------
void sWindow::generaMatrices(float anchoOnSize, float altoOnSize)
{
    float fAspect = (altoOnSize > 0) ? anchoOnSize / altoOnSize : 1.0f;

    m_perspective = glm::perspective<float>(glm::radians(m_fFov), fAspect, m_fPCercano, m_fPLejano);
   
    //----------------------------------------------------------------------
    // Tenemos pendiente probar el primer plano: no sabemos si funciona
    //----------------------------------------------------------------------
    // m_ortho = glm::ortho<float>(0.0f, anchoOnSize, 0.0f, altoOnSize, m_fPCercano, m_fPLejano);
    // glOrtho(0.0f, (GLfloat)anchoOnSize, 0.0f, (GLfloat)altoOnSize, m_fPCercano, m_fPLejano);
    // glOrtho(0.0f, (GLfloat)anchoOnSize, 0.0f, (GLfloat)altoOnSize, -10.0f, m_fPLejano);
    //----------------------------------------------------------------------
    sOpenGL::pushMatrix(eMatrixModo::eGL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, (GLfloat)anchoOnSize, 0.0f, (GLfloat)altoOnSize, -10.0f, m_fPLejano);
    glGetFloatv(GL_PROJECTION_MATRIX, m_vfProyOrtogonal);
    sOpenGL::popMatrix(eMatrixModo::eGL_PROJECTION);
    //-----------------------------------------------------------------------
    // m_ortho = glm::ortho<float>(0.0f, anchoOnSize, altoOnSize, 0.0f, m_fPCercano, m_fPLejano);
    // m_ortho = glm::ortho<float>(0.0f, anchoOnSize, 0.0f, altoOnSize, -100.0f, 100.0f);
    //-----------------------------------------------------------------------
}


//--------------------------------------------------------------------------
// Begin End
//--------------------------------------------------------------------------
bool sWindow::begin()
{
    if (!m_begin)
    {
        //------------------------------------------------------------------
        int iRend;
        glGetIntegerv(GL_RENDER_MODE, &iRend);					// El modo debe de ser el de renderizacion
        if (iRend != GL_RENDER)
            return false;										//
        //------------------------------------------------------------------
        // Establece la proyeccion ortogonal para escribir en primer plano.
        //------------------------------------------------------------------
        {
            m_eMatrixMode = sOpenGL::getMatrixModo();           // Guarda el modo de la matriz
            sOpenGL::pushMatrix(eMatrixModo::eGL_PROJECTION);   // Guardamos en Pila la Matriz de Proyeccion
            glLoadMatrixf(m_vfProyOrtogonal);                   // Cargamos la matriz ortogonal, para escribir
            sOpenGL::pushMatrix(eMatrixModo::eGL_MODELVIEW);   // Guardamos en Pila la Matriz de Modelización.
            glLoadIdentity();                                   // Se inicializa.
        }
        //------------------------------------------------------------------
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        //------------------------------------------------------------------
        //_bTex2D = cOpenGL::isEnabled(GL_TEXTURE_2D);   // Antes activamos texturas, ahora no, no creo que haga falta.
        //_bLuz = cOpenGL::disable(GL_LIGHTING);         // Desativamos luces
        //_bDepth = cOpenGL::disable(GL_DEPTH_TEST);     // Desactivamos test de profundidad.
        //------------------------------------------------------------------
        m_begin = true;
        //------------------------------------------------------------------
    }
    return m_begin;
}

//--------------------------------------------------------------------------
// Termina la escritura en primer plano
//--------------------------------------------------------------------------
void sWindow::end(void)
{
    if (m_begin)
    {
        {
            //--------------------------------------------------------------
            // Recupera la proyeccion en perspectiva original: establecida
            // en Viewport.
            //--------------------------------------------------------------
            sOpenGL::popMatrix(eMatrixModo::eGL_MODELVIEW);     //  glMatrixMode(GL_MODELVIEW);     cOpenGL::popMatrix();
            //--------------------------------------------------------------
            sOpenGL::popMatrix(eMatrixModo::eGL_PROJECTION);    //  glMatrixMode(GL_PROJECTION);    cOpenGL::popMatrix();
            //--------------------------------------------------------------
            sOpenGL::setMatrixModo(m_eMatrixMode);               // glMatrixMode(m_eMatrixMode);     // Dejamos el modo de matriz que estuviese.
            //--------------------------------------------------------------
        }
        //------------------------------------------------------------------
        //cOpenGL::restore(_bDepth, GL_DEPTH_TEST);          // Restablecemos el test de profundidad.
        //cOpenGL::restore(_bLuz, GL_LIGHTING);              // Restablecemos la iluminacion.
        //cOpenGL::restore(_bTex2D, GL_TEXTURE_2D);          // Restablecemos las texturas, si las hubieramos cambiado
        //------------------------------------------------------------------
        glPopAttrib();
        //------------------------------------------------------------------
        m_begin = false;
        //------------------------------------------------------------------
    }
}
//--------------------------------------------------------------------------


/*------------------------------------------------------------------------*\
|* Fin de sWindow.h
\*------------------------------------------------------------------------*/
