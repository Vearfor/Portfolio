/*========================================================================*\
|* cFuente.cpp
\*========================================================================*/
//==========================================================================
// 16/08/2003 : Enrique Rocafull Ortiz
//==========================================================================
//  Módulo de tratamiento de las fuentes para escribir mensajes.
//==========================================================================


#include "cFuente.h"
#include "cGestorTexturas.h"
#include "cGestorFuentes.h"
#include "../windows/sWindow.h"
#include "../cColor.h"
#include <tool/cTool.h>
#include <tool/sMath.h>


//--------------------------------------------------------------------------
// Statics
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constructor/Destructor
//--------------------------------------------------------------------------
cFuente::cFuente ( void )
	: m_tFuente()
	, m_poTextura(NULL)
    , m_iSiguienteX(0)
    , m_iSiguienteTitulo(0)
    , m_iUltimoY(0)
	, m_bActivada(false)
    , m_fAnchoXCaracter(0)
	, m_fCharMaxHeight(0.0f)
    , m_bFull(false)
    , m_iTraza(cLog::eTraza::min)
    , m_fX(-1.0f)
    , m_fY(-1.0f)
{
	mInicio(m_vfAncho);
}


cFuente::~cFuente ( void )
{
	destruye();
}



//--------------------------------------------------------------------------
// Metodos
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Iniciamos los parametros de la textura
//--------------------------------------------------------------------------
int cFuente::inicia ( const sFuente & tFuente )
{
	m_tFuente = tFuente;
	mCopia(m_tFuente.vcNombre, tFuente.vcNombre);
	mCopia(m_tFuente.vcFichero, tFuente.vcFichero);
	setNombre(m_tFuente.vcNombre);
	return 0;
}


//--------------------------------------------------------------------------
// Cargamos los valores y generamos la fuente.
//--------------------------------------------------------------------------
int cFuente::Carga ( sWindow * poWindow, int iCharSet )
{
	int	iRes = 0;

	if (!m_bActivada)
	{
		// Si la fuente necesita textura debemos cargarla
		if (m_tFuente.eTipoFon != eTipoFuentes::eFN01)
		{
			// El nombre de la textura en el gestor es el nombre de la fuente
			// No es el nombre con el directorio:
			m_poTextura = cGestorTexturas::Instancia()->GetTextura(m_tFuente.vcNombre);
			// Si lo tenemos, bien, sino habra que cargarla:
			if (!m_poTextura)
			{
				if (
						cGestorTexturas::Instancia()->CargaTextura(m_tFuente.vcFichero) ||
						(m_poTextura = cGestorTexturas::Instancia()->GetTextura(m_tFuente.vcNombre)) == nullptr
				   )
				{
					cLog::error(" Error: cFuente::Carga: no tenemos textura para la fuente\n");
					return -1;
				}
			}
		}

		iRes = Construye(poWindow, iCharSet);

		m_bActivada = !iRes;
	}
	return iRes;
}


//--------------------------------------------------------------------------
// Construye la fuente
//--------------------------------------------------------------------------
int cFuente::Construye (sWindow * poWindow, int iCharSet)
{
    const char * _metodo = __FUNCTION__;

    //----------------------------------------------------------------------
	// Generación de la fuente.
	//----------------------------------------------------------------------
	int	iRes = 0;
	//----------------------------------------------------------------------
	//------------------------------------------------------------------
    HDC hDc = poWindow->getDeviceContext();
	//------------------------------------------------------------------
	if	(m_tFuente.eTipoFon!= eTipoFuentes::eFN01)
	{
		if	(m_poTextura)
		{
			//iRes =
			//	(
			//			m_poTextura->inicia(m_tFuente.idTextura, m_tFuente.vcNombre, m_tFuente.vcFichero, m_tFuente.eTipoTex,NULL) != 0
			//		||	m_poTextura->carga() != 0
			//	);
			//miError(iRes);

			m_tFuente.iTextura = m_poTextura->getIdTex();   // Las fuentes con texturas dependen su ancho y su alto de las dimensiones
            m_tFuente.iAnchoTex = m_poTextura->getWidth();    // del fichero de textura.
			m_tFuente.iAltoTex = m_poTextura->getHeight();      // 256 x 256 (Generalmente)
		}
	}
	else
	{
		m_tFuente.iNItems = 256 - 32;	// 96;
	}
    //------------------------------------------------------------------
    GLint   iLado;       // Distinto según la textura
	memset( (void *) m_vfAncho, 0, sizeof(m_vfAncho) );
	m_fCharMaxHeight = 0;
    m_fAnchoXCaracter = 1;

	//------------------------------------------------------------------
	switch(m_tFuente.eTipoFon)
	{
		case eTipoFuentes::eFT01:
			//----------------------------------------------------------
			{
                GLfloat	    cx;		     // Coordenada X
				GLfloat	    cy;		     // Coordenada Y
				GLint       il;          // Indice del bucle.
				GLfloat     fILado;      // 1/iLado
				int			iAncho;

                iLado	= (int) sqrt((double) m_tFuente.iAnchoTex);	    // Texturas de 256 x 256, y de 256 elementos (=16x16).
				fILado	= (float) 1/iLado;								    // 1/16 = 0.0625
				iAncho	= m_tFuente.iAncho;							    // m_ptFuente.iTam;

                m_tFuente.iNItems = 256;                                  // 256 elementos.
                    
				m_tFuente.iBase = glGenLists(m_tFuente.iNItems);	    // Crea 256 listas vacías
				glBindTexture(GL_TEXTURE_2D, m_tFuente.iTextura);		    // Selecciona la textura con las fuentes

				for ( il=0; il<m_tFuente.iNItems; il++ )				    // Bucle para los 256 elementos/listas
				{
					cx=(float)(il%iLado)/iLado;								// Posición X para el caracter en curso.
					cy=(float)(il/iLado)/iLado;								// Posición Y para el caracter en curso.

					glNewList(m_tFuente.iBase+il, GL_COMPILE);			// Comienza una lista de visualización.
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
				//------------------------------------------------------
				for (int i=0;  i<m_tFuente.iNItems;  i++)
				{
					m_vfAncho[i] = (float) m_tFuente.iAncho;
				}
				m_fAnchoXCaracter = (float) m_tFuente.iAncho;
				m_fCharMaxHeight = (float) m_tFuente.iAlto;
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
				for ( k=0, j=1; j<(m_tFuente.iAltoTex/2); j+=16 )
				{
					for ( i=0; i<m_tFuente.iAnchoTex; i+=16 )
					{
						switch (m_tFuente.eTipoFon)
						{
							case eTipoFuentes::eFT02:   // Fuente TGA
								m_tFuente.fCoord[k][0] = (float) i / (float) m_tFuente.iAnchoTex;
								m_tFuente.fCoord[k][1] = (float) j / (float) m_tFuente.iAltoTex;
								m_tFuente.fCoord[k][2] = (float) i / (float) m_tFuente.iAnchoTex + (float) 15/(float)m_tFuente.iAnchoTex;
								m_tFuente.fCoord[k][3] = (float) j / (float) m_tFuente.iAltoTex  + (float) 15/(float)m_tFuente.iAltoTex;
								break;

                            case eTipoFuentes::eAT01:   // Fuente BMP
								m_tFuente.fCoord[k][0] = (float) i / (float) m_tFuente.iAnchoTex;
								m_tFuente.fCoord[k][1] = 0.5f - ((float) j / (float) m_tFuente.iAltoTex);
								m_tFuente.fCoord[k][2] = (float) i / (float) m_tFuente.iAnchoTex + (float) 15/(float)m_tFuente.iAnchoTex;
								m_tFuente.fCoord[k][3] = 0.5f - ((float) j / (float) m_tFuente.iAltoTex  + (float) 15/(float)m_tFuente.iAltoTex);
								break;
						}
						k++;
					}
				}
				//------------------------------------------------------
				for ( i=0;  i<256;  i++)
				{
					m_vfAncho[i] = (float) m_tFuente.iAncho;
				}
				m_fAnchoXCaracter = (float) m_tFuente.iAncho;
				m_fCharMaxHeight = (float) m_tFuente.iAlto;
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

                m_tFuente.bOutLine = false;
				m_tFuente.iBase = glGenLists(m_tFuente.iNItems);

				iCSet = (iCharSet == -1) ? ANSI_CHARSET : m_tFuente.iCharSet;
				iCSet = (iCSet == -1) ? ANSI_CHARSET : iCSet;

				WCHAR wcNombre[256];
				cTool::copiaMultibyteToUnicode(m_tFuente.vcNombre, wcNombre, sizeof(wcNombre));
				// Parece ser que el Ancho no nos sirve de mucho, siempre hay que recalcularlo.
				// Y la altura se pasa como negativa.
                SetLastError(0);
				hFont = CreateFont
					(
						m_tFuente.iAlto,			    // Altura de la fuente
						m_tFuente.iAncho,				// Ancho medio de los caracteres
						0,								// Angulo de escape (?)
						0,								// Angulo de orientacion de la linea base ( y el eje X)
						m_tFuente.iPeso,				// Peso de la fuente
						mBTrue(m_tFuente.bItalic),		// italic atribute option
						mBTrue(m_tFuente.bUnderLine),	// underline attribute option
						mBTrue(m_tFuente.bStrikeOut),	// strikeout attribute option
						iCSet,							// conjunto de caracteres
						OUT_TT_PRECIS,					// output precision
						CLIP_DEFAULT_PRECIS,			// clipping precision
						ANTIALIASED_QUALITY,			// output quality
						FF_DONTCARE|DEFAULT_PITCH,		// pitch and family
						wcNombre						// typeface name
					);
				miNulo(hFont);

				// Valores eArial1 en Orion:
				// iAlto = 0
				// iAncho = 0
				// angulo escape 0
				// angulo orientacion 0
				// peso = 600
				// italic false
				// under line false
				// Strike out false
				// iCSet 0
				// iOffset 12
				// sombra true
				// incSombra 1
				// Nombre "Arial"

				hOldFont = (HFONT) SelectObject(hDc, hFont);

				char* pcCharSet = cGestorFuentes::BuscaCharSet(iCSet);
				cLog::print("   Genero fuente %s %d\n", m_tFuente.vcNombre, m_tFuente.idFuente);
				cLog::print("   CharSet  %d  [%s]\n", iCSet, msNulo(pcCharSet));
				cLog::print("   + Peso %d\n", m_tFuente.iPeso );
                cLog::print("   + Alto %d\n", m_tFuente.iAlto );
                cLog::print("   + Ancho %d\n", m_tFuente.iAncho );
                cLog::print("     Outline a %s\n", msTrue(m_tFuente.bOutLine));
				if	(m_tFuente.bOutLine)
				{
					GLYPHMETRICSFLOAT agmf[256];
					// create display lists for glyphs 0 through 255 with 0.1 extrusion 
					// and default deviation. The display list numbering starts at 1000 
					// (it could be any number) 
					//wglUseFontOutlines(hdc, 0, 255, 1000, 0.0f, 0.1f, WGL_FONT_POLYGONS, &agmf);
					wglUseFontOutlines(hDc, 32, m_tFuente.iNItems, m_tFuente.iBase, 0.0f, 0.1f, WGL_FONT_POLYGONS, &agmf[0]);
				}
				else
				{
					wglUseFontBitmaps(hDc, 32, m_tFuente.iNItems, m_tFuente.iBase);
				}
				SelectObject(hDc,hOldFont);
				DeleteObject(hFont);
                //------------------------------------------------------
                for (unsigned int i=0;  i<256;  i++)
                {
                    SIZE size;
					WCHAR character = WCHAR(i);

                    if (GetTextExtentPoint32(hDc, &character,1, &size) == FALSE)
                    {
                        m_vfAncho[i] = 0.0f;
                    }
                    else
                    {
                        m_vfAncho[i] = float(size.cx);
                        m_fCharMaxHeight = max(m_fCharMaxHeight, float(size.cy));
                    }
                    m_fAnchoXCaracter = max(m_fAnchoXCaracter, m_vfAncho[i]);
                }
                //------------------------------------------------------
			}
			//----------------------------------------------------------
			break;
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
uint cFuente::getAnchoTexto ( const char * pcText, sWindow * poWindow )
{
	if (pcText == NULL)
		return 0;

	uint res = 0;
    uint uI, uiCont = (uint) strlen(pcText);
    float fValor, fAspectX;

    fAspectX = (!poWindow)? 1.0f: poWindow->getAspectX();
	for ( uI=0; uI<uiCont; uI++ )
	{
        unsigned int iC = (unsigned int) pcText[uI];
        if (iC < 256)
        {
            fValor = m_vfAncho[iC] * fAspectX;
            res += int ( fValor );
        }
	}

    res = (uint) (res * (m_tFuente.fEsc));

	return res;
}


uint cFuente::getLongitud ( std::string * posCadP, sWindow * poWindow )
{
    if (posCadP)
    {
        return getAnchoTexto(posCadP->c_str(), poWindow);
    }
    return 0;
}


uint cFuente::getLongitud ( const char * pcCad, sWindow * poWindow )
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
uint cFuente::getAltoTexto ( sWindow * poWindow )
{
    float fAlto = (float) m_fCharMaxHeight;
    if (poWindow)
    {
        fAlto = fAlto * (float) poWindow->getAspectY();
    }
    return (uint) fAlto;
}


uint cFuente::getAltura ( sWindow * poWindow )
{
    return getAltoTexto(poWindow);
}


int cFuente::getPeso ( void )
{
    return m_tFuente.iPeso;
}


//==========================================================================
int cFuente::destruye ( void )
{
	// Si hay textura se destruye en el gestor de texturas
	glDeleteLists(m_tFuente.iBase,m_tFuente.iNItems);	// Borra las 256 listas de la fuente.
	return 0;
}


int cFuente::CambiaFuente ( int iSet, float fEsc )
{
	m_tFuente.iGrupo  = iSet;
	m_tFuente.fEsc    = fEsc;
	return 0;
}


//--------------------------------------------------------------------------
// Pasa a la siguiente linea
//--------------------------------------------------------------------------
int cFuente::saltaLinea (sWindow * poWindow)
{
    m_fY = m_fY - (getAltura(poWindow) + 1);
    return 0;
}


//--------------------------------------------------------------------------
// Admite un formato variable de mensaje
//--------------------------------------------------------------------------
// #define _RISK_
//--------------------------------------------------------------------------
#ifdef _RISK_
//--------------------------------------------------------------------------
// int cFuente::escribe(sWindow* poWindow, 
//						int iXp, int iYp, int iZp, 
//						float escala, 
//						int p_iCol, 
//						const char* p_pcFormato, ...)
//--------------------------------------------------------------------------
int cFuente::escribe(
	sWindow* poWindow, 
	glm::ivec3 pos, 
	float escala, 
	glm::vec4 vCol,
	const char* p_pcFormato, ...)
{
	if (!m_bActivada)
	{
		Carga(poWindow, m_tFuente.iCharSet);
	}

	if (m_bActivada && poWindow)
	{
		va_list	tAp;
		char	vcMensaje[LON_BUFF / 4];

		mInicio(vcMensaje);
		va_start(tAp, p_pcFormato);
		vsprintf_s(vcMensaje, sizeof(vcMensaje), p_pcFormato, tAp);
		va_end(tAp);

		int iX = pos.x;
		int iY = pos.y;

		float fX, fY, fYConOffset;

		if (iX == -1) { iX = static_cast<int>(m_fX); }
		if (iY == -1) { iY = static_cast<int>(m_fY); }

		m_fX = static_cast<float>(iX);
		m_fY = sOpenGL::SetSentidoCoordY(iY, (getAltura(poWindow) + 1));	// iYp - (getAltura(poWindow) + 1);

		fYConOffset = iY - m_tFuente.fOffset;

		fX = sOpenGL::DarX(iX);				// fX = iX;
		fY = sOpenGL::DarY(fYConOffset);	// fY = fYConOffset;

		//----------------------------------
		if (m_tFuente.bSombra)
		{
			int iIncSombra;
			glm::vec4 vColor;

			iIncSombra = m_tFuente.iIncSombra;

			vColor.r = vCol.r * 0.5f;
			vColor.g = vCol.g * 0.5f;
			vColor.b = vCol.b * 0.5f;
			vColor.a = vCol.a;

			sOpenGL::color(vColor);
			escribeFuente(poWindow,
				glm::ivec3((int)fX + iIncSombra, (int)fY - iIncSombra, pos.z),
				escala,
				cColor::vNoColor,
				vcMensaje);     // Escribimos el mensaje.
		}

		escribeFuente(
			poWindow, 
			glm::ivec3((int)fX, (int)fY, pos.z), 
			escala, 
			cColor::vNoColor,
			vcMensaje);     // Escribimos el mensaje.
		//----------------------------------
	}

	return 0;
}
#else
int cFuente::escribe(
	sWindow* poWindow,
	const glm::ivec3 & pos, 
	float escala,
	const glm::vec4 & vCol,
	const char* p_pcFormato, ...)
{
	if (!m_bActivada)
	{
		Carga(poWindow, m_tFuente.iCharSet);
	}

	if (m_bActivada && poWindow)
	{
		va_list	tAp;
		char	vcMensaje[LON_BUFF / 4];

		mInicio(vcMensaje);
		va_start(tAp, p_pcFormato);
		vsprintf_s(vcMensaje, sizeof(vcMensaje), p_pcFormato, tAp);
		va_end(tAp);

		int iX = (int) pos.x;
		int iY = (int) pos.y;

		float	fX, fY, fYConOffset;

		if (iX == -1) { iX = (int) m_fX; }
		if (iY == -1) { iY = (int) m_fY; }

		m_fX = (float) iX;
		m_fY = sOpenGL::SetSentidoCoordY( (float)(iY), (float)(getAltura(poWindow) + 1));

		fYConOffset = (float)iY - m_tFuente.fOffset;

		fX = sOpenGL::DarX((float)iX);
		fY = sOpenGL::DarY(fYConOffset);

		//----------------------------------
		if (m_tFuente.bSombra)
		{
			int iIncSombra;
			glm::vec4 vColor;

			iIncSombra = m_tFuente.iIncSombra;

			vColor.r = vCol.r * 0.5f;
			vColor.g = vCol.g * 0.5f;
			vColor.b = vCol.b * 0.5f;
			vColor.a = vCol.a;

			sOpenGL::color(vColor);
			escribeFuente(
				poWindow,
				glm::ivec3((int)fX + iIncSombra, (int)fY - iIncSombra, pos.z), 
				escala, 
				cColor::vNoColor,
				vcMensaje);     // Escribimos el mensaje.
		}

		escribeFuente(
			poWindow,
			glm::ivec3((int)fX, (int)fY, pos.z), 
			escala,
			vCol, 
			vcMensaje);     // Escribimos el mensaje.
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
int cFuente::escribeFuente ( sWindow * poWindow, glm::ivec3 pos, float escala, glm::vec4 color, char * pcMsg )
{
	float fAspectX;
	float fAspectY;
	float fX, fY, fZ;
	size_t iCont;

	iCont = strlen(pcMsg);
	fX = (float) pos.x;
	fY = (float) pos.y;
	fZ = (float) pos.z;

	fAspectX = poWindow->getAspectX();
	fAspectY = poWindow->getAspectY();
	sOpenGL::color(color);
	switch(m_tFuente.eTipoFon)
	{
		case eTipoFuentes::eFT01:
			{
				int iList;
				sOpenGL::pushMatrix();
				{
					iList = m_tFuente.iBase - 32 + (128 * m_tFuente.iGrupo);	// Set: elige el conjunto de la fuente
					glTranslatef(fX,fY,fZ);										// Posición 0,0 (Abajo,Izquierda)
					if	(fAspectX != 1.0f || fAspectY!=1.0f)
					{
						glScaled(fAspectX, fAspectY, 1.0f);
					}
					m_poTextura->activa();
					glListBase(iList);
					glCallLists((GLsizei) iCont,GL_BYTE,pcMsg);			// escribe el texto en la pantalla
					m_poTextura->desActiva();
				}
				sOpenGL::popMatrix();
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
					escribeTrozo(poWindow, pos, escala, pcMsg );
					m_poTextura->desActiva();
				}
			}
			break;

		case eTipoFuentes::eFN01:
			{
                int iList = m_tFuente.iBase - 32;
                bool bTextura2D = sOpenGL::disable(GL_TEXTURE_2D);
                sOpenGL::pushMatrix();
                {
				    glListBase(iList);
                    
                    glRasterPos3d(fX, fY, fZ);

                    // La escala no le afecta en absoluto ...
                    if	(fAspectX != 1.0 || fAspectY!=1.0)
                    {
                        glScaled(fAspectX, fAspectY, 1.0);
                    }

				    glCallLists( (GLint) iCont,GL_UNSIGNED_BYTE, pcMsg);
                }
                sOpenGL::popMatrix();
                sOpenGL::restore(bTextura2D, GL_TEXTURE_2D);

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
int cFuente::escribeTrozo( sWindow  * poWindow, glm::ivec3 pos, float escala, char * pcMsg )
{
	float	fX, fY, fZ, fEsc, fIncAncho, fIncAlto;
	float	fX1, fY1;
    float fAspectX;
    float fAspectY;
	float	fGrupo;
	int		i, iLon;
	bool	bBlend;
	bool	bEspecial;
	byte	ucCar;
    byte    ucInt;

    fAspectX = poWindow->getAspectX();      // Escala actual por resolucion de la X de la Ventana
    fAspectY = poWindow->getAspectY();      // Escala actual por resolucion de la Y de la Ventana

	fX = (float) pos.x;
    fY = (float) pos.y;
    fZ = (float) pos.z;
    fEsc = m_tFuente.fEsc;                // Escala que le asociamos a la fuente

    // Estos pueden ser propios de cada caracter, aun asi, en fuentes basadas en texturas
    // estos pueden ser los mismos
    fIncAncho = m_tFuente.iAncho*fEsc;    // * (float) dAspectX;
    fIncAlto = m_tFuente.iAlto*fEsc;      // * (float) dAspectY;
	fGrupo = (m_tFuente.iGrupo)? 0.5f: 0.0f;
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

        sOpenGL::pushMatrix();
        {
		    if	(bEspecial)
		    {
				    fX1 = fX - (fX + (fIncAncho/2));
				    fY1 = fY - (fY - (fIncAlto/2));

                    // Vamos a rotar la '?', para que sea una '¿'.
                    glTranslatef(fX + (fIncAncho/2), fY - (fIncAlto/2), 0);
				    glRotatef(180,0,0,1);
                    if	(fAspectX != 1.0 || fAspectY!=1.0)
                    {
                        glScaled(fAspectX, fAspectY, 1.0);
                    }
				    glBegin(GL_QUADS);
                        glTexCoord2f( m_tFuente.fCoord[(ucCar-32)][0], m_tFuente.fCoord[(ucCar-32)][3] + fGrupo);   glVertex3f( fX1             , (fY1-fIncAlto), fZ);
                        glTexCoord2f( m_tFuente.fCoord[(ucCar-32)][2], m_tFuente.fCoord[(ucCar-32)][3] + fGrupo);   glVertex3f( (fX1+fIncAncho) , (fY1-fIncAlto), fZ);
                        glTexCoord2f( m_tFuente.fCoord[(ucCar-32)][2], m_tFuente.fCoord[(ucCar-32)][1] + fGrupo);   glVertex3f( (fX1+fIncAncho) , fY1           , fZ);
                        glTexCoord2f( m_tFuente.fCoord[(ucCar-32)][0], m_tFuente.fCoord[(ucCar-32)][1] + fGrupo);   glVertex3f( fX1		        , fY1           , fZ);
				    glEnd();
			    bEspecial = false;
		    }
		    else
		    {
                glTranslatef(fX, fY, 0);
                if	(fAspectX != 1.0 || fAspectY!=1.0f)
                {
                    glScalef(fAspectX, fAspectY, 1.01f);
                }
			    glBegin(GL_QUADS);
                    glTexCoord2f( m_tFuente.fCoord[(ucCar-32)][0], m_tFuente.fCoord[(ucCar-32)][3] + fGrupo);	glVertex3f(0.0          , -fIncAlto , fZ);
                    glTexCoord2f( m_tFuente.fCoord[(ucCar-32)][2], m_tFuente.fCoord[(ucCar-32)][3] + fGrupo);	glVertex3f(fIncAncho    , -fIncAlto , fZ);
                    glTexCoord2f( m_tFuente.fCoord[(ucCar-32)][2], m_tFuente.fCoord[(ucCar-32)][1] + fGrupo);	glVertex3f(fIncAncho    , 0.0       , fZ);
                    glTexCoord2f( m_tFuente.fCoord[(ucCar-32)][0], m_tFuente.fCoord[(ucCar-32)][1] + fGrupo);	glVertex3f(0.0          , 0.0       , fZ);
			    glEnd();
		    }
        }
        sOpenGL::popMatrix();

        fX += m_vfAncho[ucCar - 32] * fEsc * (float)fAspectX;
        m_iSiguienteX = (int)fX;
        m_iUltimoY = (int)fY;
    }

	if	(!bBlend)
		glDisable(GL_BLEND);

	return 0;
}


//--------------------------------------------------------------------------
// Divide el mensaje a escribir en titulo y detalle.
// Pensado para poner DIFERENTES COLORES en la MISMA LINEA
//--------------------------------------------------------------------------
int cFuente::detalleTitulo (
	sWindow * poWindow,
	glm::ivec3 pos,
	float escala, 
	glm::vec4 vColTitulo, 
	glm::vec4 vColDetalle, 
	const char * pcTitulo,
	const char * pcFormatDetalle, ...)
{
    // Diferentes Colores en la misma linea, como lo hacemos:

    //----------------------------------------------------------------------
    // Escribimos el Titulo
    //----------------------------------------------------------------------
    escribe(poWindow, pos, escala, vColTitulo, pcTitulo);
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
	// pero en float para no hacer un doble casting;
    float fXaux = getX();
    escribe(
        poWindow,
		glm::ivec3(DarSiguienteX(),DarUltimoY(),pos.z),
		escala,
        vColDetalle,
        vcMensaje);
    // Recupero donde empieza en X, el salto de linea.
    setX(fXaux);
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
int cFuente::detalleTituloSiguiente  (
	sWindow * poWindow, 
	int iDespX, int iZ, 
	float escala, 
	glm::vec4 vColTitulo,
	glm::vec4 vColDetalle, 
	const char * pcTitulo, const char * pcFormatDetalle, ...)
{
    //----------------------------------------------------------------------
    // Guardo donde empieza en X, el salto de linea.
    //----------------------------------------------------------------------
    float fXaux = getX();
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
    escribe(
		poWindow,
		glm::ivec3(iXFinal, iYFinal, iZ),
		escala,
		vColTitulo,
		pcTitulo);
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
		glm::ivec3(DarSiguienteX(), DarUltimoY(), iZ),
		escala,
        vColDetalle,
        vcMensaje);
    //----------------------------------------------------------------------
    // Recupero donde empieza en X, el salto de linea.
    //----------------------------------------------------------------------
    setX(fXaux);
    //----------------------------------------------------------------------

    return 0;
}


int cFuente::detalleSiguienteLinea (
	sWindow * poWindow,
	glm::ivec3 pos,			// int iX, int iY, int iZ
	float escala,
	glm::vec4 vColDetalle,
	const char * pcFormatDetalle, ...)
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
    float fXaux = getX();

    escribe(
        poWindow,
		glm::ivec3(
			(pos.x==-1)? DarSiguienteTit(): pos.x,
			pos.y,
			pos.z
		),
		escala,
        vColDetalle,
        vcMensaje);

    // Recupero donde empieza en X, el salto de linea.
    setX(fXaux);
    //----------------------------------------------------------------------
    return 0;
}


/*========================================================================*\
|* Fin de cFuente.cpp
\*========================================================================*/

