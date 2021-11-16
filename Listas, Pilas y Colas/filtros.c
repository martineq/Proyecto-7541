#include <stdio.h>
#include "TDA_TEJECUTOR_T_F.h"
#define MAX 70
#define MAXLINEA 256/*Constante de maxima longitud de linea*/
#define MAXWORD 65


/* PROTOTIPOS DE LAS FUNCIONES AUXILIARES DE LA IMPLEMENTACION DE LOS FILTROS*/

void upper (char* palabra);

void lower (char* palabra);

int ordenarpalabras (char datos[MAXWORD], TTubo* entrada, TTubo* salida,int eof);
int sacarordenadas (char datos[MAXWORD], TTubo* salida, int eof, TListaSimple* lista1);

int eliminarrepeticiones (TTubo* entrada, TTubo* salida,int eof);

int contarconsecutivas (TTubo* entrada, TTubo* salida,int eof);
int agregar (int n, char ultima[MAX], char posicion[MAX]);
int NumACad (int n, char numero[5]);
void reverse(char numero[5]);
void formatear (char posicion[MAX]);
int unir (char posicion[MAX],char ultima[MAX]);

int moverultimos(int datos_filtro, TTubo* entrada,TTubo* salida, int eof);
void completartubo(TTubo* entrada,TTubo* salida,int* cantsalida,int total);
void moverelemtubo(TTubo* entrada,TTubo* salida);
int cantidadelem(TTubo* t);

int moverprimeros(int datos_filtro, TTubo* entrada,TTubo* salida, int eof);
void llenartubo(TTubo* entrada,TTubo* salida,int* cantsalida,int total);

int moverfiltrados(TListaSimple datos_filtro,TTubo* entrada,TTubo* salida,int eof);
void filtrarelemtubo(TTubo* entrada,TTubo* salida,TListaSimple datos_filtro);
int estaenlista(TListaSimple l,char* c);

int buscarfinal (int dato,char ultima[MAX],int* final,int totalcampos);
int buscarinicio (int dato,char ultima[MAX],int* inicio);
int contarcampos (int dato, char ultima[MAX],int* totalcampos);

int extraercampo (int dato, TTubo* entrada, TTubo* salida, int eof);


/*FILTROS*/

/* Filtro que ordena las palabras lexicográficamente en orden ascendente o descendente.
** Es importante remarcar que el filtro es SENSIBLE A MAYÚSCULAS, por lo que si se desea ordenar
** en orden alfabético primero se debe utilizar el filtro de pasar todo a minúsculas o a mayúsculas.
** Este filtro solo comienza el proceso si recibió el eof, sino devuelve eof=0 y sale.
** Pedro Lynn*/
int ORDENAR(void* datos_filtro, TTubo* entrada, TTubo* salida,int eof){

if (C_Vacia(*entrada)){
    if (eof) return 1;
    else return 0;
  }
if (eof==0) return 0;

   else return ordenarpalabras ((char*) datos_filtro, entrada, salida, eof);

}
/*Saca las palabras del tubo de entrada y las ubica en una lista interna del filtro*/
int ordenarpalabras (char datos[MAXWORD], TTubo* entrada, TTubo* salida,int eof){

char ultima[MAXWORD];
TListaSimple lista1;

ls_Crear(&lista1, (MAXWORD + 5)*sizeof(char));
while (!C_Vacia(*entrada)){
      C_Sacar(entrada, ultima);
      ls_Insertar(&lista1,LS_SIGUIENTE, ultima);
      }
return sacarordenadas (datos, salida, eof, &lista1);
}
/*Las saca en orden de la lista interna y las agrega al tubo de salida.*/
int sacarordenadas (char* datos, TTubo* salida, int eof, TListaSimple* lista1){

char comparador[MAXWORD];
char auxiliar[MAXWORD];
int resultadocmp;
int resultadocmp2;
int resultadocmp3;

strcpy (comparador,"");
strcpy (auxiliar,"");
resultadocmp=0;
resultadocmp2=0;
resultadocmp3=0;

while (!ls_Vacia(*lista1)){
          ls_MoverCorriente(lista1, LS_PRIMERO);
          ls_ElemCorriente(*lista1, comparador);
          ls_BorrarCorriente(lista1);
          while ((!ls_Vacia(*lista1)) && (ls_MoverCorriente(lista1, LS_SIGUIENTE))){
                ls_MoverCorriente(lista1, LS_ANTERIOR);
                ls_ElemCorriente(*lista1, auxiliar);
                resultadocmp= (strcmp (auxiliar,comparador));
                resultadocmp2= (strcmp(datos,"ASC"));
                resultadocmp3= (strcmp(datos,"DESC"));
                if (((resultadocmp<0))  && (resultadocmp2==0) && (resultadocmp3!=0) ){
                   ls_BorrarCorriente(lista1);
                   ls_Insertar(lista1,LS_ANTERIOR, comparador);
                   strcpy (comparador,auxiliar);
                   }
                if (((resultadocmp>0))  && (resultadocmp2!=0) && (resultadocmp3==0) ){
                   ls_BorrarCorriente(lista1);
                   ls_Insertar(lista1,LS_ANTERIOR, comparador);
                   strcpy (comparador,auxiliar);
                }
                if ((resultadocmp>0) || (resultadocmp2==0)){
               }
          }
          C_Agregar(salida,comparador);
    }
return 1;
}
/*------------------------------------------------------------*/
int ELIMINAR_CONSEC(void* datos_filtro, TTubo* entrada, TTubo* salida,int eof){

if (C_Vacia(*entrada)){
    if (eof) return 1;
    else return 0;
  }
  return eliminarrepeticiones (entrada, salida, eof);
}

int eliminarrepeticiones (TTubo* entrada, TTubo* salida,int eof){

char comparador[MAXWORD];
char ultima[MAXWORD];
char auxiliar[MAXWORD];


strcpy (comparador,"");
strcpy (auxiliar,"");
while (!C_Vacia(*entrada)){
      C_Sacar(entrada, ultima);
      if ((strcmp (comparador,ultima)!= 0)){
         strcpy (comparador,ultima);
         strcpy (auxiliar,ultima);
         C_Agregar(salida,ultima);
         }
      }
return eof;
}

/*-------------------------------------------------------------*/


int CONTAR_CONSEC(void* datos_filtro, TTubo* entrada, TTubo* salida,int eof){

if (C_Vacia(*entrada)){
    if (eof) return 1;
    else return 0;
  }
  return contarconsecutivas (entrada, salida, eof);
}

int contarconsecutivas (TTubo* entrada, TTubo* salida,int eof){

char comparador[MAX];
char ultima[MAX];
char posicion[MAX];
int resultado;
int n;
int primera = 1;

n=0;
strcpy (comparador,"");

n=1;
while (!C_Vacia(*entrada)){
      C_Sacar(entrada, ultima);

      if ((strcmp (comparador,ultima))== 0){
         n++;
         resultado= (agregar (n,ultima,posicion));
         formatear (posicion);
         unir (posicion,ultima);
         //C_Agregar(salida,posicion);

         }
      if ((strcmp (comparador,ultima))!= 0) {
		 if(!primera) C_Agregar(salida,posicion);

         n=1;
         strcpy (comparador,ultima);
         resultado=agregar (n, ultima,posicion);
         formatear (posicion);
         unir (posicion,ultima);

         //C_Agregar(salida,posicion); se agrega la línea al comienzo del if
         }

		 primera = 0;
      }

if(!primera) C_Agregar(salida,posicion);

return resultado;
}

int agregar (int n, char ultima[MAX],char posicion[MAX]){



         strcpy (posicion,"0000;");
         NumACad (n, posicion);



return n;
}

int NumACad (int n, char numero[5]){

int i;
int j;

j=n;

i = 0;
    do {       /* genera digitos en orden inverso */
        numero[i++] = j % 10 + '0';   /* obtiene el siguiente digito */
    } while ((j /= 10) > 0);     /* lo borr */
    numero[i] = '\0';

    reverse(numero);

return 0;
}

void reverse(char numero[5]){

    int c, i, j;

    for (i = 0, j = strlen(numero)-1; i<j; i++, j--) {
        c = numero[i];
        numero[i] = numero[j];
        numero[j] = c;
    }

}

void formatear (char posicion[MAX]){

char auxiliar[MAX];
int j,k;
char* punt;

     punt = auxiliar;
     j=0;
     k=0;
     strcpy (auxiliar,"0000;");
     k= strlen (posicion);
     strcpy ((punt+(4-k)),posicion);
     strcpy ((punt+4),";");
     strcpy (posicion,auxiliar);


     }

int unir (char posicion[MAX],char ultima[MAX]){

          strcat(posicion,ultima);

return 0;
}
/*---------------------OTROS----------------------------------*/

int EXTRAER(void* datos_filtro, TTubo* entrada, TTubo* salida,int eof){

    int error;
    if (C_Vacia(*entrada)){
       if (eof) return 1;
       else return 0;
       }
    error= extraercampo ((int) datos_filtro, entrada, salida, eof);
    if (error) printf ("Campo no existente\n");
    if (eof) return 1;
    else return 0;
}

int extraercampo (int dato, TTubo* entrada, TTubo* salida, int eof){

    char ultima[MAX];
    char *paux;
    int i;

    if (dato<1) return -1;

    while (!C_Vacia(*entrada)){
          C_Sacar(entrada, ultima);

		  i=1;
		  paux = strtok(ultima, ";");
		  while( paux!=NULL) {
			  if(i==dato) { /* encontro el campo buscado */
				  C_Agregar(salida, paux);
				  break;
			  } else {
				  paux = strtok(NULL, ";");
				  i++;
			  }
		  }
    }
    return 0;
}

int contarcampos (int dato, char ultima[MAX],int* totalcampos){

    int i;
    i = 0;
    *totalcampos=1;

    while (i<(strlen(ultima)-1)){
          if (ultima[i] == ';'){
             *totalcampos=*totalcampos+1;

          }
          i++;
    }
return 0;
}


int buscarinicio (int dato,char ultima[MAX],int* inicio){

    int campo;

    *inicio=0;
    campo=1;

    while (campo<dato){

          while (ultima[*inicio]!=';'){
                *inicio=*inicio+1;


          }
          campo++;
          *inicio=*inicio+1;
          }
          *inicio=*inicio+1;

    return 0;
}

int buscarfinal (int dato,char ultima[MAX],int* final,int totalcampos){

    int campo;

    *final=0;
    campo=1;

    while (campo<dato+1){
          while ((ultima[*final]!=';') && ((strlen(ultima))>*final)){
                *final=*final+1;
          }
          campo++;
          *final=*final+1;
    }
    *final=*final-1;
    return 0;
}

/*---------------------------------------------------------------------*/
/*Convierte a Mayusculas una cadena
 * Guido Nahuel Ygounet*/
void upper (char* palabra){
	int i=0;
	while(palabra[i]){
			palabra[i]=toupper(palabra[i]);
			i++;
	}
}
/*Convierte todos los elementos en la cola de entrada a mayusculas y l
 * y los coloca en igual orden en la cola de salida
 * Guido Nahuel Ygounet*/
int MAYUSCULAS(void* datos_filtro,TTubo* entrada,TTubo* salida,int eof){
	if (!eof)	return 0;
	else{ 
          while (!C_Vacia(*entrada)){
		        char elemento[MAXWORD];
		        C_Sacar(entrada,elemento);
		        upper(elemento);
		        C_Agregar(salida,elemento);
           }
	 return 1;
  }

}
/*Convierte a Minusculas una cadena
 * Guido Nahuel Ygounet*/
void lower (char* palabra){
	int i=0;
	while(palabra[i]){
		palabra[i]=tolower(palabra[i]);
		i++;
	}
}
/*Convierte todos los elementos en la cola de entrada a minusculas
 * y los coloca en igual orden en la cola de salida
 * Guido Nahuel Ygounet*/
int MINUSCULAS(void* datos_filtro,TTubo* entrada,TTubo* salida,int eof){
	if (!eof)	return 0;
	else{ 
          while (!C_Vacia(*entrada)){
		        char elemento[MAXWORD];
		        C_Sacar(entrada,elemento);
		        lower(elemento);
		        C_Agregar(salida,elemento);
           }
           return 1;
 }

}
/*88888888888888888888888888888888888888888888888888888888888888888888888888888*/
/*88888888888888888888888888888888888888888888888888888888888888888888888888888*/
/*88888888888888888888888888888888888888888888888888888888888888888888888888888*/
/*88888888888888888888888888888888888888888888888888888888888888888888888888888*/


/* Función que toma los últimos N elementos del tubo de entrada y los pone en el tubo de salida.
** Si los elementos de entrada son menores que N devuelve esa cantidad de elementos.
** El parámetro datos_filtro es un puntero a una estructura conocida, es este caso un int
** que indica la cantidad de elementos que dejo pasar. El filtro devuelve true si ya terminó su proceso */
/* Autor: Martín Quiroz*/
int ULTIMOS(void* datos_filtro, TTubo* entrada,TTubo* salida, int eof){
  if (C_Vacia(*entrada)){
    if (eof) return 1;   /* En esta instancia no se filtrará ni se va a filtrar nada */
    else return 0;  /* Acá, ahora no se va a filtrar nada, pero pude ser que en otra instancia sí se filtre */
  }
    return moverultimos((int) datos_filtro,entrada,salida,eof);
}


/* Pasa una determinada cantidad de (los últimos) elementos de un tubo
 de entrada en otro tubo de salida, devuelve true si ya terminó su proceso*/
/* Autor: Martín Quiroz*/
int moverultimos(int datos_filtro, TTubo* entrada,TTubo* salida, int eof){
  int cantsalida=cantidadelem(salida); /*  */
  completartubo(entrada,salida,&cantsalida,datos_filtro);  /* Paso los elementos del tubo de entrada  al de salida */

  if (!eof) return 0;   /* Si todavia no llegué al eof, no terminó el filtro */
  else  return 1;       /* Si llegué al eof, entonces doy por terminado el filtro */
}

/* Completa un tubo de salida con elementos del tubo de entrada, **
** dejando solo una determinada cantidad de elemntos como máximo.
** Informa por parámetro la cantidad de elementos que quedaron en
** el tubo de salida    */
/* Autor: Martín Quiroz */
void completartubo(TTubo* entrada,TTubo* salida,int* cantsalida,int total){
  char* pun=(char*)malloc(sizeof(char) * MAXWORD);
  while ( !(C_Vacia(*entrada)) ) {
    moverelemtubo(entrada,salida);
    (*cantsalida)++;
    if ( (*cantsalida) > (total) ){  /* Retengo la cantidad de elementos que necesito */
      C_Sacar(salida,pun);
      (*cantsalida)--;
    }
  }

  free(pun);

}

/* Mueve un elemento de un tubo (entrada) a otro (salida)
** supongo que siempre voy a poder agregar elementos en el
** tubo de salida */
/* Autor: Martín Quiroz*/
void moverelemtubo(TTubo* entrada,TTubo* salida){
  char* pun=(char*)malloc(sizeof(char) * MAXWORD);
  C_Sacar(entrada,pun);
  C_Agregar(salida,pun);
  free(pun);
}


/* Devuelve la cantidad de elementos de un tubo */
/* Autor: Martín Quiroz*/
int cantidadelem(TTubo* t){
  int x=0;
  TTubo aux;
  C_Crear(&aux, sizeof(char) * MAXWORD);  /* uso esto porque siempre voy a tener stings de como maximo MAXWORD posiciones */
  while ( !(C_Vacia(*t)) ){
    moverelemtubo(t,&aux);
    x++;
  }
  while ( !(C_Vacia(aux)) )  moverelemtubo(&aux,t); /* vuelvo a poner la cola en su lugar */
  return x;
}


/*88888888888888888888888888888888888888888888888888888888888888888888888888888*/
/*88888888888888888888888888888888888888888888888888888888888888888888888888888*/
/*88888888888888888888888888888888888888888888888888888888888888888888888888888*/



/* Función que toma los primeros N elementos del tubo de entrada y los pone en el tubo de salida.
** Si los elementos de entrada son menores que N devuelve esa cantidad de elementos.
** El parámetro datos_filtro es un puntero a una estructura conocida, es este caso un int
** que indica la cantidad de elementos que dejo pasar. El filtro devuelve true si ya terminó su proceso*/
/* Autor: Martín Quiroz*/
int PRIMEROS(void* datos_filtro, TTubo* entrada,TTubo* salida, int eof){

  if (C_Vacia(*entrada)){
    if (eof) return 1;   /* En esta instancia no se filtrará ni se va a filtrar nada */
    else return 0;  /* Acá, ahora no se va a filtrar nada, pero pude ser que en otra instancia sí se filtre */
  }

  return moverprimeros((int) datos_filtro ,entrada,salida,eof);
}

/* Pasa una determinada cantidad de (los primeros) elementos de un tubo
 de entrada en otro tubo de salida, devuelve true si ya terminó su proceso*/
/* Autor: Martín Quiroz*/
int moverprimeros(int datos_filtro, TTubo* entrada,TTubo* salida, int eof){
  int cantsalida=cantidadelem(salida);   /*   CAMBIADO  */
  llenartubo(entrada,salida,&cantsalida,datos_filtro);    /* Paso los elementos del tubo de entrada al de salida */
  if ( (cantsalida < (datos_filtro) ) && (!eof) ) return 0;   /* Si todavia no llegué al eof y además no tengo los N elementos, no terminó el filtro */
  else  return 1;       /* En cualquier otro caso doy por terminado el filtro */
}



/* Llena un tubo de salida con todos elementos del tubo **
** de entrada, dejando solo una determinada cantidad de
** elemntos como máximo */
/* Autor: Martín Quiroz*/
void llenartubo(TTubo* entrada,TTubo* salida,int* cantsalida,int total){
  int cantnuevos=0;
  while ( (( cantnuevos + (*cantsalida) ) < total ) && (!(C_Vacia(*entrada))) ) {
    moverelemtubo(entrada,salida);
    cantnuevos++;
  }
  if ((C_Vacia(*entrada)))  C_Vaciar(entrada);
  (*cantsalida)+=cantnuevos;
}

/*88888888888888888888888888888888888888888888888888888888888888888888888888888*/
/*88888888888888888888888888888888888888888888888888888888888888888888888888888*/
/*88888888888888888888888888888888888888888888888888888888888888888888888888888*/


/* Función que filtra las palabras que le son pasadas por parámetro
** dentro de una lista(creada y no vacia), en el tubo de salida devuelve todas las demas
** palabras que no fueron filtradas. Devuelve true cuando terminó de procesar */
/* Autor: Martín Quiroz*/
int FILTRAR(void* datos_filtro, TTubo* entrada,TTubo* salida, int eof){
  if (C_Vacia(*entrada)){
    if (eof) return 1;   /* En esta instancia no se filtrará ni se va a filtrar nada */
    else return 0;       /* Acá, ahora no se va a filtrar nada, pero pude ser que en otra instancia sí se filtre */
  }
  return moverfiltrados(*((TListaSimple*) datos_filtro),entrada,salida,eof);
}

/* Función que filtra ciertas palabras del tubo de entrada, colocando
** en el tubo de salida las restantes. Devuelve true si termino el proceso*/
/* Autor: Martín Quiroz*/
int moverfiltrados(TListaSimple datos_filtro,TTubo* entrada,TTubo* salida,int eof){
  while ( (!(C_Vacia(*entrada))) ) {
    filtrarelemtubo(entrada,salida,datos_filtro);
  }
  if (!eof) return 0;   /* Si todavia no llegué al eof, no terminó el filtro */
  else  return 1;       /* Si llegué al eof, entonces doy por terminado el filtro */
}


/* Mueve un elemento de un tubo (entrada) a otro (salida), solo si no se encuetra
** en el tubo de palabras a filtrar. Supongo que siempre voy a poder agregar
** elementos en el tubo de salida. "Aclaración 2: Los filtros reciben y emiten strings" */
/* Autor: Martín Quiroz*/
void filtrarelemtubo(TTubo* entrada,TTubo* salida,TListaSimple datos_filtro){
  void* pun=malloc(sizeof(char) * MAXWORD);
  C_Sacar(entrada,pun);
  if (!estaenlista(datos_filtro,(char*)pun))
    C_Agregar(salida,pun);
  free(pun);
}

/* Devuelve true si la palabra está en la lista */
/* Autor: Martín Quiroz*/
int estaenlista(TListaSimple l,char* c){
  void* p=malloc(sizeof(char) * MAXWORD);
  ls_MoverCorriente(&l,LS_PRIMERO);
  ls_ElemCorriente(l,p);
  if ( strcmp((char*)p,c) == 0 ) { /* antes de entrar al ciclo me fijo el 1ro*/
    free(p);
    return 1;
  }
  while (ls_MoverCorriente(&l,LS_SIGUIENTE)) {
      ls_ElemCorriente(l,p);
      if ( strcmp((char*)p,c) == 0 ) return 1;
  }
  free(p);
  return 0;  /* Si llegó acá, no lo encontré*/
}

