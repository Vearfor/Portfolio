/*========================================================================*\
|* sRenderObject.cpp
\*========================================================================*/

#include "sRenderObject.h"
#include "cTextura.h"
#include "cLog.h"
#include "nComun.h"
#include "sLaberinto.h"

sRenderObject::sRenderObject(const char* pcPathTexura)
{
    m_pTextura = new cTextura(pcPathTexura);
}
    
sRenderObject::~sRenderObject()
{
    // Cuando tengamos un gestor de recursos, sacaremos este delete de aqui:
    delete m_pTextura;
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
        m_pLaberinto->checkHemosLlegado();
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
        }
        m_pLaberinto->checkHemosLlegado();
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
        }
        m_pLaberinto->checkHemosLlegado();
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
        }
        m_pLaberinto->checkHemosLlegado();
    }
}


void sRenderObject::setLaberinto(sLaberinto* lab)
{
    m_pLaberinto = lab;
}


/*========================================================================*\
|* Fin de sRenderObject.cpp
\*========================================================================*/
