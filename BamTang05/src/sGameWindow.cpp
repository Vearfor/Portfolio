/*------------------------------------------------------------------------*\
|* sGameWindow.cpp
\*------------------------------------------------------------------------*/

#include "sGameWindow.h"
#include "swat/cTeclado.h"
#include "swat/sOpenGL.h"


/*------------------------------------------------------------------------*/
sGameWindow::sGameWindow()
{
    m_pTeclado = new cTeclado();
}

sGameWindow::~sGameWindow()
{
    delete m_pTeclado;
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*/
long sGameWindow::commonWindowProc(HWND hwnd, uint uiMsg, WPARAM wparam, LPARAM lparam)
{
    m_pTeclado->onKeyboardEvent(uiMsg, wparam, lparam);

    return myWindowProc(hwnd, uiMsg, wparam, lparam);
}


/*------------------------------------------------------------------------*/
int sGameWindow::initWindow()
{
    glClearColor(0.0, 1.0, 1.0, 1.0);
    return 0;
}


/*------------------------------------------------------------------------*/
//long sGameWindow::OnKeyUp(long lTecla)
//{
//    short sTecla = (short)lTecla;
//
//    if (sTecla == VK_ESCAPE)
//    {
//        destruyeVentana();
//    }
//
//    return sWindow::OnKeyUp(lTecla);
//}


/*------------------------------------------------------------------------*\
|* Fin de sGameWindow.cpp
\*------------------------------------------------------------------------*/
