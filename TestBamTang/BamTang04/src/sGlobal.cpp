/*------------------------------------------------------------------------*\
|* sGlobal.cpp
\*------------------------------------------------------------------------*/
#include "sGlobal.h"


/*========================================================================*\
|* Statics
\*========================================================================*/
/*------------------------------------------------------------------------*\
|* m_iWidth y m_iHeight: son el ancho y el alto iniciales de la ventana
|* principal.
|* Estos permanecen fijos. Luego el evento OnSize de la ventana podria
|* tomar nuevos valores.
|* Por ello se toman los valores directamente de la ventana.
\*------------------------------------------------------------------------*/
int sGlobal::m_iWidth = 1024;
int sGlobal::m_iHeight = 768;

/*------------------------------------------------------------------------*\
|* Nivel de que determina que informacion se muestra en la esquina superior
|* izquierda.
|*
|* Con 0: evidentemente no se muestra nada
|*
|*     1: Velocidad Incial de las bolas al ejecutar con Space 'shootBall'
|*        Dir: o direccion del Origen de Disparos. Direccion con la cual
|*        se lanzaran las bolas.
|*
|*     2: Muestra si hay Gravedad y muestra si hay Friccion
|*
|*        Se presenta tambien el numero de bolas existentes limitadas por
|*        el maximo numero de bolas permitido.
|*
|*        Se pueden seleccionar bolas con el raton, y se puede mostrar la
|*        bola selccionada: su identificador, y su modulo de velocidad.
|*
|*     3: El Origen de los Disparos se puede mover con las teclas de
|*        direccion.
|*        Pero se tiene que Activar con la tecla 'O' (Origen).
|*        Cuando se activa y se mueve, muestra la Velocidad del Origen
|*        de los Disparos.
|*        De la misma manera, al activarse el origen de los disparos, el
|*        mensaje de nivel 3 cambia para avisar, de que en ese momento
|*        le afecta la Gravedad (si es que esta activa), le afecta la
|*        Fricción con el Aire (si es que esta activa), y le afectan
|*        tambien las colisiones (si es que alguna bola le choca).
|*
|*        Informa de que a todas las bolas les afecta un factor de
|*        elasticidad al chocar con los limites o al colisionas con otras
|*        bolas.
|*
|*        Informa tambien, que para asegurarse la parada de las bolas,
|*        por debajo de la Velocidad de Parada, se reducira las velocidad
|*        de las bolas.
|*
|*        Se avisa de que cuando las bolas paran, seran destruidas en un
|*        tiempo dado. Y que expllosion durante el cual se eliminan durara
|*        un tiempo a partir del cual la bola se habrá elimnado.
|*
|* Se cambia de nivel con la letra I.
|* Se empieza en 0, se sigue hasta 3, y cuando se supera, vuelve al 0.
|*
\*------------------------------------------------------------------------*/
int sGlobal::m_imostrarInfo = 3;

/*------------------------------------------------------------------------*\
|* Flag que indica si muestra o no los FPS del programa.
|* En la esquina superior derecha.
|*
|* Se activa/desactiva con la tecla F2
\*------------------------------------------------------------------------*/
bool sGlobal::m_bmostrarFps = true;

/*------------------------------------------------------------------------*\
|* Direccion iniclal del Origen de los Disparos, que se muestra con el 
|* nivel 1 de informacion.
|*
|* Esta permanece sin tocar, mientras que la del Origen de los Disparos
|* se puede cambiar con las teclas: 
|*  -- ADD (+)
|*  -- SUBTRACT (-)
|*  (m_fdir de la bola)
|*
|* Este se utilizara con la tecla R (Reset) para volver a poner al Origen
|* de los disparos otra vez en su sitio.
\*------------------------------------------------------------------------*/
float sGlobal::m_dirOrigen = 40.0f;

/*------------------------------------------------------------------------*\
|* Posicion original del Origen de los Disparos.
|*
|* Este se utilizara con la tecla R (Reset) para volver a poner al Origen
|* de los disparos otra vez en su sitio.
\*------------------------------------------------------------------------*/
glm::vec2 sGlobal::m_posOrigen
{
    (float)(sGlobal::m_iWidth / 2),
    (float)(sGlobal::m_iHeight / 7)
};

/*------------------------------------------------------------------------*\
|* Color de la Bola del Origen de los Disparos.
\*------------------------------------------------------------------------*/
glm::vec4 sGlobal::m_colorOrigen{ 0.4f, 1.0f, 0.1f, 1.0f };

/*------------------------------------------------------------------------*\
|* Color de la Flecha del Origen de los Disparos.
|* La que esta orientada por el Dir de la Bola del Origen de los Disparos.
\*------------------------------------------------------------------------*/
glm::vec4 sGlobal::m_colorFlecha{ 1.0f, 0.0f, 0.0f, 1.0f };

/*------------------------------------------------------------------------*\
|* Radio de la bola del Origen de los Disparos.
\*------------------------------------------------------------------------*/
float sGlobal::m_fRadioOrigen = 15.0f;

/*------------------------------------------------------------------------*\
|* Radio general de todas las bolas/circulos.
\*------------------------------------------------------------------------*/
float sGlobal::m_fRadio = 15.0f;

/*------------------------------------------------------------------------*\
|* Color general de todas las bolas/circulos.
|* Pero no se utiliza.
|* La funcion/metodo de shotBall combina entre Cyan, Rojo y Amarillo
\*------------------------------------------------------------------------*/
glm::vec4 sGlobal::m_colorBolas{ 1.0f, 0.0f, 0.0f, 1.0f };

/*------------------------------------------------------------------------*\
|* Limite maximo de bolas existentes en el juego.
|* Incluye como primera a la bola del Origen de los Disparos.
|* Si se ha alcanzado este limite no se pueden hacer mas, a no ser que se
|* vayan parando y auto-destruyendo.
\*------------------------------------------------------------------------*/
int sGlobal::m_limBolas = 60;

/*------------------------------------------------------------------------*\
|* Velocidad inicial que se muestra al principio, con la que se lanzan las
|* bolas.
|* Este valor se va cambiando con las teclas:
|*  - A (Aumentando)
|*  - D (Disminuyendo)
|*
|* Velocidad maxima inicial en que tus bolas se
|* - m_fVelocidadMin: es el minimo fijo
|* - m_fVelocidadMax: es el maximo fijo
\*------------------------------------------------------------------------*/
float sGlobal::m_fVelocidadInicial = 200.0f;
float sGlobal::m_fVelocidadMin = 30.0f;
float sGlobal::m_fVelocidadMax = 500.0f;

/*------------------------------------------------------------------------*\
|* Flag que determina si hay Gravedad.
|* Se activa/desactiva con la letra G
\*------------------------------------------------------------------------*/
bool sGlobal::m_hayGravedad = true;

/*------------------------------------------------------------------------*\
|* Flag que determina si hay Friccion en el Aire.
|* Se activa/desactiva con la letra F
\*------------------------------------------------------------------------*/
bool sGlobal::m_hayFriccion = true;

/*------------------------------------------------------------------------*\
|* Valor de la gravedad.
|* Solo se aplica a tu velocidad final sobre el eje y.
\*------------------------------------------------------------------------*/
float sGlobal::m_fGravedad      = - 9.8f;       // aumenta 9.8 metros por
                                                // segundo

/*------------------------------------------------------------------------*\
|* Valor de la Friccion en el Aire
|* - es un valor contrario al movimiento.
|* - Solo se produce si hay movimiento
\*------------------------------------------------------------------------*/
float sGlobal::m_fFriccionAire  = - 0.2f;       // digamos quitamos 0.2 m/s

/*------------------------------------------------------------------------*\
|* Bajamos la velocidad en cada rebote y en cada choque con otras bolas: 
|* no debe ser mayor que 1. Daria mas velocidad en cada rebote o choque
\*------------------------------------------------------------------------*/
float sGlobal::m_fElasticidad = 0.9f;

/*------------------------------------------------------------------------*\
|* Factor Maximizador de los efectos de la gravedad.
|* Los valores dados a causa de aplicar el fDeltaTime son excesivamenteg
|* pequeños.
|* Se lo agregue para mejorar visiblemente el efecto.
|* Habra que probar si MERECE LA PENA
|* Si lo ponemos a 1.0f, no tendra ningun efecto
|* Si lo ponemos a 0.0f, es como si hubieramos quitado la gravedad, no
|* afectaria nada.
|* Lo he dejado con 4.0f, si se quiere se cambia ...
\*------------------------------------------------------------------------*/
float sGlobal::m_fFactorMaximizador = 4.0f;
/*------------------------------------------------------------------------*/
//float sGlobal::m_fFactorReductor = 0.1f; // reducir o aumentar
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* La ventana tiene caption para poder arrastrarla por nuestra pantalla de
|* manera comoda.
|* Este caption provoca un valor incorrecto en el raton cuando intentamos
|* - seleccionar el Origen de los Disparos para moverlo con el raton.
|* - seleccionar una bola cualquiera. (No se selecciona la bola para
|*   moverla, sino para ver sus valores de velocidad y depurar el
|*   funcionamiento del programe)
|*
|* Utilizo este valor para corregir la seleccion de la bola por el raton
|*
|* TODO:
|* De todas formas, LA SELECCION de las bolas, a veces ESTA FALLANDO
|* seleccionando la equivocada. Hay que revisar la seleccion de las bolas
|*
\*------------------------------------------------------------------------*/
int sGlobal::m_windowCaptionSize = 40;

/*------------------------------------------------------------------------*\
|* Velocidad de Parada:
|* Por debajo de esta velocidad se aplica la parada a la bola.
|* Esta queda en cero, y se empieza a sumar el tiempo de destruccion
|*
|* Estaba pensando aplicar la parada (por debajo de esta velocidad de
|* parada) cuando:
|*   - estemos en el suelo
|*   - estemos colisionando.
|* Pero no lo tenemos claro, por ahora no.
|* Si aplicamos la parada, porque es dificil llegar al cero.
|* Y las pelotas, terminan colisionando eternamente MEZCLADAS !!
\*------------------------------------------------------------------------*/
float sGlobal::m_fVelParada = 2.0f;

/*------------------------------------------------------------------------*\
|* Este es el tiempo de destruccion, una vez parada la bola.
|* Cuando este se cumple la bola se destruye.
\*------------------------------------------------------------------------*/
float sGlobal::m_fTiempoDestruccion = 10.0f;

/*------------------------------------------------------------------------*\
|* Tiempo durante el cual el objeto explosion de la bola a destruir,
|* hace sus updates y renders. Cumplido este tiempo, la bola, si, se
|* elimina del m_vecBolas de sGame.
|*
|* Estaba en 10, lo bajamos a 5. Las bolas desaparecian, pero todavia no
|* se habian destruido.
\*------------------------------------------------------------------------*/
float sGlobal::m_fTiempoExplosion = 5.0f;

/*------------------------------------------------------------------------*\
|* Flag que indica los dos modos de actuar de la bola del Origen de los
|* Disparos:
|*
|* - Inactiva: las bolas pueden colisionar con el Origen de los Disparos,
|*             pero no le afectan en absoluto. No se mueve.
|*             Basicamente, una piedra inamobible.
|*             En este estado no le afecta ni la Gravedad, ni la Friccion
|*             del aire.
|*             Aunque puedes mover la bola con las teclas de direccion:
|*             'Left'-'Up'-'Right'-'Down'
|*
|* - Activa:   Aqui si le afecta todo.
|*             Las colisiones con las otras bolas modificaran velocidad y
|*             direccion.
|*             Si estara afectado por la Gravedad y la Friccion del Aire.
|*
|* Se activa/desactiva con la letra 'O'.
|* Con la tecla 'R' (reset) se situa en la posiion inicial.
|* Con la tecla 'S' (stop) se para la velocidad (no te esta rebotando por)
|* las paredes)
|*
\*------------------------------------------------------------------------*/
bool sGlobal::m_hayChoqueOrigen = false;


/*------------------------------------------------------------------------*\
|* Fin de sGlobal.cpp
\*------------------------------------------------------------------------*/
