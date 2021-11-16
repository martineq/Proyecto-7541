#include "TDA_TEJECUTOR_T_F.h"
#include "filtros.h"

/*--------------------------Prototipos------------------------*/
int validar_filtro(const char* palabra, const char* line,TFilter* Filter);
int Crear_lista_filtros(TListaSimple* pListaF, FILE* Fconf);
int validarsintaxis (int argc, char** argv);

int main(int argc,char** argv){
	TListaSimple Filtros;
	TEjecutor_T_F Ejecutor;
	FILE* Entrada;
	FILE* Salida;
	FILE* Config;
	int error;
	int ESintaxis, EApertura;

	   ESintaxis=0;
	   EApertura=0;

	   ESintaxis = validarsintaxis (argc,argv);
	   if (ESintaxis) return 1;
	   Entrada = fopen (argv[1],"r");
	   if (Entrada == NULL){
	      printf ("Error, no se pudo abrir el archivo de entrada\n.");
	      return 2;
	      }
	   Salida= fopen (argv[2],"w");
	   if (Salida == NULL){
	      printf ("Error, no se pudo crear el archivo de salida\n.");
	      return 3;
	      }
	   Config = fopen (argv[3],"r");
	   if (Config == NULL){
	      printf ("Error, no se pudo abrir el archivo de definicion\n.");
	      return 4;
	      }

	   error=Crear_lista_filtros(&Filtros,Config);
	   if (!error){
		   PR_Crear(&Ejecutor,Entrada,Filtros,Salida);

		   PR_Ejecutar(&Ejecutor);

		   PR_Destruir(&Ejecutor);
	   }
	   else {
		   printf("Error, Archivo de configuracion invalido...");
	   }
	   fclose(Config);
	   fclose(Entrada);
	   fclose(Salida);
	   return 0;
}



/*FUNCIONES*/

/*Función: Crear_Lista_Filtros
  Funcionamiento: Crea una lista de registros de tipo "TFilter", completando los campos de cada registro   con el nombre de la funcion (filtro) a invocar y los parámetros correspondientes; luego le pasa la   lista al ejecutor.
  Creado por: Leandro R. Bueno*/
int Crear_lista_filtros(TListaSimple* pListaF, FILE* Fconf){
	char linea[MAXLINEA];/*guarda la linea actual*/
	char palactu[MAXWORD];/*guarda los caracteres de la palabra que se va formando*/
	int  i;/*subindice de la linea actual*/
	TFilter Filtro;/*Estructura para guardar los filtros con sus parametros*/
	int error;

	ls_Crear(pListaF, sizeof(TFilter));

	memset(linea,'\0',MAXLINEA);
	memset(palactu,'\0',MAXWORD);

	while (fgets(linea, MAXLINEA, Fconf) != NULL){/*mientras no alcance el fin de archivo*/
		i = 0;
		memset(palactu,'\0',MAXWORD);
		while((linea[i]!='\0')&&(linea[i]!=' ')&&(linea[i]!='\n')){/*extraigo el nombre del filtro*/
			palactu[i] = linea[i];
			i++;
		}/*end while EOL*/
		error=validar_filtro(palactu, linea, &Filtro);
		if (error) return -1;

		if(!(ls_Insertar(pListaF,LS_SIGUIENTE,&Filtro)))return -1;/*agrego un filtro con sus parametros a la lista*/
		memset(palactu,'\0',MAXWORD);
	}/*end while EOF*/
	return 0;
}/*fin funcion*/



/*Función: validar_filtro
  Funcionamiento: Valida los parametros del archivo de configuración y carga un registro de tipo TFilter   con el nombre y los datos de un filtro
  Creado por: Leandro R. Bueno*/
int validar_filtro(const char* palabra, char* line,TFilter* Filter){
	int k = 0;
	char parametro[MAXLINEA];
		for(k=0; k < MAXLINEA; k++) parametro[k]='\0';

		if(!(strcmp(palabra,"MAYUSCULAS"))||!(strcmp(palabra,"MINUSCULAS"))||!(strcmp(palabra,"ELIMINAR_CONSEC"))||!(strcmp(palabra,"CONTAR_CONSEC"))){
			if(!(strcmp(palabra,"MAYUSCULAS")))Filter->fnProceso = MAYUSCULAS;
			if(!(strcmp(palabra,"MINUSCULAS")))Filter->fnProceso = MINUSCULAS;
			if(!(strcmp(palabra,"ELIMINAR_CONSEC")))Filter->fnProceso = ELIMINAR_CONSEC;
			if(!(strcmp(palabra,"CONTAR_CONSEC")))Filter->fnProceso = CONTAR_CONSEC;
			Filter->datos_filtro = NULL;
			return 0;
		}
		if(!(strcmp(palabra,"ORDENAR"))){
			for (k = (strlen(palabra)+ 1);(line[k]!='\0' && line[k]!='\n'); k++){/*obtencion del parametro*/
				parametro[k - (strlen(palabra)+1)] = line[k];
			}
			if(!(strcmp(parametro,"ASC"))||!(strcmp(parametro,"DESC"))){
				Filter->fnProceso = ORDENAR;
				Filter->datos_filtro = malloc(strlen(parametro)+1);
				strcpy( (char*)Filter->datos_filtro, parametro);
				return 0;
			}
			return -1;/*error por parametro incorrecto*/
		}
		if(!(strcmp(palabra,"PRIMEROS"))||!(strcmp(palabra,"ULTIMOS"))||!(strcmp(palabra,"EXTRAER"))){
				int parametro_num = 0;
				for (k = (strlen(palabra)+ 1);(line[k]!='\0' && line[k]!='\n'); k++){/*obtencion del parametro*/
					parametro[k - (strlen(palabra)+1)] = line[k];
				}
				if (!(parametro_num = atoi(parametro))) return -1;/*convierte el string a integer, si no puede devuelve error*/
				if(parametro_num < 1)return -1;/*el parametro debe ser mayor que 1*/

				Filter->datos_filtro =(void*) parametro_num;

				if(!(strcmp(palabra,"PRIMEROS")))Filter->fnProceso = PRIMEROS;
				if(!(strcmp(palabra,"ULTIMOS")))Filter->fnProceso = ULTIMOS;
				if(!(strcmp(palabra,"EXTRAER")))Filter->fnProceso = EXTRAER;
				return 0;
		}
		if(!(strcmp(palabra,"FILTRAR"))){
			TListaSimple *pLista;
			char *paux;

			line[strlen(line)-1] = '\0';

			pLista = (TListaSimple*)malloc(sizeof(TListaSimple));
			ls_Crear(pLista,MAXWORD*sizeof(char));

			paux = strchr(line, ' ')+1;
			paux = strtok(paux, ",");
			while( paux) {
				if(!(ls_Insertar(pLista,LS_SIGUIENTE,paux)))return -1;
				paux = strtok(NULL, ",");
			}

			Filter->fnProceso = FILTRAR;
			Filter->datos_filtro = pLista;

			return 0;
		}/*end if*/
		return -1;/*error por nombre del filtro incorrecto*/
} /*fin funcion*/


/*Función que corrobora que se reciben de consola 4 comandos, el ejecutable y los 3 nombres de archivos*/
int validarsintaxis (int argc, char** argv){
   if (argc!=4){
      printf ("Sintaxis incorrecta, la ejecución debe respetar:\n\tejecutar_t_f <entrada> <salida> <definicion>\n");
      return 1;
   }
   return 0;
}
