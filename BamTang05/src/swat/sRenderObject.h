/*========================================================================*\
|* sRenderObject.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Include
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Struct
//--------------------------------------------------------------------------
struct SDL_Renderer;
struct sTextura;
struct sLaberinto;
struct SDL_FRect;
//--------------------------------------------------------------------------
struct sRenderObject
{
    sRenderObject();
    ~sRenderObject();

    int setTextura(const char* pcPathTextura);
    int init(SDL_Renderer*);
    int render(SDL_Renderer*, SDL_FRect* pRectDest);

    void izquierda();
    void derecha();
    void arriba();
    void abajo();

    int m_fila{-1};
    int m_columna{-1};
    sTextura* m_pTextura{ nullptr };
    sLaberinto* m_pLaberinto{ nullptr };

    void setLaberinto(sLaberinto* lab);
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de sRenderObject.h
\*========================================================================*/
