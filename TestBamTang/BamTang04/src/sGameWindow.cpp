/*------------------------------------------------------------------------*\
|* sGameWindow.cpp
\*------------------------------------------------------------------------*/

#include "sGameWindow.h"
#include <swat/input/cTeclado.h>
#include <swat/input/cRaton.h>
#include <tool/cLog.h>


sGameWindow::sGameWindow(int width, int height)
{
    m_pTeclado = new cTeclado();
    m_pRaton = new cRaton(width, height);
}


sGameWindow::~sGameWindow()
{
    delete m_pRaton;
    delete m_pTeclado;
    cLog::print(" Destructor sGameWindow\n");
}


coverride long sGameWindow::commonWindowProc(HWND hwnd, uint uiMsg, WPARAM wparam, LPARAM lparam)
{
    // Esto completa el uso de los mensajes en: mensajes, dentro de eventos.
    m_pTeclado->onKeyboardEvent(uiMsg, wparam, lparam);
    m_pRaton->onMouseEvent(uiMsg, wparam, lparam);

    return myWindowProc(hwnd, uiMsg, wparam, lparam);
}


coverride int sGameWindow::initWindow()
{
    return 0;
}


coverride long sGameWindow::OnSize(int newWidth, int newHeight)
{
    sWindow::OnSize(newWidth, newHeight);
    return 0;
}


/*------------------------------------------------------------------------*\
|* Fin de sGameWindow.h
\*------------------------------------------------------------------------*/
