#include "TDA_D_T.h"

/*Prototipos de las funciones internas a las primitivas*/

int Compara(void* a,void* b);
int CompXNumero(TLlamada* a,TLlamada* b);
int Parser_Configuracion(TCola* pLS, char* RutaConfig);

int Parser_llamadas(TABO* ArbolLlamadas, FILE* ArchivoDetalleTelefonico);
void Procesar_Llamada(TLlamada* pL, char* valor, int nProc);
void Formato_Fecha_Hora(char* valor);

int Parser_telefonos(TABO* abn, FILE* art,char* name);
int Concatena(TListaSimple p, char* c);
int Carga(TListaSimple p,char* c);
int GuardarElemEnArbol(TABO* abn,TListaSimple* l,char* name);
void Pegar(char* p,char* c);
int Comparar(void* a,void* b);
int Comp(TUsuario_numeros* a,TUsuario_numeros* b);

int tomarusuario (TABO Usuarios,TUsuario_numeros* pusuario_aux, int mov,TABO LLamadas,TListaSimple* Resumen);
int calculartotal (TListaSimple* Detalle, float* total);
int buscarusuario (TABO* Usuarios,TUsuario_numeros* pusuario_aux,char* usuario);
int procesarusuario (TUsuario_numeros* pusuario_aux,TABO* LLamadas,TListaSimple* Detalle,TABO* Usuarios);
int buscarllamadas (char numero_aux[TELEFONO], TABO LLamadas, TListaSimple* Detalle, int MOV,TABO Usuarios,char* usuario_aux);
int procesarcosto (char* usuario_aux, TABO Usuarios, TListaSimple* Detalle, int MOV, int* divisor);
int calcularcosto (TListaSimple* Detalle, int divisor);

void ProcesarDeudaGeneral(TGastosG* GG, char* valor, int nProc);

void Rec_Recursivo_InOrden(TABO* AU,char* Tel,TListaSimple* LUsrTel);
void Procesar_Usuario(TUsuario_numeros EAU, char* Tel,TListaSimple** LUsrTel);

int LlenarLlamadas(TABO* a,TListaSimple* l);
void LlenarLlam(TABO a,TListaSimple* l,int mov);
void LlenarLista(TListaSimple* dest,TListaSimple* orig);
int LlenarNoAsignados(TABO* a,TListaSimple Ls_lla,TListaSimple* l,int ord);
void LlenarNoAsig(TABO a,TListaSimple* l,TListaSimple Ls_lla,int mov,int ord);
void LlenarNA(TListaSimple* l,TLlamada* e,TListaSimple LsNum,int ord);
int NoEstaEnLista(char n[MAXLINEA],TListaSimple l);
int LSInsertarTLlamadaOrdenadoFecha(TListaSimple * l,TLlamada* e);  /* Esta función es compartida por las dos primitivas*/
int LSInsertarCharOrdenado(TListaSimple * l,char* c);


int LlenarTelPorArea(TABO* a,char* area,TListaSimple* l,int o);
void LlenarTelArea(TABO a,char* area,TListaSimple* l,int mov,int o);
int telcmp (char* num,char* area);
void LlenarTA(TListaSimple* l,TLlamada* e,int o);
void MostrarTelPorArea(TListaSimple* l,int o);

/********************************************************************************/

/*PRIMITIVA CREAR*/
int TDT_Crear(char* ruta, TDA_D_T* Arboles, char RutaLlamadas[MAXWORD]){
	TCola ColaRutasArchivoUsuarios;/*cola con rutas de archivos y nombres de usuarios*/
	FILE* ArchAux;/*variable auxiliar para abrir y cerrar archivos*/
	tRutaArchivoUsuario RegAux;/*registro auxiliar para levantar elementos de la cola*/
	char* Path;/*variable para levantar la ruta del registro auxiliar*/
	char* NombreUsuario;/*variable para levantar el nombre del usuario del registro auxiliar*/


	NombreUsuario = malloc (sizeof(char)*MAXWORD);
	if(!NombreUsuario)return -1;
	Path = malloc (sizeof(char)*MAXWORD);
	if(!Path)return -1;

	C_Crear(&ColaRutasArchivoUsuarios, sizeof(tRutaArchivoUsuario));
	if(Parser_Configuracion(&ColaRutasArchivoUsuarios, ruta))return -1;/*mete rutas de archivos y nombres de usuarios en una cola*/

	/*ABRO EL ARCHIVO DE LLAMADOS, CREO EL ARBOL DE LLAMADOS E INSERTO TODOS LOS NODOS*/
	(*Arboles).Arbol_llamadas=(TABO*) malloc(sizeof(TABO));
	ABO_Crear(((*Arboles).Arbol_llamadas),sizeof(TLlamada),(*Compara));

	C_Sacar(&ColaRutasArchivoUsuarios,&RegAux);
	strcpy(Path,RegAux.Ruta_Archivo);
	ArchAux = fopen(Path, "r");
	if(!ArchAux)return -1;


	strcpy(RutaLlamadas,RegAux.Ruta_Archivo);

	Parser_llamadas(Arboles->Arbol_llamadas, ArchAux);
	fclose(ArchAux);

	/*ABRO LOS ARCHIVOS DE NUMEROS, CREO EL ARBOL DE USUARIO E INSERTO TODOS LOS NODOS*/
	(*Arboles).Arbol_usuarios_numeros=(TABO*) malloc(sizeof(TABO));
	ABO_Crear((*Arboles).Arbol_usuarios_numeros,sizeof(TUsuario_numeros),(*Comparar));

	while(!(C_Vacia(ColaRutasArchivoUsuarios))){
		C_Sacar(&ColaRutasArchivoUsuarios,&RegAux);
		strcpy(NombreUsuario,RegAux.Nombre_Usuario);
		strcpy(Path,RegAux.Ruta_Archivo);
		ArchAux = fopen(Path, "r");
		if(!ArchAux)return -1;
        Parser_telefonos((*Arboles).Arbol_usuarios_numeros, ArchAux, NombreUsuario);
		fclose(ArchAux);
	}

	free(NombreUsuario);
	free(Path);
	return 0;
}/*fin primitiva*/

/***********************************************************************************/

int Parser_Configuracion(TCola* pC, char* RutaConfig){
	char linea[MAXLINEA];/*guarda la linea actual*/
	char palactu[MAXWORD];/*guarda la palabra actual*/
	int  i,j;
	FILE* Archivo;/*archivo de configuracion*/
	tRutaArchivoUsuario Registro;

	Archivo = fopen(RutaConfig, "r");
	if(!Archivo){
		fprintf(stderr, "Ruta del Archivo de Configuracion invalido.\n");
		return -1;
	}

	/*ruta facturacion*/
	i = 0;
	memset(palactu,'\0',MAXWORD);
	memset(linea,'\0',MAXLINEA);
	fgets(linea, MAXLINEA, Archivo);
	while(linea[i]!='\0'){
		palactu[i] = linea[i];
		i++;
	}
	if(palactu[i-1]=='\n')palactu[i-1]= '\0';
	memset(Registro.Nombre_Usuario,'\0',MAXWORD);
	memset(Registro.Ruta_Archivo,'\0',MAXWORD);
	strcpy(Registro.Ruta_Archivo, palactu);
	if(!(C_Agregar(pC, &Registro)))return -1;

	/*ruta usuarios*/
	memset(linea,'\0',MAXLINEA);
	while (fgets(linea, MAXLINEA, Archivo) != NULL){
		memset(palactu,'\0',MAXWORD);
		i = j = 0;

		/*nombre*/
		while(linea[i]!='\t'){
			palactu[i] = linea[i];
			i++;
		}
		i++;
		memset(Registro.Nombre_Usuario,'\0',MAXWORD);
		strcpy(Registro.Nombre_Usuario, palactu);
		memset(palactu,'\0',MAXWORD);

		/*ruta*/
		while(linea[i]!='\0'){
			palactu[j] = linea[i];
			i++;j++;
		}
		if(palactu[j-1]=='\n')palactu[j-1]= '\0';
		memset(Registro.Ruta_Archivo,'\0',MAXWORD);
		strcpy(Registro.Ruta_Archivo, palactu);
		if(!(C_Agregar(pC, &Registro)))return -1;
		memset(linea,'\0',MAXLINEA);
	}
	fclose(Archivo);
	return 0;
}/*fin funcion*/


int Parser_llamadas(TABO* ArbolLlamadas, FILE* ArchivoDetalleTelefonico){
	char linea[MAXLINEA];/*guarda la linea actual*/
	char palactu[MAXWORD];/*guarda la palabra actual*/
	int  i;/*subindice de la linea actual*/
	int posactu;/*subindice de la palabra actual*/
	int  j;/*subindice de la palabra actual*/
	int flag;/*corte de control*/
	TLlamada llamada;/*registros con datos de una llamada*/
	int error;

	/*IGNORA LOS GASTOS GENERALES*/
	flag = 0;
	while(!flag){
		memset(linea,'\0',MAXLINEA);
		fgets(linea, MAXLINEA, ArchivoDetalleTelefonico);
		if(strcmp(linea, "**Detalle**\n")==0)flag = 1;
	}
	/*PROCESA LAS LLAMADAS*/
	posactu = 0;
	memset(linea,'\0',MAXWORD);
	memset(palactu,'\0',MAXWORD);
	while (fgets(linea, MAXLINEA, ArchivoDetalleTelefonico) != NULL){
		i = j = 0;
		while(linea[i]!='\0'){/*mientras no se termine la linea*/
			if(linea[i]!='\t'){/*si es una letra permitida*/
				palactu[posactu] = linea[i];
				posactu++;
			}
			else{/*si no es una letra permitida, guardo la palabra*/
				Procesar_Llamada(&llamada, palactu, j);
				posactu = 0;
				memset(palactu,'\0',MAXWORD);
				j++;
			}/*end if - else*/
			i++;
		}/*end while EOL*/
		/*antes de pasar a la otra linea guardo la palabra actual e inserto en el arbol*/
		Procesar_Llamada(&llamada, palactu, j);
		ABO_Insertar(ArbolLlamadas,&llamada,&error);

		posactu = 0;
		memset(palactu,'\0',MAXWORD);
		memset(linea,'\0',MAXWORD);
	}
	return 0;
}/*fin funcion*/


void Procesar_Llamada(TLlamada* pL, char valor[MAXWORD], int nProc){
	float valdec;
	char* remp;

	switch(nProc){
		case 0:
			strncpy(pL->numero,valor,TELEFONO);
			break;
		case 1:{
				Formato_Fecha_Hora(valor);
				strncpy(pL->fecha,valor,FECHA_HORA);
				break;
		}
		case 2:
			remp = strchr(valor,':');
			*remp = '.';
			valdec = atof(valor);
			pL->duracion= valdec;
			break;
		case 3:
			valdec = atof(valor);
			pL->importe= valdec;
			break;
	}
}/*fin funcion*/


void Formato_Fecha_Hora(char* valor){
	int i = 0;
	char anio[4];
	char mes [2];
	char dia [2];
	char hora[5];
	memset(anio,'\0',4);
	memset(mes,'\0',2);
	memset(dia,'\0',2);
	memset(dia,'\0',2);
	memset(hora,'\0',5);
	for(i = 0; i < strlen(valor)+1; i++){
		if(i > -1 && i < 2)dia[i]= valor[i];
		if(i > 2 && i < 5)mes[i-3]= valor[i];
		if(i > 5 && i < 10)anio[i-6]= valor[i];
		if(i > 10 && i < 16)hora[i-11]= valor[i];
	}
	memset(valor,'\0',FECHA_HORA);
	strncat(valor, anio, 4);
	valor[4]= '/';
	strncat(valor, mes, 2);
	valor[7]= '/';
	strncat(valor, dia, 2);
	valor[10]= ' ';
	strncat(valor, hora, 5);
}/*fin funcion*/


/*Funcion de comparacion.
 * Invoca a la funcion que compara x numeros para ordenar el arbol de llamadas
 * por este campo.
 * Autor:Guido Nahuel Ygounet*/
int Compara(void* a,void* b){
	return CompXNumero((TLlamada*)a,(TLlamada*)b);
}


/*Funcion de comparacion Por Numeros.
 * Devuelve 1 si la primer clave es menor que la segunda,-1 si es mayor y 0
 * si son iguales.
 * Autor:Guido Nahuel Ygounet*/
int CompXNumero(TLlamada* a,TLlamada* b){
	if(strcmp((a->numero),(b->numero))<0) return -1;
	if(strcmp((a->numero),(b->numero))>0) return 1;
	/*return 0;Son =*/
	else{
		if(strcmp((a->fecha),(b->fecha))<0) return -1;
		else{
			if(strcmp((a->fecha),(b->fecha))>0) return 1;
		}
	}
	return 0;/*Si llega aca ambas claves son =*/
}



int Parser_telefonos(TABO* abn, FILE* art, char* name){
  char lin[TELEFONO];
  int c_lin=0;
  int largo;
  TListaSimple pre;
  TListaSimple* l;
  memset(lin,'\0',TELEFONO);

  ls_Crear(&pre,sizeof(char)*TELEFONO);  /* Lista de prefijos  */
  l=(TListaSimple*)malloc(sizeof(TListaSimple));
  ls_Crear(l,sizeof(char)*TELEFONO);    /* Lista de telefonos */

  while (fgets(lin,TELEFONO,art) != NULL){
	if((lin[0]!= '\0')&&(lin[0]!='\n')){
		if(lin[(strlen(lin))-1]== '\n')
			lin[(strlen(lin))-1]= '\0';
		else
			lin[(strlen(lin))]= '\0';
		largo=strlen(lin);
		c_lin++;
		if( (largo <= MAXLONGPREFIJO) && (c_lin <= MAXCANTPREFIJO) )
			ls_Insertar(&pre,LS_SIGUIENTE,lin);
		else
			if(largo >= MINLONGNUMERO){
				if(Concatena(pre,lin)==TELEFONO-1)
					ls_Insertar(l,LS_SIGUIENTE,lin);
				else
					fprintf(stderr,"Error en el Nº: %s\nUsuario: %s\nLínea: %d\n",lin,name,c_lin);
			}
			else
				fprintf(stderr,"Error en el Nº: %s\nUsuario: %s\nLínea: %d\n",lin,name,c_lin);

		memset(lin,'\0',TELEFONO);
	}
  }
  if (!GuardarElemEnArbol(abn,l,name)) return 1; /* devuelvo error */
  return 0;
}

int Concatena(TListaSimple p, char* c){
  if (strlen(c)==(TELEFONO-1)) return TELEFONO-1;
  return Carga(p,c);
}


int Carga(TListaSimple p,char* c){
  char pre[TELEFONO];
  int ok;
  memset(pre,'\0',TELEFONO);
  if(ls_Vacia(p)) return 1;
  else{
    ok=ls_MoverCorriente(&p,LS_PRIMERO);
    while(ok){
      ls_ElemCorriente(p,pre);
      if ( (strlen(pre) + strlen(c)) == TELEFONO-1 ){
        Pegar(pre,c);
        return strlen(c);
      }
      ok=ls_MoverCorriente(&p,LS_SIGUIENTE);
    }
  return strlen(c);
  }
}

void Pegar(char* p,char* c){
  char aux[TELEFONO];
  memset(aux,'\0',TELEFONO);
  strncpy(aux,p,strlen(p));
  strncat(aux,c,strlen(c));
  strcpy(c,aux);
}

int GuardarElemEnArbol(TABO* abn,TListaSimple* l,char* name){
 int err;
 TUsuario_numeros elem;
 elem.usuario = malloc((sizeof(char))*MAXWORD);
 elem.numeros_usr = (TListaSimple*)malloc(sizeof(TListaSimple));
 strcpy(elem.usuario,name);
 elem.numeros_usr=l;
 ABO_Insertar(abn,&elem,&err);
 return (!err);
}

int Comparar(void* a,void* b){
return Comp((TUsuario_numeros*)a,(TUsuario_numeros*)b);
}

int Comp(TUsuario_numeros* a,TUsuario_numeros* b){
  if ( strcmp(a->usuario,b->usuario) < 0 ) return -1;  /* Se supone que los nombres son distintos, de otra forma no podria saber cual persona es cual */
  else return 1;
}

/***********************************************************************************/

/*PRIMITIVA DESTRUIR*/
int TDT_Destruir(TDA_D_T* Arboles){
	ABO_Vaciar((*Arboles).Arbol_llamadas);
	ABO_Vaciar((*Arboles).Arbol_usuarios_numeros);
	free((*Arboles).Arbol_llamadas);
	free((*Arboles).Arbol_usuarios_numeros);
	return 0;
}/*fin primitiva*/

/***********************************************************************************/


/***********************************************************************************/

/*Primitiva encargada de obtener la deuda de gastos generales del archivo de factura
 * cion.
 * Pre:ArchivoDetTel abierto para lectura.
 * Post:Devuelve una lista de estructuras de tipo TGastosG con el detalle y el monto
 * de cada uno de los gastos generales.
 * Autor:Guido Nahuel Ygounet*/
void TDT_ObtenerDeudaGastosGenerales(FILE* ArchivoDetTel,TListaSimple* GGrales){
	char linea[MAXLINDEUDA];/*guarda la linea actual*/
	char palactu[MAXPALDEUDA];/*guarda la palabra actual*/
	int  i;/*subindice de la linea actual*/
	int posactu;/*subindice de la palabra actual*/
	int  j;/*subindice de la palabra actual*/
	int finGastos=0;
	TGastosG GastosGrales;

	posactu = 0;
	memset(linea,'\0',MAXPALDEUDA);
	memset(palactu,'\0',MAXPALDEUDA);
	ls_Crear(GGrales,sizeof(TGastosG));

	while ((fgets(linea, MAXLINDEUDA, ArchivoDetTel)!=NULL)&&(!finGastos)){
		i=j=0;
		if(!strcmp(linea, "**Detalle**\n")){/*Si llega a la linea de separacion */
			finGastos=1;					/*modifica la bandera y sale del while*/
			break;
		}
		while(linea[i]!='\0'){/*mientras no se termine la linea*/
					if(linea[i]!='\t'){/*si es una letra permitida*/
						palactu[posactu] = linea[i];
						posactu++;
					}
					else{/*si no es una letra permitida, guardo la palabra*/
						ProcesarDeudaGeneral(&GastosGrales, palactu, j);
						posactu = 0;
						memset(palactu,'\0',MAXPALDEUDA);
						j++;
					}/*end if - else*/
					i++;
				}
		if(linea[0]!='\n'){	/*Saltea las lineas en blanco*/
			ProcesarDeudaGeneral(&GastosGrales, palactu, j);/*Guardo el importe*/
			posactu = 0;
			memset(palactu,'\0',MAXPALDEUDA);
			ls_Insertar(GGrales,LS_SIGUIENTE,&GastosGrales);
		}
	}
}

/*Procesa la deuda general, Depende el valor con el que se la invoque,
 * copia el char* como concepto de la estructura o convierte el char*
 * en un float y lo guarda en el importe de la estructura.
 * Autor:Guido Nahuel Ygounet*/
void ProcesarDeudaGeneral(TGastosG* GG, char* valor, int nProc){
	float valdec;

	switch(nProc){
		case 0:
			strncpy(GG->concepto,valor,MAXPALDEUDA);
			break;
		case 1:
			valdec = atof(valor);
			GG->importe= valdec;
			break;
	}
}

/***********************************************************************************/

/*Primitiva encargada de obtener los nombres de los usuarios de un telefono dado.
*Pre:ArbolUsr creado.
*Post:Devuelve una lista con los nombres de los usuarios que reconocen el telefono
* solicitado. OJO si no encuentra ningun usuario devuelve la lista vacia.
*Autor:Guido Nahuel Ygounet*/
void TDT_ObtenerUsuariosTelefono(TDA_D_T* TDA_Llamadas,char* Tel,TListaSimple* LUsrTel){
	int error;

	if(!ABO_Vacio(*(TDA_Llamadas->Arbol_usuarios_numeros))){
		ls_Crear(LUsrTel,sizeof(char)*MAXWORD);
		ABO_MoverCte(TDA_Llamadas->Arbol_usuarios_numeros,RAIZ,&error);
		Rec_Recursivo_InOrden(TDA_Llamadas->Arbol_usuarios_numeros,Tel,LUsrTel);
    }
}

/*Funcion que recorre recursivamente el arbol de usuarios e invoca a Procesar_Usuario.
 * Autor:Guido Nahuel Ygounet*/
void Rec_Recursivo_InOrden(TABO* AU,char* Tel,TListaSimple* LUsrTel){
	TUsuario_numeros EAU;
	int error;

	ABO_ElemCte(*AU,&EAU);
	if(ls_Vacia(*(EAU.numeros_usr))){
              fprintf(stderr,"Error, La lista de numeros del usuario%s  esta vacia\n",EAU.usuario);
              return;
              }
	ABO_MoverCte(AU,IZQ,&error);
	if(!error){
		Rec_Recursivo_InOrden(AU,Tel,LUsrTel);
		ABO_MoverCte(AU,PAD,&error);
	}
	Procesar_Usuario(EAU,Tel,&LUsrTel);
	ABO_MoverCte(AU,DER,&error);
	if(!error){
		Rec_Recursivo_InOrden(AU,Tel,LUsrTel);
		ABO_MoverCte(AU,PAD,&error);
	}
}

/*Funcion encargada de procesar los datos del usuario recibido; si el mismo
 * reconoce el numero solicitado agrega su nombre en la lista, sino lo ignora.
 * Autor:Guido Nahuel Ygounet*/
void Procesar_Usuario(TUsuario_numeros EAU, char* Tel,TListaSimple** LUsrTel){
	int error;
	char NumUsr[MAXLINEA];

    memset(NumUsr,'\0',MAXLINEA);
	error=ls_MoverCorriente(EAU.numeros_usr,LS_PRIMERO);
	while(error){
		ls_ElemCorriente(*(EAU.numeros_usr),NumUsr);
		if(!strncmp(NumUsr,Tel,TELEFONO-1)){/*Compara sin el \0*/
			ls_Insertar(*LUsrTel,LS_SIGUIENTE,EAU.usuario);
			return;
		}
		else
			error=ls_MoverCorriente(EAU.numeros_usr,LS_SIGUIENTE);
	}
}

/***********************************************************************************/

/* Para ord: si es ==0 lo ordeno por número sino, lo ordeno por fecha*/
int TDT_ObtenerDetalleNoAsignada(TDA_D_T* p,TListaSimple* noasig,int ord){
  TListaSimple llamadas;

  ls_Crear(&llamadas,sizeof(char[MAXLINEA]));       /* Creo una lista de llamadas a teléfonos (sacada de usr)  */
  if (!LlenarLlamadas(p->Arbol_usuarios_numeros,&llamadas)) return 1;

  if (!LlenarNoAsignados(p->Arbol_llamadas,llamadas,noasig,ord)) return 1;
  return 0;
}


int LlenarLlamadas(TABO* a,TListaSimple* l){
  if (ABO_Vacio(*a)) return 0;
  LlenarLlam(*a,l,RAIZ);
  return 1;
}

void LlenarLlam(TABO a,TListaSimple* l,int mov){
  int err;
  TUsuario_numeros* elem;
  elem=(TUsuario_numeros*)malloc(sizeof(TUsuario_numeros));
  ABO_MoverCte(&a,mov,&err);
  if (!err){
   LlenarLlam(a,l,IZQ);
   ABO_ElemCte(a,elem);
   LlenarLista(l,elem->numeros_usr);
   LlenarLlam(a,l,DER);
  }
}

void LlenarLista(TListaSimple* dest,TListaSimple* orig){
  char aux[MAXLINEA];
  memset(aux,'\0',MAXLINEA);
  if (!ls_Vacia(*orig)){
    ls_MoverCorriente(orig,LS_PRIMERO);
    ls_ElemCorriente(*orig,aux);
    LSInsertarCharOrdenado(dest,aux);
    while ( ls_MoverCorriente(orig,LS_SIGUIENTE)){
      ls_ElemCorriente(*orig,aux);
      LSInsertarCharOrdenado(dest,aux);
    }
  }
}


int LlenarNoAsignados(TABO* a,TListaSimple Ls_lla,TListaSimple* l,int ord){
  if (ABO_Vacio(*a)) return 0;
  LlenarNoAsig(*a,l,Ls_lla,RAIZ,ord);
  return 1;
}


void LlenarNoAsig(TABO a,TListaSimple* l,TListaSimple Ls_lla,int mov,int ord){
  int err;
  TLlamada* elem;
  elem=(TLlamada*)malloc(sizeof(TLlamada));

  ABO_MoverCte(&a,mov,&err);
  if (!err){
   LlenarNoAsig(a,l,Ls_lla,IZQ,ord);
   ABO_ElemCte(a,elem);
   LlenarNA(l,elem,Ls_lla,ord);
   LlenarNoAsig(a,l,Ls_lla,DER,ord);
  }
}


void LlenarNA(TListaSimple* l,TLlamada* e,TListaSimple LsNum,int ord){
   if (NoEstaEnLista((*e).numero,LsNum)){
     if (ord==0) ls_Insertar(l,LS_SIGUIENTE,e);
     else LSInsertarTLlamadaOrdenadoFecha(l,e);
   }
}


int NoEstaEnLista(char n[MAXLINEA],TListaSimple l){
  char aux[MAXLINEA];
  int nota;

  int ok=1;

  memset(aux,'\0',MAXLINEA);

  if( (ls_Vacia(l)) ) return 1;

  else{
    
    ls_ElemCorriente(l,aux);

    if((nota=strcmp(n,aux))==0) return 0;
    if (nota<0) {ok=ls_MoverCorriente(&l,LS_PRIMERO);
    ls_ElemCorriente(l,aux);}
    while ( (ok) && ((nota=strcmp(n,aux))>0) ){
      ls_ElemCorriente(l,aux);
      if ((nota=strcmp(n,aux))>0) ok=ls_MoverCorriente(&l,LS_SIGUIENTE);
      
    }
    if (nota==0) return 0;
    else return 1;

  }
}

int LSInsertarTLlamadaOrdenadoFecha(TListaSimple * l,TLlamada* e){
  TLlamada aux;
  int ok=1;
  if(ls_Vacia(*l)) return ls_Insertar(l,LS_PRIMERO,e);
  else{
    ls_ElemCorriente(*l,&aux);
    if(strcmp((*e).fecha,aux.fecha)<0) ok=ls_MoverCorriente(l,LS_PRIMERO);
    ls_ElemCorriente(*l,&aux);
    while ( (ok) && (strcmp((*e).fecha,aux.fecha)>0) ){
      ls_ElemCorriente(*l,&aux);

      ok=ls_MoverCorriente(l,LS_SIGUIENTE);
    }
    if (ok) return ls_Insertar(l,LS_ANTERIOR,e);  /*Aquí el movimiento "LS_ANTERIOR" está usado con la primitiva "ls_insertar", lo cual es válido; no así si usara este mov. en la primitiva "ls_MoverCorriente" */
    else return ls_Insertar(l,LS_SIGUIENTE,e);
  }
}



int LSInsertarCharOrdenado(TListaSimple * l,char* c){
  char aux[MAXLINEA];
  int ok=1;

  memset(aux,'\0',MAXLINEA);
  if(ls_Vacia(*l)) return ls_Insertar(l,LS_PRIMERO,c);
  else{
    ls_ElemCorriente(*l,aux);
    if(strcmp(c,aux)==0) return 1;
    if(strcmp(c,aux)<0){ ok=ls_MoverCorriente(l,LS_PRIMERO);
    ls_ElemCorriente(*l,aux);   /* nuevo */
    if(strcmp(c,aux)==0) return 1;}
    while ( (ok) && (strcmp(c,aux)>0) ){   /* nuevo  */
      ls_ElemCorriente(*l,aux);
      if(strcmp(c,aux)==0) return 1;
      if(strcmp(c,aux)>0) ok=ls_MoverCorriente(l,LS_SIGUIENTE);
    }

    if (ok) return ls_Insertar(l,LS_ANTERIOR,c);
    else return ls_Insertar(l,LS_SIGUIENTE,c);
  }
}

/******************************************************************************/

/* Para ord: si es ==0 lo ordeno por número sino, lo ordeno por fecha        */
/* el valor char* area tiene que ser un numero cualquiera pero previamente   */
/* inicializado con memset(auxnum,'\0',MAXLINEA)                             */
int TDT_ObtenerDetalleDeTelefonosPorAreaFisica(TDA_D_T* p,char* area,TListaSimple* tel,int ord){
  if (!LlenarTelPorArea(p->Arbol_llamadas,area,tel,ord)) return 1;
  return 0;
}

int LlenarTelPorArea(TABO* a,char* area,TListaSimple* l,int o){
  if (ABO_Vacio(*a)) return 0;
  LlenarTelArea(*a,area,l,RAIZ,o);
  return 1;
}

void LlenarTelArea(TABO a,char* area,TListaSimple* l,int mov,int o){
  int err,cmp;
  TLlamada* elem;
  elem=(TLlamada*)malloc(sizeof(TLlamada));

  ABO_MoverCte(&a,mov,&err);
  if (!err){
   ABO_ElemCte(a,elem);
   cmp=telcmp((*elem).numero,area);
   if (cmp>=0) LlenarTelArea(a,area,l,IZQ,o);
   if (cmp==0) LlenarTA(l,elem,o);
   if (cmp<=0) LlenarTelArea(a,area,l,DER,o);
  }
}

int telcmp (char* num,char* area){
  char auxnum[TELEFONO-1];
  char auxarea[TELEFONO-1];
  memset(auxnum,'\0',TELEFONO-1);
  memset(auxarea,'\0',TELEFONO-1);
  strncpy(auxnum,num,strlen(area));
  strcpy(auxarea,area);
  return strcmp(auxnum,auxarea);
}


void LlenarTA(TListaSimple* l,TLlamada* e,int o){
  if (o==0) ls_Insertar(l,LS_SIGUIENTE,e);  /* Inserto ordenado por numero (el árbol ya estaba ordenado por numero)*/
  else LSInsertarTLlamadaOrdenadoFecha(l,e);   /* Inserto ordenado por fecha */
}


/******************************************************************************/
/*----------------------------------------------------------------------*/
/*TDT_GenerarResumenUsuarios*/
/*Función que recibé el TDA_D_T y a partir de este genera una lista con
**los usuarios y su respectivos importes totales.
**Pedro Lynn*/

void TDT_GenerarResumenUsuarios (TDA_D_T* Arboles, TListaSimple* Resumen){
    TUsuario_numeros usuario_aux;
    int error;

    ABO_MoverCte(Arboles->Arbol_usuarios_numeros,RAIZ,&error);
    tomarusuario (*(Arboles->Arbol_usuarios_numeros), &usuario_aux, RAIZ,*(Arboles->Arbol_llamadas),Resumen);
}


int tomarusuario (TABO Usuarios,TUsuario_numeros* pusuario_aux, int mov,TABO LLamadas,TListaSimple* Resumen){

 TListaSimple Detalle;
 TResumen resumen_aux;
 int error;
 float total;

 error=0;
 ls_Crear(&Detalle, sizeof(TLlamada));
 ABO_MoverCte(&Usuarios, mov,&error);
 if (error) return 2;
 ABO_ElemCte(Usuarios,pusuario_aux);
 procesarusuario (pusuario_aux, &LLamadas, &Detalle,&Usuarios);
 calculartotal (&Detalle,&total);
 resumen_aux.importe = total;
 resumen_aux.usuario = pusuario_aux->usuario;
 ls_Insertar(Resumen,LS_SIGUIENTE,&resumen_aux);
 tomarusuario (Usuarios, pusuario_aux, IZQ,LLamadas,Resumen);
 tomarusuario (Usuarios, pusuario_aux, DER,LLamadas,Resumen);



return error;
}

int calculartotal (TListaSimple* Detalle, float* total){

    float costototal=0;
    TLlamada llamada_aux;


    if (!ls_Vacia(*Detalle)){
          ls_MoverCorriente(Detalle,LS_PRIMERO);
          ls_ElemCorriente(*Detalle,&llamada_aux);
          costototal = costototal + llamada_aux.importe;
          while (ls_MoverCorriente(Detalle,LS_SIGUIENTE)){
                ls_ElemCorriente(*Detalle,&llamada_aux);
                costototal = costototal + llamada_aux.importe;
                }
          (*total)=costototal;
          }
    else *total=0;
return 0;
}

/*----------------------------------------------------------------------*/
/*TDT_OBTENERDETALLEUSUARIO*/
/*Función que recibe el nombre de un usuario y devuelve una lista con
**todas las llamadas realizadas por ese usuario.
**Pedro Lynn*/


void TDT_ObtenerDetalleUsuario (char* usuario, TDA_D_T* Arboles, TListaSimple* Detalle){

 TUsuario_numeros usuario_aux;
 int error;


 error=0;

 ABO_MoverCte(Arboles->Arbol_usuarios_numeros,RAIZ,&error);
 error = buscarusuario (Arboles->Arbol_usuarios_numeros, &usuario_aux, usuario);
 ABO_MoverCte(Arboles->Arbol_llamadas,RAIZ,&error);
 procesarusuario (&usuario_aux, Arboles->Arbol_llamadas, Detalle,Arboles->Arbol_usuarios_numeros);

 return;
}

/* --------------*/
/*Busca el usuario del cual se pidió el detalle*/

int buscarusuario (TABO* Usuarios,TUsuario_numeros* pusuario_aux,char* usuario){

 int encontrado,comparacion,error;

 char numero_aux[TELEFONO];
 encontrado = 0;
 comparacion=0;
 error=0;

 ABO_ElemCte(*Usuarios,pusuario_aux);
 comparacion = strcmp (usuario,pusuario_aux->usuario);
 ls_MoverCorriente(pusuario_aux->numeros_usr,LS_PRIMERO);
 ls_ElemCorriente(*(pusuario_aux->numeros_usr), &numero_aux);

  if ((comparacion < 0) && (!encontrado)){
     ABO_MoverCte(Usuarios,IZQ,&error);
     if (error) return 2;
     error = buscarusuario (Usuarios,pusuario_aux,usuario);
     }
  if ((comparacion > 0) && (!encontrado)){
     ABO_MoverCte(Usuarios,DER,&error);
     if (error) return 2;
     error = buscarusuario (Usuarios,pusuario_aux,usuario);
     }
  if (comparacion == 0) return 0;

return 0;
}

/* Procesa la factura del usuario*/

int procesarusuario (TUsuario_numeros* pusuario_aux,TABO* LLamadas,TListaSimple* Detalle,TABO* Usuarios){

    char numero_aux[MAXLINEA];


/*Inicio el recorrdio por la lista de numeros del usuario*/
    ls_Vaciar(Detalle);
    ls_MoverCorriente(pusuario_aux->numeros_usr,LS_PRIMERO);
    ls_ElemCorriente(*(pusuario_aux->numeros_usr), &numero_aux);
    /*Busco llamadas hechas a ese número*/
    buscarllamadas (numero_aux, *LLamadas, Detalle, RAIZ, *Usuarios,pusuario_aux->usuario);
    /*Continúo el recorrido de la lista de números*/
    while ((ls_MoverCorriente(pusuario_aux->numeros_usr,LS_SIGUIENTE))){
          ls_ElemCorriente(*(pusuario_aux->numeros_usr), &numero_aux);
          buscarllamadas (numero_aux, *LLamadas, Detalle, RAIZ, *Usuarios,pusuario_aux->usuario);
          }
return 0;
}

/*Busca las llamadas hechas a los números indicados en la lista del usuario*/

int buscarllamadas (char numero_aux[TELEFONO], TABO LLamadas, TListaSimple* Detalle, int MOV,TABO Usuarios,char* usuario_aux){

    int error;
    TLlamada llamada_aux;
    TLlamada* pllamada_aux;
    int comparacion;
    int encontrado;
    int divisor;


    pllamada_aux = &llamada_aux;

    encontrado = 0;
    error = 0;
    divisor=1 ;
    ABO_MoverCte(&LLamadas,MOV,&error);
    if (error) return 2;
    ABO_ElemCte(LLamadas,pllamada_aux);
    comparacion = strcmp (numero_aux,pllamada_aux->numero);


    if ((comparacion < 0)){

       buscarllamadas (numero_aux, LLamadas, Detalle, IZQ, Usuarios, usuario_aux);
     }
    if ((comparacion > 0)){

       buscarllamadas (numero_aux, LLamadas, Detalle, DER, Usuarios, usuario_aux);
     }
    if (comparacion == 0){
       ls_Insertar(Detalle, LS_SIGUIENTE, pllamada_aux);
       procesarcosto (usuario_aux, Usuarios, Detalle, RAIZ, &divisor);
       calcularcosto (Detalle, divisor);
       buscarllamadas (numero_aux, LLamadas, Detalle, IZQ, Usuarios, usuario_aux);
       buscarllamadas (numero_aux, LLamadas, Detalle, DER, Usuarios, usuario_aux);
       return 0;
       }
return 0;
}

/*Chequea que ese número no esté asignado a otros usuarios*/

int procesarcosto (char* usuario_aux, TABO Usuarios, TListaSimple* Detalle, int MOV, int* divisor){

    int compar_usr = 0;
    int compar_num= 0;
    int error=0;
    TUsuario_numeros otrousuario_aux;
    char numero_aux[TELEFONO];
    TLlamada llamada_aux;

    ABO_MoverCte(&Usuarios,MOV,&error);
    if (error) return 2;

    ABO_ElemCte(Usuarios,&otrousuario_aux);
    compar_usr = strcmp (usuario_aux,otrousuario_aux.usuario);
    if (compar_usr == 0){
       procesarcosto (usuario_aux, Usuarios, Detalle, IZQ, divisor);
       procesarcosto (usuario_aux, Usuarios, Detalle, DER, divisor);
       }
    if (compar_usr != 0){
       ls_MoverCorriente(otrousuario_aux.numeros_usr, LS_PRIMERO);
       ls_ElemCorriente(*otrousuario_aux.numeros_usr, &numero_aux);
       ls_ElemCorriente(*Detalle, &llamada_aux);
       compar_num = strcmp (numero_aux, llamada_aux.numero);
       while ((compar_num != 0) && (ls_MoverCorriente(otrousuario_aux.numeros_usr, LS_SIGUIENTE))){
             ls_ElemCorriente(*otrousuario_aux.numeros_usr, &numero_aux);
             compar_num = strcmp (numero_aux, llamada_aux.numero);
             }
       if (compar_num == 0) {
          *divisor=(*divisor)+1;
          }
       procesarcosto (usuario_aux, Usuarios, Detalle, IZQ, divisor);
       procesarcosto (usuario_aux, Usuarios, Detalle, DER, divisor);
       }
return 0;
}

int calcularcosto (TListaSimple* Detalle, int divisor){

    TLlamada llamada_aux;
    ls_ElemCorriente(*Detalle, &llamada_aux);


       llamada_aux.importe = llamada_aux.importe / divisor;
       ls_ModifCorriente(Detalle, &llamada_aux);
return 0;
}
