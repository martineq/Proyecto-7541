#ifndef TDA_TEJECUTOR_T_F_H_
#define TDA_TEJECUTOR_T_F_H_
#include "Lista.h"
#include "Cola.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <memory.h>

typedef TCola TTubo;
typedef int TProcesarFn(void* datos_filtro,TTubo* entrada,TTubo* salida,int eof);

typedef struct{
	TProcesarFn* fnProceso;
	void* datos_filtro;
}TFilter;

typedef struct {
	TListaSimple* pListaFiltros;
	FILE* ArchivoEntrada;
	FILE* ArchivoSalida;
}TEjecutor_T_F;

/*--------------------------Prototipos------------------------*/

/*Primitivas*/

/*Crea el ejecutor, inicializa la estructura.
 * Pre:ejecutor no creado,archivo_entrada abierto para lectura,archivo_salida
 * abierto para escritura,filtros es una lista creada no vacia de
 * TFilter.
 * Pos: ejecutor creado
 * */
void PR_Crear(TEjecutor_T_F* ejecutor,FILE* archivo_entrada,TListaSimple filtros,FILE* archivo_salida);

/*Ejecuta la cadena de tubos y filtros.
 * Pre:ejecutor creado.
 * Post:se generó la salida y no puede volver a ejecutarse
 * */
void PR_Ejecutar(TEjecutor_T_F* ejecutor);

/*Destruye el ejecutor.
 * Pre:ejecutor creado.
 * Post:ejecutor destruido.
 * */
void PR_Destruir(TEjecutor_T_F* ejecutor);

#endif /*TDA_TEJECUTOR_T_F_H_*/
