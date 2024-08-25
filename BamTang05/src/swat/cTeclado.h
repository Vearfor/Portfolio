/*------------------------------------------------------------------------*\
|* cTeclado.h
\*------------------------------------------------------------------------*/

#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "../tool/nComun.h"
#include "../tool/cItem.h"
#include <string>
//#include "../Util/cString.h"
//#include "../Memoria/fMemoria.h"
//#include "../Devices/cConio.h"
//#include "../Item/cItem.h"
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// class
//--------------------------------------------------------------------------
class cTeclado
{
    bool    m_vbTeclaDown[256];
    bool    m_vbTeclaUp[256];
    char    m_vcPrompt[256];
    byte    m_vcKeyboard[256];
    bool    m_bCapital;
    bool    m_bExtendido;           // Empieza el teclado extendido.
    std::string m_oString;

    int     m_iVkey;
    int		m_iCar;

    bool    m_bKeyboardEvent;           // Cualquier evento de teclado

public:

    cTeclado(void);
    virtual ~cTeclado(void);

    // Todo esto es revisable:
    // PENDIENTE DE HACER LIMPIEZA !!!!!

    // Windows
    void    setKeyEvent(bool bKeyDown) { m_bKeyboardEvent = bKeyDown; }
    bool    isKeyEvent(void) { return m_bKeyboardEvent; }
    bool    teclaPulsada(void) { return (m_iVkey > 0 || m_iCar > 0); };

    // Mensajes a windows
    long    onKeyboardEvent(uint message, WPARAM wParam, LPARAM lParam);
    bool    isDown(int iIndex);
    bool    isUp(int iIndex);
    void    OnChar(long lChar);
    void    OnKeyDown(int iIndex);
    void    OnKeyUp(int iIndex);
    int	    tecla(char* p_pcClave);
    void    reset();

    // Lo mismo que la tecla pulsada.
    int     UltimaVkey(void) { return m_iVkey; }
    int	    UltimaChar(void) { return m_iCar; }
    int     Reset(void) { m_iCar = -1; return 0; }

    // Uso de consola
    void    SetPrompt(char* p_pcFormat, ...);
    void    Prompt(void);
    //char* DarLinea(void);
    //int	    Pulsa(void);
    //int     DarCar(void);
    void    SetExtendido(bool bExten) { m_bExtendido = bExten; }
    bool    Extendido(void) { return m_bExtendido; }

    // A partir de aqui voy a intentar dejar lo fijo para la cConsola
    // Uso de consola
    void    SetHandleConsola(HANDLE hInHandle);
    //bool    TeclaPulsadaConsola(void);
    bool    HayEventosConsola(void);
    //int     GetCharConsola(void);
    int     GetCurrentCar(void) { return m_iCar; }
    int     GetCurrentVkey(void) { return m_iVkey; }
    ulong   NumEvento(void);
    ulong   NumEventosLeidos(void);

    static bool getVkey(int iVkey);
    static bool isShift(void);
    static bool isAlt(void);
    static bool isCtrl(void);
};
//--------------------------------------------------------------------------


/*------------------------------------------------------------------------*\
|* Fin de cTeclado.h
\*------------------------------------------------------------------------*/
