#ifndef FILTROS_H_
#define FILTROS_H_
#define MAXLINEA 256/*Constante de maxima longitud de linea*/
#define MAXWORD 65

/*--------------------------FILTROS----------------------------*/
int MAYUSCULAS(void* datos_filtro,TTubo* entrada,TTubo* Salida,int eof);
int MINUSCULAS(void* datos_filtro,TTubo* entrada,TTubo* salida,int eof);
int ORDENAR(void* datos_filtro, TTubo* entrada, TTubo* salida,int eof);
int ELIMINAR_CONSEC(void* datos_filtro, TTubo* entrada, TTubo* salida,int eof);
int CONTAR_CONSEC(void* datos_filtro, TTubo* entrada, TTubo* salida,int eof);
int ULTIMOS(void* datos_filtro, TTubo* entrada,TTubo* salida, int eof);
int PRIMEROS(void* datos_filtro, TTubo* entrada,TTubo* salida, int eof);
int FILTRAR(void* datos_filtro, TTubo* entrada, TTubo* salida, int eof);
int EXTRAER(void* datos_filtro, TTubo* entrada, TTubo* salida,int eof);

#endif /*FILTROS_H_*/
