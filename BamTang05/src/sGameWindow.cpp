/*------------------------------------------------------------------------*\
|* sGameWindow.cpp
\*------------------------------------------------------------------------*/

#include "sGameWindow.h"
#include "swat/input/cTeclado.h"
#include "swat/sOpenGL.h"
#include "tool/consola/cConsola.h"
#include "laberinto/sLaberinto.h"
#include "tool/cTool.h"


/*------------------------------------------------------------------------*/
sGameWindow::sGameWindow(sLaberinto* pLab)
    : m_pTeclado(new cTeclado())
    , m_pLaberinto(pLab)
{
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

    switch (uiMsg)
    {
        case WM_SETFOCUS:
            OnSetFocus(m_pLaberinto);
            break;

        case WM_KILLFOCUS:
            OnKillFocus(m_pLaberinto);
            break;

        default:
            break;
    }

    return myWindowProc(hwnd, uiMsg, wparam, lparam);
}


/*------------------------------------------------------------------------*/
int sGameWindow::initWindow()
{
    return 0;
}


/*------------------------------------------------------------------------*/
void sGameWindow::OnSetFocus(sLaberinto* pLab)
{
    if (m_hWindow)
    {
        int size = 0;
        if (m_pLaberinto)
            size = m_pLaberinto->getSize();

        char vcSize[8];
        sprintf_s(vcSize, sizeof(vcSize) - 1, "%2d", size);

        std::string nombre = cConsola::getNombreProceso();
        nombre += "    Size: ";
        nombre += vcSize;
        nombre += "    Pulsa Esc para salir. A,W,S,D para moverse.";

        // cLog::print(" ganamos el foco: \n");
        WCHAR wcNombre[LON_BUFF / 8];
        cTool::copiaMultibyteToUnicode(nombre, wcNombre, sizeof(wcNombre));
        SetWindowText(m_hWindow, wcNombre);
    }
}


void sGameWindow::OnKillFocus(sLaberinto* pLab)
{
    if (m_hWindow)
    {
        int size = 0;
        if (m_pLaberinto)
            size = m_pLaberinto->getSize();

        char vcSize[8];
        sprintf_s(vcSize, sizeof(vcSize) - 1, "%2d", size);

        std::string nombre = cConsola::getNombreProceso();
        nombre += "    Size: ";
        nombre += vcSize;
        nombre += "    Toca la ventana para darle el foco";

        // cLog::print(" ganamos el foco: \n");
        WCHAR wcNombre[LON_BUFF / 8];
        cTool::copiaMultibyteToUnicode(nombre, wcNombre, sizeof(wcNombre));
        SetWindowText(m_hWindow, wcNombre);
    }
}


/*------------------------------------------------------------------------*\
|* Fin de sGameWindow.cpp
\*------------------------------------------------------------------------*/
