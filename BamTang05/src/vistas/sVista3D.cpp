/*========================================================================*\
|* sVista3D.cpp
\*========================================================================*/

#include "sVista3D.h"
#include "../tool/nComun.h"
#include "../tool/cLog.h"
#include "../tool/consola/cConsola.h"
#include "../cTextura.h"
#include "../laberinto/sMyMaze.h"
#include "../sRenderObject.h"
#include <SDL3/SDL.h>


sVista3D::sVista3D()
{
}

sVista3D::~sVista3D()
{
    libera();
}


int sVista3D::inicia(sLaberinto* lab)
{
    //m_sLaberinto = lab;

    //miError(initSDL() || createWindow(&m_pWindow, &m_pRenderer));
    //// Establecemos el color de fondo
    //SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0x00, 0xff, 0x00);

    //// Cargamos las texturas, por ahora no utilizamos un gestor de recursos
    //m_pLetraA = new cTextura(kPathLetraA.c_str());
    //m_pLetraB = new cTextura(kPathLetraB.c_str());
    //m_pMuro = new cTextura(kPathMuro.c_str());
    //m_pVacio = new cTextura(kPathVacio.c_str());
    //m_pMarca = new cTextura(kPathPunto.c_str());

    //// Y el render object no debe pertenecer a la vista:
    //m_pPunto = new sRenderObject(kPathPuntoAmarillo.c_str());

    //// No es el sitio: lo reorganizaremos de otra manera, pero metemos las condiciones del Laberinto aqui para que el 'punto'
    //// sepa las condiciones del laberinto.
    //m_pPunto->setLaberinto(lab);
    //m_pPunto->m_fila = 1;
    //m_pPunto->m_columna = 1;

    //miError(
    //    m_pLetraA->init(m_pRenderer) ||
    //    m_pLetraB->init(m_pRenderer) ||
    //    m_pMuro->init(m_pRenderer) ||
    //    m_pVacio->init(m_pRenderer) ||
    //    m_pMarca->init(m_pRenderer) ||
    //    m_pPunto->init(m_pRenderer)
    //);
    return 0;
}


int sVista3D::initSDL()
{
    auto isInitialized = SDL_Init(
        SDL_INIT_VIDEO |
        SDL_INIT_EVENTS
    ) >= 0;
    if (!isInitialized)
    {
        cLog::error(" init Error: inicializando SDL: [%s]: %d\n", SDL_GetError(), isInitialized);
        return -1;
    }
    return 0;
}


int sVista3D::createWindow(SDL_Window** pWindow, SDL_Renderer** pRenderer)
{
    //int size = 0;
    //if (m_sLaberinto)
    //    size = m_sLaberinto->getSize();

    //char vcSize[8];
    //sprintf_s(vcSize, sizeof(vcSize) - 1, "%2d", size);

    //std::string nombre = cConsola::getNombreProceso();
    //nombre += "    Size: ";
    //nombre += vcSize;
    //nombre += "    Pulsa Esc para cerrar la ventana";

    //SDL_CreateWindowAndRenderer(
    //    nombre.c_str(),
    //    kWidth, kHeight,
    //    0,
    //    pWindow, pRenderer);
    //if (*pWindow == NULL) {
    //    cLog::error(" CreateWindow Error: Window could not be created! SDL_Error: %s\n", SDL_GetError());
    //    return -1;
    //}
    //if (*pRenderer == NULL) {
    //    cLog::error(" CreateWindow Error: Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    //    return -1;
    //}
    return 0;
}


int sVista3D::libera()
{
    //// El render se borra,:
    //delete m_pPunto;

    //// Las texturas tambien se borran:
    //delete m_pMarca;
    //delete m_pVacio;
    //delete m_pMuro;
    //delete m_pLetraB;
    //delete m_pLetraA;

    //if (m_pRenderer)
    //{
    //    SDL_DestroyRenderer(m_pRenderer);
    //    m_pRenderer = nullptr;
    //}

    //if (m_pWindow)
    //{
    //    SDL_DestroyWindow(m_pWindow);
    //    m_pWindow = nullptr;
    //}

    SDL_Quit();

    return 0;
}


int sVista3D::update()
{
    return 0;
}


int sVista3D::mainLoop(sLaberinto* lab)
{
    bool mustQuit = !!inicia(lab);

    while (!mustQuit)
    {
        update();
        dibuja(lab);
        mustQuit = eventos();
    }
    return 0;
}


bool sVista3D::eventos()
{
    bool mustQuit = false;
    SDL_Event e;
    while (SDL_PollEvent(&e) && !mustQuit)
    {
        mustQuit =
            (e.type == SDL_EVENT_QUIT) ||
            (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
            ;

        switch (e.type)
        {
        case SDL_EVENT_QUIT:
            mustQuit = true;
            break;

            //case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
            switch (e.key.key)
            {
            case SDLK_ESCAPE:
                mustQuit = true;
                break;

            case SDLK_A:
            case SDLK_LEFT:
                // izquierda
                //m_pPunto->izquierda();
                break;
            case SDLK_W:
            case SDLK_UP:
                // arriba
                //m_pPunto->arriba();
                break;
            case SDLK_S:
            case SDLK_DOWN:
                // abajo
                //m_pPunto->abajo();
                break;
            case SDLK_D:
            case SDLK_RIGHT:
                // derecha
                //m_pPunto->derecha();
                break;

            case SDLK_SPACE:
                // m_sLaberinto->creaLaberintoFrame();
                // m_sLaberinto->calculaCaminoMasLargo();
                break;

            default:
                break;
            }
            break;


        case SDL_EVENT_WINDOW_FOCUS_GAINED:
            laVentanaGanaFoco();
            break;

        case SDL_EVENT_WINDOW_FOCUS_LOST:
            laVentanaPierdeFoco();
            break;

        default:
            break;
        }
    }

    return mustQuit;
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


int sVista3D::dibuja(sLaberinto* lab)
{
    //SDL_RenderClear(m_pRenderer);

    //// ... y aqui dibujamos ...
    //int size = lab->getSize();
    //char** matriz = lab->getMatriz();
    //SDL_FRect rectDest;
    //char valor = 0;

    //for (int fila = -1; fila < size + 1; fila++)
    //{
    //    for (int columna = -1; columna < size + 1; columna++)
    //    {
    //        if (fila<0 || fila>size - 1 || columna<0 || columna>size - 1)
    //        {
    //            valor = kVacio;
    //        }
    //        else
    //        {
    //            int iValor = matriz[fila][columna];
    //            valor = char(iValor);
    //        }
    //        calculaRect(fila, columna, size, &rectDest);
    //        dibujaChar(valor, &rectDest);
    //    }
    //}

    //calculaRect(m_pPunto->m_fila, m_pPunto->m_columna, size, &rectDest);
    //m_pPunto->render(m_pRenderer, &rectDest);

    //SDL_RenderPresent(m_pRenderer);
    //SDL_Delay(1);
    return 0;
}


void sVista3D::dibujaChar(char car, SDL_FRect* pRectDest)
{
    //cTextura* pTex = { nullptr };
    //switch (car)
    //{
    //case kVacio:
    //    pTex = m_pVacio;
    //    break;
    //case kMuro:
    //    pTex = m_pMuro;
    //    break;
    //case kInicio:
    //    pTex = m_pLetraA;
    //    m_pVacio->render(m_pRenderer, pRectDest);
    //    break;
    //case kFin:
    //    pTex = m_pLetraB;
    //    m_pVacio->render(m_pRenderer, pRectDest);
    //    break;
    //case kNulo:
    //    pTex = m_pMarca;
    //    m_pVacio->render(m_pRenderer, pRectDest);
    //    break;
    //}

    //if (pTex)
    //{
    //    pTex->render(m_pRenderer, pRectDest);
    //}
}


void sVista3D::calculaRect(int fila, int columna, int size, SDL_FRect* pOutRect)
{
    //float diffx = (float(kWidth) / 2.0f - (float(size * kCelda) / 2.0f));
    //float diffy = (float(kHeight) / 2.0f - (float(size * kCelda) / 2.0f));
    //pOutRect->x = diffx + columna * kCelda;
    //pOutRect->y = diffy + fila * kCelda;
    //pOutRect->w = kCelda;
    //pOutRect->h = kCelda;
}


/*========================================================================*\
|* Fin de sVista3D.cpp
\*========================================================================*/
