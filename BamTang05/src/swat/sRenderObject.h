/*========================================================================*\
|* sRenderObject.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Include
//--------------------------------------------------------------------------
#include <GLM/glm.hpp>
#include "windows/tRect.h"
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Struct
//--------------------------------------------------------------------------
class cTextura;
class cMalla;
class cShader;
//--------------------------------------------------------------------------
struct sRenderObject
{
    sRenderObject();
    ~sRenderObject();

    // Esto solo es una primera aproximacion de lo que pueder ser al final
    // un render object
    // En este caso lo tengo pensado para ser utilizado en el universo de
    // un Laberinto.
    // Segun esto los render objects podran ser:
    // - el punto, o personaje que se mueve dentro del laberinto
    // - el inicio del laberinto, donde pondremos la textura de una A
    // - el fin del laberinto, donde pondremos la textura de una B
    // - y el muro, donde pondremos la malla y la textura de una pared.
    //   o algo que lo parezca
    // Así que vamos a crear estos elementos como miembros de sLaberinto
    // y agregaremos los elementos que luego seran necesarios para 
    // renderizarlos.

    // Fila y columna seran la posicion que tengan en el laberinto:
    int m_fila{-1};
    int m_columna{-1};

    // La textura que se utilizara para identficar cada personaje:
    cTextura* m_pTextura{ nullptr };

    // La malla que se utilizara para dibujarlas:
    cMalla* m_pMalla{ nullptr };

    // Y la escala que se utilizará para modificar a voluntad
    // el dibujo final.
    glm::vec3 m_escala{ 1.0, 1.0, 1.0 };

    void render(cShader* pShader, int locModel, cRect<float>* pInRect);
    void render(cShader* pShader, int locModel, glm::vec3 posRender);
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de sRenderObject.h
\*========================================================================*/
