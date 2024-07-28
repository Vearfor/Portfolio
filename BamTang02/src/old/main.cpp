/*------------------------------------------------------------------------*\
|* main.cpp
\*------------------------------------------------------------------------*/

#include <stdio.h>
#include <string>
#include <conio.h>
#include <cctype>
#include <vector>
#include <map>


// No usamos fichero
// std::string sPathFile = "recursos/cifrado.txt";
std::string freqLang = "TEOAISRHNUCMDLGWFPYKJBVQX";


// Utilizaremos esta estructura para contabilizar la frecuencia por letra:
struct sChar
{
    // desde 'A' hasta la 'Z'. Se guardara y comparara como mayusculas
    char ascciCode{ 0 };    // El 0, es el nul, no tenemos nada guardado
    int cont{ 0 };
};


//--------------------------------------------------------------------------
// Prototipos
//--------------------------------------------------------------------------
sChar* buscarChar(std::vector<sChar*>& myVector, char car);
void ordenaVector(std::vector<sChar*>& myVector);
std::string construyeFreqLang(const std::string& cifrado);
int getPosicion(char car, const std::string& freqLang);
//--------------------------------------------------------------------------
// La funcion que realmente nos piden
//--------------------------------------------------------------------------
std::string decryptMessage(const std::string& cifrado, const std::string& freqLang);
//--------------------------------------------------------------------------


int main()
{
    // lo que vamos a hacer:
    // Abrimos fichero:
    // Contabilizamos los caracteres.
    // Creamos la funcion

    printf("\n");
    printf(" LABERINTO\n");
    printf("\n");
    printf(" 1 - Parseamos los parametros necesarios\n");
    printf("     Obligando a que sean mayores que 3 e impar. Y no se si aplicar un limite\n");
    printf("     Y no se si aplicar un limite, se nos saldrian de la pantalla\n");
    printf("     Un solo camino entre A y B. No puede haber mas de uno.\n");
    printf("     A estara en en el primer hueco de la esquina superior izquierda. Lo traduzco a la posicion (2.2)\n");
    printf("     B al final de ese camino mas largo.\n");
    printf("     Los caminos no pueden ser diagonales\n");
    printf("\n");
    printf(" 2 - Utilizando los parametros generariamos una estructura de datos en tabla para contener\n");
    printf("     el laberinto.\n");
    printf("     Veremos de meter esos agujeros aleatorios\n");
    printf("\n");
    printf(" 3 - y mostraremos la representación por pantalla\n");
    printf("\n");
    printf(" Pulsa tecla para terminar\n");
    printf("\n");

    _getch();
    return 0;
}



//--------------------------------------------------------------------------
// El vector de sChar no termina ordenado por el mayor
// Con esta funcion/metodo lo hacemos
//--------------------------------------------------------------------------
void ordenaVector(std::vector<sChar*>& myVector)
{
    //for (size_t i = 0; i < myVector.size(); i++)
    //{
    //    for (size_t j = 0; j < myVector.size() - i - 1; j++)
    //    {
    //        if (myVector[j]->cont < myVector[j + 1]->cont)
    //        {
    //            // Intercambiamos los elementos
    //            sChar* sSwap = myVector[j];
    //            myVector[j] = myVector[j + 1];
    //            myVector[j + 1] = sSwap;
    //        }
    //    }
    //}
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
std::string construyeFreqLang(const std::string& miCifrado)
{
    std::string salida = "";
    return salida;
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
int getPosicion(char car, const std::string& freqLang)
{
    // -1, no ha encontrado el caracter
    int pos = -1;
    return pos;
}


//==========================================================================
// La funcion que debemos construir:
//==========================================================================
std::string createMaze2D(int n)
{
    std::string salida = "";
    return salida;
}


/*------------------------------------------------------------------------*\
|* Fin de main.cpp
\*------------------------------------------------------------------------*/
