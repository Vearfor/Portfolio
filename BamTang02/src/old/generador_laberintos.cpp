/*******************************************************************************
ALGORÍTMICA 19-20 -- PRÁCTICA 5: Backtracking

Ejercicio individual: Laberinto

  Archivo auxiliar para la generación de laberintos 7x7
  para resolver con Backtracking.

*******************************************************************************/
#include <cstdlib>
#include <iostream>
#define tamanio 7

enum casilla{
  suelo  = 0,
  pared  = 1,
  puerta = 3
};

enum orientacion {
  horizontal,
  vertical
};

using namespace std;

/******************************************************************************/
void dibujaLaberinto(int laberinto[][tamanio]){
  cout << "╔";

  for(int i = 0; i < tamanio; i++)
    cout << "═";

  cout << "╗" << endl;

  for(int i = 0; i < tamanio; i++){
    cout << "║";
    for(int j = 0; j < tamanio; j++) {
      switch (laberinto[i][j]) {
        case casilla::suelo:
          cout << " ";
        break;

        case casilla::pared:
          cout << "█";
        break;
      }
    }
    cout << "║" << endl;
  }

  cout << "╚";

  for(int i = 0; i < tamanio; i++)
    cout << "═";

  cout << "╝";
  cout << endl;
}

/******************************************************************************/
void ponParedHorizontal(int laberinto[][tamanio], int fila, int inicio, int fin){             //Dibujar pared horizontal en la fila entre las posiciones de la fila pos a y b
  for(int i=inicio; i<fin; i++)
    laberinto[fila][i] = casilla::pared;
}

/******************************************************************************/
void ponParedVertical(int laberinto[][tamanio], int columna, int inicio, int fin){               //Dibujar pared vertical en la columna entre las posiciones de la columna pos a y b
  for(int i=inicio; i<fin; i++)
    laberinto[i][columna] = casilla::pared;
}

/******************************************************************************/
void abrePuertaEnHorizontal(int laberinto[][tamanio], int fila, int inicio, int fin){         //Abrir una puerta en la fila entre las posiciones de la fila pos a y b
  int puerta;                                                                   //Las puertas en paredes horizontales se ponen en alturas pares
  int despl = inicio + (inicio%2);

  puerta = (rand() % (fin-inicio-1));

  if(puerta % 2 != 0)
    puerta++;

  if(puerta + despl >= inicio)
    puerta -= 2;

  laberinto[fila][puerta + despl] = casilla::puerta;
}

/******************************************************************************/
void abrePuertaEnVertical(int laberinto[][tamanio], int columna, int inicio, int fin){           //Abrir una puerta en la columna entre las posiciones de la columna pos a y b
  int puerta;                                                                   //Las puertas en paredes horizontales se ponen en anchuras impares
  int despl = inicio + (inicio%2);

  puerta = (rand() % (fin-inicio-1));

  if(puerta % 2 == 0)
    puerta++;

  if(puerta + despl >= fin)
    puerta -= 2;

  laberinto[puerta + despl][columna] = casilla::puerta;
}

/******************************************************************************/
orientacion escogeOrientacion(int ancho, int alto){                                     //Escoger una orientacion para la siguiente pared. Favorece donde hay más espacio
  orientacion orientacion;

  if (ancho > alto) {
    orientacion = orientacion::horizontal;
  }
  else if (alto > ancho) {
    orientacion = orientacion::vertical;
  }
  else {
    switch (rand() % 2) {
      case 0:
        orientacion = orientacion::horizontal;
      break;

      case 1:
        orientacion = orientacion::vertical;
      break;
    }
  }

  return orientacion;
}

/******************************************************************************/
void generaLaberintoRecursivo(int laberinto[][tamanio], int posX_izda, int posX_dcha, int posY_arr, int posY_abj){
  int ancho = posX_dcha - posX_izda;
  int alto  = posY_abj  - posY_arr;

  if(ancho < 2 || alto < 2 || posX_izda < 0 || posX_dcha > tamanio
                           || posY_arr  < 0  || posY_abj > tamanio){
    laberinto[0][0] = casilla::puerta;
    laberinto[tamanio-1][tamanio-1] = casilla::puerta;

  }
  else{
    orientacion orientacion = (escogeOrientacion(ancho, alto));

    if(orientacion == orientacion::horizontal) {
      int posicion_pared = -1;
      int i = ancho / 2 + posX_izda;

      for(; posicion_pared == -1; i++){                                         //Las paredes verticales se ponen en anchuras impares en las que no haya puertas
        if(i%2 != 0 && laberinto[i+posY_arr][posY_arr] != casilla::puerta && laberinto[i+posY_arr][posY_abj] != casilla::puerta)
            posicion_pared = i;

        if(i == posX_dcha)
          i = 0;
      }

      int despl = posX_izda + (posX_izda%2);

      if(posicion_pared + despl > posX_izda)
        posicion_pared -= 2;

      posicion_pared += despl;

      ponParedVertical(laberinto, posicion_pared, posY_arr, posY_abj);
      abrePuertaEnVertical(laberinto, posicion_pared, posY_arr, posY_abj);

      generaLaberintoRecursivo(laberinto, posX_izda, posicion_pared, posY_arr, posY_abj);
      generaLaberintoRecursivo(laberinto, posicion_pared+1, posX_dcha, posY_arr, posY_abj);
    }

    if(orientacion == orientacion::vertical){

      int posicion_pared = -1;
      int i = alto / 2 + posY_arr;

      for(; posicion_pared == -1; i++){                                         //Las paredes horizontales se ponen en alturas pares en las que no haya puertas
        if(i % 2 == 0 && laberinto[posX_izda][i+posX_izda-1] != casilla::puerta && laberinto[posX_dcha][i+posX_izda] != casilla::puerta)
            posicion_pared = i;

        if(i == posY_abj)
          i = 0;
      }

      int despl = posY_arr + (posY_arr%2);

      if(posicion_pared + despl > posY_abj)
        posicion_pared -= 2;

      posicion_pared += despl;

      ponParedHorizontal(laberinto, posicion_pared, posX_izda, posX_dcha);
      abrePuertaEnHorizontal(laberinto, posicion_pared, posX_izda, posX_dcha);

      generaLaberintoRecursivo(laberinto, posX_izda, posX_dcha, posY_arr, posicion_pared);
      generaLaberintoRecursivo(laberinto, posX_izda, posX_dcha, posicion_pared+1, posY_abj);
    }
  }
}

/******************************************************************************/
void abrePuertas(int laberinto[][tamanio]){                                           //Convierte las puertas (3) en pasillos (0)
  for(int i=0; i<tamanio; i++){
    for(int j=0; j<tamanio; j++){
      if(laberinto[i][j] == casilla::puerta)
        laberinto[i][j] = 0;
    }
  }
}

/******************************************************************************/
/******************************************************************************/
int main(){
  int laberinto[tamanio][tamanio];
  srand(time(NULL));

  //Inicializamos el laberinto vacío
  for (int i=0; i<tamanio; i++){
    for(int j=0; j<tamanio; j++)
      laberinto[i][j] = 0;
  }

  generaLaberintoRecursivo(laberinto, 0, tamanio, 0, tamanio);
  abrePuertas(laberinto);

  dibujaLaberinto(laberinto);

  return 0;
}
