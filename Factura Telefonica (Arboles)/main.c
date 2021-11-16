#include "TDA_D_T.h"
#define MAXRUTA 30

void Detallarusuario (char* usuario, TDA_D_T* TDA_Detalle);
void Resumengeneral (TDA_D_T* TDA_Detalle, char* RutaFactu);
void DetallarLlamNumero(TDA_D_T TDA_Detalle,char* Numero); /*Es la que se invoca desde el main*/
int validarsintaxis (int argc);
int calculartot (TListaSimple* Detalle, float* total);
void MostrarNoAsignados(TListaSimple* l,int o);
void MostrarTelPorArea(TListaSimple* l,int o);

int main(int argc,char** argv){
	TDA_D_T TDA_Detalle;/*TDA Detalle telefonico*/
	char RutaArchLlamadas[MAXRUTA];

	if (validarsintaxis(argc)) return -1;
	if(TDT_Crear(argv[1], &TDA_Detalle, RutaArchLlamadas))return -1;

	switch(argc){
		case 4:{
			if(!(strcmp(argv[3],"overview"))){
				Resumengeneral (&TDA_Detalle, RutaArchLlamadas);
                break;
			}
			fprintf(stderr, "Error de sintaxis.\n");
			break;
		}
		case 5:{
			if(!(strcmp(argv[3],"usr"))){
				Detallarusuario (argv[4], &TDA_Detalle);

				break;
			}
			if(!(strcmp(argv[3],"unknown"))){
				TListaSimple* pnoasig;
				int j;
				pnoasig=(TListaSimple*)malloc(sizeof(TListaSimple));
                ls_Crear(pnoasig,sizeof(TLlamada));        /* Creo una lista de teléfonos no asignados */
				if(!strcmp(argv[4],"numero")) j=0;

                if(!strcmp(argv[4],"fecha")) j=1;

                TDT_ObtenerDetalleNoAsignada(&TDA_Detalle,pnoasig,j); /*la lleno*/
                MostrarNoAsignados(pnoasig,j); /*la muestro*/
                break;
			}
			if(!(strcmp(argv[3],"num"))){
				DetallarLlamNumero(TDA_Detalle,argv[4]);
                break;
			}
			fprintf(stderr, "Error de sintaxis.\n");
			break;
		}
		case 6:{
				TListaSimple* tel;
				int j;
                tel=(TListaSimple*)malloc(sizeof(TListaSimple));
                ls_Crear(tel,sizeof(TLlamada));
				if(!strcmp(argv[5],"numero")) j=0;

                if(!strcmp(argv[5],"fecha")) j=1;
                TDT_ObtenerDetalleDeTelefonosPorAreaFisica(&TDA_Detalle,argv[4],tel,j);/*la lleno*/
                MostrarTelPorArea(tel,j);  /*la muestro*/
                break;
			}
			fprintf(stderr, "Error de sintaxis.\n");
			break;
	}/*end swich*/

	getchar();
    TDT_Destruir(&TDA_Detalle);
    return 0;
}/*FIN DEL PROGRAMA*/

/******************************************************************************/

/*Función que corrobora que se reciba la cantidad necesaria de parametros*/
int validarsintaxis (int argc){
   if (argc < 4){
      fprintf (stderr,"Sintaxis incorrecta, faltan argumentos.\n");
      return 1;
   }
   if (argc > 6){
      fprintf (stderr,"Sintaxis incorrecta, hay demasiados argumentos.\n");
      return 1;
   }
   return 0;
}

/******************************************************************************/

/*Implementacion*/
/*Funcion encargada de detallar los las llamadas hechas a un numero dado.
 * PRE:TDA_Detalle creado.
 * POST:imprime por pantalla el resultado con el formato pedido; en caso de produ
 * cirse un error lo detalla e imprime por stderr.
 * Autor:Guido Nahuel Ygounet*/
void DetallarLlamNumero(TDA_D_T TDA_Detalle,char* Numero){
	int error,CantUsr;
	float MontoTotal,MontoxUsuario;
	TListaSimple ListaLlamadas;
	TListaSimple ListaUsuarios;
	TLlamada Llamada;
	char Usuario[MAXWORD];

	ls_Crear(&ListaLlamadas,sizeof(TLlamada));
	TDT_ObtenerDetalleDeTelefonosPorAreaFisica(&TDA_Detalle,Numero,&ListaLlamadas,0);/*Obtengo una lista con las llamadas realizadas a el numero solicitado*/
    	if(!ls_Vacia(ListaLlamadas)){
		TDT_ObtenerUsuariosTelefono(&TDA_Detalle,Numero,&ListaUsuarios);
        if(!ls_Vacia(ListaUsuarios)){
			CantUsr=0;
			error=ls_MoverCorriente(&ListaUsuarios,LS_PRIMERO);
			while(error){/*Cuento la cantidad de usuarios para el numero*/
				CantUsr++;
				error=ls_MoverCorriente(&ListaUsuarios,LS_SIGUIENTE);
			}

			MontoTotal=0;
			error=ls_MoverCorriente(&ListaLlamadas,LS_PRIMERO);

			while(error){
				ls_ElemCorriente(ListaLlamadas,&Llamada);
				MontoxUsuario=Llamada.importe/CantUsr;/*Obtengo el monto por usuario*/
				MontoTotal+=Llamada.importe;
				fprintf(stdout,"%s\t%s\t%.2f\t%.2f\t",Llamada.fecha,Llamada.numero,Llamada.importe,MontoxUsuario);
				error=ls_MoverCorriente(&ListaUsuarios,LS_PRIMERO);
                while(error){
					ls_ElemCorriente(ListaUsuarios,&Usuario);
					fprintf(stdout,"%s,",Usuario);/*Enumero los usuarios que reconocen el telefono*/
					error=ls_MoverCorriente(&ListaUsuarios,LS_SIGUIENTE);
				}
				fprintf(stdout,"\n");
				error=ls_MoverCorriente(&ListaLlamadas,LS_SIGUIENTE);
			}
			fprintf(stdout,"\n **TOTAL**%.2f",MontoTotal);
		}
		else{
			fprintf(stderr,"Error:No se encontro ningun usuario para ese numero telefonico\n");
		}
	}
	else{
		fprintf(stderr,"Error:No se encontraron llamadas realizadas a ese numero telefonico\n");
	}
}

/*----------------------------------------------------------------------*/
/*Detallarusuario
**Función que presenta por stdout todas las llamadas realizadas por un
**usuario y el importe total.
**Pedro Lynn*/
void Detallarusuario (char* usuario, TDA_D_T* TDA_Detalle){

    TLlamada    llamada_aux;
    TListaSimple Detalle;
    float costototal=0;

    ls_Crear(&Detalle, sizeof(TLlamada));
    TDT_ObtenerDetalleUsuario (usuario, TDA_Detalle, &Detalle);
    if (!ls_Vacia(Detalle))
    {
     ls_MoverCorriente (&Detalle,LS_PRIMERO);
     ls_ElemCorriente (Detalle,&llamada_aux);
    }
    fprintf(stdout,"DETALLE DE LLAMADAS REALIZADAS POR %s:\n",usuario);
    fprintf(stdout,"-------------------------------------------------\n");
    fprintf(stdout,"FECHA\t\t    TELEFONO   IMPORTE\n");
    if (!ls_Vacia(Detalle))
    {
     fprintf(stdout,"%-20s%-11s%.2f\n",llamada_aux.fecha,llamada_aux.numero,llamada_aux.importe);
     costototal= costototal + llamada_aux.importe;
     while (ls_MoverCorriente (&Detalle,LS_SIGUIENTE)){
           ls_ElemCorriente (Detalle,&llamada_aux);
           costototal= costototal + llamada_aux.importe;
           fprintf (stdout,"%-20s%-11s%.2f\n",llamada_aux.fecha,llamada_aux.numero,llamada_aux.importe);
           }
     }
    fprintf (stdout,"-------------------------------------------------\n");
    fprintf (stdout,"**TOTAL**: %.2f\n",costototal);
    fprintf (stdout,"-------------------------------------------------\n");
    return;

}
/*----------------------------------------------------------------------*/
/*Resumengeneral
**Función que imprime por stdout el resumen de gastos para cada usuario,
**llamadas no asignadas y gastos generales.
**Pedro Lynn*/
void Resumengeneral (TDA_D_T* TDA_Detalle, char RutaFactu[MAXRUTA]){

    TResumen resumen_aux;
    TListaSimple Resumen;
    TListaSimple GastosGenerales;
    TListaSimple noasig;
    TGastosG DeudaG;
	FILE* ArchivoDetalle;
	float costototal=0;
	float costonoasig=0;
	float gastogen=0;
	int ord=0;

	ls_Crear(&Resumen, sizeof(TResumen));
	ls_Crear(&noasig,sizeof(TLlamada));
    ArchivoDetalle = fopen(RutaFactu,"r");

    TDT_GenerarResumenUsuarios (TDA_Detalle, &Resumen);
    if (!ls_Vacia(Resumen)){
          ls_MoverCorriente (&Resumen,LS_PRIMERO);
          ls_ElemCorriente (Resumen,&resumen_aux);
          fprintf (stdout,"RESUMEN DE GASTOS:\n");
          fprintf (stdout,"-------------------------------------------------\n");
          fprintf (stdout,"USUARIO\t\t\t       IMPORTE\t\n");
          fprintf (stdout,"%s\t\t\t\t %.2f\n",resumen_aux.usuario,resumen_aux.importe);
          costototal= costototal + resumen_aux.importe;
          while (ls_MoverCorriente (&Resumen,LS_SIGUIENTE)){
                ls_ElemCorriente (Resumen,&resumen_aux);
                fprintf (stdout,"%s\t\t\t\t %.2f\t\n",resumen_aux.usuario,resumen_aux.importe);
                costototal= costototal + resumen_aux.importe;
                }
          }
    TDT_ObtenerDeudaGastosGenerales(ArchivoDetalle,&GastosGenerales);

    if (!ls_Vacia(GastosGenerales)){
          ls_MoverCorriente (&GastosGenerales,LS_PRIMERO);
          ls_ElemCorriente (GastosGenerales,&DeudaG);
          gastogen= gastogen + DeudaG.importe;
          costototal= costototal + DeudaG.importe;
          while (ls_MoverCorriente (&GastosGenerales,LS_SIGUIENTE)){
                ls_ElemCorriente (GastosGenerales,&DeudaG);
                gastogen= gastogen + DeudaG.importe;
                costototal= costototal + DeudaG.importe;
                }
          fprintf (stdout,"Gastos Generales\t\t %.2f\n",gastogen);
          }
    TDT_ObtenerDetalleNoAsignada(TDA_Detalle,&noasig,ord);
    calculartot (&noasig,&costonoasig);
    costototal= costototal + costonoasig;
    fprintf (stdout,"Deuda no asignada\t\t %.2f\n",costonoasig);
    fprintf (stdout,"-------------------------------------------------\n");
    fprintf (stdout,"**TOTAL**: %.2f\n",costototal);
    fprintf (stdout,"-------------------------------------------------\n");
}
/*----------------------------------------------------------------------*/
int calculartot (TListaSimple* Detalle, float* total){

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
void MostrarNoAsignados(TListaSimple* l,int o){
  TLlamada aux;
  int ok;
  fprintf(stdout,"Detalle de llamadas no reconocidas:\n-------------------------------------------------\n\n");
  if(ls_Vacia(*l)) fprintf(stdout,"No hay llamadas no reconocidas \n");
  else{
    ok=ls_MoverCorriente(l,LS_PRIMERO);
    while (ok) {
      ls_ElemCorriente(*l,&aux);
      if (o==0) fprintf(stdout,"%s\t%s\t%.2f\n",aux.numero,aux.fecha,aux.importe);                               /*imprimir*/
      else fprintf(stdout,"%s\t%s\t%.2f\n",aux.fecha,aux.numero,aux.importe);
      ok=ls_MoverCorriente(l,LS_SIGUIENTE);
    }
  }
  fprintf(stdout,"\n-------------------------------------------------\n");
}

void MostrarTelPorArea(TListaSimple* l,int o){
  TLlamada aux;
  int ok;
  fprintf(stdout,"Detalle de telefonos por area:\n-------------------------------------------------\n\n");
  if(ls_Vacia(*l)) fprintf(stdout,"No existen teléfonos con esa área\n");
  else{
    ok=ls_MoverCorriente(l,LS_PRIMERO);
    while (ok) {
      ls_ElemCorriente(*l,&aux);
      if (o==0) fprintf(stdout,"%s\t%s\t%.2f\n",aux.numero,aux.fecha,aux.importe);                               /*imprimir*/
      else fprintf(stdout,"%s\t%s\t%.2f\n",aux.fecha,aux.numero,aux.importe);
      ok=ls_MoverCorriente(l,LS_SIGUIENTE);
    }
  }
  fprintf(stdout,"\n-------------------------------------------------\n");
}
