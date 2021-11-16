#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <memory.h>
#include "Lista.h"
#include "Cola.h"
#include "TDA_TEJECUTOR_T_F.h"

#define MAXLINEA 256/*Constante de maxima longitud de linea*/
#define MAXWORD 65
#define MAX 70


/*--------------------------Prototipos------------------------*/
/*Funciones Internas al TDA*/
void Parsear_Arch_Entrada(TEjecutor_T_F* ejecutor,TTubo* ColaEntrada);
void Guardar_Elementos_Archivo_Salida(TEjecutor_T_F*ejecutor,TTubo* Salida);
void Crear_Lista_Tubos(TEjecutor_T_F* ejecutor,TListaSimple* LTubos);
void Liberar_Lista_Tubos(TListaSimple *LTubos);


/*Funcion que libera los recursos utilizados en la lista de tubos
 * Pre: Lista creada y no vacia.
 * Post: memoria liberada y lista vacia
 * Guido Nahuel Ygounet*/
void Liberar_Lista_Tubos(TListaSimple *LTubos){
	TTubo *pTubo;
	int error;
	//pTubo=(TTubo*)malloc(sizeof(TTubo));
	ls_MoverCorriente(LTubos,LS_PRIMERO);
	//ls_ElemCorriente(*LTubos,pTubo); se reemplaza por la de abajo
	ls_ElemCorriente(*LTubos,&pTubo);
	free(pTubo);
	while((error=ls_MoverCorriente(LTubos,LS_SIGUIENTE))){
		//pTubo=(TTubo*)malloc(sizeof(TTubo));
		//ls_ElemCorriente(*LTubos,pTubo); se reemplaza por la de abajo
		ls_ElemCorriente(*LTubos,&pTubo);
		free(pTubo);
	}
	ls_Vaciar(LTubos);
}

/*Ejecuta la cadena de tubos y filtros.
 * Pre:ejecutor creado.
 * Post:se generó la salida y no puede volver a ejecutarse
 * Guido Nahuel Ygounet*/
void PR_Ejecutar(TEjecutor_T_F* ejecutor){
	TListaSimple ListaTubos;
	TTubo *ColaEntrada;
	TTubo *ColaSalida;
	TFilter filtro;
	int eof=0;/*Lo inicializo en FALSE por defecto*/
	int error;
	ColaEntrada=(TTubo*)malloc(sizeof(TTubo));
	ColaSalida=(TTubo*)malloc(sizeof(TTubo));

    ls_Crear(&ListaTubos,sizeof(TTubo*));
    Crear_Lista_Tubos(ejecutor,&ListaTubos);
	Parsear_Arch_Entrada(ejecutor,ColaEntrada);
	ls_Insertar(&ListaTubos,LS_PRIMERO,&ColaEntrada);

	while(!eof){
		eof=1;/*el primer filtro recibe eof==TRUE*/
		error=ls_MoverCorriente(ejecutor->pListaFiltros,LS_PRIMERO);
		error=ls_MoverCorriente(&ListaTubos,LS_PRIMERO);
		ls_ElemCorriente(ListaTubos,&ColaEntrada);
        ls_MoverCorriente(&ListaTubos,LS_SIGUIENTE);
        do{
            ls_ElemCorriente(*ejecutor->pListaFiltros,&filtro);
            ls_ElemCorriente(ListaTubos,&ColaSalida);
            eof=filtro.fnProceso(filtro.datos_filtro,ColaEntrada,ColaSalida,eof);
			error=ls_MoverCorriente(&ListaTubos,LS_SIGUIENTE);
			error=ls_MoverCorriente(ejecutor->pListaFiltros,LS_SIGUIENTE);
			ColaEntrada=ColaSalida;
        }while(error);
	Guardar_Elementos_Archivo_Salida(ejecutor,ColaSalida);
	}
	Liberar_Lista_Tubos(&ListaTubos);
	//free(ColaEntrada);
	//free(ColaSalida);
}

/*Crea el ejecutor, inicializa la estructura.
 * Pre:ejecutor no creado,archivo_entrada abierto para lectura,archivo_salida
 * abierto para escritura,filtros es una lista creada no vacia de
 * TFilter.
 * Pos: ejecutor creado
 * Guido Nahuel Ygounet*/
void PR_Crear(TEjecutor_T_F* ejecutor,FILE* archivo_entrada,TListaSimple filtros,FILE* archivo_salida){
	ejecutor->pListaFiltros=(TListaSimple*)malloc(sizeof(TListaSimple));
	memcpy(ejecutor->pListaFiltros,&filtros,sizeof(TListaSimple));
	ejecutor->ArchivoEntrada=archivo_entrada;
	ejecutor->ArchivoSalida=archivo_salida;
}

/*Destruye el ejecutor.
 * Pre:ejecutor creado.
 * Post:ejecutor destruido.
 * Guido Nahuel Ygounet.*/
void PR_Destruir(TEjecutor_T_F* ejecutor){
	ls_Vaciar(ejecutor->pListaFiltros);
}

/*Crea una lista de tubos, para ello crea una lista de colas
 * de strings vacias. Crea tantas colas como sean necesarias
 * para ejecutar la cantidad de filtros del programa
 * Pre:TEjecutor_T_F esta creado, y el campo pListaFiltros contiene
 * una lista ya creada de filtros.
 * Post: devuelve la lista de tubos creada con punteros a colas.
 * Guido Nahuel Ygounet*/
void Crear_Lista_Tubos(TEjecutor_T_F* ejecutor,TListaSimple* LTubos){
	TCola* pCola;
	TCola** ppCola;
	int error;

	pCola=(TCola*)malloc(sizeof(TCola));
	//ppCola=(TCola**)malloc(sizeof(TCola*));

	C_Crear(pCola,MAXWORD*sizeof(char));
	ppCola=&pCola;
	ls_MoverCorriente(ejecutor->pListaFiltros,LS_PRIMERO);
    ls_Insertar(LTubos,LS_PRIMERO,ppCola);

	while((error=ls_MoverCorriente(ejecutor->pListaFiltros,LS_SIGUIENTE))){
		pCola=(TCola*)malloc(sizeof(TCola));
		ppCola=(TCola**)malloc(sizeof(TCola*));
		C_Crear(pCola,MAXWORD*sizeof(char));
		ppCola=&pCola;
		ls_Insertar(LTubos,LS_SIGUIENTE,ppCola);
	}
	//free(pCola);
	//free(ppCola);
}

/*Guarda la salida del ultimo tubo en el archivo de salida.
 * Pre:TEjecutor_T_F creado, el campo ArchivoSalida es un
 * archivo abierto para escritura.
 * Post:palabras guardadas en el ArchivoSalida.
 * Post:ColaSalida vacia
 * Guido Nahuel Ygounet*/
void Guardar_Elementos_Archivo_Salida(TEjecutor_T_F*ejecutor,TTubo* Salida){
	char elemento[MAXWORD];

	while(!C_Vacia(*Salida)){
		C_Sacar(Salida,elemento);
		fprintf(ejecutor->ArchivoSalida,"%s\n",elemento);
	}
}

/*-------------------------------------------------------------*/

/*Leandro Bueno*/
void Parsear_Arch_Entrada(TEjecutor_T_F* ejecutor,TTubo* ColaEntrada){
	char palactu[MAXWORD];/*guarda los caracteres de la palabra que se va formando*/
	int  ret;/*subindice de la linea actual*/

	C_Crear(ColaEntrada,MAXWORD*sizeof(char));

	ret = fscanf(ejecutor->ArchivoEntrada, "%[A-Za-z0-9]%*[^A-Za-z0-9]", palactu);
	while(ret != EOF) {
		C_Agregar(ColaEntrada,palactu);
		ret = fscanf(ejecutor->ArchivoEntrada, "%[A-Za-z0-9]%*[^A-Za-z0-9]", palactu);
	}
}
