#include "tac.h"
#include "Prototipos.h"

/* 88888888888888888888888888888888888888888888 */
/*     Ac· se encuntran los cÛdigos de las      */
/*    funciones que se usan en las primitivas   */
/* 88888888888888888888888888888888888888888888 */

/* 88888888888888888888888888888888888888888888 */
/* CÛdigo de las funciones que usa el TAC_Abrir */
/* 88888888888888888888888888888888888888888888 */

/*Esta funcion valida que no haya caracteres repetidos en la matriz del archivo de configuracion
para ello utiliza un vector auxiliar en el que va cargando cada caracter que se obtiene de
dicha matriz, no sin antes recorrer el vector y fijarse que ya no se haya cargado ese caracter,
Pre: el vector esta cargado en memoria con valores K(constante definida) y tiene suficiente
espacio para almacenar M valores.El caracter pertenece al juego de caracteres validos para
la matriz.
Post:devuelve 0 si el caracter no estaba en el vector y agrega el caracter al final del vector;
devuelve -3(archivo de configuracion invalido) si el caracter es un caracter repetido.
Guido Nahuel Ygounet*/
int valida_caracteres_repetidos(char vector[],int caracter){
	int i;
	i=0;
	while((vector[i]>0)&&(i<M)){
		if (vector[i]==caracter) return -3;/*si encuentra el caracter en el vector,esta repetido*/
		i++;
	}
	if (vector[i]<0){            /* si salio xq no encontro ninguna vez ese caracter en el vector*/
		vector[i]=caracter; /*y encontro una posicion con un valor <0*/
		return 0;          /*entonces agrega el caracter en esa posicion del vector*/
	}
	return 0;
}
/*Esta funcion carga un vector auxiliar de M posiciones, con el valor K(cnstante
con caracter no valido para la matriz auxiliar, para luego optimizar el recorrido del
vector.
Pre:el vector tiene lugar suficiente para almacenar M valores.
Post: vector cargado en sus M posiciones con el valor K
Guido Nahuel Ygounet*/
void cargar_vector(char vector[]){
	int i;
	for(i=0;i<M;i++) vector[i]=K;
}
/*Carga la matriz del archivo de configuracion a la estructura.
 * Pre: tac tiene suficiente memoria para guardar una estructura tipo TAC.
 * archConfig es una ruta valida y la existencia del archivo y
 * validez de su contenido fueron validadas antes de invocar esta funcion,
 * solo se limita a abrirlo para lectura.
 * Post: Devuelve la matriz del archivo de configuracion en
 * la estructura(tac.matriz)
 * Guido Nahuel Ygounet*/
void cargar_matriz_a_memoria(TAC* tac,char* archConfig){
	int c,j;
	unsigned char caract;
	FILE* p_arch_config;
	p_arch_config=fopen(archConfig,"r");/*abre el archivo.Validaciones ya realizadas*/

    for(j=0;(c=getc(p_arch_config))!=EOF;j++){ /*Recorre cada columna hasta que encuentra el EOF*/

        int i;
		for(i=0;c!='\n';i++){/*Recorre cada fila hasta que encuentra el caracter de fin de linea*/
			caract=c;
			tac->matriz [i][j]=caract;/*asigna el caracter a la posicion correspondiente de la matriz en memoria*/
			c=getc(p_arch_config);/*lee el siguiente caracter*/
		}
	}
}
/*Valida Caracteres agregados por el espaÒol.
 * Pre:c es un integer ya asignado
 * Post: devuelve 0 si c no es un caracter permitido,
 * !=0 si c es un caracter permitido.
 * Guido Nahuel Ygounet*/
int caracteres_espanioles_permitidos(int c){
	unsigned char permitidos [CANTPERMITIDOS+1]="·ÈÌÛ˙¡…Õ”⁄¸‹Ò—";
	int i;
	int valido=0;
	for(i=0;i<CANTPERMITIDOS;i++){
		if (permitidos[i]==c) valido++;
	}
	return valido;
}
/* Esta funcion comprueba que exista el archivo de configu-
 * racion y que sea valido.
 * Pre:ruta valida archivo de configuracion.
 * Post: -1 si no puede abrirlo,-3 si es invalido,0 si es
 * valido
 * Guido Nahuel Ygounet*/
int comprobar_arch_config(char*ruta){
	FILE* p_arch_config;
	char vector[M];
	int countcantcolum,c;
	p_arch_config=fopen(ruta,"r");
	if (!p_arch_config) return -1;/*si no puede abrilo devuelve -1*/
	countcantcolum=0;
	cargar_vector(vector);/*Invoca a la funcion*/
	while ((c=getc(p_arch_config))!= EOF){/*Recorre el archivo de config*/
		int countelemfil;
		countelemfil=0;
		while(c!='\n'){
			int CaractValido;/*Valida caracteres permitidos*/
			int Repetido;
			CaractValido=caracteres_espanioles_permitidos(c);/*Invoca la funcion*/
			if (!((isalnum(c) )||(c=='@')||(c=='.')||(c==',')||(c=='\t')||(c==' ')||(CaractValido!=0))){
				fclose(p_arch_config);
				return -3;
			}
			Repetido=valida_caracteres_repetidos(vector,c);/*Invoca la funcion*/
			if (Repetido!=0){/*Si el caracter esta repetido*/
				fclose(p_arch_config);
				return -3;
			}

			countelemfil++;
			c=getc(p_arch_config);
		}
		if (countelemfil!=N){
			fclose(p_arch_config);
			return -3;
		}
		countcantcolum++;
	}
	if (countcantcolum!=N){
		fclose(p_arch_config);
		return -3;
	}
	fclose(p_arch_config);
	return 0;
}


/* 8888888888888888888888888888888888888888888888888888888888 */
/* CÛdigo de la funciÛn que usa el TAC_Leer y el TAC_Escribir */
/* 8888888888888888888888888888888888888888888888888888888888 */

int findchar(TAC* tac,char* c,int* x,int* y) {/* FunciÛn que busca un caracter dentro de la matriz y devuelve sus coordenadas de posicionamiento toma como par·metros los punteros a: el TAC, el caracter a buscar, y las coordenadas de posicionamiento. Devuelve error (-1) si no encuentra el caracter. */
  (*x)=0;
  while (*x < N) {
    (*y)=0;
    while  ((*y) < N) {
      if ( (tac->matriz[(*x)][(*y)]) == (*c) )
        return 0;
      else
        (*y)++;
      }
      (*x)++;
  }
  return -1;
}



/* 888888888888888888888888888888888888888888 */
/* CÛdigo de las funciones que usa el TAC_Leer*/
/* 888888888888888888888888888888888888888888 */

int descifra(char* cifrado,TAC* tac,char* mensaje){
char c;   /* Variable que se va a usar en el descifrado del texto, toma el 1er caracter de cada "tanda" */
int i,j;   /* Variables para recorrer el vector de caracteres cifrados, dentro del ciclo iterativo */
i=j=0;
 while ( ( (c=cifrado[i++]) != '\0') && (j<MAXBUFFER)  ) {           /* Mientras queden caracteres para leer (leo el primer caracter para determinar que voy a hacer) y mientras no sobrepase la capacidad admitida (MAXBBUFER) */
    char c1,c2;                                    /* Una vez que comienza el ciclo preparo otras 2 variables que usarÈ en el descifrado del texto */
    switch (c){                                    /* Realizo las operaciones seg˙n cual sea el valor de c */
      case '&':                                    /* Si es "&" */
        c1=cifrado[i++];                           /* Copio el siguiente caracter */
        mensaje[j++]=c1;
        mensaje[j++]=c1;                         /* Paso los caracteres descifrados al vector de salida */
        break;

      case '$':                                    /* Si es "$" */
        c1=cifrado[i++];
        c2=cifrado[i++];                           /* Copio los 2 siguientes caracteres */
        if (convcol(tac,&c1) != 0) return -2;      /* Descifro el caracter seg˙n las reglas (igual fila) y la matriz */
        if (convcol(tac,&c2) != 0) return -2;      /* Si no puedo convertir (convcol distinto de cero) devuelvo un error */
        mensaje[j++]=c1;
        mensaje[j++]=c2;
        break;

      case '%':                                     /* Si es "%" */
        c1=cifrado[i++];
        c2=cifrado[i++];                            /* Copio los 2 siguientes caracteres */
        if (convfil(tac,&c1) != 0) return -2;       /* Descifro el caracter seg˙n las reglas (igual fila) y la matriz */
        if (convfil(tac,&c2) != 0) return -2;       /* Si no puedo convertir (convfil distinto de cero) devuelvo un error */
        mensaje[j++]=c1;
        mensaje[j++]=c2;
        break;

      default:
        c1=c;
        c2=cifrado[i++];                            /* Copio los 2 siguientes caracteres */
        if (convrec(tac,&c1,&c2) != 0) return -2;   /* Descifro los caracteres seg˙n las reglas (distinta fila y columna)... */
        mensaje[j++]=c1;                           /* ...y la matriz. Si no puedo convertir (convrec distinto de cero) devuelvo un error */
        mensaje[j++]=c2;
        break;
    }    /* Fin switch (c) */
  }    /* Fin while (cifrado[i] != ¥\0¥) */
  if (j>=MAXBUFFER) return -2;      /* Si el mensaje sobrepasÛ la capacidad admitida (MAXBBUFER) devuelvo un error (-2) */
  else
    if ( mensaje[j-1] == '@')     /* Si el ˙ltimo caracter es un "@" */
      mensaje[--j]='\0';          /* Borro ese caracter y lo cambio por un caracter de fin de lÌnea ¥\0¥ */
    else
      mensaje[j]='\0';            /* Sino, luego de el ˙ltimo caracter v·lido le agrego el caracter de fin de lÌnea ¥\0¥ */

  return 0;
}

int convfil(TAC* tac,char* letra){  /* FunciÛn que descifra el caracter seg˙n la regla (igual fila), los argumentos son el puntero al TAC para usar la matriz y el puntero al caracter a decifrar (este vuelve ya convertido), devuelve error (-1) si falla la b˙squeda. */
  int x,y;
  if ((findchar(tac,letra,&x,&y)) != 0) return -1;
  if (x==0){
    if (y==0){
      x=N-1;
      y=N-1;
    }
    else{
     x=N-1;
     y--;
    }
  }
  else  x--;
  (*letra)=tac->matriz[x][y];
  return 0;
}

int convcol(TAC* tac,char* letra){/* FunciÛn que descifra el caracter seg˙n la regla (igual columna), los argumentos son el puntero al TAC para usar la matriz y el puntero al caracter a decifrar (este vuelve ya convertido), devuelve error (-1) si falla la b˙squeda. */
int x,y;
  if ((findchar(tac,letra,&x,&y)) != 0) return -1;
  if (y==0){
    if (x==0){
      x=N-1;
      y=N-1;
    }
    else{
     x--;
     y=N-1;
    }
  }
  else  y--;
  (*letra)=tac->matriz[x][y];
  return 0;
}

int convrec(TAC* tac,char* letra1,char* letra2){ /* FunciÛn que descifra el caracter seg˙n la regla (distinta fila y columna), los argumentos son el puntero al TAC para usar la matriz y los punteros a los caracteres a decifrar (estos vuelven ya convertidos), devuelve error (-1) si falla la b˙squeda. */
  int x1,y1,x2,y2;
  if ( (findchar(tac,letra1,&x1,&y1) != 0) || findchar(tac,letra2,&x2,&y2) != 0) return -1;
  (*letra1)=tac->matriz[x2][y1];
    (*letra2)=tac->matriz[x1][y2];
  return 0;
}



/* 8888888888888888888888888888888888888888888888 */
/* CÛdigo de las funciones que usa el TAC_Escribir*/
/* 8888888888888888888888888888888888888888888888 */

int revisamsg(TAC* tac,char* m){
  int i;
  i=0;
  if (m[i] == '\0')  return -2;
  else{
    while ( (m[i] != '\0') ) {
      if ( haychar(tac,m[i]) == -1) return-1;
      i++;
    }   /* fin while*/

    if ( ( i%2) != 0 ){
      m[i++]='@';
      m[i]='\0';
      return 0;
    } /* fin if */
  }  /* fin else */
  return 0;
}

int haychar(TAC* tac,char c) {/* FunciÛn que busca un caracter dentro de la matriz y devuelve si se encuentra 0 sino -1*/
  int x,y;
  (x)=0;
  while (x < N) {
    (y)=0;
    while  ((y) < N) {
      if ( (tac->matriz[(x)][(y)]) == (c) )
        return 0;
      else
        (y)++;
      }
      (x)++;
  }
  return -1;
}

int caso( TAC* tac,char c1,char c2){
  int x1,y1,x2,y2;
  if (c1 == c2) return 1;
  else{
    findchar(tac,&c1,&x1,&y1);
    findchar(tac,&c2,&x2,&y2);
    if (x1 != x2){
       if (y1 != y2) return 4;
       else return 3;
    }
    else return 2;
  }
}

int cifcol(TAC* tac,char* letra){  /* FunciÛn que cifra el caracter seg˙n la regla (igual fila), los argumentos son el puntero al TAC para usar la matriz y el puntero al caracter a decifrar (este vuelve ya convertido), devuelve error (-1) si falla la b˙squeda. */
  int x,y;
  if ((findchar(tac,letra,&x,&y)) != 0) return -1;
  if (y==N-1){
    if (x==N-1){
      x=0;
      y=0;
    }
    else{
     x++;
     y=0;
    }
  }
  else  y++;
  (*letra)=tac->matriz[x][y];
  return 0;
}

int ciffil(TAC* tac,char* letra){  /* FunciÛn que cifra el caracter seg˙n la regla (igual fila), los argumentos son el puntero al TAC para usar la matriz y el puntero al caracter a decifrar (este vuelve ya convertido), devuelve error (-1) si falla la b˙squeda. */
  int x,y;
  if ((findchar(tac,letra,&x,&y)) != 0) return -1;
  if (x==N-1){
    if (y==N-1){
      y=0;
      x=0;
    }
    else{
     y++;
     x=0;
    }
  }
  else  x++;
  (*letra)=tac->matriz[x][y];
  return 0;
}

int cifrec(TAC* tac,char* letra1,char* letra2){ /* FunciÛn que cifra el caracter seg˙n la regla (distinta fila y columna), los argumentos son el puntero al TAC para usar la matriz y los punteros a los caracteres a decifrar (estos vuelven ya convertidos), devuelve error (-1) si falla la b˙squeda. */
  int x1,y1,x2,y2;

  if ( (findchar(tac,letra1,&x1,&y1) != 0) || findchar(tac,letra2,&x2,&y2) != 0) return -1;

  (*letra1)=tac->matriz[x2][y1];
  (*letra2)=tac->matriz[x1][y2];
  return 0;
}

int cifra(TAC* tac,char* mensaje,char* cifrado){
  int i,j;                                                              /* i: Variable para recorrer el vector de caracteres a cifrar, dentro de un ciclo iterativo */
  char c1,c2;                                                                      /* j: Variable para recorrer el vector de caracteres que van a ser cifrados, dentro del ciclo iterativo */

  i=j=0;
  c1=mensaje[i++];
  while ( ( c1 != '\0' ) && ( ((c2=mensaje[i++])) != '\0' ) && (j < (MAXBUFFER-2) )  ){
    switch ( caso(tac,c1,c2) ){

      case 1:                   /* Caracteres iguales */
        cifrado[j++]='&';
        cifrado[j++]=c1;
        break;

      case 2:                   /* Misma columna */
        cifrado[j++]='$';
        if (cifcol(tac,&c1) != 0) return -2;
        if (cifcol(tac,&c2) != 0) return -2;
        cifrado[j++]=c1;
        cifrado[j++]=c2;
        break;

      case 3:                   /* Misma fila */
        cifrado[j++]='%';
        if (ciffil(tac,&c1) != 0) return -2;
        if (ciffil(tac,&c2) != 0) return -2;
        cifrado[j++]=c1;
        cifrado[j++]=c2;
        break;

      case 4:                   /* distinta fila y columna, adem·s de distintos caracteres */
        if ( cifrec(tac,&c1,&c2) != 0)  return -2;
        cifrado[j++]=c1;
        cifrado[j++]=c2;
        break;
    }  /* Fin switch */
    c1=mensaje[i++];
  }   /* Fin ciclo while */
  if (j>=(MAXBUFFER-2)) return -2;    /* Si salÌ del ciclo por superar el lÌmite emito error*/
  else{
    cifrado[j]='\0';                   /* Agrego el caracter de fin de lÌnea para finalizar la lÌnea cifrada*/
    return 0;
  }
}

int guardamsg(TAC* tac,char*cifrado){
  if ( (fputs(cifrado,tac->archivo) == EOF) ) return -2; /* Si no puedo guardar la lÌnea cifrada en el archivo emito error */
  else {
    fputs ("\n",tac->archivo);
    return 0;
  }
}


