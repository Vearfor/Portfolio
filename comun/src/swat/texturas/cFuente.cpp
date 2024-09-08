//==========================================================================
//  cFuente.cpp
//==========================================================================
// 16/08/2003 : Enrique Rocafull Ortiz
//==========================================================================
//  Módulo de tratamiento de las fuentes para escribir mensajes.
//==========================================================================


#include "cFuente.h"
#include "cGestorColores.h"
#include "cGestorFuentes.h"
#include "Texturas/cTextura.h"
#include "../Windows/cGLWindow.h"
#include "../Files/cLog.h"
#include "../Memoria/fMemoria.h"
#include "../Util/cError.h"
#include "cOpengl.h"


//--------------------------------------------------------------------------
// Constructor/Destructor
//--------------------------------------------------------------------------
cFuente::cFuente ( void )
	: m_ptFuente(NULL)
	, m_poTextura(NULL)
    , m_iSiguienteX(0)
    , m_iSiguienteTitulo(0)
    , m_iUltimoY(0)
	, m_bActivada(false)
    , m_fAnchoXCaracter(0)
	, m_fCharMaxHeight(0.0f)
    , m_bFull(false)
    , m_iTraza(eTMIN)
    , m_iX(-1)
    , m_iY(-1)
{
	setNew(this,"cFuente");
    memset((void *)m_vfAncho, 0, sizeof(m_vfAncho));
}


cFuente::~cFuente ( void )
{
	Destruye();
}



//--------------------------------------------------------------------------
// Metodos
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Iniciamos los parametros de la textura
//--------------------------------------------------------------------------
int cFuente::Inicia ( sFuente * p_ptFuente )
{
	mDelete(m_ptFuente);
	// m_ptFuente = (sFuente*)cMemoria::malloc(sizeof(miFuente), "miFuente");
	m_ptFuente = new sFuente();
	miNulo(m_ptFuente);

    // Esta asignacion es muy pobre y peligrosa !!
	*m_ptFuente = *p_ptFuente;

	setNombre(m_ptFuente->vcNombre);

	return 0;
}


//--------------------------------------------------------------------------
// Cargamos los valores y generamos la fuente.
//--------------------------------------------------------------------------
int cFuente::Carga ( cGLWindow * poWindow, int iCharSet )
{
	int	iRes = 0;

	if	(!m_bActivada)
	{
		mDelete(m_poTextura);
		if (m_ptFuente->eTipoFon!= eTipoFuentes::eFN01)
		{
			m_poTextura = new cTextura;
			miNulo(m_poTextura);
		}

        iRes = Construye(poWindow, iCharSet);

		m_bActivada = !iRes;
	}

	return iRes;
}


//--------------------------------------------------------------------------
// Construye la fuente
//--------------------------------------------------------------------------
int cFuente::Construye (cGLWindow * poWindow, int iCharSet)
{
    const char * _metodo = __FUNCTION__;

    //----------------------------------------------------------------------
	// Generación de la fuente.
	//----------------------------------------------------------------------
	// Nuevo: modo de Orionis_p, ¿ modo suave ? : Listas
	//----------------------------------------------------------------------
	int		iRes = 0;
	//----------------------------------------------------------------------
	if	(m_ptFuente)
	{
		//------------------------------------------------------------------
		cError	oError;
        HDC hDc = poWindow->getDeviceContext();
		//------------------------------------------------------------------
		if	(m_ptFuente->eTipoFon!= eTipoFuentes::eFN01)
		{
			if	(m_poTextura)
			{
				iRes =
					(
							m_poTextura->inicia(m_ptFuente->idTextura, m_ptFuente->vcNombre, m_ptFuente->vcFichero, m_ptFuente->eTipoTex,NULL) != 0
						||	m_poTextura->carga() != 0
					);
				miError(iRes);

				m_ptFuente->iTextura = m_poTextura->getGLTextura();   // Las fuentes con texturas dependen su ancho y su alto de las dimensiones
                m_ptFuente->iAnchoTex = m_poTextura->getAncho();    // del fichero de textura.
				m_ptFuente->iAltoTex = m_poTextura->getAlto();      // 256 x 256 (Generalmente)
			}
		}
		else
		{
			m_ptFuente->iNItems = 256 - 32;	// 96;
		}
        //------------------------------------------------------------------
        GLint   iLado;       // Distinto según la textura
		memset( (void *) m_vfAncho, 0, sizeof(m_vfAncho) );
		m_fCharMaxHeight = 0;
        m_fAnchoXCaracter = 1;

		//------------------------------------------------------------------
		switch(m_ptFuente->eTipoFon)
		{
			case eTipoFuentes::eFT01:
				//----------------------------------------------------------
				{
                    GLfloat	    cx;		     // Coordenada X
					GLfloat	    cy;		     // Coordenada Y
					GLint       il;          // Indice del bucle.
					GLfloat     fILado;      // 1/iLado
					int			iAncho;

                    iLado	= (int) sqrt((double) m_ptFuente->iAnchoTex);	    // Texturas de 256 x 256, y de 256 elementos (=16x16).
					fILado	= (float) 1/iLado;								    // 1/16 = 0.0625
					iAncho	= m_ptFuente->iAncho;							    // m_ptFuente.iTam;

                    m_ptFuente->iNItems = 256;                                  // 256 elementos.
                    
					m_ptFuente->iBase = glGenLists(m_ptFuente->iNItems);	    // Crea 256 listas vacías
                    miError(cOpenGL::Error(0, "glGenLists:eFT01"));

					glBindTexture(GL_TEXTURE_2D, m_ptFuente->iTextura);		    // Selecciona la textura con las fuentes
					miError(cOpenGL::Error(0, "glBindTexture"));

					for ( il=0; il<m_ptFuente->iNItems; il++ )				    // Bucle para los 256 elementos/listas
					{
						cx=(float)(il%iLado)/iLado;								// Posición X para el caracter en curso.
						cy=(float)(il/iLado)/iLado;								// Posición Y para el caracter en curso.

						glNewList(m_ptFuente->iBase+il, GL_COMPILE);			// Comienza una lista de visualización.
							glBegin(GL_QUADS);									// Usamos rectágulos/quads para cada caracter.
								//------------------------------------------------------
								glTexCoord2f(cx			,1-cy-fILado);			// Coord Textura (Abajo izquierda).
								glVertex2i	(0			,0			);			// Coord Vértice (Abajo izquierda).
								//------------------------------------------------------
								glTexCoord2f(cx+fILado	,1-cy-fILado);			// Coord Textura (Abajo derecha).
								glVertex2i	(iLado		,0			);			// Coord Vértice (Abajo derecha).
								//------------------------------------------------------
								glTexCoord2f(cx+fILado	,1-cy		);			// Coord Textura (Arriba derecha).
								glVertex2i	(iLado		,iLado		);			// Coord Vértice (Arriba derecha).
								//------------------------------------------------------
								glTexCoord2f(cx			,1-cy		);			// Coord Textura (Arriba izquierda).
								glVertex2i	(0			,iLado		);			// Coord Vértice (Arriba izquierda).
								//------------------------------------------------------
							glEnd();											// Nuestro rectangulo/quad construido (Caracter).
							glTranslated(iAncho,0,0);							// Mueve a la derecha del Caracter.
						glEndList();											// Lista de visualización del caracter construida.

                        m_vfAncho[il] = (float) iLado;
					}															// Para los 256 elementos de la textura.
					miError(cOpenGL::Error(0, "cFuente:Construye:[tipo eFT01]"));
					//------------------------------------------------------
					for (int i=0;  i<m_ptFuente->iNItems;  i++)
					{
						m_vfAncho[i] = (float) m_ptFuente->iAncho;
					}
					m_fAnchoXCaracter = (float) m_ptFuente->iAncho;
					m_fCharMaxHeight = (float) m_ptFuente->iAlto;
					//------------------------------------------------------
				}
                //----------------------------------------------------------
				break;

			case eTipoFuentes::eFT02:
			case eTipoFuentes::eAT01:
				//----------------------------------------------------------
				{
					int i, j, k;

					// Ahora establecemos las coordenadas.
					for ( k=0, j=1; j<(m_ptFuente->iAltoTex/2); j+=16 )
					{
						for ( i=0; i<m_ptFuente->iAnchoTex; i+=16 )
						{
							switch (m_ptFuente->eTipoFon)
							{
								case eTipoFuentes::eFT02:   // Fuente TGA
									m_ptFuente->fCoord[k][0] = (float) i / (float) m_ptFuente->iAnchoTex;
									m_ptFuente->fCoord[k][1] = (float) j / (float) m_ptFuente->iAltoTex;
									m_ptFuente->fCoord[k][2] = (float) i / (float) m_ptFuente->iAnchoTex + (float) 15/(float)m_ptFuente->iAnchoTex;
									m_ptFuente->fCoord[k][3] = (float) j / (float) m_ptFuente->iAltoTex  + (float) 15/(float)m_ptFuente->iAltoTex;
									break;

                                case eTipoFuentes::eAT01:   // Fuente BMP
									m_ptFuente->fCoord[k][0] = (float) i / (float) m_ptFuente->iAnchoTex;
									m_ptFuente->fCoord[k][1] = 0.5f - ((float) j / (float) m_ptFuente->iAltoTex);
									m_ptFuente->fCoord[k][2] = (float) i / (float) m_ptFuente->iAnchoTex + (float) 15/(float)m_ptFuente->iAnchoTex;
									m_ptFuente->fCoord[k][3] = 0.5f - ((float) j / (float) m_ptFuente->iAltoTex  + (float) 15/(float)m_ptFuente->iAltoTex);
									break;
							}
							k++;
						}
					}
					//------------------------------------------------------
					for ( i=0;  i<256;  i++)
					{
						m_vfAncho[i] = (float) m_ptFuente->iAncho;
					}
					m_fAnchoXCaracter = (float) m_ptFuente->iAncho;
					m_fCharMaxHeight = (float) m_ptFuente->iAlto;
					//------------------------------------------------------
				}
                //----------------------------------------------------------
				break;

			case eTipoFuentes::eFN01:
				//----------------------------------------------------------
				// Fuentes Windows
				//----------------------------------------------------------
				{
					HFONT	hFont, hOldFont;
					int		iCSet;

                    m_ptFuente->bOutLine = false;
					m_ptFuente->iBase = glGenLists(m_ptFuente->iNItems);
                    miError(cOpenGL::Error(0, "%s: glGenLists", _metodo));

					iCSet = (iCharSet == -1) ? ANSI_CHARSET : m_ptFuente->iCharSet;
					iCSet = (iCSet == -1) ? ANSI_CHARSET : iCSet;

					// Parece ser que el Ancho no nos sirve de mucho, siempre hay que recalcularlo.
					// Y la altura se pasa como negativa.
                    SetLastError(0);
					hFont = CreateFont
						(
							m_ptFuente->iAlto,			    // Altura de la fuente
							m_ptFuente->iAncho,				// Ancho medio de los caracteres
							0,								// Angulo de escape (?)
							0,								// Angulo de orientacion de la linea base ( y el eje X)
							m_ptFuente->iPeso,				// Peso de la fuente
							mBTrue(m_ptFuente->bItalic),	// italic atribute option
							mBTrue(m_ptFuente->bUnderLine),	// underline attribute option
							mBTrue(m_ptFuente->bStrikeOut),	// strikeout attribute option
							iCSet,							// conjunto de caracteres
							OUT_TT_PRECIS,					// output precision
							CLIP_DEFAULT_PRECIS,			// clipping precision
							ANTIALIASED_QUALITY,			// output quality
							FF_DONTCARE|DEFAULT_PITCH,		// pitch and family
							m_ptFuente->vcNombre			// typeface name
						);
					miNulo(hFont);
					oError.mensaje(0,"cFuente:Construye:[tipo eFN01] CreateFont [%s]", m_ptFuente->vcNombre);

					hOldFont = (HFONT) SelectObject(hDc, hFont);

					char* pcCharSet = cGestorFuentes::BuscaCharSet(iCSet);
					cLog::traza(m_iTraza, "   Genero fuente %s %d\n", m_ptFuente->vcNombre, m_ptFuente->idTextura);
					cLog::traza(m_iTraza, "   CharSet  %d  [%s]\n", iCSet, msNulo(pcCharSet));
					cLog::traza(m_iTraza, "   + Peso %d\n", m_ptFuente->iPeso );
                    cLog::traza(m_iTraza, "   + Alto %d\n", m_ptFuente->iAlto );
                    cLog::traza(m_iTraza, "   + Ancho %d\n", m_ptFuente->iAncho );
                    cLog::traza(m_iTraza, "     Outline a %s\n", msTrue(m_ptFuente->bOutLine));
					if	(m_ptFuente->bOutLine)
					{
						GLYPHMETRICSFLOAT agmf[256];
						// create display lists for glyphs 0 through 255 with 0.1 extrusion 
						// and default deviation. The display list numbering starts at 1000 
						// (it could be any number) 
						//wglUseFontOutlines(hdc, 0, 255, 1000, 0.0f, 0.1f, WGL_FONT_POLYGONS, &agmf);
						wglUseFontOutlines(hDc, 32, m_ptFuente->iNItems, m_ptFuente->iBase, 0.0f, 0.1f, WGL_FONT_POLYGONS, &agmf[0]);
					}
					else
					{
						wglUseFontBitmaps(hDc, 32, m_ptFuente->iNItems, m_ptFuente->iBase);
					}
					oError.mensaje(0,"cFuente:Construye:[tipo eFN01] [%s]", m_ptFuente->vcNombre);
					SelectObject(hDc,hOldFont);
					DeleteObject(hFont);
                    //------------------------------------------------------
                    for (unsigned int i=0;  i<256;  i++)
                    {
                        SIZE size;
                        char character = char(i);

                        if (GetTextExtentPoint32(hDc, &character,1, &size) == FALSE)
                        {
                            m_vfAncho[i] = 0.0f;
                        }
                        else
                        {
                            m_vfAncho[i] = float(size.cx);
                            m_fCharMaxHeight = Max(m_fCharMaxHeight, float(size.cy));
                        }
                        m_fAnchoXCaracter = Max(m_fAnchoXCaracter, m_vfAncho[i]);
                    }
                    //------------------------------------------------------
				}
				//----------------------------------------------------------
				/*
					int		iIndex		;
					int		iAltura		;				// Altura de la letra
					int		iAncho		;				// Ancho ocupado por cada letra
					int		iPeso		;
					bool	bOutline	;
					char	vcNombreTipo[LON_STRING];	// Nombre del tipo de letra
					HFONT	hFont		;				// Handle devuelto por CreateFont

					{ -1, -12, 0, FW_BOLD	, false	, "Arial"		, NULL	},		// FON_BASE1
				*/
				//----------------------------------------------------------
				break;
		}
		//------------------------------------------------------------------
	}
	//----------------------------------------------------------------------

	return iRes;
}


//==========================================================================
// Longitud de un Texto segun la Fuente : LO MISMO FUNCIONA
//==========================================================================
//--------------------------------------------------------------------------
// Devuelve la longitud en pixels de un texto dado.
//--------------------------------------------------------------------------
uint cFuente::getAnchoTexto ( const char * pcText, cGLWindow * poWindow )
{
	if (pcText == NULL)
		return 0;

	uint res = 0;
    uint uI, uiCont = (uint) strlen(pcText);
    float fValor, fAspectX;

    fAspectX = (!poWindow)? 1.0f: (float) poWindow->getAspectX();
	for ( uI=0; uI<uiCont; uI++ )
	{
        unsigned int iC = (unsigned int) pcText[uI];
        if (iC < 256)
        {
            fValor = m_vfAncho[iC] * fAspectX;
            res += int ( fValor );
        }
	}

    res = (uint) (res * (m_ptFuente->fEsc));

	// Cuidado, puede modificar comportamientos de otras aplicaciones
	// res = (uint)(res * 1.3);

	return res;
}


uint cFuente::getLongitud ( cString * posCadP, cGLWindow * poWindow )
{
    if (posCadP)
    {
        return getAnchoTexto(posCadP->Cad(), poWindow);
    }
    return 0;
}


uint cFuente::getLongitud ( const char * pcCad, cGLWindow * poWindow )
{
    if (pcCad)
    {
        return getAnchoTexto(pcCad,poWindow);
    }
    return 0;
}


//--------------------------------------------------------------------------
// Solo me falta algo que me de la Altura en Pixeles de una fuente
//--------------------------------------------------------------------------
uint cFuente::getAltoTexto ( cGLWindow * poWindow )
{
    float fAlto = (float) m_fCharMaxHeight;
    if (poWindow)
    {
        fAlto = fAlto * (float) poWindow->getAspectY();
    }
    return (uint) fAlto;
}


uint cFuente::getAltura ( cGLWindow * poWindow )
{
    return getAltoTexto(poWindow);
}


int cFuente::getPeso ( void )
{
    return m_ptFuente->iPeso;
}


//==========================================================================
int cFuente::Destruye ( void )
{
	mDelete(m_poTextura);
	glDeleteLists(m_ptFuente->iBase,m_ptFuente->iNItems);	// Borra las 256 listas de la fuente.
	mDelete(m_ptFuente);

	return 0;
}


int cFuente::CambiaFuente ( int iSet, float fEsc )
{
	m_ptFuente->iGrupo  = iSet;
	m_ptFuente->fEsc    = fEsc;
	return 0;
}


//--------------------------------------------------------------------------
// Pasa a la siguiente linea
//--------------------------------------------------------------------------
int cFuente::saltaLinea (cGLWindow * poWindow)
{
    m_iY = m_iY - (getAltura(poWindow) + 1);

    return 0;
}


//--------------------------------------------------------------------------
// Admite un formato variable de mensaje
//--------------------------------------------------------------------------
// #define _RISK_
#ifdef _RISK_
int cFuente::escribe(cGLWindow* poWindow, int iXp, int iYp, int iZp, float escala, int p_iCol, const char* p_pcFormato, ...)
{
	if (!m_bActivada)
	{
		Carga(poWindow, m_ptFuente->iCharSet);
	}

	if (m_bActivada && poWindow)
	{
		va_list	tAp;
		char	vcMensaje[LON_BUFF / 4];

		mInicio(vcMensaje);
		va_start(tAp, p_pcFormato);
		vsprintf_s(vcMensaje, sizeof(vcMensaje), p_pcFormato, tAp);
		va_end(tAp);

		int iX = iXp;
		int iY = iYp;

		double	dX, dY, dYConOffset;

		if (iX == -1) { iX = m_iX; }
		if (iY == -1) { iY = m_iY; }

		m_iX = iX;
		m_iY = (int)cOpenGL::SetSentidoCoordY(iY, (getAltura(poWindow) + 1));	// iYp - (getAltura(poWindow) + 1);

		dYConOffset = iY - m_ptFuente->fOffset;

		dX = cOpenGL::DarX(iX);
		dY = cOpenGL::DarY(dYConOffset);
		//dX = iX;
		//dY = dYConOffset;

		//----------------------------------
		if (m_ptFuente->bSombra)
		{
			int iIncSombra;
			const TvColor* ptColor;
			TvColor tColor;

			iIncSombra = m_ptFuente->iIncSombra;
			ptColor = cColor::getColor(p_iCol);

			tColor.vCol[0] = ptColor->vCol[0] * 0.5f;
			tColor.vCol[1] = ptColor->vCol[1] * 0.5f;
			tColor.vCol[2] = ptColor->vCol[2] * 0.5f;
			tColor.vCol[3] = ptColor->vCol[3];

			cColor::color(&tColor);
			escribeFuente(poWindow, (int)dX + iIncSombra, (int)dY - iIncSombra, iZp, escala, -1, vcMensaje);     // Escribimos el mensaje.
		}

		escribeFuente(poWindow, (int)dX, (int)dY, iZp, escala, p_iCol, vcMensaje);     // Escribimos el mensaje.
		//----------------------------------
	}

	return 0;
}
#else
int cFuente::escribe(cGLWindow* poWindow, int iXp, int iYp, int iZp, float escala, int p_iCol, const char* p_pcFormato, ...)
{
	if (!m_bActivada)
	{
		Carga(poWindow, m_ptFuente->iCharSet);
	}

	if (m_bActivada && poWindow)
	{
		va_list	tAp;
		char	vcMensaje[LON_BUFF / 4];

		mInicio(vcMensaje);
		va_start(tAp, p_pcFormato);
		vsprintf_s(vcMensaje, sizeof(vcMensaje), p_pcFormato, tAp);
		va_end(tAp);

		int iX = iXp;
		int iY = iYp;

		double	dX, dY, dYConOffset;

		if (iX == -1) { iX = m_iX; }
		if (iY == -1) { iY = m_iY; }

		m_iX = iX;
		m_iY = (int)cOpenGL::SetSentidoCoordY(iY, (getAltura(poWindow) + 1));	// iYp - (getAltura(poWindow) + 1);

		dYConOffset = iY - m_ptFuente->fOffset;

		dX = cOpenGL::DarX(iX);
		dY = cOpenGL::DarY(dYConOffset);

		//----------------------------------
		if (m_ptFuente->bSombra)
		{
			int iIncSombra;
			const TvColor* ptColor;
			TvColor tColor;

			iIncSombra = m_ptFuente->iIncSombra;
			ptColor = cColor::getColor(p_iCol);

			tColor.vCol[0] = ptColor->vCol[0] * 0.5f;
			tColor.vCol[1] = ptColor->vCol[1] * 0.5f;
			tColor.vCol[2] = ptColor->vCol[2] * 0.5f;
			tColor.vCol[3] = ptColor->vCol[3];

			cColor::color(&tColor);
			escribeFuente(poWindow, (int)dX + iIncSombra, (int)dY - iIncSombra, iZp, escala, -1, vcMensaje);     // Escribimos el mensaje.
		}

		escribeFuente(poWindow, (int)dX, (int)dY, iZp, escala, p_iCol, vcMensaje);     // Escribimos el mensaje.
		//----------------------------------
	}

	return 0;
}
#endif // _RISK_

//--------------------------------------------------------------------------
// escribe en una proyeccion ortogonal por medio de la funcion escribeLista
// No se preocupa de tener un formato libre, este ya se ha convertido
// por la función 'escribe'.
//--------------------------------------------------------------------------
int cFuente::escribeFuente ( cGLWindow * poWindow, int iX, int iY, int iZ, float escala, int iCol, char * pcMsg )
{
    double      dAspectX;
    double      dAspectY;
	float		fX, fY, fZ;
	int			iCont;

	iCont = (int) strlen(pcMsg);
	fX = (float) iX;
	fY = (float) iY;
	fZ = (float) iZ;

    dAspectX = poWindow->getAspectX();
    dAspectY = poWindow->getAspectY();
    cColor::color(iCol);
	switch(m_ptFuente->eTipoFon)
	{
			case eTipoFuentes::eFT01:
			{
				int iList;

                cOpenGL::pushMatrix();
                {
					iList = m_ptFuente->iBase - 32 + (128 * m_ptFuente->iGrupo);	// Set: elige el conjunto de la fuente

					glTranslatef(fX,fY,fZ);						// Posición 0,0 (Abajo,Izquierda)
                    if	(dAspectX != 1.0 || dAspectY!=1.0)
                    {
                        glScaled(dAspectX, dAspectY, 1.0);
                    }
					m_poTextura->activa();
					glListBase(iList);
					glCallLists(iCont,GL_BYTE,pcMsg);			// escribe el texto en la pantalla
					m_poTextura->desActiva();
                }
                cOpenGL::popMatrix();

                fX += getAnchoTexto(pcMsg, poWindow);
                m_iSiguienteX = (int)fX;
                m_iUltimoY = (int)fY;
            }
			break;

		case eTipoFuentes::eFT02:
		case eTipoFuentes::eAT01:
			{
				if	(m_poTextura)
				{
					m_poTextura->activa();
					escribeTrozo(poWindow, iX, iY, iZ, escala, pcMsg );
					m_poTextura->desActiva();
				}
			}
			break;

		case eTipoFuentes::eFN01:
			{
                int iList = m_ptFuente->iBase - 32;
                bool bTextura2D = cOpenGL::disable(GL_TEXTURE_2D);
                cOpenGL::pushMatrix();
                {
				    glListBase(iList);
                    
                    glRasterPos3d(fX, fY, fZ);
                    //glRasterPos2d(fX, fY);

                    // La escala no le afecta en absoluto ...
                    if	(dAspectX != 1.0 || dAspectY!=1.0)
                    {
                        glScaled(dAspectX, dAspectY, 1.0);
                    }

				    glCallLists( (GLint) iCont,GL_UNSIGNED_BYTE, pcMsg);
                }
                cOpenGL::popMatrix();
                cOpenGL::restore(bTextura2D, GL_TEXTURE_2D);

                fX += getAnchoTexto(pcMsg, poWindow);
                m_iSiguienteX = (int)fX;
                m_iUltimoY = (int)fY;
            }
			break;
	}
	return 0;
}


//--------------------------------------------------------------------------
// escribe trozos de la textura en la que se ha convertido la fuente
//--------------------------------------------------------------------------
int cFuente::escribeTrozo( cGLWindow  * poWindow, int iX, int iY, int iZ, float escala, char * pcMsg )
{
	float	fX, fY, fZ, fEsc, fIncAncho, fIncAlto;
	float	fX1, fY1;
    double  dAspectX;
    double  dAspectY;
	float	fGrupo;
	int		i, iLon;
	bool	bBlend;
	bool	bEspecial;
	byte	ucCar;
    byte    ucInt;

    dAspectX = poWindow->getAspectX();      // Escala actual por resolucion de la X de la Ventana
    dAspectY = poWindow->getAspectY();      // Escala actual por resolucion de la Y de la Ventana

	fX = (float) iX;
    fY = (float) iY;
    fZ = (float) iZ;
    fEsc = m_ptFuente->fEsc;                // Escala que le asociamos a la fuente

    // Estos pueden ser propios de cada caracter, aun asi, en fuentes basadas en texturas
    // estos pueden ser los mismos
    fIncAncho = m_ptFuente->iAncho*fEsc;    // * (float) dAspectX;
    fIncAlto = m_ptFuente->iAlto*fEsc;      // * (float) dAspectY;
	fGrupo = (m_ptFuente->iGrupo)? 0.5f: 0.0f;
	iLon= (int) strlen(pcMsg);
    ucInt = '¿';

	bBlend = mbCond(glIsEnabled(GL_BLEND));
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	if	(!bBlend)
		glEnable(GL_BLEND);

	for ( i=0, bEspecial=false; i<iLon; i++ )
	{
		ucCar = (byte) pcMsg[i];
		if	(ucCar==ucInt)
		{
			bEspecial = true;
			ucCar = '?';
		}

        cOpenGL::pushMatrix();
        {
		    if	(bEspecial)
		    {
				    fX1 = fX - (fX + (fIncAncho/2));
				    fY1 = fY - (fY - (fIncAlto/2));

                    // Vamos a rotar la '?', para que sea una '¿'.
                    glTranslatef(fX + (fIncAncho/2), fY - (fIncAlto/2), 0);
				    glRotatef(180,0,0,1);
                    if	(dAspectX != 1.0 || dAspectY!=1.0)
                    {
                        glScaled(dAspectX, dAspectY, 1.0);
                    }
				    glBegin(GL_QUADS);
                        glTexCoord2f( m_ptFuente->fCoord[(ucCar-32)][0], m_ptFuente->fCoord[(ucCar-32)][3] + fGrupo);   glVertex3f( fX1             , (fY1-fIncAlto), fZ);
                        glTexCoord2f( m_ptFuente->fCoord[(ucCar-32)][2], m_ptFuente->fCoord[(ucCar-32)][3] + fGrupo);   glVertex3f( (fX1+fIncAncho) , (fY1-fIncAlto), fZ);
                        glTexCoord2f( m_ptFuente->fCoord[(ucCar-32)][2], m_ptFuente->fCoord[(ucCar-32)][1] + fGrupo);   glVertex3f( (fX1+fIncAncho) , fY1           , fZ);
                        glTexCoord2f( m_ptFuente->fCoord[(ucCar-32)][0], m_ptFuente->fCoord[(ucCar-32)][1] + fGrupo);   glVertex3f( fX1		        , fY1           , fZ);
				    glEnd();
			    bEspecial = false;
		    }
		    else
		    {
                glTranslatef(fX, fY, 0);
                if	(dAspectX != 1.0 || dAspectY!=1.0)
                {
                    glScaled(dAspectX, dAspectY, 1.0);
                }
			    glBegin(GL_QUADS);
                    glTexCoord2f( m_ptFuente->fCoord[(ucCar-32)][0], m_ptFuente->fCoord[(ucCar-32)][3] + fGrupo);	glVertex3f(0.0          , -fIncAlto , fZ);
                    glTexCoord2f( m_ptFuente->fCoord[(ucCar-32)][2], m_ptFuente->fCoord[(ucCar-32)][3] + fGrupo);	glVertex3f(fIncAncho    , -fIncAlto , fZ);
                    glTexCoord2f( m_ptFuente->fCoord[(ucCar-32)][2], m_ptFuente->fCoord[(ucCar-32)][1] + fGrupo);	glVertex3f(fIncAncho    , 0.0       , fZ);
                    glTexCoord2f( m_ptFuente->fCoord[(ucCar-32)][0], m_ptFuente->fCoord[(ucCar-32)][1] + fGrupo);	glVertex3f(0.0          , 0.0       , fZ);
			    glEnd();
		    }
        }
        cOpenGL::popMatrix();

        fX += m_vfAncho[ucCar - 32] * fEsc * (float)dAspectX;
        m_iSiguienteX = (int)fX;
        m_iUltimoY = (int)fY;
    }

	if	(!bBlend)
		glDisable(GL_BLEND);

	return 0;
}


//--------------------------------------------------------------------------
// Divide el mensaje a escribir en titulo y detalle.
// Pensado para poner diferentes colores en la misma linea
//--------------------------------------------------------------------------
int cFuente::detalleTitulo (cGLWindow * poWindow, int iX, int iY, int iZ, float escala, int iColTitulo, int iColDetalle, const char * pcTitulo, const char * pcFormatDetalle, ...)
{
    // Diferentes Colores en la misma linea, como lo hacemos:

    //----------------------------------------------------------------------
    // Escribimos el Titulo
    //----------------------------------------------------------------------
    escribe(poWindow, iX, iY, iZ, escala, iColTitulo, pcTitulo);
    //----------------------------------------------------------------------

    //----------------------------------------------------------------------
    // Guardamos posicion titulo
    //----------------------------------------------------------------------
    m_iSiguienteTitulo = DarSiguienteX();
    //----------------------------------------------------------------------

    //----------------------------------------------------------------------
    // Escribimos el Detalle
    //----------------------------------------------------------------------
    va_list	tAp;
    char	vcMensaje[LON_BUFF / 4];

    mInicio(vcMensaje);
    va_start(tAp, pcFormatDetalle);
    vsprintf_s(vcMensaje, sizeof(vcMensaje), pcFormatDetalle, tAp);
    va_end(tAp);

    // Guardo donde empieza en X, el salto de linea.
    iX = getX();
    escribe(
        poWindow,
        DarSiguienteX(),
        DarUltimoY(),
        iZ,
		escala,
        iColDetalle,
        vcMensaje);
    // Recupero donde empieza en X, el salto de linea.
    setX(iX);
    //----------------------------------------------------------------------
    return 0;
}


//--------------------------------------------------------------------------
// Lo mismo que el anterior, pero continua en la misma linea.
// Pensado para poner diferentes colores en la misma linea
//
// Solo tendria sentido detras de un:
//  - detalleTitulo
//  - otro detalleTituloSiguiente
//
//--------------------------------------------------------------------------
int cFuente::detalleTituloSiguiente  (cGLWindow * poWindow, int iDespX, int iZ, float escala, int iColTitulo, int iColDetalle, const char * pcTitulo, const char * pcFormatDetalle, ...)
{
    //----------------------------------------------------------------------
    // Guardo donde empieza en X, el salto de linea.
    //----------------------------------------------------------------------
    int iX = getX();
    //----------------------------------------------------------------------
    // En todos se realiza el setX(iX) que restablece la X anterior.
    // Nosotros nos quedaremos con la que termino el ultimo detalle , a la
    // que añadiremos el desplazamiento iDespX
    //----------------------------------------------------------------------
    int iXFinal = m_iSiguienteX + iDespX;
    int iYFinal = DarUltimoY();
    //----------------------------------------------------------------------

    //----------------------------------------------------------------------
    // Escribimos el Titulo
    //----------------------------------------------------------------------
    escribe(poWindow, iXFinal, iYFinal, iZ, escala, iColTitulo, pcTitulo);
    //----------------------------------------------------------------------

    //----------------------------------------------------------------------
    // NO Guardamos posicion titulo, aqui no hace falta
    //----------------------------------------------------------------------
    // m_iSiguienteTitulo = DarSiguienteX();
    //----------------------------------------------------------------------

    //----------------------------------------------------------------------
    // Escribimos el Detalle
    //----------------------------------------------------------------------
    va_list	tAp;
    char	vcMensaje[LON_BUFF / 4];

    mInicio(vcMensaje);
    va_start(tAp, pcFormatDetalle);
    vsprintf_s(vcMensaje, sizeof(vcMensaje), pcFormatDetalle, tAp);
    va_end(tAp);

    escribe(
        poWindow,
        DarSiguienteX(),
        DarUltimoY(),
        iZ,
		escala,
        iColDetalle,
        vcMensaje);
    //----------------------------------------------------------------------
    // Recupero donde empieza en X, el salto de linea.
    //----------------------------------------------------------------------
    setX(iX);
    //----------------------------------------------------------------------

    return 0;
}


int cFuente::detalleSiguienteLinea (cGLWindow * poWindow, int iX, int iY, int iZ, float escala, int iColDetalle, const char * pcFormatDetalle, ...)
{
    //----------------------------------------------------------------------
    // Escribimos el Detalle
    //----------------------------------------------------------------------
    va_list	tAp;
    char	vcMensaje[LON_BUFF / 4];

    mInicio(vcMensaje);
    va_start(tAp, pcFormatDetalle);
    vsprintf_s(vcMensaje, sizeof(vcMensaje), pcFormatDetalle, tAp);
    va_end(tAp);


    // Guardo donde empieza en X, el salto de linea.
    int iXaux = getX();

    escribe(
        poWindow,
        (iX==-1)? DarSiguienteTit(): iX,
        iY,
        iZ,
		escala,
        iColDetalle,
        vcMensaje);

    // Recupero donde empieza en X, el salto de linea.
    setX(iXaux);
    //----------------------------------------------------------------------
    return 0;
}


//==========================================================================
// 'escribeW()' - Vamos a mantener una funcion que escriba segun el modo
// de Windows.
//==========================================================================
void escribeW (HWND hWnd, int x, int y,
    GLubyte  cFondo, GLubyte cTexto,
    char * format, ...)
{
    cColor	  *	poColor = cColor::Instancia();
    HDC			hdc;
    va_list		ap;			// Argument pointer
    char		vcMensaje[1024];
    const TvColor * ptColor;

    if (format == NULL)
        return;

    va_start(ap, format);
    vsprintf_s(vcMensaje, sizeof(vcMensaje), format, ap);
    va_end(ap);

    hdc = GetDC (hWnd);           // Contexto de dispositivo

    ptColor = poColor->getColor(cFondo);
    SetBkColor(hdc, RGB(ptColor->vCol[0], ptColor->vCol[1], ptColor->vCol[2]));

    ptColor = poColor->getColor(cTexto);
    SetTextColor(hdc, RGB(ptColor->vCol[0], ptColor->vCol[1], ptColor->vCol[2]));

    TextOut(hdc, x, y, vcMensaje, (int)strlen(vcMensaje));
}



//==========================================================================
//  Fin de cFuente.cpp
//==========================================================================

