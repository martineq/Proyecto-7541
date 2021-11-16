#include "tac.h"

/* Prototipos de las funciones que usa el TAC_Abrir*/
int comprobar_arch_config(char* ruta);
void cargar_matriz_a_memoria(TAC* tac,char* archConfig);
int caracteres_espanioles_permitidos(int c);
int valida_caracteres_repetidos(char vector[],int caracter);
void cargar_vector(char vector[]);


/* Prototipo de las funcion que usa el TAC_Leer y TAC__Escribir */
int findchar(TAC* tac,char* c,int* x,int* y);

/* Prototipos de las funciones que usa el TAC_Leer */
int convcol(TAC* tac,char* letra);
int convfil(TAC* tac,char* letra);
int convrec(TAC* tac,char* letra1,char* letra2);
int descifra(char* cifrado,TAC* tac,char* mensaje);

/* Prototipos de las funciones que usa el TAC_Escribir */
int revisamsg(TAC* tac,char* m);
int haychar(TAC* tac,char c);
int caso( TAC* tac,char c1,char c2);
int cifcol(TAC* tac,char* letra);
int ciffil(TAC* tac,char* letra);
int cifrec(TAC* tac,char* letra1,char* letra2);
int cifra(TAC* tac,char* mensaje,char* cifrado);
int guardamsg(TAC* tac,char*cifrado);
