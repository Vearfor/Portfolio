/*------------------------------------------------------------------------*\
|* BamTang04.cpp
\*------------------------------------------------------------------------*/

#include "BamTang04.h"
#include "sGame.h"
#include <tool/consola/cConsola.h>
#include <tool/consola/cConio.h>
#include <tool/cLog.h>
#include <tool/cTime.h>
#include <iostream>


//--------------------------------------------------------------------------
// Prototipos
//--------------------------------------------------------------------------
void presentacion();
//--------------------------------------------------------------------------


int main()
{
    cConsola::SetActiva(true);
    cConsola::setNombreProceso("FISICAS   " TITLE);

    presentacion();

    // que haya game: la lista de bolas lanzadas pertenecera al game
    // al game asociaremos un init, un 'eventos', un 'update', y un 'render':
    sGame juego;
    cTime time(60);

    for (juego.init(); juego.isRunning(); juego.eventos())
    {
        juego.update(time.getDeltaTime());
        time.espera(true);
    }

    cConio::SetColor(eTextColor::eTexNormal);
    cConsola::PulsaTecla(" Pulsa tecla para terminar ");
    std::cout << "\n";

    return 0;
}


void presentacion()
{
    cConio::SetColor(eTextColor::eTexAmarillo);
    cConio::Cls();
    cLog::print("\n");
    cLog::print(" Lo que hay que hacer:\n");
    cLog::print("\n");
    cConio::SetColor(eTextColor::eTexCeleste);
    cLog::print("  + Empezemos dibujando el recuadro\n");
    cLog::print("    y mover un circulo.\n");
    cLog::print("  + El origen de disparo con una flecha que indique la direccion\n");
    cLog::print("  + Diferentes longitudes de flecha para diferentes velocidades.\n");
    cLog::print("  + posibilidad con OpenGL Clasico/No Moderno ?, la libreria glew3.dll no\n");
    cLog::print("   seria necesaria.\n");
    cLog::print(" \n");
    cLog::print("  + Asociar a la tecla de Espacio la funcion 'shootball'.\n");
    cLog::print("\n");
    cLog::print("  + ya veremos:\n");
    cLog::print("         colisiones con las paredes.\n");
    cLog::print("         colisiones con las otras pelotas\n");
    cLog::print("         como afecta la gravedad\n");
    cLog::print("         + agregar un coeficiente de friccion\n");
    cLog::print("         + elasticidad: para los rebotes.\n");
    cLog::print(" \n");
    cLog::print("  + si se detienen solo puede ser por gravedad en el suelo\n");
    cLog::print("    (activar/desativar gravedad)\n");
    cLog::print("  + desaparecer despues de un tiempo de parada.\n");
    cLog::print("    (al desaparecer y haber hueco se caen las que hay)\n");
    cLog::print("\n");
    cConio::SetColor(eTextColor::eTexAmarillo);
    cConsola::PulsaTecla(" Pulsa tecla para continuar ");
    cConio::Cls();
}


/*------------------------------------------------------------------------*\
|* Fin de BamTang04.cpp
\*------------------------------------------------------------------------*/
