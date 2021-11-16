#include "ABO.h"


void ABO_Crear(TABO *a,int tdato, int (*comp)(void *,void *)){
	a->tamdato = tdato;
	a->raiz = NULL;
	a->cte = NULL;
	a->fcomp = comp;
}


int buscar_lugar(TABO *a, TNodoABO **p, void *elem){
	int c;
	if (*p != NULL){
		c = (*(a->fcomp))((*p)->elem,elem);
		if (c == 1)
			buscar_lugar(a,&((*p)->izq),elem);
		else if (c == -1)
			buscar_lugar(a,&((*p)->der),elem);
		else
			return 1;
	}
	else{
		*p = (TNodoABO*) malloc(sizeof(TNodoABO));
		(*p)->izq = NULL;
		(*p)->der = NULL;
		if (*p){
			(*p)->elem = malloc(a->tamdato);
			if ((*p)->elem){
				memcpy((*p)->elem,elem,a->tamdato);
				return 0;
			}
			free(*p);
			return 1;
		}
		return 1;
	}
	return 1;
}


void ABO_Insertar(TABO *a, void *elem,int *error){
	*error = buscar_lugar(a,&(a->raiz),elem);
}


void ABO_ElemCte(TABO a,void *elem){
	memcpy(elem,a.cte->elem,a.tamdato);
}


void ABO_ModifCte(TABO *a,void *elem){
	memcpy(a->cte->elem,elem,a->tamdato);
}


TNodoABO* BuscarPadre(TNodoABO *padre,TNodoABO *hijo){
	TNodoABO *paux = NULL;
	if ((padre->izq == hijo) || (padre->der == hijo))return padre;
	else{
		if (padre->izq != NULL)
			paux = BuscarPadre(padre->izq,hijo);
		if ((padre->der != NULL) && (paux == NULL))
			paux = BuscarPadre(padre->der,hijo);
		return paux;
	}
}


void ABO_MoverCte(TABO *a,int mov,int *error){
	*error = 0;
	switch (mov){
		case IZQ : 
			if (a->cte->izq != NULL)a->cte = a->cte->izq;
			else *error = 1;
			break;
		case DER : 
			if (a->cte->der != NULL)a->cte = a->cte->der;
			else *error = 1;
			break;
		case PAD : 
			if (a->cte != a->raiz)a->cte = BuscarPadre(a->raiz,a->cte);
			else *error = 1;
			break;
		case RAIZ : 
			if (a->raiz != NULL)a->cte = a->raiz;
			else *error = 1;
			break;
		default : 
			*error = 1;
	}
}


void VaciarSub(TNodoABO *pnodo){
	if (pnodo != NULL){
		if ((pnodo->izq == NULL) && (pnodo->der == NULL)){
			free(pnodo->elem);
			free(pnodo);
		}
		else{
			VaciarSub(pnodo->izq);
			VaciarSub(pnodo->der);
			free(pnodo->elem);
			free(pnodo);
		}
	}
}


void ABO_Vaciar(TABO *a){
	VaciarSub(a->raiz);
	a->raiz = NULL;
	a->cte = NULL;
}



int ABO_Vacio(TABO a){
	if (a.raiz == NULL)return 1;
	else return 0;
}
