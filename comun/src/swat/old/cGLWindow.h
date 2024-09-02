/*========================================================================*\
|* cGLWindows.h
\*========================================================================*/


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "cVentana.h"
#include "IViewport.h"
#include "../Procesos/cCSeccion.h"
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------------------
class cApp;
//--------------------------------------------------------------------------
typedef long (*PFNProcMensajes)(cApp* instancia, HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
//--------------------------------------------------------------------------
// Callback de Mensajes
//--------------------------------------------------------------------------
typedef struct stFunctorProcMensajes
{
    cApp* _instancia;
    PFNProcMensajes _metodo;

    stFunctorProcMensajes()
    {
        _instancia = nullptr;
        _metodo = nullptr;
    }

    bool Activo()
    {
        return (_instancia && _metodo);
    }

} TFunctorProcMensajes;
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class cOpenGL;
class cListaTrazas;
//--------------------------------------------------------------------------
class _mExport cGLWindow
    : public cVentana
    , public IViewport
{
    void setFullWindow(int iLeft, int iTop, int iAncho, int iAlto);
    void mostrarErrorFullWindow(long result, int iAncho, int iAlto);
    const char * errorPorChangeDisplaySettings(long result);

protected:

    cGLWindow ( HINSTANCE hInstancia, eTypeVentana eType = eTypeVentana::eGLWINDOW_NORMAL, int iIdent = -1, const char * pcNombre = NULL );

    static	HGLRC       m_hRcmain;			// Hd. contexto de generacion del hDC: principal para toda la aplicacion.
    static	int         m_iNumWindows;		// Numero de ventanas
    static	cCSection	m_xSeccion;			// Critical section, lo mismo no es necesaria, vamos probando ...
    //----------------------------------------------------------------------
    // static	bool        m_bIniSeccion;		// Flag que indica si se ha inicializado la sección critica.
    //----------------------------------------------------------------------

    PIXELFORMATDESCRIPTOR	m_pfd;				// Registro del pixel/Format.
    HGLRC					m_hRc;				// Contexto de renderizacion.
    HGLRC                   m_hRcKeep;          // A utilizar por keepRenderContext(); y unKeepRenderContext();
    DEVMODE					m_dmScreenSettings;	// Modo del dispositivo
    int						m_iPixelFormat;		// Numero del pixelformat activado
    int						m_iBitsColor;
    int						m_iBitsDepth;		// Numero de bits de profundidad.
    bool					m_bFullDesktop;		// Indica si ya está activa la pantalla completa.
    bool                    m_bGlFinish;        // uso de glFinish o glFlush
    TPersp                  m_tPersp;           //
    double                  m_dAspectX;         // Relacion resolucion absoluta X / resolucion relativa X
    double                  m_dAspectY;         // Relacion resolucion absoluta Y / resolucion relativa Y
    int                     m_iAnchoOnSize;     // Ancho final que tienen las ventanas.
    int                     m_iAltoOnSize;      // Alto final que tiene las ventanas.
    
    // Significara el Ancho y el Alto de la escena asociada a la Ventana: viewport
    int                     m_iScreenX;         // Ancho absoluto, con el que se pensaron las posiciones de los controles
    int                     m_iScreenY;         // Alto absoluto, con el que se pensaron las posiciones de los controles.

    float                   m_fAlpha;           // Alpha del Clear Color
    int                     m_iErrorCicloSwap;  // Error final en el ciclo de Swap, para solo marcrlo una vez y no estar todo el rato señalando el error.

    //----------------------------------------------------------------------
    // Callbacks de suatWindows
    //----------------------------------------------------------------------
    TFunctorProcMensajes _tFunctorProcMensajes;
    //----------------------------------------------------------------------

    bool m_bTrazas; // Trazas activas para addTraza y getTraza

    int	setFullDesktop(void);
    int	recuperaDesktop(bool bDestruirVentana = false);
    int	crearPixelFormat(HDC hDC);
    int	crearRenderContext(HDC hDC);
    int	destruyeRenderContext(void);
    bool enPantallaCompleta(void);

    //----------------------------------------------------------------------
    // Virtuales Heredados : sobreescritos
    //----------------------------------------------------------------------
    // Mensajes
    //----------------------------------------------------------------------
    _override long OnCreate             (HWND hWnd);
    _override long OnDestroy            (void);
    _override long OnSize               (int iXwin, int iYwin);
    _override long OnChar               (long lTecla);
    _override long OnKeyDown            (long lTecla, long lParam);
    _override long OnKeyUp              (long lTecla);
    _override long OnMouseMove          (int iXpos, int iYpos);
    _override long OnLeftButtonDown     (int iXpos, int iYpos, WPARAM wParam);
    _override long OnLeftButtonUp       (int iXpos, int iYpos, WPARAM wParam);
    _override long OnMouseWheel         (int iKeys, int iDelta, int xPos, int yPos);
    _override long OnLeftButtonDblclk   (int iXpos, int iYpos, WPARAM wParam);
    _override long OnTimer              (int iIdTimer);
    _override long OnClose              (void);
    _override long OnPaint              (void);
    //----------------------------------------------------------------------

public:

    virtual ~cGLWindow(void);

    // Deberiamos meter otras, como virtuales puras
    virtual int initWindow(void) = 0;

    int     setSincronizacion   (bool bGlFinish) { m_bGlFinish = bGlFinish; return 0; }
    bool    getSincronizacion   (void) { return m_bGlFinish; }
    double  getAspectX(void);
    double  getAspectY(void);
    void    setAnchoOnSize(int iValor) { m_iAnchoOnSize = iValor; }
    void    setAltoOnSize(int iValor) { m_iAltoOnSize = iValor; }
    int     getAnchoOnSize(void) { return m_iAnchoOnSize; }
    int     getAltoOnSize(void) { return m_iAltoOnSize; }
    void    setScreenX(int iValor) { m_iScreenX = iValor; }
    void    setScreenY(int iValor) { m_iScreenY = iValor; }
    int     getScreenX(void) { return m_iScreenX; }
    int     getScreenY(void) { return m_iScreenY; }
    bool    estaEnPantallaCompleta ( void ) { return m_bFullDesktop; }

    int	setRenderContext(bool bSinContexto = false);
    int	setRenderContext(HDC hDC, HGLRC hRC);
    int	unSetRenderContext(bool bSinContexto = false);
    int	cambiaDesktop(void);
    int	entraSeccion(void);
    int saleSeccion(void);

    int keepRenderContext(void);
    int unKeepRenderContext(void);

    void clean(void);
    void swap(HDC hDc);
    void setPerspectiva(TPersp tPerspectiva)	{ m_tPersp = tPerspectiva; }
    void setFov(double dFov) { m_tPersp.dFov = dFov; }
    void setPCercano(double dPCercano) { m_tPersp.dPCercano = dPCercano; }
    void setPLejano(double dPLejano) { m_tPersp.dPLejano = dPLejano; }
    double getFov(void) { return m_tPersp.dFov; }
    double getPCercano(void) { return m_tPersp.dPCercano; }
    double getPLejano(void) { return m_tPersp.dPLejano; }
    const TPersp & getPersp(void) { return m_tPersp; }
    void setPrimerPlano(cEscena * poPrimerPlano);

    //----------------------------------------------------------------------
    // Virtuales Propios
    //----------------------------------------------------------------------
    // Creacion
    //----------------------------------------------------------------------
    virtual	int	crearGLWindow(
        const char* pcTitulo,
        const char* pcClase,
        int         iLeft,
        int         iTop,
        int         iAncho,
        int         iAlto,
        int         nBitsColor,
        int         nBitsDepth,
        bool        bPCompleta,
        eEstiloW    p_eType,
        double      fFov,
        double      fPCercano,
        double      fPLejano,
        HINSTANCE   hInstancia,
        WNDPROC     pfWndProc,
        HWND        hWinPadre);
    //----------------------------------------------------------------------
    //virtual	int	crearGLControl(
    //    char	  *	pcTitulo,
    //    char	  *	pcClase,
    //    int			iBitsColor,
    //    int			iBitsDepth,
    //    // cGLControl * poControl,      ????
    //    cControl  * poControl,
    //    cDialogo  *	poDialog);
    //----------------------------------------------------------------------
    // La estructura sera TCreaVentana, o se cambiara en las heredadas.
    //----------------------------------------------------------------------
    int creaVentana(void * pEstruct) override;
    int inicioCicloVentana(void) override;
    int dibuja(double dDeltaTime) override;
    //----------------------------------------------------------------------
    virtual int onsizeWindow(int iAncho, int iAlto);
    virtual int dibujaWindow(double dDeltaTime);
    //----------------------------------------------------------------------

    void setTraza(bool bTrazas) { m_bTrazas = bTrazas; }
    bool hayTrazas(void) { return m_bTrazas; }
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cGLWindows.h
\*========================================================================*/
