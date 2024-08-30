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


//--------------------------------------------------------------------------
// El texto cifrado:
//--------------------------------------------------------------------------
std::string cifrado =
"Bgc - bfufb tegaedppqna ql aggv zge xof tegaedppfe'l lgjb.\n"
"Xof adpf vflqanfe logjbvn'x hf pdwqna d cgebv qn coqro xof tbdkfe ql mjlx d lpdbb tdex. Xof tbdkfe QL XOF HGLL; qx'l kgje vjxk xg fnxfexdqn oqp ge ofe.\n"
"Zgrjl ql d pdxxfe gz vfrqvqna codx xoqnal kgj def ngx agqna xg vg.\n"
"Xof rglx gz dvvqna d zfdxjef qln'x mjlx xof xqpf qx xdwfl xg rgvf qx. Xof rglx dblg qnrbjvfl xof dvvqxqgn gz dn ghlxdrbf xg zjxjef fstdnlqgn. Xof xeqrw ql xg tqrw xof zfdxjefl xodx vgn'x zqaox fdro gxofe. - Mgon Rdepdrw.\n"
"(ccc.adpdljxed.rgp / uqfc / nfcl / 234346 ? utkjpvbjr)\n"
"(ccc.hedqnkijgxf.rgp / ijgxfl / djxogel / m / mgon_rdepdrw.oxpb)\n";
//--------------------------------------------------------------------------


int main()
{
    // lo que vamos a hacer:
    // Abrimos fichero:
    // Contabilizamos los caracteres.
    // Creamos la funcion

    printf("\n");
    printf(" CYPHER\n");
    printf("\n");
    printf(" 1 - No utilizaremos fichero con el contenido encriptado: lo pone el enunciado\n");
    printf("     Mostraremos el cifrado:\n");
    printf("\n");
    printf(" 2 - Generaremos la cadena freqLang para el mensaje cifrado\n");
    printf("     Y la mostraremos.\n");
    printf("\n");
    printf(" 3 - y ahora haremos la funcion 'decryptMessage'. E integraremos la funcion anterior en esta\n");
    printf("     Y aqui mostramos el contenido desencriptado:\n");
    printf("\n");
    printf(" Pulsa tecla para continuar\n");
    while (!_kbhit());
    _getch();
    printf("     Mostramos el cifrado:\n");
    printf("\n");
    printf("%s", cifrado.c_str());
    printf("\n");
    printf("\n");
    std::string sMiFreqLang = construyeFreqLang(cifrado);
    printf(" El size de mi freqLang es: %2zd  [%s]\n", sMiFreqLang.size(), sMiFreqLang.c_str());
    printf("\n");
    printf("\n");
    std::string descifrado = decryptMessage(cifrado, freqLang);
    printf("     Mostramos el des-cifrado:\n");
    printf("\n");
    printf("%s", descifrado.c_str());
    printf("\n");
    printf("\n");
    printf(" Pulsa tecla para terminar\n");
    printf("\n");

    _getch();
    return 0;
}


//--------------------------------------------------------------------------
// No hemos utilizado un std::unordered_map
//  - indice: el asci en mayusculas de las letras
//  - contenido: el contador de letras
//
// Pero vamos a utilizar un vector de estructuras sChar
// Tenemos que tener una función/metodo que busque en el vector el sChar
// que corresponde a una 'car' letra dada
// Para luego incrementar el contador.
//--------------------------------------------------------------------------
sChar* buscarChar(std::vector<sChar*>& myVector, char car)
{
    sChar* found = nullptr;

    for (size_t i = 0; i < myVector.size() && !found; i++)
    {
        sChar* sMyCar = myVector[i];
        // Comparamos mayusculas con mayusculas
        char carUpper = toupper(car);
        if (carUpper == sMyCar->ascciCode)
            found = sMyCar;
    }
    return found;
}


//--------------------------------------------------------------------------
// El vector de sChar no termina ordenado por el mayor
// Con esta funcion/metodo lo hacemos
//--------------------------------------------------------------------------
void ordenaVector(std::vector<sChar*>& myVector)
{
    for (size_t i = 0; i < myVector.size(); i++)
    {
        for (size_t j = 0; j < myVector.size() - i - 1; j++)
        {
            if (myVector[j]->cont < myVector[j + 1]->cont)
            {
                // Intercambiamos los elementos
                sChar* sSwap = myVector[j];
                myVector[j] = myVector[j + 1];
                myVector[j + 1] = sSwap;
            }
        }
    }
}


//--------------------------------------------------------------------------
// Construye NUESTRA cadena de frecuencias a partir del texto cifrado.
// Utiliza los metodos anteriores:
// - buscaChar
// - ordenaVector
//--------------------------------------------------------------------------
std::string construyeFreqLang(const std::string& miCifrado)
{
    std::string salida = "";

    // Y un vector, mejor de punteros, para el swap entre elememtos.
    // Quiza mejor un unordered_map, pero ya lo dejamos para la optimizacion.
    std::vector<sChar*> vecFrequency{};

    // Nos vamos recorriendo el cifrado y vamos contabilizando:
    for (size_t i = 0; i < miCifrado.size(); i++)
    {
        char car = miCifrado[i];
        // Solo las letras
        if (std::isalpha(car))
        {
            // buscar sChar del car:
            sChar* sMyCar = buscarChar(vecFrequency, car);
            // si encuentra, incrementa, ordena el vector
            if (sMyCar)
            {
                sMyCar->cont++;
            }
            else
            // no lo encuentra, no existe ...
            // incrementa en 1, lo introduce,.
            {
                sMyCar = new sChar();
                // las guardamos como mayusculas
                sMyCar->ascciCode = toupper(car);
                sMyCar->cont = 1;
                vecFrequency.push_back(sMyCar);
            }
            // y ordena el vector
            // Estabamos ordenando por caracter leido.
            // ordenaVector(vecFrequency);
            // Nos basta con hacerlo un vez al final
        }
    }
    // Ordenamos vector
    ordenaVector(vecFrequency);

    // Utilizaremos este buffer char para ir metiendo valores.
    // Sabemos que el numero 32 es superior a los 25/26 que tendremos
    // que guardar.
    // Podria optimizarse reservando vecFrequenzy.size()+1.
    char vcFreqLang[32];

    // Dejamos todos los valores a nulo/0.
    memset(vcFreqLang, 0, sizeof(vcFreqLang));

    // Repasa el vector, y lo introduce en vcFreqLang
    for (size_t i = 0; i < vecFrequency.size(); i++)
    {
        vcFreqLang[i] = vecFrequency[i]->ascciCode;
    }

    // liberamos los sChar: no queremos memory leaks,
    // de nuestro vector de frecuencias
    for (size_t i = 0; i < vecFrequency.size(); i++)
    {
        delete vecFrequency[i];
        vecFrequency[i] = nullptr;
    }

    salida = vcFreqLang;

    return salida;
}


//--------------------------------------------------------------------------
// Nos devuelve la posicion que ocupa el car/letra en la cadena de
// frecuencias.
//--------------------------------------------------------------------------
int getPosicion(char car, const std::string& freqLang)
{
    // -1, no ha encontrado el caracter
    int pos = -1;
    // Se compara en mayusculas.
    char upperCar = toupper(car);
    for (size_t i = 0; i < freqLang.size() && pos==-1; i++)
    {
        if (freqLang[i] == upperCar)
        {
            pos = (int) i;
        }
    }

    return pos;
}


//==========================================================================
// La funcion que debemos construir:
// Utiliza:
//  - construyeFreqLang
//  - getPosicion
//==========================================================================
std::string decryptMessage(const std::string& cifrado, const std::string& freqLang)
{
    std::string salida = "";

    std::string miFicFrecLang = construyeFreqLang(cifrado);

    // La longitud final será:
    size_t lenFinal = cifrado.size();

    // Reservamos espacio en buffer char:
    // Lo hare con malloc, con +1, reservando espacio para el nulo final.
    char* pBuffer = (char*)malloc(lenFinal + 1);
    if (pBuffer == nullptr)
    {
        fprintf(stderr, " decryptMessage: Error: el malloc ha devuelto un nulo");
        return salida;
    }

    // Limpiamos, así todo nos queda a cero: y el ultimo ya tiene el nulo
    memset((void *)pBuffer, 0, lenFinal + 1);

    // Recorremos cifrado y vamos poniendo lo que corresponde en pBuffer:
    for (size_t i = 0; i < cifrado.size(); i++)
    {
        char descifrado = cifrado[i];
        if (std::isalpha(descifrado))
        {
            // Es minuscula si es superior a 'Z' en Ascii
            // podria haber utilizado std:islower ...
            bool islower = (descifrado > 'Z');

            // Buscamos la posicion en nuestra freqLang
            int pos = getPosicion(descifrado, miFicFrecLang);

            // No puede ocurrir el -1, pero ... lo obviamos.
            // Si ocurriera no pasaría nada ¿lo debemos esconder?
            if (pos > -1)
            {
                descifrado = freqLang[pos];
                if (islower)
                {
                    descifrado = tolower(descifrado);
                }
            }
        }
        pBuffer[i] = descifrado;
    }

    salida = pBuffer;

    // liberamos pBuffer con free
    free(pBuffer);

    return salida;
}


/*------------------------------------------------------------------------*\
|* Fin de main.cpp
\*------------------------------------------------------------------------*/
