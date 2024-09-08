//--------------------------------------------------------------------------
// cGestorFuentes.cpp
//--------------------------------------------------------------------------


#include "cGestorFuentes.h"
#include "cGestorColores.h"
#include "../Files/cLog.h"
#include "../Files/cFileXml.h"
#include "../Util/cError.h"
#include "../Mensajes/cInterfazSwat.h"
#include "../Windows/cGLWindow.h"


//--------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------
TItem vInfCharSet [] =
{
	{	DEFAULT_CHARSET		, "DEFAULT_CHARSET"		, NULL	},
	{	ANSI_CHARSET		, "ANSI_CHARSET"		, NULL	},
	{	JOHAB_CHARSET		, "JOHAB_CHARSET"		, NULL	},
	{	HEBREW_CHARSET		, "HEBREW_CHARSET"		, NULL	},
	{	ARABIC_CHARSET		, "ARABIC_CHARSET"		, NULL	},
	{	GREEK_CHARSET		, "GREEK_CHARSET"		, NULL	},
	{	TURKISH_CHARSET		, "TURKISH_CHARSET"		, NULL	},
	{	VIETNAMESE_CHARSET	, "VIETNAMESE_CHARSET"	, NULL	},
	{	THAI_CHARSET		, "THAI_CHARSET"		, NULL	},
	{	EASTEUROPE_CHARSET	, "EASTEUROPE_CHARSET"	, NULL	},
	{	RUSSIAN_CHARSET		, "RUSSIAN_CHARSET"		, NULL	},
	{	MAC_CHARSET			, "MAC_CHARSET"			, NULL	},
	{	BALTIC_CHARSET		, "BALTIC_CHARSET"		, NULL	},
	{	-1					, "CharSet Desconocido"	, NULL	},
};
//--------------------------------------------------------------------------
TItem vInfFontType [] =
{
	{	DEVICE_FONTTYPE		, "DEVICE_FONTTYPE"		, NULL	},
	{	RASTER_FONTTYPE		, "RASTER_FONTTYPE"		, NULL	},
	{	TRUETYPE_FONTTYPE	, "TRUETYPE_FONTTYPE"	, NULL	},
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Construction/Destruction
//--------------------------------------------------------------------------
cGestorFuentes::cGestorFuentes()
    : m_iNumFuentes(0)
    , m_bCargadas(false)
    , m_bGeneraFuentes(false)
    , m_iTraza(eTMIN)
{
	setNew(this,"cGestorFuentes");
	mInicio(m_vcDirFuentes);
}

cGestorFuentes::~cGestorFuentes()
{
    mSingletonNull;
}



//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Instancia: Singleton.
//--------------------------------------------------------------------------
mSingleton(cGestorFuentes);
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Creacion de la iLista
//--------------------------------------------------------------------------
iLista * cGestorFuentes::creaNewLista(int iIdent)
{
    cGestorFuentes * poNewLista = new cGestorFuentes();

    return poNewLista;
}


//--------------------------------------------------------------------------
// Inicio de la lista de fuentes
//
// Cabecera que nos hemos dado:
//
// [Fuentes: Indice: Tamaño Base: Espacio Base: Grupo: Tipo: Fichero: Ancho: Alto:
//
//--------------------------------------------------------------------------
int cGestorFuentes::init(cFileXml * p_poXml, char* pcDirFuentes, cInterfaz * poInterfaz)
{
    miNulo(p_poXml);

    cTagXML * poArbol = p_poXml->getArbol();
    miNulo(poArbol);

	cTagXML * poRama;
	int	iRes = 0;

	poRama = poArbol->getTag(m_sPatron.Cad());
	if	(
				poRama
                &&	poRama->getListaTags()->Count()>-1
		)
	{
		cTagXML	  *	poRegistro;
		cFuente	  *	poFuente;
        sFuente	tFuente;
        cString     sAux;
		char	  * pcCad;
		int			i, j, k, iIdent;

        cLog::traza(m_iTraza, "\n");
        cLog::traza(m_iTraza, " Inicia lista Fuentes\n");
        // El identificador ya se introduce por el init de la lista de listas
        // Quizas solo comprobar que el identificador es el mismo ...
        cAtributo * poIndex = poRama->getAtributo("id");
        if (poIndex)
        {
            iIdent = poIndex->getValor().getInt();
            if (getIdent() != iIdent)
            {
                // Señalar el posible error de que el identificador este mal
                cLog::traza(m_iTraza, "  - Identificador diferente: %d  [%d]\n", iIdent, getIdent());
            }
        }

        m_iNumFuentes = poRama->getListaTags()->Count();
		mCopia(m_vcDirFuentes,pcDirFuentes);

        //------------------------------------------------------------------
        cListaP<cTagXML>::cIterator it = poRama->getListaTags()->begin();
		for	( i=0, iRes=0; !it.esFin() && !iRes; it++, i++ )
		{
			poRegistro = **it;
			if	(poRegistro)
			{
                cAtributo * poAtributo = poRegistro->getAtributo ("id");
                // Para indicar si son fuentes redimensionables, con Full Desktop.
                cAtributo * poFull = poRegistro->getAtributo ("full");
				//----------------------------------------------------------
				// [Fuentes: Indice: Tamaño Base: Espacio Base: Grupo: Tipo: Fichero: Escale:
				//  < eDef    , 16, -3, 0, FT2, def.tga    , 0 >
				//----------------------------------------------------------
                iIdent = poAtributo->getValor().getInt();
                tFuente.idTextura = iIdent;
                tFuente.iAlto = poRegistro->getHoja(0).getInt();                // iAlto/Tamaño base
                tFuente.iAncho = poRegistro->getHoja(1).getInt();               // iAncho/Espacio base
                tFuente.iGrupo = poRegistro->getHoja(2).getInt();               // Grupo: 0 :1 
                tFuente.eTipoFon = (eTipoFuentes) poRegistro->getHoja(3).getInt();		        // Tipo Fuente
                tFuente.eTipoTex = (eTipoTextura) poRegistro->getHoja(4).getInt();		        // Tipo Textura
                pcCad = (char *) poRegistro->getHoja(5).getString();		    // Fichero textura
                tFuente.fEsc = (float)poRegistro->getHoja(6).getDouble();       // Escala, generalmente a 1
                tFuente.fOffset = (float)poRegistro->getHoja(7).getDouble();    // Offset corrector altura, generalmente a 0.
                tFuente.iPeso = poRegistro->getHoja(8).getInt();                // Peso, bold, y todo eso.
                tFuente.bSombra = poRegistro->getHoja(9).getBool();             // Sombra / Relieve
                tFuente.iIncSombra = 1;

                sprintf_s(tFuente.vcNombre, sizeof(tFuente.vcNombre), "%s", pcCad);
                sprintf_s(tFuente.vcFichero, sizeof(tFuente.vcFichero), "%s/%s", m_vcDirFuentes, pcCad);

                poFuente = getFuente(iIdent);
                if (poFuente)
                {
                    // Existe, posiblemente dado de alta en el Ini.
                    // Prevalecen los valores del Ini y no se modifica
                    // Pasamos a la siguiente
                    cLog::traza(m_iTraza, " - Fuente: %2d  %2d  [%s]  Ya cargada.\n", i, iIdent, tFuente.vcNombre);
                    continue;
                }
                //----------------------------------------------------------
				for	( j=0; j<128; j++ )
				//----------------------------------------------------------
				{
					for ( k=0; k<4; k++ )
						tFuente.fCoord[j][k] = 0.0f;
				}
				//----------------------------------------------------------
                cLog::traza(m_iTraza, " - Fuente: %2d  %2d  %s  [%s]\n",
                    i,
                    tFuente.idTextura,
                    poAtributo->getValorOrigen().toString(sAux),
                    tFuente.vcNombre);
				//----------------------------------------------------------
				poFuente = new cFuente();
				if	(poFuente)
				{
					iRes = poFuente->Inicia(&tFuente);
					if	(!iRes)
					{
                        // No lo hemos estado utilizando, pero las fuentes tambien son cItem
                        // Le damos el identificador de cItem
                        poFuente->setIdent(tFuente.idTextura);
                        poFuente->setFull ((poFull) ? poFull->getValor ().getBool () : false);

                        Insertar(poFuente);
					}
				}
            }
            mDo(poInterfaz)->EnviaInc();
		}
        //------------------------------------------------------------------
	}

	return iRes;
}


//--------------------------------------------------------------------------
// Carga individual de una unica fuente.
// Se carga con un identificador dado, si este ya existe con ese
// identificador no se carga, porque se da por cargada
// No se considera error si es asi.
//--------------------------------------------------------------------------
int cGestorFuentes::cargaFuente(
	int iIdent,				// Necesario para identificarlo en la lista del Gestor
	const char* pcNombre,
	int iAncho,
	int iAlto,
	int peso,
	bool bItalic,
	bool bUnderLine,
	bool bStrikeOut,
	bool sombra,
	// int iGrupo,
	// eTipoFuentes eTipoFon,	// Fijo eFN01
	// eTipoTextura eTipoTex,	// eTNoReg
	// float fEsc,				// Esc, generalmente a 1, el Angulo de escape de CreateFont ?
	// float fOffset,
	int iCharSet,
	const char* pcDirFuentes,
	bool full)
{
	int iRes = 0;
	cFuente* poFuente = getFuente(iIdent);
	if (poFuente)
	{
		cLog::traza(m_iTraza, " - Fuente: %2d  [%s]  Ya cargada.\n", iIdent, pcNombre);
		return iRes;
	}

	sFuente tFuente;
	// Valores por defecto dados por Reset.
	tFuente.reset();
	tFuente.idTextura = iIdent;
	tFuente.iAlto = iAlto;			// iAlto/Tamaño base
	tFuente.iAncho = iAncho;		// iAncho/Espacio base
	tFuente.iPeso = peso;			// Peso, bold, y todo eso.
	tFuente.bItalic = bItalic;
	tFuente.bUnderLine = bUnderLine;
	tFuente.bStrikeOut = bStrikeOut;
	tFuente.bSombra = sombra;       // Sombra / Relieve
	tFuente.iCharSet = iCharSet;	// Charset

	sprintf_s(tFuente.vcNombre, sizeof(tFuente.vcNombre), "%s", pcNombre);
	if (pcDirFuentes)
		sprintf_s(tFuente.vcFichero, sizeof(tFuente.vcFichero), "%s/%s", pcDirFuentes, pcNombre);
	else
		sprintf_s(tFuente.vcFichero, sizeof(tFuente.vcFichero), "%s", pcNombre);

	poFuente = new cFuente();
	if (poFuente)
	{
		iRes = poFuente->Inicia(&tFuente);
		if (!iRes)
		{
			poFuente->setIdent(tFuente.idTextura);
			// Para indicar si son fuentes redimensionables, con Full Desktop.
			poFuente->setFull(full);
			Insertar(poFuente);
		}
	}

	return iRes;
}


//--------------------------------------------------------------------------
//  Carga las fuentes y las Encola a la lista de texturas.
//--------------------------------------------------------------------------
int cGestorFuentes::genera(bool bFuerzaCarga, const char* pcOrigen)
{
	// 
	return generaCR(bFuerzaCarga, pcOrigen, true);
}

// CR Contexto Renderizado ?
int cGestorFuentes::generaCR(bool bFuerzaCarga, const char * pcOrigen, bool bSinContexto)
{
    const char * _metodo = __FUNCTION__;

    if	(!m_bCargadas || bFuerzaCarga)
	{
        cError oError;
        cInterfazSwat * poInterfaz = (cInterfazSwat *)(cInterfaz::Instancia());

        if (!poInterfaz)
        {
            return oError.mensaje("cGestorFuentes::carga: sin interfaz window");
        }

        cGLWindow * poWindow = poInterfaz->getWindow();
        if (!poWindow)
        {
            return oError.mensaje("cGestorFuentes::carga: sin Ventana en el interfaz window");
        }

		int iNumElementos;
		iNumElementos = Count();
		if (iNumElementos>0)
		{
            cNodo<cFuente *> * poNodo;
		    cFuente	* poFuente;
			char vcMensaje	[LON_BUFF];

            sprintf_s(vcMensaje, sizeof(vcMensaje), "Genera Fuentes  %s", ((pcOrigen) ? pcOrigen : ""));

            cLog::traza(m_iTraza, "\n");
            cLog::traza(m_iTraza, " %s\n", vcMensaje);

            // Pensada para que este la ventana de carga ...
			poInterfaz->Escribe(eMENS_CARGA1,vcMensaje);

            for (poNodo = Begin(); poNodo; poNodo = getNodoSiguiente())
			{
                poFuente = *poNodo->getDato();
                if	(poFuente)
				{
                    cLog::traza(m_iTraza, " - '%s'\n", poFuente->DarNombre());
                    if (!poFuente->estaActivada())
                    {
                        sprintf_s(vcMensaje, sizeof(vcMensaje), "%s", poFuente->DarNombre());
                        poInterfaz->Escribe(eMENS_CARGA2, vcMensaje);			// ¡¡¡ Fuera de la seccion crititca !!!

                        poWindow->entraSeccion();
						{
							poWindow->setRenderContext(bSinContexto);
							//poWindow->keepRenderContext();
							poFuente->Carga(poWindow);
							//poWindow->unKeepRenderContext();
							poWindow->unSetRenderContext(bSinContexto);
						}
                        poWindow->saleSeccion();
                        poInterfaz->EnviaInc();
                    }
                    else
                    {
                        cLog::traza(m_iTraza, " - '%s' ya generada.\n", poFuente->DarNombre());
                    }
                }
			}
		    m_bCargadas = true;
		}
	}

	return 0;
}


//--------------------------------------------------------------------------
// Da una fuente segun el index dado.
//--------------------------------------------------------------------------
cFuente * cGestorFuentes::getFuente ( int iIndex )
{
	cFuente	  *	poFuente = NULL;
	sFuente  *	ptFuente;
	bool		bFound;
	int			iNum;

	iNum = Count();
	if (iNum)
	{
        cIterator it = begin();
		for	( bFound=false; !it.esFin() && !bFound; it++ )
		{
			poFuente = **it;
            if (poFuente)
            {
                ptFuente = poFuente->DarRegFuente();
                if (ptFuente->idTextura == iIndex)
                {
                    bFound = true;
                }
            }
		}

        if (bFound)
        {
            return poFuente;
        }
	}

	return NULL;
}


//--------------------------------------------------------------------------
// Establece el directorio de fuentes.
//--------------------------------------------------------------------------
int	cGestorFuentes::SetDirFuentes	( char * p_pcDirFuentes )
{
	mCopia(m_vcDirFuentes,p_pcDirFuentes);

	return 0;
}


//--------------------------------------------------------------------------
// Construccion de fuente sin fichero de textura
//--------------------------------------------------------------------------
int	cGestorFuentes::ConsFuente	( cGLWindow * poWindow, int iIdTex, char * pcNombre, int iAncho, int iAlto, int iCharSet )
{
	sFuente tFuente;
	cFuente * poFuen;
    cError oErr;

	poFuen = new cFuente();
	memset((void *) &tFuente, 0, sizeof(tFuente));
	tFuente.idTextura = iIdTex;
	tFuente.eTipoFon = eTipoFuentes::eFN01;
	tFuente.fEsc = 1.0f;
	mCopia(tFuente.vcNombre, pcNombre);
	tFuente.iAlto = iAlto;
	tFuente.iAncho = iAncho;
	poFuen->Inicia(&tFuente);
	if (poFuen->Carga(poWindow, iCharSet))
	{
		oErr.mensaje("- No tenemos fuente [%s]", pcNombre);
		delete poFuen;
	}
    Insertar(poFuen);

	return 0;
}


//--------------------------------------------------------------------------
// Crea una fuente igual a la fuente identificada por iFuente
//--------------------------------------------------------------------------
cFuente* cGestorFuentes::creaFuente(int iFuente)
{
	cFuente* poNewFuente = NULL;
	if (iFuente > -1)
	{
		cFuente* poFuente = getFuente(iFuente);
		if (poFuente)
		{
			cFuente* poFound;
			sFuente tFuente;
			sFuente* ptFuente;
			int iNewIdent;

			iNewIdent = iFuente * 100;
			for (poFound = getFuente(iNewIdent); poFound; iNewIdent++)
			{
				poFound = getFuente(iNewIdent);
			}

			ptFuente = poFuente->DarRegFuente();
			tFuente.idTextura = iNewIdent;
			tFuente.iAlto = ptFuente->iAlto;            // iAlto/Tamaño base
			tFuente.iAncho = ptFuente->iAncho;          // iAncho/Espacio base
			tFuente.iGrupo = ptFuente->iGrupo;          // Grupo: 0 :1 
			tFuente.eTipoFon = ptFuente->eTipoFon;      // Tipo Funete ?
			tFuente.eTipoTex = ptFuente->eTipoTex;      // Tipo Textura
			tFuente.fEsc = ptFuente->fEsc;              // Escala, generalmente a 1
			tFuente.fOffset = ptFuente->fOffset;        // Offset corrector altura, generalmente a 0.
			tFuente.iPeso = ptFuente->iPeso;            // Peso, bold, y todo eso.
			tFuente.bSombra = ptFuente->bSombra;        // Sombra / Relieve
			tFuente.iIncSombra = 1;
			strcpy_s(tFuente.vcNombre, sizeof(tFuente.vcNombre), ptFuente->vcNombre);
			strcpy_s(tFuente.vcFichero, sizeof(tFuente.vcFichero), ptFuente->vcFichero);
			//----------------------------------------------------------
			int j, k;
			for (j = 0; j < 128; j++)
			{
				for (k = 0; k < 4; k++)
					tFuente.fCoord[j][k] = 0.0f;
			}
			//----------------------------------------------------------
			poNewFuente = new cFuente();
			if (poNewFuente)
			{
				poNewFuente->setIdent(tFuente.idTextura);
				poNewFuente->setFull(poFuente->getFull());
				poNewFuente->Inicia(&tFuente);
				Insertar(poNewFuente);
			}
		}
	}
	return poNewFuente;
}


//--------------------------------------------------------------------------
// Enumeramos las fuentes
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// EnumFontFamExProc
//--------------------------------------------------------------------------
int CALLBACK EnumFontFamExProc(
							    ENUMLOGFONTEX *lpelfe,    // logical-font data
							    NEWTEXTMETRICEX *lpntme,  // physical-font data
							    DWORD FontType,           // type of font
							    LPARAM lParam             // application-defined data
							  )
{
	cGestorFuentes * poGFuentes = (cGestorFuentes *) lParam;
	cString	oFound;

	poGFuentes->BuscaTipoFuente(FontType,oFound);
	cLog::print(" + -- Font: %2d [%s]   [%s]  [%s]\n",
		cGestorFuentes::iFuentes+1,
		lpelfe->elfFullName,
		lpelfe->elfStyle,
		lpelfe->elfScript);
	cLog::print("     Tipo Fuente: [%s]\n", oFound.Cad());
	cLog::print("     .   CharSet: %d [%s]\n", lpelfe->elfLogFont.lfCharSet, poGFuentes->BuscaCharSet(lpelfe->elfLogFont.lfCharSet));
	cLog::print("     .     Ancho: %d\n", lpelfe->elfLogFont.lfWidth);
	cLog::print("     .      Alto: %d\n", lpelfe->elfLogFont.lfHeight);
	cLog::print("     .      Peso: %d\n", lpelfe->elfLogFont.lfWeight);
	cLog::print("     .    Italic: %s  %d\n", msTrue(lpelfe->elfLogFont.lfItalic), lpelfe->elfLogFont.lfItalic);
	cLog::print("     . UnderLine: %s  %d\n", msTrue(lpelfe->elfLogFont.lfUnderline), lpelfe->elfLogFont.lfUnderline);
	cLog::print("     . StrikeOut: %s  %d\n", msTrue(lpelfe->elfLogFont.lfStrikeOut), lpelfe->elfLogFont.lfStrikeOut);
	cLog::print("     .    Escape: %d\n", lpelfe->elfLogFont.lfEscapement);

	cLog::print("     ==Datos Fisicos== ...\n");
	cLog::print("     Alto (Asc+Des): %d\n", lpntme->ntmTm.tmHeight);
	cLog::print("          -  Ascent: %d\n", lpntme->ntmTm.tmAscent);
	cLog::print("          - Descent: %d\n", lpntme->ntmTm.tmDescent);
	cLog::print("        Ancho (Avg): %d\n", lpntme->ntmTm.ntmAvgWidth);
	cLog::print("        Ancho (Max): %d\n", lpntme->ntmTm.tmMaxCharWidth);
	cLog::print("        Peso       : %d\n", lpntme->ntmTm.tmWeight);
	cLog::print("         First Char: %d %c\n", lpntme->ntmTm.tmFirstChar, lpntme->ntmTm.tmFirstChar);
	cLog::print("          Last Char: %d %c\n", lpntme->ntmTm.tmLastChar, lpntme->ntmTm.tmLastChar);
	//cLog::print("     Break Char: %d\n", lpntme->ntmTm->tmBreakChar);
	//cLog::print(" \n", lpntme->ntmTm->);
	//cLog::print(" \n", lpntme->ntmTm->);
	cLog::print("\n");

	cGestorFuentes* gestor = cGestorFuentes::Instancia();

	gestor->cargaFuente(
		cGestorFuentes::iFuentes,
		(const char *) lpelfe->elfFullName,
		lpelfe->elfLogFont.lfWidth,
		lpelfe->elfLogFont.lfHeight,
		lpelfe->elfLogFont.lfWeight,
		lpelfe->elfLogFont.lfItalic,
		lpelfe->elfLogFont.lfUnderline,
		lpelfe->elfLogFont.lfStrikeOut,
		false,
		lpelfe->elfLogFont.lfCharSet);

	cGestorFuentes::iFuentes++;

	return 1;
}


cstatic int cGestorFuentes::iFuentes = 0;
cstatic void cGestorFuentes::CargaTest(cGLWindow* poWindow, int iNivelTraza, const char* pcFontName)
{
	if (cLog::Instancia()->getNivelTraza() <= iNivelTraza)
	{
		iFuentes = 0;
		EnumeraFuentes(poWindow, eTMIN, pcFontName, DEFAULT_CHARSET);
		cLog::print(" Hemos encontrado %d fuentes\n", iFuentes);
		cLog::print("\n");
		iFuentes = 0;
	}
}

cstatic int cGestorFuentes::iInicio = 0;
const int KNUM_FUENTES_TEST = 8;
cstatic void cGestorFuentes::arriba()
{
	if (iInicio - KNUM_FUENTES_TEST > -1)
		iInicio -= KNUM_FUENTES_TEST;
}

cstatic void cGestorFuentes::abajo()
{
	if (iInicio + KNUM_FUENTES_TEST < m_poInstancia->Count())
		iInicio += KNUM_FUENTES_TEST;
}

cstatic void cGestorFuentes::PruebaTest(cGLWindow* poWindow, int iFuenteTest)
{
	int iLineas = 0;

	cString frase = "";
	int x = 20;
	int y = 30;
	int iNum = m_poInstancia->Count();

	cFuente* poFuenteDef = *cGestorFuentes::m_poInstancia->getDato(iFuenteTest);
	poFuenteDef->escribe(poWindow, x, y, 0, 1.0f, colROJO, "Hay %d fuentes", iNum);
	x = poFuenteDef->getX();
	y = poFuenteDef->getY() + 30;

	for(int iIndex=iInicio; iIndex<iNum && iLineas < KNUM_FUENTES_TEST; iIndex++, iLineas++)
	{
		if (iInicio != 0)
		{
			int mibreak = 0;
		}
		// Por diseño, al cargar y crear los fuentes, el identificador es el Indice
		cFuente* poFuente = *cGestorFuentes::m_poInstancia->getDato(iIndex);
		if (poFuente)
		{
			frase.format(":%3d:%s: Con eñe ¡valores! y ¿Funciona?", iIndex, poFuente->getNombre());
			poFuente->setX(x);
			poFuente->setY(y);
			poFuente->escribe(poWindow, -1, -1, 0, 1.0f, colAMARILLO, *frase);
			x = poFuente->getX();
			y = poFuente->getY() + 30;
		}
	}
	poFuenteDef->escribe(poWindow, -1, RISK_HEIGHT - 20, 0, 1.0f, colROJO, "Pulsa Arr.  Abj.  para cambiar.     Esc para terminar ... ");
}

int	cGestorFuentes::EnumeraFuentes	( cGLWindow * poWindow, int iNivelTraza, const char * pcFaceName, int iCharset )
{
    if (cLog::Instancia()->getNivelTraza() <= iNivelTraza)
	{
        HDC hDc = poWindow->getDeviceContext();
		LOGFONT tLogFont;
        
		memset( (void *) &tLogFont, 0, sizeof(tLogFont));
		tLogFont.lfCharSet = iCharset;
		mInicio(tLogFont.lfFaceName);
		if (pcFaceName)
		{
			mCopia(tLogFont.lfFaceName,pcFaceName);
		}
		tLogFont.lfPitchAndFamily = 0;
		cLog::print("\n");
		cLog::print(" Fuentes: [%s]\n", (pcFaceName)? pcFaceName: "Todas");
		EnumFontFamiliesEx(hDc, &tLogFont, (FONTENUMPROC) EnumFontFamExProc, (LPARAM) m_poInstancia, 0);
		cLog::print("\n");
	}

	return 0;
}


//--------------------------------------------------------------------------
// Busca la cadena que identifica el CharSet
//--------------------------------------------------------------------------
cstatic char * cGestorFuentes::BuscaCharSet		( int iCharSet )
{
	int i;
	bool bFound;
	int iLong;
	char * pcFound = NULL;

	iLong = sizeof(vInfCharSet)/sizeof(TItem);
	for ( bFound=false, i=0; i<iLong && !bFound; i++ )
	{
		pcFound = vInfCharSet[i].vcNombre;
		if (vInfCharSet[i].iIndex == iCharSet)
		{
			bFound = true;
		}
	}

	return pcFound;
}


//--------------------------------------------------------------------------
// Busca la cadena que identifica el Tipo de fuente, devuelve la combinacion
// final del tipo dado.
//--------------------------------------------------------------------------
cstatic bool cGestorFuentes::BuscaTipoFuente ( int iTipoFuente, cString & oFound )
{
	int i;
	bool bFound;
	int iLong;

	iLong = sizeof(vInfFontType)/sizeof(TItem);
	for ( bFound=false, oFound = "", i=0; i<iLong; i++ )
	{
		if (vInfFontType[i].iIndex & iTipoFuente)
		{
			if (bFound)
			{
				oFound += "";
			}
			oFound += vInfFontType[i].vcNombre;
			bFound = true;
		}
	}
	return bFound;
}


//--------------------------------------------------------------------------
// Fin de cGestorFuentes.cpp
//--------------------------------------------------------------------------
