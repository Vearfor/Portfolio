/*------------------------------------------------------------------------*\
|* sWindow.h
\*------------------------------------------------------------------------*/

#pragma once
//--------------------------------------------------------------------------
// Include
//--------------------------------------------------------------------------
#include "cMonitor.h"
#include <Windows.h>
#include <string>
#include "../../tool/nComun.h"
#include "../../tool/cItem.h"
#include <GLM/gtc/matrix_transform.hpp>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Estilos de ventana
//--------------------------------------------------------------------------
enum class eEstiloW : int
{
    ePopup = 0,		// Estilo 'Pop-up', centrado.
    eMain,		    // Ventana principal: su estilo es popup, y Pantalla completa
    eControl,		// Estilo 'Pop-up', pero en un control, para colocar en cualquier parte.
    eCaption,       // Con nombre en el caption.
    eSystem,        // Con boton de cierre de sistema.
    eWindow,        // Con todo.
    eMainV,		    // Ventana principal: uso de pantalla Virtual, Monitor Secundario.
    eControlV,      // Estilo 'Pop-up', uso de pantalla Virtual, Monitor Secundario (a la Derecha)
    eNumEstilosW    // Numero de estilos del enumerado.
};
//--------------------------------------------------------------------------
// Posiciones Controles/Ventanas.
//--------------------------------------------------------------------------
enum class ePosControl : int
{
    eLEFT = -2,
    eRIGHT = -3,
    eCENTER = -4,
    eTOP = -5,
    eBOT = -6,
    eNUM_POS_CONTROLES
};
/*------------------------------------------------------------------------*/


//--------------------------------------------------------------------------
// sCreaVentana
//--------------------------------------------------------------------------
struct sCreaVentana
{
    // Valores miembros que no se uitlizan en CreateWindow
    cRect<long>     tRect;
    eEstiloW        eType;
    bool            bFull;

    // Valores miembros que si se utilizan en CreateWindow
    dword			dwExEstilo;				// Solo CreateWindowEx.
    char			vcClassName[32];
    char			vcWindowName[LON_BUFF / 4];
    dword			dwEstilo;
    int				x;
    int				y;
    int				iAncho;
    int				iAlto;
    HWND			hWndPadre;
    HMENU			hMenu;
    HINSTANCE		hInstancia;
    CREATESTRUCT* ptCreate;

    sCreaVentana(void)
    {
        eType = eEstiloW::eWindow;
        bFull = false;

        // Registro de creacion de ventana
        mInicio(vcClassName);
        mInicio(vcWindowName);
        dwExEstilo = 0;
        dwEstilo = 0;
        //-------------------
        x = CW_USEDEFAULT;
        y = CW_USEDEFAULT;
        //-------------------
        iAncho = 0;
        iAlto = 0;
        //-------------------
        hWndPadre = NULL;
        hMenu = NULL;
        hInstancia = NULL;
        ptCreate = NULL;		// Que sea externo, por ahora no lo borra el. Podria ser estatico.
    }
};
//--------------------------------------------------------------------------


/*------------------------------------------------------------------------*/
struct sWindow
    : public cItem
{
    sWindow();
    ~sWindow();

    int crea(
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
        const char * pcWindowName,
        const char * pcClassName,
        WNDPROC	pFuncion);

    int muestraVentana();
    int ocultaVentana();
    void clean();
    void swap();
    int destruyeVentana(void);
    int	setRenderContext(bool bSinContexto = false);
    int setViewPort();
    int setViewPort(int x, int y, int width, int height);

    HDC getDeviceContext(void) { return m_hDC; }
    glm::mat4 getPerspProjection() { return m_projection; }
    glm::mat4 getOrthoProjection() { return m_ortho; }

    static bool GetWindowRect(HWND hwnd, cRect<long>* pRectRes);

protected:

    HWND m_hWindow{ NULL };
    int m_iWidth{ 600 };
    int m_iHeight{ 400 };
    int m_iBitsColor{ 0 };
    int m_iBitsDepth{ 0 };
    double m_dFov{ 45.0 };
    double m_dPCercano{ 0.1 };
    double m_dPLejano{ 100.0 };
    bool m_bRegistrada{ false };        // Indica si esta registrada.
    bool m_bVisible{ false };           // Visible o no la ventana.
    HDC m_hDC;		                    // Handle del dispositivo.
    PIXELFORMATDESCRIPTOR m_pfd;		// Registro del pixel/Format.
    uint m_iPixelFormat;                // Numero del pixel format establecido.
    HGLRC m_hRc;                        // Contexto de renderizacion.
    DEVMODE	m_dmScreenSettings;         // Modo del dispositivo
    bool m_bFullDesktop{ false };       // Para marcar si tenemos que estar en pantalla completa. Recuerda que tenemos m_tVentana.bFull.
    cRect<long> m_wCurrentRect;               // Guardamos los actuales: Area de rectangulo de la ventana. Al terminar de crear la ventana.
    cRect<long> m_wInitRect;		            // Guardamos los iniciales, para recuperar despues de pantalla completa.

    // Del OnSize
    // No tiene mucho sentido, pero poco a poco ..
    int m_iScreenX{ 0 };                // Ancho absoluto, con el que se pensaron las posiciones de los controles
    int m_iScreenY{ 0 };                // Alto absoluto, con el que se pensaron las posiciones de los controles.
    double  m_dAspectX{ 1.0 };          // Relacion resolucion absoluta X / resolucion relativa X
    double  m_dAspectY{ 1.0 };          // Relacion resolucion absoluta Y / resolucion relativa Y
    int m_iAnchoOnSize{ 0 };            // Ancho final que tienen las ventanas.
    int m_iAltoOnSize{ 0 };             // Alto final que tiene las ventanas.

    // Que se modifica en cada OnSize:
    glm::mat4 m_projection{ 1.0 };
    glm::mat4 m_ortho{ 1.0 };

    sCreaVentana m_tVentana{};

    int registra(HINSTANCE hInstancia, const char* pcClassName, WNDPROC pFuncion, eEstiloW eEstilo);
    int crea(void* pEstructuraVentana);
    int setWindowSettings(sCreaVentana* ptVentana, cRect<long>* ptWindowRect);
    int	setWindowStyles(sCreaVentana* ptVentana, cRect<long>* ptWindowRect);

    int creaDeviceContext(HWND hwnd);
    int destruyeDeviceContext();
    int creaPixelFormat(HDC hdc);
    int creaRenderContext(HDC hdc);
    int destruyeRenderContext();
    int	setRenderContext(HDC hDC, HGLRC hRC);
    int	unSetRenderContext(bool bSinContexto = false);
    bool enPantallaCompleta();
    int	setFullDesktop();
    void setFullWindow(int iLeft, int iTop, int iAncho, int iAlto);
    void mostrarErrorFullWindow(long result, int iAncho, int iAlto);
    const char* errorPorChangeDisplaySettings(long result);
    int	recuperaDesktop(bool bDestruirVentana = false);
    int	cambiaDesktop();

    double  getAspectX(void) { return m_dAspectX; }
    double  getAspectY(void) { return m_dAspectY; }
    void    setAnchoOnSize(int iValor) { m_iAnchoOnSize = iValor; }
    void    setAltoOnSize(int iValor) { m_iAltoOnSize = iValor; }
    int     getAnchoOnSize(void) { return m_iAnchoOnSize; }
    int     getAltoOnSize(void) { return m_iAltoOnSize; }
    void    setScreenX(int iValor) { m_iScreenX = iValor; }
    void    setScreenY(int iValor) { m_iScreenY = iValor; }
    int     getScreenX(void) { return m_iScreenX; }
    int     getScreenY(void) { return m_iScreenY; }

    virtual long OnCreate(HWND hwnd);
    virtual long OnDestroy();
    virtual long OnSize(int newWidth, int newHeight);
    virtual long OnClose();
    virtual long OnKeyDown(long lTecla);
    virtual long OnKeyUp(long lTecla);

    static sWindow* m_pThis;
    static HGLRC m_hRcmain;                 // Hd. contexto de generacion del hDC: principal para toda la aplicacion.
    static int m_iNumWindows;               // Numero de ventanas
    static WNDPROC m_pFuncion;              // Funcion de registro de mensajes de la ventana

    //----------------------------------------------------------------------
    static long windowProc(HWND hwnd, uint uiMsg, WPARAM wparam, LPARAM lparam);
    //----------------------------------------------------------------------
    virtual long commonWindowProc(HWND hwnd, uint uiMsg, WPARAM wparam, LPARAM lparam) = 0;
    virtual int initWindow() = 0;
    //----------------------------------------------------------------------
    long myWindowProc(HWND hwnd, uint uiMsg, WPARAM wparam, LPARAM lparam);
    //----------------------------------------------------------------------

    static sItem m_vTipoEstilosVentana[static_cast<int>(eEstiloW::eNumEstilosW)];
};
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Fin de sWindow.h
\*------------------------------------------------------------------------*/
