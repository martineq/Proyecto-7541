#include "Lista.h"
#include <malloc.h>
#include <memory.h>

/* ls_Crear
Pre: Ls no fue creada.
Post: Ls creada y vacía */
void ls_Crear(TListaSimple *pLs, int TamanioDato){
  pLs->Corriente = NULL;
  pLs->Primero = NULL;
  pLs->TamanioDato = TamanioDato;
}

/* ls_Vaciar
Pre: Ls creada.
Post: Ls vacía.*/
void ls_Vaciar(TListaSimple *pLs){
  TNodoSimple *pNodo, *siguiente;
  for(pNodo = pLs->Primero;(pNodo);pNodo=siguiente){
    siguiente = pNodo->Siguiente;
    free(pNodo->Elem);
    free(pNodo);
  }
  pLs->Primero=pLs->Corriente=NULL;
}

/* ls_Vacia
Pre: Ls creada.
Post: Si Ls tiene elementos devuelve FALSE sino TRUE.*/
int ls_Vacia(TListaSimple Ls){
  return (Ls.Primero == NULL);
}

/* ls_ElemCorriente
Pre: Ls creada y no vacía.
Post: Se devuelve en E el elemento corriente de la lista.*/
void ls_ElemCorriente(TListaSimple Ls, void *pE){
  memcpy(pE, Ls.Corriente->Elem, Ls.TamanioDato);
}

/* ls_ModifCorriente
Pre: Ls creada y no vacía.
Post: El contenido del elemento actual quedo actualizado con E. */
void ls_ModifCorriente(TListaSimple *pLs, void* pE){
  memcpy(pLs->Corriente->Elem, pE, pLs->TamanioDato);
}

/* ls_MoverCorriente
Pre: Ls creada y no vacía.
Post: Si Ls está vacía, devuelve FALSE. Sino:
Si M = LS_PRIMERO, el nuevo elemento corriente es el primero. Devuelve TRUE
Si M = LS_SIGUIENTE, el nuevo elemento corriente es el siguiente al
anterior. Si estaba en el último elemento, devuelve FALSE, sino TRUE.
Si M = LS_ANTERIOR, devuelve FALSE. */
int ls_MoverCorriente(TListaSimple *pLs, TMovimiento_Ls M){
  switch (M){
    case LS_PRIMERO: pLs->Corriente=pLs->Primero;
    break;
    case LS_SIGUIENTE: if (pLs->Corriente->Siguiente==NULL)
    return FALSE;
    else
    pLs->Corriente=pLs->Corriente->Siguiente;
    break;
    case LS_ANTERIOR:return FALSE;
  }
  return TRUE;
}

/* ls_BorrarCorriente
Pre: Ls creada y no vacía.
Post: Se eliminó el elemento corriente, El nuevo elemento es el siguiente o
el anterior si el corriente era el último elemento.*/
void ls_BorrarCorriente(TListaSimple *pLs){
  TNodoSimple *PNodo=pLs->Corriente;
  if (pLs->Corriente==pLs->Primero){
    pLs->Primero = pLs->Corriente->Siguiente;
    pLs->Corriente = pLs->Primero;
  }
  else{
    TNodoSimple *PAux=pLs->Primero;
    while (PAux->Siguiente!=pLs->Corriente)
      PAux = PAux->Siguiente;
    PAux->Siguiente=pLs->Corriente->Siguiente;
    if (PAux->Siguiente) /* Si no es el último */
      pLs->Corriente = PAux->Siguiente;
    else
      pLs->Corriente = PAux; /* Si es el último queda en el anterior al borrado */
   }
  free(PNodo->Elem);
  free(PNodo);
}


/* ls_Insertar
Pre: Ls creada.
Post: E se agregó a la lista y es el actual elemento corriente.
Si M=LS_PRIMERO: se insertó como primero de la lista.
Si M=LS_SIGUIENTE: se insertó después del elemento corriente.
Si M=LS_ANTERIOR: se insertó antes del elemento corriente.
Si pudo insertar el elemento devuelve TRUE, sino FALSE.*/
int ls_Insertar(TListaSimple *pLs, TMovimiento_Ls M, void* pE){
  TNodoSimple *pNodo = (TNodoSimple*) malloc(sizeof(TNodoSimple));
  if (!pNodo)
    return FALSE; /* No hay memoria disponible */
  if ((pLs->Primero == NULL) || (M==LS_PRIMERO) || ((M==LS_ANTERIOR) && (pLs->Primero==pLs->Corriente))) { /* Si está vacía o hay que insertar en el primero o hay que insertar en el anterior y el actual es el primero */
    pNodo->Siguiente = pLs->Primero;
    pLs->Primero = pLs->Corriente = pNodo;
  }
  else{
    if (M == LS_SIGUIENTE){
      pNodo->Siguiente = pLs->Corriente->Siguiente;
      pLs->Corriente->Siguiente = pNodo;
    }
    else { /* LS_ANTERIOR */
      TNodoSimple *pAux=pLs->Primero;
      while (pAux->Siguiente!=pLs->Corriente)
        pAux = pAux->Siguiente;
      pAux->Siguiente = pNodo;
      pNodo->Siguiente = pLs->Corriente;
    }
  }
  pNodo->Elem = malloc (pLs->TamanioDato);
  memcpy(pNodo->Elem, pE, pLs->TamanioDato);
  pLs->Corriente=pNodo;
  return TRUE;
}
