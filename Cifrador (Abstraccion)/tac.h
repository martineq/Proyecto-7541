#ifndef TAC_H_
#define TAC_H_
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* DEFINES PROPIOS DE LA ESTRUCTURA */
#define MAXBUFFER 255
#define N 9

#define M N*N               /* constante para vector para validar caracteres*/
#define CANTPERMITIDOS 14   /*Constante para funcion caracteres españoles permitidos*/
#define K -1                /*defino una constante con un valor no valido como caracter para llenar*/
		                    /*el vector auxiliar,se usara para optimizar los recorridos*/

/* DEFINICIÓN DE LA ESTRUCTURA */
typedef struct TAC{
     char matriz[N][N];                     /* Clave del algoritmo Playfair*/
     FILE* archivo;                         /* Archivo donde tengo el texto a descifrar o cifrar según sea el caso */
} TAC;

/* 88888888888888888888888888888888888888888888 */
/*       Acá se encuntran los prototipos        */
/*                 de las primitivas            */
/* 88888888888888888888888888888888888888888888 */

/* Prototipos de las primitivas*/
int TAC_Abrir(TAC* tac,char* archMensaje,char tipoAcceso,char* archConfig);
int TAC_Leer(TAC* tac,char* mensaje);
int TAC_Escribir(TAC* tac,char* mensaje);
int TAC_Cerrar(TAC* tac);


#endif /*TAC_H_*/
