#include "tac.h"
#include "Prototipos.h"
void limpiarcadena (char cadena[]);

/* 8888888888888888888888888888888888888888888888 */
/* Acá se encuntran los códigos de las primitivas */
/* 8888888888888888888888888888888888888888888888 */

/* Código de la primitiva TAC_Abrir */
/*Esta primitiva se encarga de abrir la estructura TAC
 * y validar la existencia del archivo de configuracion,
 *y que este sea valido, tanto en tamaño como en caracteres
 * contenidos.
 * Pre: tac tiene suficiente memoria reservada para
 * una estructura de tipo TAC. archMensaje es una ruta de archivo
 * válida. tipoAcceso es r o w. archConf es una ruta de archivo
 * válida.
 * Post:Devuelve un codigo de error de acuerdo a cada
 * posible falla, -1 no se pudo abrir archivo de configuracion,
 * -2 si se llamo al archivo cifrado para lectura y no pudo
 * abrirselo y -3 si el archivo de configuracion no es valido
 * por algun motivo.
 * Devuelve 0 si todo es correcto y la estructura fue creada
 * satisfactoriamente
 * Guido Nahuel Ygounet*/
int TAC_Abrir(TAC* tac,char* archMensaje,char tipoAcceso,char* archConfig){
	int error;
	error=comprobar_arch_config(archConfig);/*invoca a la funcion*/
	if (error==0){					/*valida que el arch de config sea valido*/
		cargar_matriz_a_memoria(tac,archConfig);/*invoca a la funcion*/
		if (tipoAcceso=='r'){
		  tac->archivo=fopen(archMensaje,"r");/*intenta abrir el arch cifrado p lectura*/
		  if (!tac->archivo) return -2;/*si no puede abrirlo devuelve el error*/
		}
		if (tipoAcceso=='w'){
		  tac->archivo=fopen(archMensaje,"w");/*trata de abrir el arch p escritura*/
		}								/*si ya existia lo pisa*/
	}
	return error;
}/* Fin TAC_Abrir */


/* Código de la primitiva TAC_Leer */
int TAC_Leer(TAC* tac,char* mensaje){                                 /* Primitiva del TAC */
  char cifrado[MAXBUFFER];      /* Acá guardo la linea de texto que se encuatra cifrada para luego tratarla */

   if ( fgets(cifrado,MAXBUFFER,tac->archivo) == NULL) return -1;      /* Con fgets tomo una linea de texto (incluido el caracter "\0" de fin de linea). Si devuelve NULL hay fin de archivo*/
  limpiarcadena (cifrado);

  switch ( descifra(cifrado,tac,mensaje) ){
    case -2 : return -2; break;
    case 0  : return 0;  break;
  }
  return -2;     /*Si llegue hasta acá quiere decir que no salio con el return 0, o sea es un error  */
} /* Fin TAC_Leer */



/* Código de la primitiva TAC_Escribir */
int TAC_Escribir(TAC* tac,char* mensaje){
  char cifrado[MAXBUFFER];                                            /* Acá guardo la linea de texto que se va a cifrar, para luego ésta guardarla en archivo */

  switch ( revisamsg(tac,mensaje) ) {  /* Dejo el mensaje listo para cifrar sino mando error */
    case -1: return -1;  /* caracter inválido */
      break;
    case -2: return -2;
      break;      /* otro error: solo se encuentra el '\0, o sea no hay ningun caracter */
  }
  switch ( cifra(tac,mensaje,cifrado) ) {
    case -2: return -2;  break;
    case 0:  return (guardamsg(tac,cifrado)); break;    /* El return devuelve el valor de la func guardamsg (-2 si hay error al guardar, 0 si se guarda bien) */
  }
  return -2;     /* Si no pasó por el return 0 anterirmente, quiere decir que hubo error, entonces devuelvo -2 */
}  /* Fin TAC_Escribir */


/* Código de la primitiva TAC_Cerrar */
/* ************************************************************************* */
/* Primitiva: TAC_Cerrar                                                     */
/* Autor: Leandro R. Bueno                                                   */
/*								           */
/* Variables: tac -> manipulador del archivo a cerrar                        */
/*									   */
/* Pre: tac abierto.                                                         */
/*									   */
/* Post: Si devuelve 0, tac cerrado; en otro caso devuelve -1.               */
/*									   */
/* Explicación: Esta primitiva cierra el archivo cifrado tac, devolviendo    */
/* un código de error si no puede realizar esta acción.                      */
/* ************************************************************************* */
int TAC_Cerrar(TAC* tac){
	if (fclose(tac->archivo)!= 0) return -1;  /*intenta cerrar el archivo apuntado por el tac, si no puede devuelve -1*/
	return 0;

}/* Fin TAC_Cerrar */
