#ifndef TDA__D_T_H_
#define TDA__D_T_H_

#define TELEFONO 11
#define FECHA_HORA 17
#define MAXLINEA 50
#define MAXWORD 30   
#define MAXLONGPREFIJO 5  /* la cantidad máxima de numeros que tiene un prefijo (incluido el \n) */
#define MAXCANTPREFIJO 3  /* la cantidad máxima de prefijos que puede haber en una archivo de nºs telefonicos*/
#define MINLONGNUMERO 7   /* la cantidad mínima de numeros que tiene un Nº telefonico (incluido el \n) */
#define MAXLINDEUDA 100
#define MAXPALDEUDA 70


#include "Lista.h"
#include "Cola.h"
#include "ABO.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <memory.h>

typedef struct{
	char numero[TELEFONO];
	char fecha[FECHA_HORA];
	float duracion;
	float importe;
}TLlamada;

typedef struct {
	char* usuario;
	TListaSimple* numeros_usr;
}TUsuario_numeros;

typedef struct{
	TABO* Arbol_llamadas;
	TABO* Arbol_usuarios_numeros;
}TDA_D_T;

typedef struct{
	char Nombre_Usuario[MAXWORD];
	char Ruta_Archivo[MAXWORD];
}tRutaArchivoUsuario;

typedef struct{
	char concepto[MAXPALDEUDA];
	float importe;
}TGastosG;/*estructura que devuelve la primitiva TDT_ObtenerDeudaGastosGenerales*/

typedef struct{
	char* usuario;
	float importe;
}TResumen;

typedef struct{
	char* usuario;
	TListaSimple* detalle;
}TListaDetalles;


/**** PRIMITIVAS ****/

int TDT_Crear(char* ruta, TDA_D_T* Arboles, char* RutaLlamadas);
int TDT_Destruir(TDA_D_T* Arboles);
void TDT_ObtenerDetalleUsuario (char* usuario, TDA_D_T* Arboles, TListaSimple* Detalle);
void TDT_GenerarResumenUsuarios (TDA_D_T* Arboles, TListaSimple* Resumen);
void TDT_ObtenerDeudaGastosGenerales(FILE* ArchivoDetTel,TListaSimple* GGrales);
void TDT_ObtenerUsuariosTelefono(TDA_D_T* TDA_Llamadas,char* Tel,TListaSimple* LUsrTel);
int TDT_ObtenerDetalleNoAsignada(TDA_D_T* p,TListaSimple* noasig,int ord);
int TDT_ObtenerDetalleDeTelefonosPorAreaFisica(TDA_D_T* p,char* area,TListaSimple* tel,int ord);

#endif /*TDA__D_T_H_*/
