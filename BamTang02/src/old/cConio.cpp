//--------------------------------------------------------------------------
// cConio.cpp
//--------------------------------------------------------------------------


#ifdef _CYGWIN_
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#else
#include <conio.h>
#endif

#include "cConio.h"
#include "cLog.h"

//#include "../Consola/cShell.h"
//#include "../Consola/cConsola.h"
//#ifndef _SIN_cError
//#include "../Util/cError.h"
//#endif
//#include "../Memoria/fMemoria.h"


//--------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------
#ifdef _CYGWIN_
static struct termios oldconsola,consola;
#endif
bool    cConio::m_bIniciado = false;
HANDLE  cConio::m_hInHandle = NULL;
HANDLE  cConio::m_hOutHandle = NULL;

CONSOLE_SCREEN_BUFFER_INFO	cConio::m_csbi;

// Deberia ser el anterior: &cConio::m_csbi
void  * cConio::m_pTextAtributos = nullptr;

word    cConio::m_wColor = eTexNormal;
word    cConio::m_wErrorColor = eTexRojo;
word    cConio::m_wNormalColor = eTexNormal;

INPUT_RECORD    cConio::m_tRecord[LON_BUFF / 8];   // 1024/8 = 128
int     cConio::m_iCar = 0;
int     cConio::m_iVKey = 0;
int     cConio::m_iCKState = 0;
long    cConio::m_l = 0;
long    cConio::m_lNumLeidos = 0;
long  * cConio::m_plNumLeidos = &cConio::m_lNumLeidos;
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constructor / Destructor
//--------------------------------------------------------------------------
// No hay
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
// mSingleton(cConio);
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Abrimos el servicio de la consola
//--------------------------------------------------------------------------
int	cConio::open	( void )
{
	if	(!m_bIniciado)
	{
#ifdef _CYGWIN_
		// Leer los parametros asociados a la consola
		if (tcgetattr(STDIN_FILENO,&consola)==-1)
		{
			perror("Error en tcgetattr\n");
			return -1;
		}
		oldconsola = consola;	// Parametros para su posterior recup.

		// Cambiar parametros.
		consola.c_cc[VMIN]=1;
		consola.c_lflag&=~(ECHO|ICANON); 

		if (tcsetattr(STDIN_FILENO,TCSANOW,&consola)==-1)
		{
			perror("Error en tcsetattr\n");
			return -1;
		}

		// printf(" - Consola de teclado iniciado.\n");
#endif
        m_bIniciado = true;
        // Lo mismo nos sobra
        //--------------------------------------------------------------
        m_hOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (m_hOutHandle == INVALID_HANDLE_VALUE)
        {
#ifndef _SIN_cError
            cLog::error("cConio::open: Error: GetStdHandle STD_OUTPUT_HANDLE\n");
            return -1;
#endif // _SIN_cError
        }
        //--------------------------------------------------------------
        m_hInHandle = GetStdHandle(STD_INPUT_HANDLE);
        if (m_hInHandle == INVALID_HANDLE_VALUE)
        {
#ifndef _SIN_cError
            cLog::error("cConio::open: Error: GetStdHandle STD_INPUT_HANDLE\n");
            return -1;
#endif // _SIN_cError
        }
        //--------------------------------------------------------------
// #ifdef _WINMAIN_
        memset(m_tRecord, 0, sizeof(m_tRecord));   // 1024/8 = 128
        m_iCar          = 0;
        m_iVKey         = 0;
        m_iCKState      = 0;
        m_l             = 0;
        m_lNumLeidos    = 0;
        m_plNumLeidos   = &m_lNumLeidos;
// #endif
        SetColor(m_wColor);
	}

	return 0;
}


//--------------------------------------------------------------------------
// Cerramos
//--------------------------------------------------------------------------
int	cConio::close	( void )
{
	if	(m_bIniciado)
	{
#ifdef _CYGWIN_
		if (tcsetattr(STDIN_FILENO,TCSANOW,&oldconsola)==-1) 
		{
			perror("Error en tcsetattr");
		}

		// printf(" - Consola de teclado cerrado.\n");
#endif
		m_bIniciado = false;
        m_hInHandle = NULL;
        m_hOutHandle = NULL;
    }

	return 0;
}


//--------------------------------------------------------------------------
// Vamos a generar una funcion de 'keystroke', mediante una espera con
// el handle de entrada de la consola
//--------------------------------------------------------------------------
int cConio::Kbhit()
{
    // Leche
    if (m_bIniciado)
    {
#ifdef _CYGWIN_
        int n;
        fd_set rfd;
        struct timeval timeout;

        timeout.tv_sec = 0;
        timeout.tv_usec = TIMEOUT_USEC;

        FD_ZERO(&rfd);
        FD_SET(STDIN_FILENO, &rfd);

        n = select(STDIN_FILENO + 1, &rfd, NULL, NULL, &timeout);

        return n;
#else
        // #ifdef _WINMAIN_
        // El _kbhit funciona para definida AllocConsole ????
        // SetInHandle(hInConsola);
        // #endif
        return _kbhit();
#endif
    }

    return 0;

    //----------------------------------------------------------------------
    // Si esto funcionase seria una solucion.
    //
    // Tube la esperanza de que el hadle de entrada de la consola
    // funcionase como un objeto event, y si hay un evento de entrada
    // sobre la consola, este produciria el evento por el cual un
    // WaitForSingleObject distinguiria entre pulsar tecla o fin de
    // la espera.
    // Ademas eliminarimos la espera de: m_poHiloTeclado->Espera(50);
    //
    // No ha funcionado, a revisar la estrategia.
    //
    //----------------------------------------------------------------------
    // if (m_oConio.Kbhit(m_hInHandle))
    // {
    //    iCar = m_oConio.Getch();
    //    ConstruyeComando(iCar);
    // }
    //----------------------------------------------------------------------
    // 
    // En realidad no se espera pero si hay un evento que cambie (signaled)
    // el handle de entrada de la consola debido a un evento de teclado
    // devolveremos 1, en vez de 0.
    //
    // if (WaitForSingleObject(hInConsola, 50))
    // {
    //     return 1;
    // }
    //
    //----------------------------------------------------------------------
}


//--------------------------------------------------------------------------
// Getch: sustituimos la macro getch
//--------------------------------------------------------------------------
int	cConio::Getch(bool bEspera)
{
    if (m_bIniciado)
    {
        if (bEspera)
        {
            while (!Kbhit());   // No sigue, hasta pulsar tecla.
        }

#ifdef _CYGWIN_
        int n;
        char c;

        n = read(STDIN_FILENO, &c, 1);

        return c;
#else
        return _getch();
#endif  // ifdef _CYGWIN_
    }

    return 0;
}


int cConio::Cls()
{
    int iRes = 0;

#ifndef __CYGWIN__
    if (m_hOutHandle)
    {
        COORD coordScreen = { 0, 0 };    // home for the cursor 
        DWORD cCharsWritten;
        // CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD dwConSize;

        // Get the number of character cells in the current buffer. 
		if (!GetConsoleScreenBufferInfo(m_hOutHandle, &m_csbi))
		{
            return -1;
        }
        dwConSize = m_csbi.dwSize.X * m_csbi.dwSize.Y;

        iRes =
            (
                    // Fill the entire screen with blanks.
                    !FillConsoleOutputCharacter(m_hOutHandle, TEXT(' '), dwConSize, coordScreen, &cCharsWritten)
                    // Get the current text attribute.
                ||  !GetConsoleScreenBufferInfo(m_hOutHandle, &m_csbi)
                    // Set the buffer's attributes accordingly.
                ||  !FillConsoleOutputAttribute(m_hOutHandle, m_csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten)
                    // Put the cursor at its home coordinates.
                ||  !SetConsoleCursorPosition(m_hOutHandle, coordScreen)
            );
    }
#else
    // Es dificil que esto funcione, creo que era para terminales vT400x y cosas parecidas.
    // fprintf(stdout, "33[2j");
    // fprintf(stdout, "33[1;1H");
#endif

    return (iRes)? -1: 0;
}


int cConio::GotoXY(int x, int y)
{
	int iRes = 0;
	if (m_hOutHandle)
	{
		// Get the number of character cells in the current buffer. 
		if (!GetConsoleScreenBufferInfo(m_hOutHandle, &m_csbi))
		{
			return -1;
		}
		if ((x < (m_csbi.srWindow.Right - m_csbi.srWindow.Left)) && (y < (m_csbi.srWindow.Bottom - m_csbi.srWindow.Top)))
		{
			COORD coordScreen = { (SHORT) x, (SHORT) y };    // home for the cursor 
			// DWORD cCharsWritten;
			// DWORD dwConSize = m_csbi.dwSize.X * m_csbi.dwSize.Y;

			// Put the cursor at its home coordinates.
			iRes = ( !SetConsoleCursorPosition(m_hOutHandle, coordScreen) );
		}
	}
	return iRes;
}


int	cConio::SetLastXY()
{
	int iRes = 0;
	int x = m_csbi.dwCursorPosition.X;
	int y = m_csbi.dwCursorPosition.Y;
	COORD coordScreen = { (SHORT)x, (SHORT) y };

	iRes = (SetConsoleCursorPosition(m_hOutHandle, coordScreen));

	return iRes;
}


int cConio::WriteConsole(char * pcMensaje, long lenMen, ulong * pused)
{
    int iRes = 0;
    if (m_hOutHandle)
    {
        ::WriteConsoleA(m_hOutHandle, pcMensaje, lenMen, pused, 0);
    }
    return (iRes) ? -1 : 0;
}


int cConio::MaxX(int lonSalvar)
{
	// Get the number of character cells in the current buffer. 
	if (!GetConsoleScreenBufferInfo(m_hOutHandle, &m_csbi))
	{
		return -1;
	}

	//int lonfinal = m_csbi.dwSize.X - lonSalvar;
	int lonfinal = (m_csbi.srWindow.Right - m_csbi.srWindow.Left) - lonSalvar;

	return lonfinal;
}


int cConio::MaxY(int lonSalvar)
{
	// Get the number of character cells in the current buffer. 
	if (!GetConsoleScreenBufferInfo(m_hOutHandle, &m_csbi))
	{
		return -1;
	}

	//int lonfinal = m_csbi.dwSize.Y - lonSalvar;
	int lonfinal = (m_csbi.srWindow.Bottom - m_csbi.srWindow.Top) - lonSalvar;

	return lonfinal;
}


/*==========================================================================*\
|* Procesamiento con ReadConsoleInput del teclado, para consolas creadas
|* con cConsola (AllocConsole)
\*==========================================================================*/
// #ifdef _WINMAIN_
//--------------------------------------------------------------------------
// Handle de entrada de la consola
//--------------------------------------------------------------------------
void cConio::setInHandle(HANDLE hInHandle)
{
    if (hInHandle != NULL)
    {
        m_hInHandle = hInHandle;
    }
}


//--------------------------------------------------------------------------
// Handle de salida de la consola
//--------------------------------------------------------------------------
void cConio::setOutHandle(HANDLE hOutHandle)
{
    if (hOutHandle != NULL)
    {
        m_hOutHandle = hOutHandle;
    }
}


//--------------------------------------------------------------------------
// Preguntamos si nos quedan eventos de consola que procesar.
//--------------------------------------------------------------------------
bool cConio::HayEventosConsola(void)
{
    return (m_l<m_lNumLeidos);
}


//--------------------------------------------------------------------------
// Recogida de eventos de la Consola
//--------------------------------------------------------------------------
int cConio::WaitConsole()
{
    if (m_hInHandle && m_hInHandle != INVALID_HANDLE_VALUE)
    {
        if (HayEventosConsola())
        {
            return GetConsole();
        }
        else
        {
            m_l = -1;           // por poner el m_l++ en el GetConsole, antes.
            m_lNumLeidos = 0;
            //FlushConsoleInputBuffer(m_hInHandle);
            if (ReadConsoleInput(m_hInHandle, m_tRecord, sizeof(m_tRecord), (DWORD *)m_plNumLeidos))
            {
                return GetConsole();
            }
            else
            {
#ifndef _SIN_cError
                cLog::error(" WaitConsole: Error: ReadConsoleInput\n");
                return -1;
#endif
            }
        }
    }

    return 0;
}


//--------------------------------------------------------------------------
// GetConsole de ReadInputConsole
//--------------------------------------------------------------------------
int cConio::GetConsole(void)
{
    int iRes = -1;  // Si devolvemos -1, que no pase por ConstruyeComando.

    m_l++;

    switch(m_tRecord[m_l].EventType)
    {
        case KEY_EVENT:
            if (m_tRecord[m_l].Event.KeyEvent.bKeyDown)
            {
                m_iCar = (int)m_tRecord[m_l].Event.KeyEvent.uChar.AsciiChar;
                m_iVKey = (int) m_tRecord[m_l].Event.KeyEvent.wVirtualKeyCode;
                m_iCKState = (int) m_tRecord[m_l].Event.KeyEvent.dwControlKeyState;

                iRes = m_iCar;

                //Escribe (" iCar: %c %d \n", (isprint(iCar))? iCar: 0 , iCar);
                //if (lCKState & CAPSLOCK_ON) Escribe(" [Caps On]");
                //if (lCKState & RIGHT_ALT_PRESSED) Escribe(" [RAlt On]");
                //if (lCKState & RIGHT_CTRL_PRESSED) Escribe(" [RCtrl On]");
                //if (lCKState & LEFT_ALT_PRESSED) Escribe(" [LAlt On]");
                //if (lCKState & LEFT_CTRL_PRESSED) Escribe(" [LCtrl On]");

                // ConstruyeComando(m_iCar, m_lVKey, m_lCKState);
                // Escribe(" iCar: %ld | '%c' |  vk: %ld ", iCar, isprint(iCar)? iCar: 32, lVKey);

                //--------------------------------------------------
                // dwControlKeyState :
                // CAPSLOCK_ON         The caps lock light is on. 
                // ENHANCED_KEY        The key is enhanced. 
                // LEFT_ALT_PRESSED    The left alt key is pressed. 
                // LEFT_CTRL_PRESSED   The left ctrl key is pressed. 
                // NUMLOCK_ON          The num lock light is on. 
                // RIGHT_ALT_PRESSED   The right alt key is pressed. 
                // RIGHT_CTRL_PRESSED  The right ctrl key is pressed. 
                // SCROLLLOCK_ON       The scroll lock light is on. 
                // SHIFT_PRESSED       The shift key is pressed. 
                //--------------------------------------------------
            }
            break;

        case FOCUS_EVENT:
            break;

        // Deben de activarse con algo semejante a:
        //  fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
        //  if (!SetConsoleMode(hStdin, fdwMode))
        //      ErrorExit("SetConsoleMode");
        case MOUSE_EVENT:
        case WINDOW_BUFFER_SIZE_EVENT:
        case MENU_EVENT:
        default:
            break;
    }

    return iRes;
}
// #endif  // _WINMAIN_


void cConio::setCar(int iCar)
{
    m_iCar = iCar;
}


void cConio::setVKey(int iVkey)
{
    m_iVKey = iVkey;
}


void cConio::setCKState(int iState)
{
    m_iCKState = iState;
}


HANDLE cConio::getInHandle(void)
{
    return m_hInHandle;
}


int cConio::getCurrentCar(void)
{
    return m_iCar;
}


int cConio::getCurrentVkey(void)
{
    return m_iVKey; 
}


int cConio::getCurrentCKState(void)
{
    return m_iCKState;
}


long cConio::NumEvento(void)
{
    return m_l;
}


ulong cConio::NumEventosLeidos(void)
{
    return m_lNumLeidos;
}


/*========================================================================*\
|* Set Atributos Color del Texto:
\*========================================================================*/
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Para cambiar los colores del Texto se cambian los atributos del texto
// De la misma manera necesitamos saber, cuales son los que hay para
// restaurarlos:
//--------------------------------------------------------------------------
word cConio::GetAtributos(void)
{
    word atributos = 0;
    // Solo si hemos sido iniciados
    // Y tenemos el handle de salida de la consola
    if (m_bIniciado && m_hOutHandle != NULL)
    {
        //CONSOLE_SCREEN_BUFFER_INFOEX tInfoConsola, *ptInfoConsola;
        CONSOLE_SCREEN_BUFFER_INFO tInfoConsola, *ptInfoConsola;
        ptInfoConsola = &tInfoConsola;
        //GetConsoleScreenBufferInfoEx(m_hOutHandle, ptInfoConsola);
        GetConsoleScreenBufferInfo(m_hOutHandle, ptInfoConsola);
        atributos = ptInfoConsola->wAttributes;
    }
    return atributos;
}


void cConio::SetAtributos(word atributos)
{
    // Solo si hemos sido iniciados
    // Y tenemos el handle de salida de la consola
    if (m_bIniciado && m_hOutHandle != NULL)
    {
        SetConsoleTextAttribute(m_hOutHandle, atributos);
    }
}


//--------------------------------------------------------------------------
// No entendemos bien como esta funcionando, pero si asignamos el que
// leemos de GetConsoleScreenBufferInfoEx
//
// atributos = ptInfoConsola->wAttributes;
//
// en: SetConsoleTextAttribute(m_hOutHandle, atributos);
//
// No Nos sale lo que queremos.
//
// Nos guardamos mejor un m_wColor, como color actual
// y hacemos el set con ellos.
// 
//--------------------------------------------------------------------------
word cConio::GetColor(void)
{
    // No me vale :
    // GetConsoleScreenBufferInfoEx(m_hOutHandle, ptInfoConsola);
    // m_wColor = ptInfoConsola->wAttributes;

    return m_wColor;
}


void cConio::SetColor(word wColor)
{
    m_wColor = wColor;
    // Solo si hemos sido iniciados
    // Y tenemos el handle de salida de la consola
    if (m_bIniciado && m_hOutHandle != NULL)
    {
        SetConsoleTextAttribute(m_hOutHandle, m_wColor);
    }
}


word cConio::GetErrorColor(void)
{
    return m_wErrorColor;
}


void cConio::SetErrorColor(word wColor)
{
    m_wErrorColor = wColor;
}


word cConio::GetNormalColor(void)
{
    return m_wNormalColor;
}


void cConio::SetNormalColor(word wColor)
{
    m_wNormalColor = wColor;
}



//--------------------------------------------------------------------------
// Fin de cConio.cpp
//--------------------------------------------------------------------------

