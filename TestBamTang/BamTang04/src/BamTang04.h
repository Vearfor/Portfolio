/*------------------------------------------------------------------------*\
|* BamTang04.h
\*------------------------------------------------------------------------*/


#pragma once
/*------------------------------------------------------------------------*\
|* Includes
\*------------------------------------------------------------------------*/
#include "sGlobal.h"
/*------------------------------------------------------------------------*/


//--------------------------------------------------------------------------
// Organizacion del Programa
//--------------------------------------------------------------------------
// Debemos tener:
//  - un init
//  - si todo va bien entramos en bucle
//  - que pregunta por unos eventos/mensajes
//  - estos eventos modifican datos
//  - y en el bucle se actualiza: update
//  - y en estas aplicaciones tenemos un renderizado.
// 
// Otra estructura sería:
// 
//  - hay un Gestor de Entidades - lista de entidades - que participan en
//    el juego
//  - hay un Gestor del Juego    - 
// 
//  - y hay varios 'sistemas'
//    + Sistema de Fisicas:   lo que vamos a simular
//    + Sistema de Inputs:    checquea la existencia de eventos, entradas, 
//                            en este caso
//    + ... otros sistemas
//    + Sistema de Render
// y todos deben cumplir con el esquema anterior:
//
//    inicio.
//    Bucle(hasta fin)
//          update del inputsystem -
//          update del phisicssystem
//          --- otros updates otros sistemas
//          update del render system
// 
//    Las entidades pueden ser muy diversas, y tienen distintos componentes.
//    Los componentes forman listas segun el sistema al cual pertenecen
//    Y cuando en un sistema se hace un update el update se hace sobre cada
//    lista de componentes
//    Se renderiza el resultado: el render system solo dibuja
//    Estas actualizaciones de los componentes afectan a sus Entidades
//    Estas afectan al juego que determina si hemos terminado.
// 
//    Quedemonos con lo simple del enunciado:
//      - cuarto rectangular cerrado
//      - hay gravedad
//      - las pelotas chocan contra las paredes y entre ellas.
//      - podemos mover el origen del disparo
//      - cambiamos el angulo de disparo (distintas orientaciones)
//      - cambiamos velocidad del disparo (longitud flecha disparo), pueden
//        ser longitudes discretas, no continuas: 3/4/5 valores.
// 
//  + Empezemos dibujando el recuadro
//    y mover un circulo.
//  + El origen de disparo con una flecha que indique la direccion
//  + Diferentes longitudes de flecha para diferentes velocidades.
//  + posibilidad con OpenGL Clasico/No Moderno ?, la libreria glew3.dll no
//   sería necesaria.
// 
//  + Asociar a la tecla de Espacio la funcion 'shootball'.
//
//  + ya veremos:
//         colisiones con las paredes.
//         colisiones con las otras pelotas
//         como afecta la gravedad
//         + agregar un coeficiente de friccion
//         + elasticidad: para los rebotes.
// 
//  + si se detienen solo puede ser por gravedad en el suelo
//    (activar/desativar gravedad)
//  + desaparecer despues de un tiempo de parada.
// 
//--------------------------------------------------------------------------
// 
// Cuarto rectangular cerrado.
// Gravedad G
// Las pelotas chocan contra las paredes y contra las otras pelotas
// Despues de estar un tiempo inmoviles deben desaparecer.
// 
// Coeficientes de friccion - se frenan
// Coeficientes de elasticidad - rebotan.
// 
// El origen de disparo se puede mover.
// Se puede cambiar el angulo inicial
// Se puede cambiar la velocidad inicial V
// 
//--------------------------------------------------------------------------


/*------------------------------------------------------------------------*\
|* Fin de BamTang04.h
\*------------------------------------------------------------------------*/
