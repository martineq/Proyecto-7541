#include "tac.h"

/* Prototipos de las funciones que usa el Main */
int Error_Abrir (int error);
int Error_Escribir (int error);
int Resultado_Leer (int error);
int Error_Cerrar (int error);
void Error_sintaxis ();
void limpiarcadena (char cadena[]);
int Validar_archconf (char nombre[]);
int cifrapantalla (TAC* tac1,char* mensaje);
int cifraarchivo (TAC* tac1,char* nombre,char* mensaje);
int descifraapantalla (TAC* tac1,char* mensaje);
int descifraarchivo (TAC* tac1,char* nombre,char* mensaje);

/* 888888888888888 */
/* Código del Main */
/* 888888888888888 */

int main(int argc,char ** argv){

TAC tac1;
int error_a,error_c,e_archconf;
char mensaje[MAXBUFFER];
char tipoAcceso;

/*Incializa el control de errores*/
error_a=0;
error_c=0;

printf ("Bienvenido al Cifrador/Descifrador.\n");
/*Valida los caracteres de la ruta y el nombre del archivo de configuración*/
e_archconf = Validar_archconf (argv[1]);
if (e_archconf!=1 && e_archconf!=2){


switch (argv[2][1]) {/*Si el usuario indica -c cifra, si pasa -d descifra, sino devuelve error de sintaxis*/
       case 'c': {
            tipoAcceso = 'w';
            error_a= TAC_Abrir(&tac1,argv[3], tipoAcceso, argv[1]);
            /*Si TAC_Abrir devuelve un error, reporta el mismo por pantalla*/
            Error_Abrir (error_a);
            switch (argc) {

                   /*Recibe la cadena por pantalla, la cifra y la guarda en un archivo*/
                   case 4:{

                        if (error_a==0){
                           cifrapantalla (&tac1,mensaje);
						   }
						if ( (error_a==0) && (error_c==0)  ) {
                           printf("Se cifraron y se guardaron correctamente las líneas.\n");
                           }
                        break;
                        }

                   /*Obtiene las cadenas de un archivo de entrada, las cifra y las guarda en otro archivo*/
                   case 5:{

                        printf ("----------------------------------\n");
						if (error_a==0){
                           cifraarchivo (&tac1,argv[4],mensaje);
                           printf ("----------------------------------\n");
						   }
                        break;
                        }

                   default: {Error_sintaxis ();

                            break;
                            }

                   }
         error_c = TAC_Cerrar (&tac1);
         Error_Cerrar (error_c);
         break;
            }
       case 'd': {
            tipoAcceso = 'r';
            error_a=TAC_Abrir(&tac1,argv[3], tipoAcceso, argv[1]);
            /*Si TAC_Abrir devuelve un error, reporta el mismo por pantalla*/
            Error_Abrir (error_a);
            switch (argc) {
                   /*Abre un archivo cifrado, lo descifra y lo muestra por pantalla*/
                   case 4: {
                        printf ("Archivo a descifrar: %s\n",argv[3]);
                        descifraapantalla (&tac1,mensaje);
                        break;
                        }

                   /*Abre un archivo cifrado, lo descifra y lo guarda en otro archivo*/
                   case 5: {
                        descifraarchivo (&tac1,argv[4],mensaje);
                        break;
                        }

                   default: {Error_sintaxis ();
                            break;}
                   }

            error_c = TAC_Cerrar (&tac1);
            Error_Cerrar (error_c);
            break;
            }

       default: {Error_sintaxis ();

                break;}

       }
}
getchar();
return 0;
}



/* 888888888888888888888888888888888888888 */
/* Código de las funciones que usa el Main */
/* 888888888888888888888888888888888888888 */

/*Reporta por pantalla los errores al abrir el archivo de configuración*/
int Error_Abrir (int error){
if (error == -1) {printf("Error: no se pudo abrir el archivo de configuración.\n");}
if (error == -2) {printf("Error: no se pudo abrir el abrir el archivo cifrado.\n");}
if (error == -3) {printf("Error: archivo de configuración inválido.\n");}
return 0;
}

/*Reporta por pantalla los errores devueltos por TAC_Escribir*/
int Error_Escribir (int error){
if (error == -1) printf("Error: caracteres no válidos en el mensaje.\n");
if (error == -2) printf("Error: desconocido.\n");
return 0;
}

/*Reporta por pantalla los errores devueltos por TAC_Leer*/
int Resultado_Leer (int error){
if (error == -1) {
   		  printf ("----------------------------------\n");
   		  printf ("Se alcanzó el fin de archivo.\n");
		  }
if (error == -2) printf("Error desconocido.\n");
return 0;
}

/*Reporta por pantalla los errores devueltos pro TAC_Cerrar*/
int Error_Cerrar (int error){
if (error == -1) printf("Error: No se pudo cerrar el TAC.\n");
return 0;
}

/*Cuando la sintaxis no cumple los requisitos básicos (cantidad de parámetros y 3er parámetro igual a -c o -d, reporta el error.*/
void Error_sintaxis () {
printf("Error de sintaxis. La sintaxis correcta es: <config> -c/-d <cifrado> (<descifrado>).\n");
}

/*Reemplaza el \n final por \0*/
void limpiarcadena (char cadena[]){
  char *p;
  p = strchr (cadena, '\n');
  if (p)
    *p = '\0';
}

/*Valida que el archivo de configuración no tenga un nombre con caracteres no válidos*/
int Validar_archconf (char nombre[]){

    int v1,v2;
    int errorinv;
    char invalidos[10] = {'#','%','&','*','|','"','<','>','?',' '};
    errorinv=0;
    printf("Archivo de configuración: %s\n",nombre);
    if (nombre != NULL) {
       for (v1=0;nombre[v1]!='\0';v1++){
           for (v2=0;v2<11;v2++){
               if (nombre[v1]==invalidos[v2]) errorinv=1;
           }
       }
    }
    else errorinv=2;
    if (errorinv==1) printf("Nombre de archivo de configuración incorrecto.\nRevise el mismo y ejecute nuevamente el programa.\n");
    if (errorinv==2) printf("Error de sintaxis. La sintaxis correcta es: <config> -c/-d <cifrado> (<descifrado>).\n");
    return errorinv;
}

/*Cifra líneas ingresadas por consola*/
int cifrapantalla (TAC* tac1,char* mensaje){

    int e1;

    e1=0;
    printf ("----------------------------------\n");

    printf ("Ingrese las líneas a cifrar (para finalizar, ingrese una línea en blanco):\n");
    gets (mensaje);
    while (strlen (mensaje) != 0)
          {
          e1 = TAC_Escribir (tac1, mensaje);
          Error_Escribir (e1);
          gets (mensaje);
          }

    printf ("----------------------------------\n");

return 0;
}

/*Cifra cadenas obtenidas de un archivo de texto*/
int cifraarchivo (TAC* tac1,char* nombre,char* mensaje){
    FILE* archivodescifrado;
    int e1,e2;

    e1=0;
    archivodescifrado = fopen(nombre, "r");
    if (archivodescifrado == NULL){
       e1=1;
       printf("Error: Imposible abrir el archivo a descifrar.");
       }
    if (e1==0){
       while (fgets(mensaje, MAXBUFFER, archivodescifrado) != NULL){
             limpiarcadena (mensaje);
             e2 = TAC_Escribir (tac1, mensaje);
             Error_Escribir (e2);
             if (e2==0) {printf ("Se leyó y se cifró la siguiente línea: %s\n", mensaje);}
             }
       if (e1==0) fclose (archivodescifrado);
       }
 /*00000 ACÁ HAY QUE PONER UN VALOR DE RETORNO DE LA FUNCIÓN !!!  0000*/
}

/*Descifra el archivo cifrado y devuelve el resutlado por pantalla*/
int descifraapantalla (TAC* tac1,char* mensaje){

int e;

e=0;
    printf ("----------------------------------\n");
	printf ("Resultado:\n");
    while (e==0){
          e= TAC_Leer (tac1, mensaje);
          Resultado_Leer (e);
          if (e==0) printf ("%s\n", mensaje);
          }
    printf ("----------------------------------\n");
    return 0;
}

/*Descifra el archivo cifrado y lo guarda en un archivo de salida*/
int descifraarchivo (TAC* tac1,char* nombre,char* mensaje){

FILE* archivodescifrado;
int e1,e2;

e1=0;
e2=0;

    printf ("hola,%s",nombre);
    archivodescifrado = fopen(nombre, "w");/*Crea el archivo de salida*/

    if (archivodescifrado == NULL){
       e1=1;
       printf("Error: Imposible crear el archivo de salida");
       }
    if (e1==0 && e2==0){

    e1 = TAC_Leer (tac1, mensaje);
    while (e1==0){/*Si e es igual a 0, hay más líneas para descifrar*/
          Resultado_Leer (e1);
          fputs(mensaje, archivodescifrado);/*Escribe la línea descifrada en el archivo de salida*/
          fputs("\n", archivodescifrado);
          e1 = TAC_Leer (tac1, mensaje);
          }
    if (e2==0) fclose (archivodescifrado);/*Cierra el archivo con la línea*/

}
return 0;
}
