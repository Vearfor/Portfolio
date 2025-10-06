//--------------------------------------------------------------------------
// cConio.h
//--------------------------------------------------------------------------


#pragma once
//--------------------------------------------------------------------------
// Include
//--------------------------------------------------------------------------
#include "nComun.h"
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Defines
//--------------------------------------------------------------------------
#define TIMEOUT_USEC	10000
#define GETCH_ESPERA    true
#define GETCH_NO_ESPERA false
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Enumerados
//--------------------------------------------------------------------------
// Text Color Consola
//--------------------------------------------------------------------------
enum eTextColor
{
    eTexGris = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
    eTexNormal = eTexGris,
    eTexAzul = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    eTexVerde = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    eTexRojo = FOREGROUND_RED | FOREGROUND_INTENSITY,
    eTexCeleste = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    eTexAmarillo = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY,
    eTexVioleta = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY,
    eTexBlanco = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY,
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class cShell;
//--------------------------------------------------------------------------
class _mExport cConio
{
	static  bool    m_bIniciado;
    static  HANDLE  m_hInHandle;
    static  HANDLE  m_hOutHandle;
    static  HANDLE  m_hErrHandle;

	static	CONSOLE_SCREEN_BUFFER_INFO	m_csbi;

	static  INPUT_RECORD    m_tRecord[LON_BUFF / 8];   // 1024/8 = 128
    static  int             m_iCar;
    static  int             m_iVKey;
    static  int             m_iCKState;
    static  long            m_l;
    static  long            m_lNumLeidos;
    static  long          * m_plNumLeidos;

    static void * m_pTextAtributos;
    static word m_wColor;
    static word m_wErrorColor;
    static word m_wNormalColor;

public:

    static  int     open(void);
    static  int     close(void);

    static  int     Kbhit();
    static  int     Getch(bool bEspera = false);
    static  int     Cls();
	static  int     GotoXY(int x, int y);
	static	int		SetLastXY();
	static  int     WriteConsole(char * vcMensaje, long lenMen, ulong * pused);
	static  int		MaxX(int lon);
	static  int		MaxY(int lon);

    static  int     WaitConsole();
    static  int     GetConsole(void);
    static  bool    HayEventosConsola(void);

    //static  int     EventosConsola(piProcP fInterpreta);
    //static  int     EventosConsola(piProcM fInterpreta);
	//static  int     EventosConsola(piProcII fInterpreta);
    static  void    setInHandle(HANDLE hInHandle);
    static  void    setOutHandle(HANDLE hOutHandle);

    static  void    setCar(int iCar);
    static  void    setVKey(int iVkey);
    static  void    setCKState(int iState);

    static  HANDLE  getInHandle(void);
    static  int     getCurrentCar(void);
    static  int     getCurrentVkey(void);
    static  int     getCurrentCKState(void);

    static  long    NumEvento(void);
    static  ulong   NumEventosLeidos(void);

    static  word    GetAtributos(void);
    static  void    SetAtributos(word atributos);

    static  word    GetColor(void);
    static  void    SetColor(word wColor);

    static  word    GetErrorColor(void);
    static  void    SetErrorColor(word wColor);

    static  word    GetNormalColor(void);
    static  void    SetNormalColor(word wColor);
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Fin de cConio.h.
//--------------------------------------------------------------------------
