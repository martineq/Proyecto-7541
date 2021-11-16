#ifndef ABO_H_
#define ABO_H_

#include <stdlib.h>
#include <memory.h>

#define IZQ 1
#define DER 2
#define PAD 3
#define RAIZ 4


typedef struct TNodoABO{
	void* elem;
	struct TNodoABO *izq, *der;
} TNodoABO;

typedef struct TABO{
	TNodoABO *raiz,*cte;
	int tamdato;
	int (*fcomp)(void *,void *); /* Puntero a la función de comparación */
} TABO;


void ABO_Crear(TABO *a,int tdato, int (*comp)(void *,void *));

void ABO_ElemCte(TABO a,void *elem);

void ABO_ModifCte(TABO *a,void *elem);

void ABO_MoverCte(TABO *a,int mov,int *error);

void ABO_Vaciar(TABO *a);

int ABO_Vacio(TABO a);

void ABO_Insertar(TABO *a,void *elem,int *error);

#endif /*ABO_H_*/
