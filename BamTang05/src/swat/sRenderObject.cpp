/*========================================================================*\
|* sRenderObject.cpp
\*========================================================================*/

#include "sRenderObject.h"
//#include "sTextura.h"
#include "../tool/cLog.h"
#include "../tool/nComun.h"
#include "../laberinto/sLaberinto.h"

sRenderObject::sRenderObject()
{
}
    
sRenderObject::~sRenderObject()
{
    // Cuando tengamos un gestor de recursos, sacaremos este delete de aqui:
    //delete m_pTextura;
}


int sRenderObject::setTextura(const char* pcPathTextura)
{
    //delete m_pTextura;
    //m_pTextura = new sTextura(pcPathTextura);
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
        }
        cLog::print(" izquierda:  [ %2d, %2d]\n", m_fila, m_columna);
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
        cLog::print(" derecha:    [ %2d, %2d]\n", m_fila, m_columna);
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
        cLog::print(" arriba:     [ %2d, %2d]\n", m_fila, m_columna);
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
        cLog::print(" abajo:      [ %2d, %2d]\n", m_fila, m_columna);
    }
}

void sRenderObject::setLaberinto(sLaberinto* lab)
{
    m_pLaberinto = lab;
}


/*========================================================================*\
|* Fin de sRenderObject.cpp
\*========================================================================*/
