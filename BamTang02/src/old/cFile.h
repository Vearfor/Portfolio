/*
 * cFile.h
 *
 *  Created on: 07/12/2015
 *      Author: Enrique Rocafull Ortiz
 */

#ifndef CFILE_H_
#define CFILE_H_

//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <stdio.h>
#include "../Util/nComun.h"
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Defines
//--------------------------------------------------------------------------
#define mConst(x)   (const char *)(x)
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Enumerados
//--------------------------------------------------------------------------
enum eNivelTraza
{
    eTERR = 0,		    // Traza de error, siempre se ve.
    eSiempre = 0,       // Siempre se ve, al menos para tener el nombre y no dejarlo asociado a error.
    eTRAZA_DEFECTO = 2,
    eTMIN = 2,		    // Minimo nivel de traza
    eTprueba = 3,		// Traza a utilizar en pruebas
    eTNOR = 5,		    // Nivel de traza normal
    eTMAX = 9			// Maximo nivel de traza.
};

enum class eFileMode
{
    eLEER_TODO = 0,          // Implica la lectura con buffer, leetodo el fichero
                        // y lo cierra
    eWRITE,
    eAPPEND,
    eMEMORY,
    eREAD_SIN,          // Implica la lectura sin buffer.
                        // No lo cierra.
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// class
//--------------------------------------------------------------------------
class cString;
class cToken;
class cTime;
//--------------------------------------------------------------------------
class _mExport cFile
{
private:

    bool m_bShow;
    int m_iNivelTraza;
    int m_iX;
    int m_iY;
    cString * m_poBuffer;
    long m_lPuntero;

    void setTimeString(void);

protected:

    FILE * m_pFile;
    char m_vcNombreFichero[LON_BUFF];
    eFileMode m_eMode;
    bool m_bShowTime;
    uint m_uiThreadId;      // Hilo que ha 'creado' el file, y quizas solo lo use el.
    bool m_bPausa;

    static cString * leerTodo(FILE * fp);

public:

    cFile (void);
    virtual ~cFile (void);

    //----------------------------------------------------------------------
    // Static
    //----------------------------------------------------------------------
    // Abre fichero, Lee fichero, todo, Cierra fichero, devuelve Buffer
    static cString * leerTodo(const char * pcFile);
    static bool existe(const char * pcFile);
    static FILE * abrirFichero(const char * pcNombreFichero, eFileMode eMode);
    static void cerrarFichero(FILE * pFile);
    static int copiarFichero(FILE * pfDest, char * pcFichero);
    static int borrarFichero(char * pcFichero);

    //----------------------------------------------------------------------
    // Quiero un acceso directo a esta variable, alla donde este
    // Global, global, por los siglos de los siglos, Amen!!
    //----------------------------------------------------------------------
    static int giTraza;
    //----------------------------------------------------------------------

    //----------------------------------------------------------------------
    // No Static
    //----------------------------------------------------------------------
    virtual int init(const char * pcNombreCompletoFichero, eFileMode eMode);

    bool EstaAbierto(void);
    int abrir(void);
    int cerrar(void);
    bool finFichero (void);
    int escribe (char * pcBuffer, int ilonBuffer);
    int lee (char * pcBuffer, int ilonBuffer);
    cString * leerTodo(void);
    cString * getBuffer(void);

    //----------------------------------------------------------------------
    // Lecturas con el fichero ya leido, y cargado en buffer.
    // El fichero queda cerrado, pero su contenido en memoria
    //----------------------------------------------------------------------
    char leerCar(void);
    char * leerLinea(cToken * poTok, char * pcDest = NULL, int iSizeDest = 0);
    void setPuntero(long lPuntero) { m_lPuntero = lPuntero; }
    long getPuntero(void) { return m_lPuntero; }
    //----------------------------------------------------------------------

    void setShowTime(bool bShow);
    void showTime(void);
    void hideTime(void);
    bool isShowTime(void);

    void setNivelTraza(int iNivel) { m_iNivelTraza = iNivel; }
    int getNivelTraza(void) { return m_iNivelTraza; }

    void setShow(bool bShow) { m_bShow = bShow; };
    bool isShow(void) { return m_bShow; };
    void SetXY(int iX, int iY) { m_iX = iX; m_iY = iY; }

    int print(const char * pcFormat, ...);
    int traza(int iNivel, const char * pcFormat, ...);

    // Todos los print, traza, log y logtraza llaman a este
    int escribeLista(int iNivelTraza, va_list pstList, const char * pcFormat);

    FILE * getFile(void);
    const char * getNombreCompletoFichero(void) { return m_vcNombreFichero; }

    void setDwThread (dword dwThread) { m_uiThreadId = (uint) dwThread; }
    dword getDwThread (void) { return (dword)m_uiThreadId; }

    void setPausa(bool activaPausa) { m_bPausa = activaPausa; }
    bool esPausa(void) { return m_bPausa; }
};
//--------------------------------------------------------------------------


#endif /* CFILE_H_ */
