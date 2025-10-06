//--------------------------------------------------------------------------
// cTeclado.cpp
//--------------------------------------------------------------------------


#include "cTeclado.h"
#include "nCommonKeys.h"
#include "../../tool/cLog.h"
#include <ctype.h>
#include <stdio.h>

#ifndef _CYGWIN_
#include <conio.h>
#endif

#ifdef _MINGW_
#include "../Deprecated/fDeprecated.h"
#endif


//--------------------------------------------------------------------------
// Lista de patrones de teclado, a introducir en el fichero de datos para
// su reconocimiento.
//--------------------------------------------------------------------------
static sItem s_vtTeclado[] =
{
    {   VK_RETURN   ,   "etIntro"       , NULL  },
    {	VK_ESCAPE	,	"etEsc"			, NULL	},
    {	VK_PRIOR	,	"etRePag"		, NULL	},
    {	VK_NEXT		,	"etAvPag"		, NULL	},
    {	VK_END		,	"etFin"			, NULL	},
    {	VK_HOME		,	"etInicio"		, NULL	},
    {	VK_LEFT		,	"etIzquierda"	, NULL	},
    {	VK_UP		,	"etArriba"		, NULL	},
    {	VK_RIGHT	,	"etDerecha"		, NULL	},
    {	VK_DOWN		,	"etAbajo"		, NULL	},
    {	'0'			,	"et0"			, NULL	},
    {	'1'			,	"et1"			, NULL	},
    {	'2'			,	"et2"			, NULL	},
    {	'3'			,	"et3"			, NULL	},
    {	'4'			,	"et4"			, NULL	},
    {	'5'			,	"et5"			, NULL	},
    {	'6'			,	"et6"			, NULL	},
    {	'7'			,	"et7"			, NULL	},
    {	'8'			,	"et8"			, NULL	},
    {	'9'			,	"et9"			, NULL	},
    {	VK_SPACE	,	"etEspacio"		, NULL	},
    {	'A'			,	"etA"			, NULL	},
    {	'B'			,	"etB"			, NULL	},
    {	'C'			,	"etC"			, NULL	},
    {	'D'			,	"etD"			, NULL	},
    {	'E'			,	"etE"			, NULL	},
    {	'F'			,	"etF"			, NULL	},
    {	'G'			,	"etG"			, NULL	},
    {	'H'			,	"etH"			, NULL	},
    {	'I'			,	"etI"			, NULL	},
    {	'J'			,	"etJ"			, NULL	},
    {	'K'			,	"etK"			, NULL	},
    {	'L'			,	"etL"			, NULL	},
    {	'M'			,	"etM"			, NULL	},
    {	'N'			,	"etN"			, NULL	},
    {	'Ñ'			,	"etÑ"			, NULL	},
    {	'O'			,	"etO"			, NULL	},
    {	'P'			,	"etP"			, NULL	},
    {	'Q'			,	"etQ"			, NULL	},
    {	'R'			,	"etR"			, NULL	},
    {	'S'			,	"etS"			, NULL	},
    {	'T'			,	"etT"			, NULL	},
    {	'U'			,	"etU"			, NULL	},
    {	'V'			,	"etV"			, NULL	},
    {	'W'			,	"etW"			, NULL	},
    {	'X'			,	"etX"			, NULL	},
    {	'Y'			,	"etY"			, NULL	},
    {	'Z'			,	"etZ"			, NULL	},
    {	VK_LSHIFT	,	"etIzqShift"	, NULL	},
    {	VK_RSHIFT	,	"etDerShift"	, NULL	},
    {	VK_LCONTROL	,	"etIzqContr"	, NULL	},
    {	VK_RCONTROL	,	"etDerContr"	, NULL	},
    {	VK_F1		,	"etF01"			, NULL	},
    {	VK_F2		,	"etF02"			, NULL	},
    {	VK_F3		,	"etF03"			, NULL	},
    {	VK_F4		,	"etF04"			, NULL	},
    {	VK_F5		,	"etF05"			, NULL	},
    {	VK_F6		,	"etF06"			, NULL	},
    {	VK_F7		,	"etF07"			, NULL	},
    {	VK_F8		,	"etF08"			, NULL	},
    {	VK_F9		,	"etF09"			, NULL	},
    {	VK_F10		,	"etF10"			, NULL	},
    {	VK_F11		,	"etF11"			, NULL	},
    {	VK_F12		,	"etF12"			, NULL	},
    {	VK_ADD		,	"etMas"			, NULL	},
    {	VK_SUBTRACT	,	"etMenos"		, NULL	},
    {	VK_TAB		,	"etTab"			, NULL	},
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constructor / Destructor
//--------------------------------------------------------------------------
cTeclado::cTeclado	( void )
{
    m_iVkey=-1;
    m_iCar=-1;
    
    mInicio(m_vbTeclaDown);
    mInicio(m_vbTeclaUp);

    mInicio(m_vcPrompt);
    mInicio(m_vcKeyboard);

    GetKeyboardState(m_vcKeyboard);
    m_bCapital = !!m_vcKeyboard[VK_CAPITAL];
    m_bExtendido = false;

    m_bKeyboardEvent = false;
}


cTeclado::~cTeclado	( void )
{
    cLog::print(" Destructor de cTeclado\n");
}



//--------------------------------------------------------------------------
// Metodos
//--------------------------------------------------------------------------
// mSingleton(cTeclado);
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Mensajes
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Intentamos recoger todos los eventos del TECLADO
//--------------------------------------------------------------------------
long cTeclado::onKeyboardEvent (uint message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CHAR:
            OnChar((long)wParam);          // Character code of the key
            setKeyEvent(true);
            break;

        case WM_KEYDOWN:
            OnKeyDown((int)wParam);
            OnChar(0);               // Desactivamos la ejecucion,  para OnChar
            setKeyEvent(true);
            break;

        case WM_KEYUP:
            OnKeyUp((int)wParam);
            setKeyEvent(true);
            break;
    }

    return 0;
}


// Preguntamos por la tecla, generalmente el indice corresponde con la tecla
//--------------------------------------------------------------------------
bool cTeclado::isDown(int iIndex)
{
    if (iIndex > -1 && sizeof(m_vbTeclaDown) > (uint)iIndex)
    {
        return m_vbTeclaDown[iIndex];
    }
    return false;
}


bool cTeclado::isUp(int iIndex)
{
    if (iIndex > -1 && sizeof(m_vbTeclaUp) > (uint)iIndex)
    {
        return m_vbTeclaUp[iIndex];
    }
    return false;
}


void cTeclado::OnChar ( long lChar )
{
    // Igual que la m_iVkey, indicara el utlimo char Ascii pulsado.
    // Debe de ser llamado con WM_CHAR
    m_iCar = lChar;
}


void cTeclado::OnKeyDown ( int iIndex )
{
	if	(iIndex>-1 && sizeof(m_vbTeclaDown)>(uint)iIndex)
	{
        // Solo me indicara la ultima pulsada, pero nada mas.
        // El siguiente KeyDown borrara esta informacion. NO: Falso !! hay que hacer un reset.
        // Debe ser llamado con WM_KEYDOWN
        m_iVkey = iIndex;
		m_vbTeclaDown[iIndex] = true;
    }
}


void cTeclado::OnKeyUp	( int iIndex )
{
	if	(iIndex>-1 && iIndex<(int)sizeof(m_vbTeclaUp))
	{
        // Deshace el CHAR, no lo tengo claro.
		m_iCar = -1;
        m_iVkey = -1;

		m_vbTeclaUp[iIndex] = true;
    }
}


//--------------------------------------------------------------------------
// Devuelve el valor de la tecla seleccionada por la cadena
//--------------------------------------------------------------------------
int cTeclado::tecla ( char * pcClaveP )
{
	int i, iTecla, iNum;

    iNum = mSizeArray(s_vtTeclado);     // sizeof(s_vtTeclado) / sizeof(miIndNombre);
	for	( i=0, iTecla=-1; i<iNum && iTecla==-1; i++ )
	{
		if	( !strcmp(s_vtTeclado[i].vcNombre, pcClaveP) )
		{
			iTecla = s_vtTeclado[i].iIndex;
		}
	}

	return iTecla;
}


void cTeclado::reset()
{
    mInicio(m_vbTeclaDown);
    mInicio(m_vbTeclaUp);
}


//--------------------------------------------------------------------------
// Consola
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Establece un prompt para la consola de la aplicacion
//--------------------------------------------------------------------------
void cTeclado::SetPrompt ( char * p_pcFormato, ... )
{
	mInicio(m_vcPrompt);
	va_list stLista;

	va_start (stLista,p_pcFormato);
	vsprintf_s (m_vcPrompt, sizeof(m_vcPrompt), p_pcFormato, stLista);
	va_end   (stLista);
}


void cTeclado::Prompt ( void )
{
	printf("\n");

	if	(mNoVacia(m_vcPrompt))
		printf("%s",m_vcPrompt);
}



//--------------------------------------------------------------------------
// Preguntamos si nos quedan eventos de consola que procesar.
//--------------------------------------------------------------------------
#ifdef _WINMAIN_
bool cTeclado::HayEventosConsola(void)
{
    return m_oConio.HayEventosConsola();
}
#endif


//--------------------------------------------------------------------------
// Da el GetAsyncKeyState da la Vkey dada
// Puede que haya mas de una tecla pulsada.
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Utilizamos las fuciones de lectura de estado de teclado para las
// Teclas de Shisft, Ctrl, y Alt
//--------------------------------------------------------------------------
bool cTeclado::getVkey ( int iVkey )
{
    return !!GetAsyncKeyState(iVkey);
}


bool cTeclado::isShift(void)
{
    return (getVkey(VK_LSHIFT) || getVkey(VK_RSHIFT));
}


bool cTeclado::isAlt(void)
{
    return (getVkey(VK_LMENU) || getVkey(VK_RMENU));
}


bool cTeclado::isCtrl(void)
{
    return (getVkey(VK_LCONTROL) || getVkey(VK_RCONTROL));
}



//--------------------------------------------------------------------------
// Antiguos metodos inline
//--------------------------------------------------------------------------
#ifdef _WINMAIN_
void cTeclado::SetHandleConsola(HANDLE hInHandle)
{
    m_oConio.setInHandle(hInHandle);
}


ulong cTeclado::NumEvento(void)
{
    return m_oConio.NumEvento();
}


ulong cTeclado::NumEventosLeidos(void)
{
    return m_oConio.NumEventosLeidos();
}
#endif


//--------------------------------------------------------------------------
// Fin de cTeclado.cpp
//--------------------------------------------------------------------------
