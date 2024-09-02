//--------------------------------------------------------------------------
// cGLWindow.cpp
//--------------------------------------------------------------------------


#include "cGLWindow.h"
#include "cDialogo.h"
#include "cMonitor.h"
#include "Escenas/cLisEscenas.h"
#include "../Graficos/cOpengl.h"
#include "../Graficos/cGestorColores.h"
#include "../Memoria/fMemoria.h"
#include "../Files/cLog.h"
#include "../Util/cUtil.h"
#include "../Util/cError.h"
#include "../Devices/cTeclado.h"
#include "../Devices/cRaton.h"
#include "../Mensajes/cInterfazSwat.h"


#ifndef _MINGW_
#pragma region Atributos
#endif
static char * pcFinal = nullptr;
static const char * _modulo = quitarPath(__FILE__, &pcFinal);
#ifndef _MINGW_
#pragma endregion Atributos
#endif


//--------------------------------------------------------------------------
// Statics
//--------------------------------------------------------------------------
HGLRC cGLWindow::m_hRcmain	= NULL; // Contexto de generacion prin. a compartir.
int cGLWindow::m_iNumWindows = 0;
cCSection cGLWindow::m_xSeccion("Swat");      // Critical section
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Resultados cambio dinamico de la pantalla
//--------------------------------------------------------------------------
TItem vtChangeDisplaySettingsEx[]
{
    { DISP_CHANGE_SUCCESSFUL, "El cambio de configuración fue exitoso.", nullptr },
    { DISP_CHANGE_BADDUALVIEW, "El sistema es compatible con DualView.", nullptr },
    { DISP_CHANGE_BADFLAGS, "Se pasó un conjunto de banderas no válido.", nullptr },
    { DISP_CHANGE_BADMODE, "El modo de gráficos no es compatible.", nullptr },
    { DISP_CHANGE_BADPARAM, "Se pasó un parámetro no válido.", nullptr },
    { DISP_CHANGE_FAILED, "El controlador de pantalla falló en el modo de gráficos dado.", nullptr },
    { DISP_CHANGE_NOTUPDATED, "No se puede escribir la configuración en el registro.", nullptr },
    { DISP_CHANGE_RESTART, "Debe reiniciar el ordenador para que funcione el nuevo modo.", nullptr },
};
//--------------------------------------------------------------------------


#ifndef _MINGW_
#pragma region Constructor & Destructor
#endif
//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cGLWindow::cGLWindow(HINSTANCE hInstancia, eTypeVentana eType, int iIdent, const char * pcNombre)
    : cVentana(hInstancia, eType, iIdent, pcNombre)
    , IViewport()
    , m_hRc(0)                  // Contexto de renderizacion.
    , m_hRcKeep(0)              // A utilizar por keepRenderContext(); y unKeepRenderContext();
	, m_iPixelFormat(0)         // Numero del pixelformat activado
	, m_iBitsColor(0)
	, m_iBitsDepth(0)           // Numero de bits de profundidad.
	, m_bFullDesktop(false)     // Indica si ya está activa la pantalla completa.
    , m_bGlFinish(false)
    , m_dAspectX(1.0)
    , m_dAspectY(1.0)
    , m_iAnchoOnSize(1)     // Por no poner un Cero, 
    , m_iAltoOnSize(1)      // por si las divisiones por cero.
    , m_fAlpha(1.0)
    , m_iErrorCicloSwap(0)
    , m_bTrazas(false)      // Por defecto, no permitimos trazas
{
    setNew(this, "cGLWindow");
	memset(&m_dmScreenSettings,0,sizeof(m_dmScreenSettings));
	memset((void *) &m_pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
}

cGLWindow::~cGLWindow(void)
{
}
#ifndef _MINGW_
#pragma endregion
#endif


/*========================================================================*\
|* Resto de metodos
\*========================================================================*/
/*========================================================================*\
|* Metodos Public
|* - setPrimerPlano
|* - getAspectX
|* - getAspectY
\*========================================================================*/
#ifndef _MINGW_
#pragma region Metodos Publicos
#endif
//--------------------------------------------------------------------------
// SetPrimerPlano
//--------------------------------------------------------------------------
void cGLWindow::setPrimerPlano(cEscena * poPrimerPlano)
{
    if (poPrimerPlano)
    {
        // Y que pasa si ya había un primer plano ????
        // Hay que quitarlo !!!!
        if (m_poPrimerPlano)
        {
            m_poPrimerPlano->DesActivaEventos();
            m_poLisEscenas->Remove(m_poPrimerPlano);
        }
        // Ponemos el primer plano
        m_poEscena->DesActivaEventos();
        // Y ahora ponemos el primer plano
        m_poPrimerPlano = poPrimerPlano;
        m_poLisEscenas->Insertar(m_poPrimerPlano);
        m_poPrimerPlano->ActivaEventos();
    }
    else
    {
        if (m_poPrimerPlano)
        {
            m_poPrimerPlano->DesActivaEventos();
            m_poLisEscenas->Remove(m_poPrimerPlano);
        }
        m_poPrimerPlano = NULL;
        m_poEscena->ActivaEventos();
    }
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
double  cGLWindow::getAspectX(void)
{
    return m_dAspectX;
}


//--------------------------------------------------------------------------
// La de la Y
//--------------------------------------------------------------------------
double  cGLWindow::getAspectY(void)
{
    return m_dAspectY;
}

#ifndef _MINGW_
#pragma endregion Metodos Publicos
#endif


/*========================================================================*\
|* Metodos Protected
|* - crearPixelFormat
|* - crearRenderContext
|* - destruyeRenderContext
|* - setRenderContext (2 sobrecargas)
|* - unSetRenderContext
|* - setFullDesktop
|* - recuperaDesktop
|* - cambiaDesktop
|* - entraSeccion
|* - saleSeccion
|* - enPantallaCompleta
\*========================================================================*/
#ifndef _MINGW_
#pragma region Metodos Protected
#endif

//--------------------------------------------------------------------------
// Establece el pixel format
//--------------------------------------------------------------------------
int	cGLWindow::crearPixelFormat(HDC hDC)
{
    miNulo(hDC);

    cError	oError;
    uint	iPixelFormat;
    //----------------------------------------------------------------------
    // PixelFormatDescriptor
    //----------------------------------------------------------------------
    m_pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);  // Tamaño del descriptor del formato de pixel
    m_pfd.nVersion = 1;                    // Numero de version
    m_pfd.dwFlags = PFD_DRAW_TO_WINDOW |  // Debe soportar ventanas
        PFD_SUPPORT_OPENGL |  // Debe soportar OpenGL
        PFD_DOUBLEBUFFER;     // Soporta doble buffer
    m_pfd.iPixelType = PFD_TYPE_RGBA;		// Formato de color RGBA
    m_pfd.cColorBits = m_iBitsColor;			// Profundidad de color
    //m_pfd.cRedBits		= 3;				// Para nuestra paleta particular 
    m_pfd.cRedBits = 0;				// 
    m_pfd.cRedShift = 0;				//
    //m_pfd.cGreenBits		= 3;				// Para nuestra paleta particular
    m_pfd.cGreenBits = 0;				//
    //m_pfd.cGreenShift		= 3;				// Para nuestra paleta particular
    m_pfd.cGreenShift = 0;				//
    //m_pfd.cBlueBits		= 2;				// Para nuestra paleta particular
    m_pfd.cBlueBits = 0;				//
    //m_pfd.cBlueShift		= 6;				// Para nuestra paleta particular
    m_pfd.cBlueShift = 0;				//
    m_pfd.cAlphaBits = 0;				// No queremos buffer alpha
    m_pfd.cAlphaShift = 0;				// Ignoramos Shift Bit
    m_pfd.cAccumBits = 0;				// No queremos buffer de acumulacion
    m_pfd.cAccumRedBits = 0;				//
    m_pfd.cAccumGreenBits = 0;				//	Por lo que ignoramos estos bits
    m_pfd.cAccumBlueBits = 0;				//
    m_pfd.cAccumAlphaBits = 0;				//
    m_pfd.cDepthBits = m_iBitsDepth;		// Buffer de profundidad de 16 bits
    m_pfd.cStencilBits = 0;				// No usamos el buffer de pincel ??? (estarcido)
    m_pfd.cAuxBuffers = 0;				// No usamos buffers auxiliares
    m_pfd.iLayerType = PFD_MAIN_PLANE;   // Dibuja en el plano principal ???
    m_pfd.bReserved = 0;				// Reservado
    m_pfd.dwLayerMask = 0;				// Se ignoran niveles de mascara ???
    m_pfd.dwVisibleMask = 0;
    m_pfd.dwDamageMask = 0;
    //----------------------------------------------------------------------
    iPixelFormat = ChoosePixelFormat(hDC, &m_pfd);
    if (!iPixelFormat)	// Hemos obtenido el formato de pixel?
    {
        return oError.mensaje(dERR, "No podemos encontrar un formato de pixel aceptable.");
    }
    //----------------------------------------------------------------------
    // ¿Podemos establecer el formato de pixel?
    //----------------------------------------------------------------------
    if (!SetPixelFormat(hDC, iPixelFormat, &m_pfd))
    {
        return oError.mensaje(dERR, "No se puede establacer el formato de Pixel.");
    }
    //----------------------------------------------------------------------
    iPixelFormat = GetPixelFormat(hDC);
    if (!iPixelFormat)
    {
        return oError.mensaje(dERR, "No podemos obtener el formato de pixel del Contexto.");
    }
    m_iPixelFormat = iPixelFormat;
    iPixelFormat = DescribePixelFormat(hDC, m_iPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &m_pfd);
    if (!iPixelFormat)
    {
        return oError.mensaje(dERR, "No podemos obtener la informacion del formato de pixel.");
    }
    //----------------------------------------------------------------------
    if (cLog::siTraza(m_iTraza))
    {
        cLog::print("   * Pixelformat: %d\n", m_iPixelFormat);
        cLog::print("     cColorBits: %d\n", m_pfd.cColorBits);
        cLog::print("     cDepthBits: %d\n", m_pfd.cDepthBits);
        cLog::print("     Maximo Pixelformat: %d\n", iPixelFormat);
    }
    //----------------------------------------------------------------------
    return 0;
}


//--------------------------------------------------------------------------
// Para un dispositivo dado crea el contexto de renderizacion.
//--------------------------------------------------------------------------
int	cGLWindow::crearRenderContext(HDC hDC)
{
    if (hDC)
    {
        cError	oError;

        //------------------------------------------------------------------
        if (!m_hRcmain)
        {
            // El principal, que luego se comparte ...
            m_hRcmain = wglCreateContext(hDC);
            if (!m_hRcmain)			// Podemos obtener un contexto de renderización?
            {
                return oError.mensaje(dERR, "No se puede crear el GL Rendering Context Principal.");
            }
        }
        //------------------------------------------------------------------
        m_hRc = wglCreateContext(hDC);
        if (!m_hRc)			// Podemos obtener un contexto de renderización?
        {
            return oError.mensaje(dERR, "No se puede crear un GL Rendering Context.");
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
                return oError.mensaje(dERR, "No se puede compartir el GL Rendering Context.");
            }
            m_iNumWindows++;	// Con contexto de renderizacion compartido.
        }
        //------------------------------------------------------------------
    }
    return 0;
}


//--------------------------------------------------------------------------
// Destruye los contextos de renderizacion
//--------------------------------------------------------------------------
int	cGLWindow::destruyeRenderContext(void)
{
    if (m_hWindow)
    {
        if (m_hRc)							// ¿Hay un Rendering Context?
        {
            cError oError;
            HGLRC	hRc;

            hRc = wglGetCurrentContext();
            if (m_hRc == hRc)				// Deberia ser el mismo contexto que tenemos, pero puede que no.
            {
                cLog::traza(m_iTraza, " Tenemos el contexto actual\n");
            }

            if (m_hDC)
            {
                if (!wglMakeCurrent(m_hDC, NULL))	// ¿Podemos eliminar el DC Context y el RC Context?
                {
                    oError.mensaje(dERR, "Fallo al quitar el current RC del DC.");
                    // sera un error pero no salimos, sin return.
                }
            }

            if (!wglDeleteContext(m_hRc))	// ¿Podemos eliminar el RC?
            {
                oError.mensaje(dERR, "Fallo al eliminar el RC.");
                // sera un error pero no salimos, sin return.
            }
            m_hRc = NULL;

            m_iNumWindows--;	// Con error o sin error esto se disminuye.
            if (m_iNumWindows == 0 && m_hRcmain)		// Si es el ultimo...
            {
                if (!wglDeleteContext(m_hRcmain))
                {
                    oError.mensaje("DeleteContext del hRc principal");
                    // sera un error pero no salimos, sin return.
                }
                m_hRcmain = NULL;
            }
        }
    }
    return 0;
}


//--------------------------------------------------------------------------
// Para un dispositivo dado establece cual es el que esta en curso.
//--------------------------------------------------------------------------
int	cGLWindow::setRenderContext(bool bSinContexto)
{
    if (bSinContexto)
        return 0;
    return setRenderContext(m_hDC, m_hRc);
}


//--------------------------------------------------------------------------
// Quita el render del contexto actual
//--------------------------------------------------------------------------
int	cGLWindow::unSetRenderContext(bool bSinContexto)
{
    if (bSinContexto)
        return 0;
    return setRenderContext(m_hDC, NULL);
}


int	cGLWindow::setRenderContext(HDC hDC, HGLRC hRC)
{
    //----------------------------------------------------------------------
    if (!wglMakeCurrent(hDC, hRC))
    {
        cError	oError("cGLWindow::SetRenderContext: No se puede activar / desactivar el GL Render Context.");
        // return oError.mensaje("cGLWindow::SetRenderContext: No se puede activar/desactivar el GL Render Context.");
        return oError.getLastError();
    }
    //----------------------------------------------------------------------
    return 0;
}


//--------------------------------------------------------------------------
// Comprueba si estamos dentro del render context
// Si no lo esta, lo establece
//--------------------------------------------------------------------------
int cGLWindow::keepRenderContext(void)
{
    m_hRcKeep = wglGetCurrentContext();
    if (!m_hRcKeep)
    {
        setRenderContext();
    }
    return 0;
}


//--------------------------------------------------------------------------
// Quita el render context
// Pero si hubiera estado activo en la primera llamada a keepRenderContext
// Lo deja y no lo quita.
//--------------------------------------------------------------------------
int cGLWindow::unKeepRenderContext(void)
{
    if (!m_hRcKeep)
    {
        unSetRenderContext();
    }
    return 0;
}


//--------------------------------------------------------------------------
//  Funcion que establece la resolución de la pantalla
//--------------------------------------------------------------------------
int	cGLWindow::setFullDesktop(void)
{
    if (!m_bFullDesktop)
    {
        //------------------------------------------------------------------
        // Guarda los valores iniciales de la ventana en m_wInitRect.
        // Sobre el m_hWindow de la ventana que ha recibido el evento
        // de FullDeskTop
        //------------------------------------------------------------------
        getWindowRect(m_wInitRect);
        //------------------------------------------------------------------
        int iMonitor = (m_eEstilo == eMainV && (getNumMonitors() > 1)) ? 1 : 0;

        cMonitor * poMonitor = (cMonitor *)getMonitor(iMonitor);
        if (poMonitor)
        {
            int iTop = 0;
            int iLeft = 0;
            int iFullAncho = m_tVentana.iAncho;		    // Ancho de la pantalla
            int iFullAlto = m_tVentana.iAlto;			// Altura de la pantalla

            if (esFullMonitor())
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
            cLog::traza(eTMIN, "\n");
            cLog::traza(eTMIN, " Cambiamos el monitor: [%s] a pantalla completa.\n", *(poMonitor->m_sDisplayName));
            cLog::traza(eTMIN, " - Ancho: %d\n", iFullAncho);
            cLog::traza(eTMIN, " -  Alto: %d\n", iFullAlto);
            cLog::traza(eTMIN, " +   Top: %d\n", iTop);
            cLog::traza(eTMIN, " +  Left: %d\n", iLeft);
            long result = ChangeDisplaySettingsEx(poMonitor->m_sDisplayName.Cad(), &m_dmScreenSettings, NULL, CDS_FULLSCREEN, NULL);
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
                    result = ChangeDisplaySettingsEx(poMonitor->m_sDisplayName.Cad(), &m_dmScreenSettings, NULL, CDS_FULLSCREEN, NULL);
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
        getWindowRect(m_wCurrentRect);
    }
    return 0;
}


void cGLWindow::setFullWindow(int iLeft, int iTop, int iAncho, int iAlto)
{
    m_bFullDesktop = true;
    if (m_hWindow)
    {
        cError oError;

        if (!MoveWindow(m_hWindow, iLeft, iTop, iAncho, iAlto, true))
        {
            oError.mensaje("cGLWindow::setFullDesktop: m_bFullDesktop: %s", msTrue(m_bFullDesktop));
        }
    }
}


void cGLWindow::mostrarErrorFullWindow(long result, int iAncho, int iAlto)
{
    const char * pcError = errorPorChangeDisplaySettings(result);
    nsUtil::cUtil * poTool = nsUtil::cUtil::Instancia();
    bool bDisplay = poTool->getDisplay();
    poTool->Display();
    poTool->displayMensaje(
        NULL, MB_OK, m_tVentana.vcWindowName,
        "El modo pantalla completa [%d:%d] no se puede establecer.\n[%s]", iAncho, iAlto, (pcError) ? pcError : "Razon desconocida");
    poTool->setDisplay(bDisplay);
}


const char * cGLWindow::errorPorChangeDisplaySettings(long result)
{
    const char * pcError = nullptr;

    int iNumArray = mSizeArray(vtChangeDisplaySettingsEx);

    for (int i=0; i < iNumArray && !pcError; i++)
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
int	cGLWindow::recuperaDesktop(bool bDestruirVentana)
{
    if (m_bFullDesktop)
    {
        int iMonitor = (m_eEstilo == eMainV && (getNumMonitors() > 1)) ? 1 : 0;

        cMonitor * poMonitor = (cMonitor *) getMonitor(iMonitor);
        if (poMonitor)
        {
            ChangeDisplaySettingsEx(poMonitor->m_sDisplayName.Cad(), NULL, NULL, 0, NULL);		// Devuelve la antigua configuración
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
                getWindowRect(m_wCurrentRect);
            }
        }
    }
    return 0;
}


//--------------------------------------------------------------------------
// Usando las funciones anteriores, va cambiando el modo de pantalla
// De completa a normal, y de normal a completa.
//--------------------------------------------------------------------------
int	cGLWindow::cambiaDesktop(void)
{
    if (m_bFullDesktop)
        recuperaDesktop();
    else
        setFullDesktop();

    return 0;
}


//--------------------------------------------------------------------------
// Fucniones seccion critica
//--------------------------------------------------------------------------
int	cGLWindow::entraSeccion(void)
{
    m_xSeccion.entrar();
    return 0;
}


int	cGLWindow::saleSeccion(void)
{
    m_xSeccion.salir();
    return 0;
}


//--------------------------------------------------------------------------
// Si es en pantalla completa los estilos estan muy claros.
//--------------------------------------------------------------------------
bool cGLWindow::enPantallaCompleta(void)
{
    if (m_bCompleta && m_eEstilo != eControl && m_eEstilo != eControlV)							// Estamos en modo pantalla completa?
    {
        memset(&m_dmScreenSettings, 0, sizeof(m_dmScreenSettings));

        setFullDesktop();
        m_tVentana.dwExEstilo |= WS_EX_APPWINDOW;						// Estilo extendido de ventana (Window Extended Style)
        m_tVentana.dwEstilo |= WS_POPUP;								// Estilo de ventana

        return false;	// Si es false ya no continua el ciclo.	(Esto hay que probarlo)
    }

    return true;		//	Devuelve true, porque no es pantalla completa y debe continuar el 'ciclo'.
}
#ifndef _MINGW_
#pragma endregion Metodos Protected
#endif


/*========================================================================*\
|* Metodos Virtuales
|* - crearGLWindow
|* - creaVentana
|* - destruyeVentana
\*========================================================================*/
#ifndef _MINGW_
#pragma region Metodos Virtuales
#endif

//--------------------------------------------------------------------------
// Creacion de la ventana OpenGL
//--------------------------------------------------------------------------
int	cGLWindow::crearGLWindow(
    const char	  *	pcTitulo,
    const char	  *	pcClase,
    int			iLeftP,
    int			iTopP,
    int			iAnchoP,
    int			iAltoP,
    int			nBitsColor,
    int			nBitsDepth,
    bool		bPCompleta,
    eEstiloW	peType,
    double      dFov,
    double      dPCercano,
    double      dPLejano,
    HINSTANCE	hInstancia,
    WNDPROC		pWndProc,
    HWND		hWinPadre)
{
    TCreaVentana    tVentana;
    cRECT           tRect;
    int             iRes;
    bool            bFull;
    eEstiloW        eType;

    bFull = false;
    eType = peType;
    m_eEstilo = peType;
    if (m_eEstilo == eMain || m_eEstilo == eMainV)
    {
        bFull = bPCompleta;
        eType = peType;
    }

    int iMonitor;
    int iTop, iLeft;
    int iAnchoW, iAltoW;

    iMonitor = ((m_eEstilo == eMainV || m_eEstilo == eControlV) && getNumMonitors() > 1) ? 1 : 0;
    cMonitor * poMonitor = (cMonitor *)getMonitor(iMonitor);
    miNulo(poMonitor);

    iAnchoW = poMonitor->m_oRect.getAncho();
    iAltoW = poMonitor->m_oRect.getAlto();
    iTop = poMonitor->m_oRect.top;
    iLeft = poMonitor->m_oRect.left;

    if (m_eEstilo == eControl || m_eEstilo == eControlV)
    {
        switch (iLeftP)
        {
            case eRIGHT:	iLeft += iAnchoW - iAnchoP;       break;
            case eCENTER:	iLeft += (iAnchoW - iAnchoP) / 2; break;
            case eLEFT:		iLeft += 0;                       break;
        }
        switch (iTopP)
        {
            case eTOP:		iTop += 0;                      break;
            case eCENTER:	iTop += (iAltoW - iAltoP) / 2;  break;
            case eBOT:		iTop += iAltoW - iAltoP;        break;
        }
    }

    tRect.top = iTop;
    tRect.left = iLeft;
    tRect.bottom = iAltoP + iTop;
    tRect.right = iAnchoP + iLeft;

    m_bCompleta = bFull;
    m_iBitsColor = nBitsColor;
    m_iBitsDepth = nBitsDepth;

    tVentana.eType = eType;
    tVentana.tRect = tRect;
    tVentana.x = 0;
    tVentana.y = 0;
    tVentana.iAlto = iAltoP;
    tVentana.iAncho = iAnchoP;
    tVentana.hWndPadre = hWinPadre;
    tVentana.hInstancia = hInstancia;
    mCopia(tVentana.vcClassName, pcClase);
    mCopia(tVentana.vcWindowName, pcTitulo);

    //setScreenX((iScreenX == -1) ? iAnchoP : iScreenX);
    //setScreenY((iScreenY == -1) ? iAltoP : iScreenY);

    setScreenX(iAnchoP);
    setScreenY(iAltoP);

    setFov(dFov);
    setPCercano(dPCercano);
    setPLejano(dPLejano);

    cLog::traza(m_iTraza, "\n");
    cLog::traza(m_iTraza, " cGLWindow::CrearGLWindow: [%s][%d]\n", tVentana.vcWindowName, getIdent());
    iRes =
        (
                registraVentana(hInstancia, pcClase, pWndProc, eType) != 0
            ||  creaVentana(&tVentana) != 0
        );

    return iRes;
}


//--------------------------------------------------------------------------
// Creacion de ventana sobre el rectangulo de un control
//
//  Por ahora, hasta que no lo vea no lo activo!!
//
//  No lo Borres !!! ... todavia ...
//
//--------------------------------------------------------------------------
//int cGLWindow::crearGLControl (
//    char	  *	pcTitulo,
//    char	  *	pcClase,
//    int			iBitsColor,
//    int			iBitsDepth,
//    // cGLControl * poControl,
//    cControl  * poControl,
//    cDialogo  *	poDialogPadre)
//{
//    TCreaVentana	tVentana;
//    cRECT			tGlRect;
//    cRECT			tDiaRect;
//    cRECT			tConRect;
//    cRECT			tRectNew;
//    int				iRes;
//
//    poDialogPadre->getWindowRect(tDiaRect);
//    poDialogPadre->getClientRect(tConRect);
//    poControl->getWindowRect(tGlRect);
//
//    tRectNew.top = tGlRect.top - tDiaRect.top - (tDiaRect.Alto() - tConRect.Alto());
//    tRectNew.bottom = tGlRect.bottom - tDiaRect.top - (tDiaRect.Alto() - tConRect.Alto());
//    tRectNew.left = tGlRect.left - tDiaRect.left - (tDiaRect.Ancho() - tConRect.Ancho());
//    tRectNew.right = tGlRect.right - tDiaRect.left - (tDiaRect.Ancho() - tConRect.Ancho());
//
//    m_bCompleta = false;
//    m_iBitsColor = iBitsColor;
//    m_iBitsDepth = iBitsDepth;
//
//    tVentana.eType = eControl;
//    tVentana.ptRect = &tRectNew;
//    tVentana.iAlto = 0;
//    tVentana.iAncho = 0;
//    tVentana.hWndPadre = poDialogPadre->getWindow();
//    tVentana.hInstancia = poDialogPadre->getInstancia();
//    mCopia(tVentana.vcClassName, pcClase);
//    mCopia(tVentana.vcWindowName, pcTitulo);
//
//    cLog::traza(m_iTraza, " cGLWindow::CrearGLControl: [%s][%d]\n", tVentana.vcWindowName, getIdent());
//    iRes =
//        (
//                registraVentana(tVentana.hInstancia, tVentana.vcClassName, m_pFuncion, tVentana.eType)
//            ||  creaVentana(&tVentana)
//        );
//
//    return iRes;
//}


//--------------------------------------------------------------------------
// Creacion de nuestras ventanas
//--------------------------------------------------------------------------
coverride int cGLWindow::creaVentana(void * pEstruct)
{
    if (m_bRegistrada && m_hWindow == NULL && pEstruct)
    {
        TCreaVentana *	ptVentana;
        cRECT			tWindowRect;
        cError			oError;
        int				iRes;

        ptVentana = (TCreaVentana *) pEstruct;
        //------------------------------------------------------------------
        iRes =
            (
                    setWindowSettings(ptVentana, &tWindowRect) !=0
                ||  setWindowStyles(&tWindowRect) !=0
            );
        //------------------------------------------------------------------
        miError(iRes);
        //------------------------------------------------------------------

        cLog::print(" cGLWindow::creaVentana: \n");
        cLog::print("   XIni: %d\n", tWindowRect.left);
        cLog::print("   YIni: %d\n", tWindowRect.top);
        cLog::print("  Ancho: %d\n", tWindowRect.right - tWindowRect.left);
        cLog::print("   Alto: %d\n", tWindowRect.bottom - tWindowRect.top);

        //------------------------------------------------------------------
        // Creacion de la ventana
        //------------------------------------------------------------------
        m_hWindow = CreateWindow(
            m_tVentana.vcClassName,
            m_tVentana.vcWindowName,
            m_tVentana.dwEstilo | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
            m_tVentana.x,
            m_tVentana.y,				// Posición de la ventana
            m_tVentana.iAncho,			// Ancho de la ventana
            m_tVentana.iAlto,			// Alto de la ventana
            m_tVentana.hWndPadre,
            m_tVentana.hMenu,
            m_tVentana.hInstancia,
            m_tVentana.ptCreate);
        //------------------------------------------------------------------
        if (m_hWindow == NULL)
        {
            return oError.mensaje(dERR, "cGLWindow::CreateWindow");
        }
        //------------------------------------------------------------------
        getWindowRect(m_wCurrentRect);
        //------------------------------------------------------------------
        enPantallaCompleta();
    }

    return 0;
}



//--------------------------------------------------------------------------
// Destruye la ventana
//--------------------------------------------------------------------------
//int cGLWindow::destruyeVentana(void)
//{
//    cLog::traza(m_iTraza, " cGLWindow::DestruyeVentana: [%s][%d]\n", m_tVentana.vcWindowName, getIdent());
//    // EntraSeccion();
//    {
//        destruyeRenderContext();
//        cVentana::destruyeVentana();
//    }
//    // SaleSeccion();
//    return 0;
//}
#ifndef _MINGW_
#pragma endregion Metodos Virtuales
#endif


/*========================================================================*\
|* Dibujado de la ventana.
|* - iniciaVentanaGL
|* - clean
|* - dibuja
|* - swap
\*========================================================================*/
#ifndef _MINGW_
#pragma region Dibujado de la Ventana
#endif
//--------------------------------------------------------------------------
// Valores de inicio de la Windows/Escena/Pantalla
// Trasladamos esta funcion de estar en Escena a cGLWindow
//
// De cEscena::Init(HDC)  a   cGLWindow::IniciaEstadoOpenGL( void )
//
// Nos aseguramos unas condiciones iniciales en la ejecucion de toda la
// ventana.
// Espero no tener que hacerlo ppor cada Escena/Pantalla.
//--------------------------------------------------------------------------
// Dejare aqui la explicacion de los valores introducidos
// para limpiar el codigo de comentarios
//--------------------------------------------------------------------------
//
//    //Por lo que se ve, para ver los modelos TXT es necesario que:
//    // - el Culling : GL_CULL_FACE, ocultacion de las caras ocultas esta activo.
//    // - el test de profundidad, asta activo.
//    // y con sus valores por defecto
//
//    // El puto y enrevesado tema del Culling: ocultacion (o no renderizado) de caras
//    glEnable(GL_CULL_FACE);         // Activo el culling: por defecto desactivado, queda activado
//                                    // Valor por defecto
//    glCullFace(GL_BACK);		    // El valor por defecto: GL_BACK: borrara la trasera.
//                                    // Determina cuales son las caras frontales.
//    glFrontFace(GL_CCW);		    // El valor por defecto: 
//                                    // GL_CCW counterclockwise: 
//                                    // - indica la cara frontal:
//                                    //   + en sentido contrario a las agujas del reloj
//
//    glEnable(GL_DEPTH_TEST);
//    // Este es el valor por defecto del Test de profundidad.
//    glDepthFunc(GL_LESS);
//
//    //glEnable(GL_ALPHA_TEST);                                // Test de canal alpha activo ! ?????
//    //glEnable(GL_BLEND);                                     // Transparencia activa ! (mirar explosiones)
//    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);      // La funcion de blending, no se si es la que uso.
//
//    glShadeModel(GL_SMOOTH);
//
//--------------------------------------------------------------------------
int cGLWindow::inicioCicloVentana(void)
{
    cVentana::inicioCicloVentana();
    bool bPrimeraVez = cOpenGL::iniciaValores();

    // La primera vez comprueba el estado para saber el estado por defecto
    if (bPrimeraVez)
    {
        int iValor;
        cLog::print("\n");
        cLog::print(" Estado OpenGL Inicial:\n");
        cLog::print(" - Depth Test: %s\n", msTrue(glIsEnabled(GL_DEPTH_TEST)));
        cLog::print(" - Alpha Test: %s\n", msTrue(glIsEnabled(GL_ALPHA_TEST)));
        cLog::print(" - Blend activo: %s\n", msTrue(glIsEnabled(GL_BLEND)));
        cLog::print(" - Ocultacion Caras Ocultas (Cull Face): %s\n", msTrue(glIsEnabled(GL_CULL_FACE)));

        glGetIntegerv(GL_CULL_FACE_MODE, &iValor);
        const char * pcCarasAOcultar = cOpenGL::getNombreOpenGL(iValor);
        // cLog::print("   + Caras a ocultar: %s\n", ((iValor == GL_FRONT_AND_BACK) ? "GL_FRONT_AND_BACK" : ((iValor == GL_BACK) ? "GL_BACK" : "GL_FRONT")));
        cLog::print("   + Caras a ocultar: %s\n", pcCarasAOcultar);

        glGetIntegerv(GL_FRONT_FACE, &iValor);
        const char * pcCaraFrontal = cOpenGL::getNombreOpenGL(iValor);
        // cLog::print("   + Caras Frontales: %s\n", ((iValor == GL_CCW) ? "GL_CCW" : "GL_CW"));
        cLog::print("   + Caras Frontales: %s\n", pcCaraFrontal);
        cLog::print("\n");

        int vPoligonMode[2] = { 0, 0 };

        glGetIntegerv(GL_POLYGON_MODE, vPoligonMode);
        const char * pcPoligonMode0 = cOpenGL::getNombreOpenGL(vPoligonMode[0]);
        const char * pcPoligonMode1 = cOpenGL::getNombreOpenGL(vPoligonMode[1]);

        //cLog::print(" - Polygon Mode:  %s  %s\n",
        //    (vPoligonMode[0] == GL_FRONT_AND_BACK) ? "GL_FRONT_AND_BACK" :
        //    (vPoligonMode[0] == GL_BACK) ? "GL_BACK" :
        //    (vPoligonMode[0] == GL_FRONT) ? "GL_FRONT" : "face",
        //    (vPoligonMode[1] == GL_POINT) ? "GL_POINT" :
        //    (vPoligonMode[1] == GL_LINE) ? "GL_LINE" :
        //    (vPoligonMode[1] == GL_FILL) ? "GL_FILL" : "mode");

        cLog::print(" - Polygon Mode:  %s  %s\n", pcPoligonMode0, pcPoligonMode1);
        //----------------------------------------------------------------------
        // Dibujado de poligonos: antialiasing
        //----------------------------------------------------------------------
        // glEnable(GL_POLYGON_SMOOTH);
        //----------------------------------------------------------------------
    }

    return 0;
}


//--------------------------------------------------------------------------
// Clean de los buffers inicio de escena
//--------------------------------------------------------------------------
void cGLWindow::clean(void)
{
    cOpenGL::clearColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


//==========================================================================
// Dibujado de la ventana.
//==========================================================================
int cGLWindow::dibuja(double dDeltaTime)
{
    if (m_bVisible)
    {
        if (m_hDC && m_hRc)
        {
            entraSeccion();
            {
                setRenderContext(m_hDC, m_hRc);
                inicioCicloVentana();
                clean();
                //----------------------------------------------------------
                if (m_poLisEscenas && m_poLisEscenas->Count()>0)
                {
                    cLisEscenas::cIterator itEscena;
                    cEscena * poEscena;
#ifdef _DEBUG
                     int iNum;
                     iNum = m_poLisEscenas->Count();
#endif // _DEBUG
                    for ( itEscena=m_poLisEscenas->begin(); !itEscena.esFin(); itEscena++ )
                    {
                        poEscena = **itEscena;
                        if (poEscena)
                        {
                            poEscena->setWindow(this);
                            poEscena->preUpdate();
                            poEscena->updateControles(m_tPersp, poEscena->getUpdate(), "cGLWindow::dibuja");
                            poEscena->viewport(poEscena->getUpdate(), poEscena->getXpos(), poEscena->getYpos(), poEscena->getAncho(), poEscena->getAlto(), poEscena->getSceneType(), this);
                            poEscena->usaMatrices(poEscena->DarPerspectiva(), poEscena->getUpdate());
                            poEscena->dibuja(dDeltaTime);
                            poEscena->setUpdate(false);
                        }
                    }
                }
                //----------------------------------------------------------
                else
                {
                    // Si es false, no hay actualizacion, los valores de actualizacion pueden ser nulos
                    viewport(false, 0, 0, 0, 0, eSceneType::eWindowScene, nullptr);

                    // Nos falta la perspectiva en la cGLWindow
                    usaMatrices(m_tPersp, false);

                    // no hay escenas, no tiene sentido a no ser que
                    // nos fabriquemos el functor aqui que llame a 
                    // la funcion de dibujo que hayamos decidido.
                    // Hacemos que se llame en esta funcion:
                    dibujaWindow(dDeltaTime);
                }
                //----------------------------------------------------------
                swap(m_hDC);
                //--------------------------------------------------------------
                unSetRenderContext();
                //--------------------------------------------------------------
            }
            saleSeccion();
        }
    }

    return 0;
}
//==========================================================================
int cGLWindow::dibujaWindow(double dDeltaTime)
{
    return 0;
}
//==========================================================================


//--------------------------------------------------------------------------
// El Swap de los buffers
//--------------------------------------------------------------------------
void cGLWindow::swap(HDC hDc)
{
    if (m_bGlFinish)    // Por defecto a false
    {
        glFinish();     // obliga pintar. espera a que el Hw termine
    }
    else                //
    {                   // Por defecto estamos en este ...
        glFlush();      // obliga a pintar, no espera a que el Hw termine.
    }

    if (!SwapBuffers(hDc))
    {
        cError oError;
        oError.mensaje("cGLWindow::Dibuja::SwapBuffers");
    }

    // mvError(cOpenGL::Error("Fin de ciclo"));
    cOpenGL::Error(m_iErrorCicloSwap, "Fin de ciclo");
    m_iErrorCicloSwap = cOpenGL::GetError();

    // Restablecimiento de valores al fin del ciclo:
    //--------------------------------------------------------------
    setCambiaEscena(false); // Cambio de escenas Ventana/cGLWindow
    //--------------------------------------------------------------
}
#ifndef _MINGW_
#pragma endregion Dibujado de la Ventana
#endif


/*========================================================================*\
|* Mensajes
|* - Onsize
|* - OnCreate
|* - OnDestroy
|* - OnChar
|* - OnKeyDown
|* - OnKeyUp
|* - OnMouseMove
|* - OnLeftButtonDown
|* - OnLeftButtonUp
|* - OnMouseWheel
|* - OnPaint
|* - OnTimer
|* - OnClose
\*========================================================================*/
#ifndef _MINGW_
#pragma region Funciones por Mensajes Recibidos
#endif
//--------------------------------------------------------------------------
// Sobre-escribimos el metodo de OnCreate
//--------------------------------------------------------------------------
coverride long cGLWindow::OnCreate	(HWND p_hWnd)
{
    const char * _metodo = __FUNCTION__;

    int iRes;

    cLog::traza(m_iTraza, "\n");
    cLog::traza(m_iTraza, "   %s: [%s][%d]\n", _metodo, m_tVentana.vcWindowName, getIdent());

    iRes =
        (
                // Aqui lanza m_hDC = crearDeviceContext
                cVentana::OnCreate(p_hWnd) != 0
            ||  crearPixelFormat(m_hDC) != 0
            ||  crearRenderContext(m_hDC) != 0
            ||  setRenderContext() != 0
            ||  initWindow() != 0
            ||  unSetRenderContext() != 0
        );

    // Normal que produzca error porque hemos cerrado el contexto al terminar 
    // la carga, no tiene sentido preguntar por el error
    // A no ser que admitamos omitamos este tipo de error.
    // cOpenGL::Error("%s: comprobando error Inicios de Ventana: %s", _metodo, m_tVentana.vcWindowName);

    return iRes;
}


//--------------------------------------------------------------------------
// Sobre-escribimos el metodo de OnSize
//--------------------------------------------------------------------------
coverride long cGLWindow::OnSize(int iXwin, int iYwin)
{
    const char * _metodo = __FUNCTION__;

    cLog::traza(m_iTraza, "\n");
    cLog::traza(m_iTraza, " %s::%s: [%s][%d]\n", _modulo, _metodo, m_tVentana.vcWindowName, getIdent());

    setAnchoOnSize(iXwin);
    setAltoOnSize(iYwin);

    // Si es eGLWINDOW_TRACE, el aspect debe ser 1.
    if (m_eTipoVentana != eTypeVentana::eGLWINDOW_TRACE)
    {
        m_dAspectX = (double)getAnchoOnSize() / (double)getScreenX();
        m_dAspectY = (double)getAltoOnSize() / (double)getScreenY();
    }

    cLog::traza(m_iTraza, " - iXwin (AnchoOnSize): %4d  %4d\n", iXwin, getAnchoOnSize());
    cLog::traza(m_iTraza, " - iYwin (AltoOnSize) : %4d  %4d\n", iYwin, getAltoOnSize());
    cLog::traza(m_iTraza, " -            ScreenX : %6.2f\n", (double)getScreenX());
    cLog::traza(m_iTraza, " -            ScreenY : %6.2f\n", (double)getScreenY());
    cLog::traza(m_iTraza, " -            AspectX : %6.2f\n", m_dAspectX);
    cLog::traza(m_iTraza, " -            AspectY : %6.2f\n", m_dAspectY);

    //m_xSeccion.entrar();
    //{
    if (m_hDC && m_hRc)
    {
        entraSeccion();
        {
            //----------------------------------------------------------
            setRenderContext();
            //----------------------------------------------------------
            if (m_poLisEscenas && m_poLisEscenas->Count() > 0)
            {
                cLisEscenas::cIterator itEscena;
                cEscena* poEscena;

                for (itEscena = m_poLisEscenas->begin(); !itEscena.esFin(); itEscena++)
                {
                    poEscena = (cEscena*)**itEscena;
                    if (poEscena)
                    {
                        poEscena->setWindow(this);
                        poEscena->updateControles(m_tPersp, true, "cGLWindow::OnSize:: Siempre True");
                        poEscena->viewport(
                            true,
                            poEscena->getXpos(), poEscena->getYpos(),
                            poEscena->getAncho(), poEscena->getAlto(),
                            poEscena->getSceneType(), this);
                        poEscena->usaMatrices(poEscena->DarPerspectiva(), true);
                        poEscena->setUpdate(false);
                    }
                }
            }
            else
            {
                viewport(true, 0, 0, m_iAnchoOnSize, m_iAltoOnSize, eSceneType::eWindowScene, this);
                // viewport(true, 0, 0, m_iScreenX, m_iScreenY, eSceneType::eWindowScene, this);
                usaMatrices(m_tPersp, true);
                onsizeWindow(iXwin, iYwin);
            }
            //----------------------------------------------------------
            unSetRenderContext();
            //----------------------------------------------------------
        }
        saleSeccion();
    }
    //}
    //m_xSeccion.salir();

    return 0;
}
//--------------------------------------------------------------------------
int cGLWindow::onsizeWindow(int iAncho, int iAlto)
{
    return 0;
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Mensaje WM_DESTROY
//--------------------------------------------------------------------------
// IMPORTANTE, MUY IMPORTANTE:
// Por haber puesto 'long' y no 'void' en el tipo de funcion,
// el OnDestroy llamado en la clase base 'cVentana' es el OnDestroy de
// 'oGLWindow' de la clase heredada y no el de la clase base. (¿Porqué?)
//--------------------------------------------------------------------------
coverride long cGLWindow::OnDestroy	( void )
{
    cLog::traza(m_iTraza, "\n");
    cLog::traza(m_iTraza, "   cGLWindow::OnDestroy: [%s][%d]\n", m_tVentana.vcWindowName, getIdent());
    destruyeRenderContext();
    recuperaDesktop(true);
	return 0;
}


//--------------------------------------------------------------------------
// Mensajes Teclado
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// OnChar
//--------------------------------------------------------------------------
coverride long cGLWindow::OnChar  ( long lTecla )
{
    return cVentana::OnChar(lTecla);
}


//--------------------------------------------------------------------------
// OnKeyDown
//--------------------------------------------------------------------------
// (wParam, lParam)
//--------------------------------------------------------------------------
coverride long cGLWindow::OnKeyDown	( long lTecla, long lParam )
{
    return cVentana::OnKeyDown(lTecla, lParam);
}


//--------------------------------------------------------------------------
// OnKeyUp
//--------------------------------------------------------------------------
coverride long cGLWindow::OnKeyUp ( long lTecla )
{
    return cVentana::OnKeyUp(lTecla);
}


//--------------------------------------------------------------------------
// Mensajes Raton
//--------------------------------------------------------------------------
// Hay que unificar en:
//
//  m_poRaton->OnUpdate(dDeltaTime)
//
// O algo parecido
//--------------------------------------------------------------------------
// OnMouseMove
//--------------------------------------------------------------------------
coverride long cGLWindow::OnMouseMove	( int iXpos, int iYpos )
{
    return cVentana::OnMouseMove(iXpos, iYpos);
}


//--------------------------------------------------------------------------
// OnLeftButtonDown
//--------------------------------------------------------------------------
coverride long cGLWindow::OnLeftButtonDown( int iXpos, int iYpos, WPARAM wParam )
{
    cVentana::OnLeftButtonDown(iXpos, iYpos, wParam);

	return 0;
}


//--------------------------------------------------------------------------
// OnLeftButtonUp
//--------------------------------------------------------------------------
coverride long cGLWindow::OnLeftButtonUp	( int iXpos, int iYpos, WPARAM wParam )
{
    cVentana::OnLeftButtonUp(iXpos, iYpos, wParam);

	return 0;
}


coverride long cGLWindow::OnLeftButtonDblclk(int iXpos, int iYpos, WPARAM wParam)
{
    return cVentana::OnLeftButtonDblclk(iXpos, iYpos, wParam);
}


//--------------------------------------------------------------------------
// OnMouseWheel
//
// Posibles conflictos entre este evento y los controles de free camara !!!
//
//--------------------------------------------------------------------------
coverride long cGLWindow::OnMouseWheel( int iKeys, int iDelta, int xPos, int yPos )
{
    cVentana::OnMouseWheel(iKeys, iDelta, xPos, yPos);

    return 0;
}



//--------------------------------------------------------------------------
// Otros mensajes
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Sobre-escribimos el metodo de OnPaint
//--------------------------------------------------------------------------
coverride long cGLWindow::OnPaint(void)
{
    // Generalmente, no utilizamos el OnPaint.
    // Se dibuja en cada ciclo, quizás se pudiera separar, el dibujado
    // de una GameWindow y el dibujado de una DialogWindow.

    // Por aqui los dibujados de los controles estaticos.
    // mandando mensaje WM_PAINT (DialogWindow)
    // - habria que separar los controles -
    //   + controles estaticos
    //   + controles dinamicos

    // Y en cada ciclo el dibujado bueno y rapido
    // - GameWindow y los que se clasifiquen para dibujar fuera de aqui.
    return cVentana::OnPaint();
}


//--------------------------------------------------------------------------
// Mensaje WM_TIMER
//--------------------------------------------------------------------------
coverride long cGLWindow::OnTimer(int iIdTimer)
{
    return cVentana::OnTimer(iIdTimer);
}


//--------------------------------------------------------------------------
// Close de la ventana
//--------------------------------------------------------------------------
coverride long cGLWindow::OnClose ( void )
{
	return cVentana::OnClose();
}
#ifndef _MINGW_
#pragma endregion Funciones por Mensajes Recibidos
#endif



//--------------------------------------------------------------------------
// Fin de cGLWindow.cpp
//--------------------------------------------------------------------------
