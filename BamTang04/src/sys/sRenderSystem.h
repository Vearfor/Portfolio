/*------------------------------------------------------------------------*\
|* sRenderSystem.h
\*------------------------------------------------------------------------*/

#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "../sGameWindow.h"
//--------------------------------------------------------------------------

/*------------------------------------------------------------------------*/
struct sGame;
/*------------------------------------------------------------------------*/
struct sRenderSystem
{
    sRenderSystem(int width, int height);
    ~sRenderSystem();

    int init();
    int update(sGame*, float fDeltaTime);

    sGameWindow* getMainWindow() { return m_pWindow; }

    // Necesitamos los valores actualizados en los OnSize de Width y Height
    // que son los limites de 'nuestro mundo' en este programa:
    // y accedo directamente al puntero de la Main Window.
    // Si este es nulo que no sea un error oculto si este ocurriera
    // que ocurra, para corregirlo
    // (no me gustan los assert, los tengo que escribir)
    int getWidth() { return m_pWindow->getAnchoOnSize(); }
    int getHeight() { return m_pWindow->getAltoOnSize(); }

private:

    // Estos valores se quedaran fijos
    int m_initWidth{ 0 };
    int m_initHeight{ 0 };

    // Los valores que luego hay que coger son estos, que variaran con el
    // OnSize de la ventana.
    sGameWindow* m_pWindow{ nullptr };
};
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*\
|* Fin de sRenderSystem.h
\*------------------------------------------------------------------------*/
