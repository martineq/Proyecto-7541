#include "Cola.h"
#include <malloc.h>
#include <memory.h>
/*C_Crear
Pre: C no fue creada.
Post: C creada y vacía. */
void C_Crear(TCola *pC, int TamanioDato)
{
	pC->Primero = pC->Ultimo = NULL;
	pC->TamanioDato = TamanioDato;
}
/*C_Vaciar
Pre: C creada.
Post: C vacía. */
void C_Vaciar(TCola *pC)
{
	TNodoCola *pAux = pC->Primero;
	TNodoCola *pSig;
	while (pAux)
	{
		pSig = pAux->Siguiente;
		free(pAux->Elem);
		free(pAux);
		pAux = pSig;
	}
	pC->Primero = pC->Ultimo = NULL;
}
/*C_Vacia
Pre: C creada.
Post: Si C está vacía devuelve TRUE, sino FALSE. */
int C_Vacia(TCola C)
{
	return (C.Primero==NULL);
}
/*C_Agregar
Pre: C creada.
Post: E se agregó como último elemento de C.
Devuelve FALSE si no pudo agregar E, sino devuelve TRUE.*/
int C_Agregar(TCola *pC, void* pE)
{
	TNodoCola *pNodo = (TNodoCola*) malloc(sizeof(TNodoCola));
	if (!pNodo)
		return FALSE;
	else
	{
		if (pC->Ultimo)
			pC->Ultimo->Siguiente = pNodo;
		if (!pC->Primero) /*Está vacía*/
			pC->Primero = pNodo;
		pNodo->Siguiente = NULL;
		pC->Ultimo = pNodo;
		pNodo->Elem = malloc (pC->TamanioDato);
		memcpy(pNodo->Elem, pE, pC->TamanioDato);
		return TRUE;
	}
}
/*C_Sacar
Pre: C creada y no vacia.
Post: Se extrajo de C el primer elemento y se devuelve en E.
Si no pudo extraer el elemento (porque la cola estaba vacía) devuelve FALSE,
sino devuelve TRUE.*/
int C_Sacar(TCola *pC, void* pE)
{
	TNodoCola *pAux = pC->Primero;
	pC->Primero = pC->Primero->Siguiente;
	if (!pC->Primero)
		pC->Ultimo = NULL;
	memcpy(pE, pAux->Elem, pC->TamanioDato);
	free(pAux->Elem);
	free(pAux);
	return TRUE;
}
