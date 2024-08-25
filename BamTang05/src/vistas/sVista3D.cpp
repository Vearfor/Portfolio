/*========================================================================*\
|* sVista3D.cpp
\*========================================================================*/

#include "sVista3D.h"
#include "../laberinto/sMyMaze.h"
#include "../sGameWindow.h"
#include "../tool/cLog.h"
#include "../tool/consola/cConsola.h"
#include "../swat/cTeclado.h"


sVista3D::sVista3D()
{
}

sVista3D::~sVista3D()
{
    delete m_mainWindow;
}


int sVista3D::inicia(sLaberinto* lab)
{
    m_pLaberinto = lab;
    return 0;
}


int sVista3D::creaWindow(int width, int height)
{
    sGameWindow* pWin = new sGameWindow();
    if (!pWin)
    {
        cLog::error(" Error: sRenderSystem:init: error en la creacion de la ventana");
        return -1;
    }

    miError(pWin->crea(
        (int)ePosControl::eCENTER,
        (int)ePosControl::eCENTER,
        width, height,
        0.1, 100.0, 45.0, 32, 32, false,
        eEstiloW::eCaption,
        cConsola::getNombreProceso(), "Window OpenGL", nullptr));

    m_mainWindow = pWin;

    return 0;
}


int sVista3D::eventos()
{
    int iRes = mensajes();

    return !iRes;
}


int sVista3D::update()
{
    sGameWindow* pGameWindow = dynamic_cast<sGameWindow*>(m_mainWindow);
    if (pGameWindow)
    {
        cTeclado* pTeclado = pGameWindow->m_pTeclado;
        if (pTeclado)
        {
            if (pTeclado->isDown(VK_ESCAPE))
            {
                m_mainWindow->destruyeVentana();
            }

            if (pTeclado->isUp(VK_UP) || pTeclado->isUp('W'))
            {
                m_pLaberinto->arriba();
            }

            if (pTeclado->isUp(VK_RIGHT) || pTeclado->isUp('D'))
            {
                m_pLaberinto->derecha();
            }

            if (pTeclado->isUp(VK_DOWN) || pTeclado->isUp('S'))
            {
                m_pLaberinto->abajo();
            }

            if (pTeclado->isUp(VK_LEFT) || pTeclado->isUp('A'))
            {
                m_pLaberinto->izquierda();
            }

            // Hay que hacer reset para el siguiente control de eventos
            pTeclado->reset();

            //    //        case SDLK_SPACE:
            //    //            // m_sLaberinto->creaLaberintoFrame();
            //    //            // m_sLaberinto->calculaCaminoMasLargo();
            //    //            break;
            
            //    //    case SDL_EVENT_QUIT:
            //    //        mustQuit = true;
            //    //        break;
            // 
            //    //    case SDL_EVENT_WINDOW_FOCUS_GAINED:
            //    //        laVentanaGanaFoco();
            //    //        break;
            //
            //    //    case SDL_EVENT_WINDOW_FOCUS_LOST:
            //    //        laVentanaPierdeFoco();
            //    //        break;
        }
    }

    return 0;
}


int sVista3D::render()
{
    m_mainWindow->clean();

    // ... dibujado de nuestro laberinto:

    m_mainWindow->swap();

    return 0;
}


void sVista3D::laVentanaGanaFoco()
{
    //int size = 0;
    //if (m_sLaberinto)
    //    size = m_sLaberinto->getSize();

    //char vcSize[8];
    //sprintf_s(vcSize, sizeof(vcSize) - 1, "%2d", size);

    //std::string nombre = cConsola::getNombreProceso();
    //nombre += "    Size: ";
    //nombre += vcSize;
    //nombre += "    Pulsa Esc para salir. A,W,S,D para moverse.";

    //// cLog::print(" ganamos el foco: \n");

    //SDL_SetWindowTitle(m_pWindow, nombre.c_str());
}


void sVista3D::laVentanaPierdeFoco()
{
    //int size = 0;
    //if (m_sLaberinto)
    //    size = m_sLaberinto->getSize();

    //char vcSize[8];
    //sprintf_s(vcSize, sizeof(vcSize) - 1, "%2d", size);

    //std::string nombre = cConsola::getNombreProceso();
    //nombre += "    Size: ";
    //nombre += vcSize;
    //nombre += "    Toca la ventana para darle el foco";

    //// cLog::print(" perdemos el foco: \n");

    //SDL_SetWindowTitle(m_pWindow, nombre.c_str());
}


//int sVista3D::dibuja(sLaberinto* lab)
//{
//    //SDL_RenderClear(m_pRenderer);
//
//    //// ... y aqui dibujamos ...
//    //int size = lab->getSize();
//    //char** matriz = lab->getMatriz();
//    //SDL_FRect rectDest;
//    //char valor = 0;
//
//    //for (int fila = -1; fila < size + 1; fila++)
//    //{
//    //    for (int columna = -1; columna < size + 1; columna++)
//    //    {
//    //        if (fila<0 || fila>size - 1 || columna<0 || columna>size - 1)
//    //        {
//    //            valor = kVacio;
//    //        }
//    //        else
//    //        {
//    //            int iValor = matriz[fila][columna];
//    //            valor = char(iValor);
//    //        }
//    //        calculaRect(fila, columna, size, &rectDest);
//    //        dibujaChar(valor, &rectDest);
//    //    }
//    //}
//
//    //calculaRect(m_pPunto->m_fila, m_pPunto->m_columna, size, &rectDest);
//    //m_pPunto->render(m_pRenderer, &rectDest);
//
//    //SDL_RenderPresent(m_pRenderer);
//    //SDL_Delay(1);
//    return 0;
//}


//void sVista3D::dibujaChar(char car, SDL_FRect* pRectDest)
//{
//    //cTextura* pTex = { nullptr };
//    //switch (car)
//    //{
//    //case kVacio:
//    //    pTex = m_pVacio;
//    //    break;
//    //case kMuro:
//    //    pTex = m_pMuro;
//    //    break;
//    //case kInicio:
//    //    pTex = m_pLetraA;
//    //    m_pVacio->render(m_pRenderer, pRectDest);
//    //    break;
//    //case kFin:
//    //    pTex = m_pLetraB;
//    //    m_pVacio->render(m_pRenderer, pRectDest);
//    //    break;
//    //case kNulo:
//    //    pTex = m_pMarca;
//    //    m_pVacio->render(m_pRenderer, pRectDest);
//    //    break;
//    //}
//
//    //if (pTex)
//    //{
//    //    pTex->render(m_pRenderer, pRectDest);
//    //}
//}


//void sVista3D::calculaRect(int fila, int columna, int size, SDL_FRect* pOutRect)
//{
//    //float diffx = (float(kWidth) / 2.0f - (float(size * kCelda) / 2.0f));
//    //float diffy = (float(kHeight) / 2.0f - (float(size * kCelda) / 2.0f));
//    //pOutRect->x = diffx + columna * kCelda;
//    //pOutRect->y = diffy + fila * kCelda;
//    //pOutRect->w = kCelda;
//    //pOutRect->h = kCelda;
//}


/*========================================================================*\
|* Fin de sVista3D.cpp
\*========================================================================*/
