/*========================================================================*\
|* sRenderObject.cpp
\*========================================================================*/

#include "sRenderObject.h"
#include "cTextura.h"
#include "tool/cLog.h"
#include "tool/nComun.h"
#include "laberinto/sLaberinto.h"

sRenderObject::sRenderObject()
{
}
    
sRenderObject::~sRenderObject()
{
    // Cuando tengamos un gestor de recursos, sacaremos este delete de aqui:
    delete m_pTextura;
}


int sRenderObject::setTextura(const char* pcPathTextura)
{
    delete m_pTextura;
    m_pTextura = new cTextura(pcPathTextura);
    return 0;
}


int sRenderObject::init(SDL_Renderer*pRenderer)
{
    if (!m_pTextura)
    {
        cLog::error(" sRenderObject::init: Error: el objeto textura del render object no esta instanciado\n");
        return -1;
    }

    miError(m_pTextura->init(pRenderer));

    return 0;
}


int sRenderObject::render(SDL_Renderer* pRenderer, SDL_FRect* pRectDest)
{
    // Tenemos la posicion dada por m_fila y comlumna, nos falta info del contexto de la ventana:
    m_pTextura->render(pRenderer, pRectDest);
    
    return 0;
}


void sRenderObject::izquierda()
{
    // Si la posicion siguiente es valida
    char** matriz = m_pLaberinto->getMatriz();
    if (matriz)
    {
        char valor = matriz[m_fila][m_columna - 1];
        if (valor == kVacio || valor == kFin || valor == kInicio)
        {
            m_columna--;
            //cLog::print(" izquierda:  [ %2d, %2d]\n", m_fila, m_columna);
        }
    }
}


void sRenderObject::derecha()
{
    // Si la posicion siguiente es valida
    char** matriz = m_pLaberinto->getMatriz();
    if (matriz)
    {
        char valor = matriz[m_fila][m_columna + 1];
        if (valor == kVacio || valor == kFin || valor == kInicio)
        {
            m_columna++;
            //cLog::print(" derecha:    [ %2d, %2d]\n", m_fila, m_columna);
        }
    }
}


void sRenderObject::arriba()
{
    // Si la posicion siguiente es valida
    char** matriz = m_pLaberinto->getMatriz();
    if (matriz)
    {
        char valor = matriz[m_fila - 1][m_columna];
        if (valor == kVacio || valor == kFin || valor == kInicio)
        {
            m_fila--;
            //cLog::print(" arriba:     [ %2d, %2d]\n", m_fila, m_columna);
        }
    }
}


void sRenderObject::abajo()
{
    char** matriz = m_pLaberinto->getMatriz();
    if (matriz)
    {
        char valor = matriz[m_fila + 1][m_columna];
        if (valor == kVacio || valor == kFin || valor == kInicio)
        {
            m_fila++;
            //cLog::print(" abajo:      [ %2d, %2d]\n", m_fila, m_columna);
        }
    }
}

void sRenderObject::setLaberinto(sLaberinto* lab)
{
    m_pLaberinto = lab;
}


/*========================================================================*\
|* Fin de sRenderObject.cpp
\*========================================================================*/
