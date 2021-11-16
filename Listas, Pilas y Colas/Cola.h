#ifndef __COLA_H__
#define __COLA_H__
#if !defined(NULL)
	#define NULL 0
#endif
#if !defined(FALSE)
	#define FALSE 0
#endif
#if !defined(TRUE)
	#define TRUE 1
#endif
typedef struct TNodoCola
{
	void* Elem;
	struct TNodoCola *Siguiente;
	} TNodoCola;
typedef struct
{
	TNodoCola *Primero, *Ultimo;
	int TamanioDato;
	} TCola;
/*C_Crear
Pre: C no fue creada.
Post: C creada y vacía. */
void C_Crear(TCola *pC, int TamanioDato);

/*C_Vaciar
Pre: C creada.
Post: C vacía. */
void C_Vaciar(TCola *pC);

/*C_Vacia
Pre: C creada.
Post: Si C está vacía devuelve TRUE, sino FALSE. */
int C_Vacia(TCola C);

/*C_Agregar
Pre: C creada.
Post: E se agregó como último elemento de C.
Devuelve FALSE si no pudo agregar E, sino devuelve TRUE.*/
int C_Agregar(TCola *pC, void* pE);

/*C_Sacar
Pre: C creada y no vacia.
Post: Se extrajo de C el primer elemento y se devuelve en E.
Si no pudo extraer el elemento (porque la cola estaba vacía) devuelve FALSE,
sino devuelve TRUE.*/
int C_Sacar(TCola *pC, void* pE);

#endif
