#ifndef __ListaSimple_h__
#define __ListaSimple_h__
#if !defined(NULL)
#define NULL 0
#endif
#if !defined(FALSE)
#define FALSE 0
#endif
#if !defined(TRUE)
#define TRUE 1
#endif

typedef enum{
  LS_PRIMERO,
  LS_SIGUIENTE,
  LS_ANTERIOR
} TMovimiento_Ls;

typedef struct TNodoSimple{
  void* Elem;
  struct TNodoSimple *Siguiente;
} TNodoSimple;

typedef struct{
  TNodoSimple *Primero, *Corriente;
  int TamanioDato;
} TListaSimple;

/* ls_Crear
Pre: Ls no fue creada.
Post: Ls creada y vacía */
void ls_Crear(TListaSimple *pLs, int TamanioDato);

/* ls_Vaciar
Pre: Ls creada.
Post: Ls vacía.*/
void ls_Vaciar(TListaSimple *pLs);

/* ls_Vacia
Pre: Ls creada.
Post: Si Ls tiene elementos devuelve FALSE sino TRUE.*/
int ls_Vacia(TListaSimple Ls);

/* ls_ElemCorriente
Pre: Ls creada y no vacía.
Post: Se devuelve en E el elemento corriente de la lista.*/
void ls_ElemCorriente(TListaSimple Ls, void* pE);

/* ls_ModifCorriente
Pre: Ls creada y no vacía.
Post: El contenido del elemento actual quedo actualizado con E. */
void ls_ModifCorriente(TListaSimple *pLs, void* pE);

/* ls_MoverCorriente
Pre: Ls creada y no vacía.
Post: Si Ls está vacía, devuelve FALSE. Sino:
Si M = LS_PRIMERO, el nuevo elemento corriente es el primero. Devuelve TRUE
Si M = LS_SIGUIENTE, el nuevo elemento corriente es el siguiente al
anterior. Si estaba en el último elemento, devuelve FALSE, sino TRUE.
Si M = LS_ANTERIOR, devuelve FALSE. */
int ls_MoverCorriente(TListaSimple *pLs, TMovimiento_Ls M);

/* ls_BorrarCorriente
Pre: Ls creada y no vacía.
Post: Se eliminó el elemento corriente, El nuevo elemento es el siguiente o
el anterior si el corriente era el último elemento.*/
void ls_BorrarCorriente(TListaSimple *pLs);

/* ls_Insertar
Pre: Ls creada.
Post: E se agregó a la lista y es el actual elemento corriente.
Si M=LS_PRIMERO: se insertó como primero de la lista.
Si M=LS_SIGUIENTE: se insertó después del elemento corriente.
Si M=LS_ANTERIOR: se insertó antes del elemento corriente.
Si pudo insertar el elemento devuelve TRUE, sino FALSE.*/
int ls_Insertar(TListaSimple *pLs, TMovimiento_Ls M, void* E);

#endif

