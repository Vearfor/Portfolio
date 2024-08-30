/*------------------------------------------------------------------------*\
|* sConsoleWindow.h
\*------------------------------------------------------------------------*/

#include "sConsoleWindow.h"
#include <swat/input/cTeclado.h>


sConsoleWindow::sConsoleWindow()
    : m_pTeclado(new cTeclado())
{
}

sConsoleWindow::~sConsoleWindow()
{
    delete m_pTeclado;
}

/*------------------------------------------------------------------------*/
long sConsoleWindow::commonWindowProc(HWND hwnd, uint uiMsg, WPARAM wparam, LPARAM lparam)
{
    m_pTeclado->onKeyboardEvent(uiMsg, wparam, lparam);

    //switch (uiMsg)
    //{
    //    default:
    //        break;
    //}

    return myWindowProc(hwnd, uiMsg, wparam, lparam);
}


/*------------------------------------------------------------------------*/
int sConsoleWindow::initWindow()
{
    return 0;
}


/*------------------------------------------------------------------------*\
|* Fin de sConsoleWindow.h
\*------------------------------------------------------------------------*/
