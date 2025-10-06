/*------------------------------------------------------------------------*\
|* cTextura.cpp
\*------------------------------------------------------------------------*/

#include "cTextura.h"
#include "cLog.h"
#include <SDL3_image/SDL_image.h>

//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------
cTextura::cTextura(const char* pcPathFile)
    : m_pathFile(pcPathFile)
{
}


cTextura::~cTextura()
{
    release();
}


int cTextura::init(SDL_Renderer* pRenderer)
{
    // si no esta inicializada la textura, se inicializa
    if (!m_pTextura)
    {
        SDL_Surface* surAux = nullptr;
        surAux = IMG_Load(m_pathFile.c_str());
        if (!surAux)
        {
            cLog::error(" init: IMG_Load Error: no se pudo abrir: %s. [%s]\n", m_pathFile.c_str(), SDL_GetError());
            return -1;
        }
        m_pTextura = SDL_CreateTextureFromSurface(pRenderer, surAux);
        SDL_DestroySurface(surAux);
        if (!m_pTextura)
        {
            cLog::error(" init: SDL_CreateTextureFromSurface Error: [%s]\n", SDL_GetError());
            return -1;
        }

        m_rectSource.x = 0.0;
        m_rectSource.y = 0.0;
        SDL_GetTextureSize(m_pTextura, &m_rectSource.w, &m_rectSource.h);

        cLog::log(" Fichero Textura: %s\n", m_pathFile.c_str());
        cLog::log(" width : %d\n", (int)m_rectSource.w);
        cLog::log(" height: %d\n", (int)m_rectSource.h);
        cLog::log("\n");
    }
    return 0;
}

int cTextura::render(SDL_Renderer* pRenderer, const SDL_FRect* pdest) const
{
    SDL_RenderTextureRotated(pRenderer, m_pTextura, &m_rectSource, pdest,
        0, nullptr, SDL_FlipMode::SDL_FLIP_NONE);
    return 0;
}


void cTextura::setSource(SDL_FRect* source)
{
    m_rectSource.x = source->x;
    m_rectSource.y = source->y;
    m_rectSource.w = source->w;
    m_rectSource.h = source->h;
}

void cTextura::release()
{
    if (m_pTextura)
    {
        SDL_DestroyTexture(m_pTextura);
        m_pTextura = nullptr;
    }
}

/*------------------------------------------------------------------------*\
|* Fin de cTextura.cpp
\*------------------------------------------------------------------------*/
