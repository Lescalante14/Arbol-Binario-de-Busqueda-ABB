#ifndef __TDA_PRUEBAS_H__
#define __TDA_PRUEBAS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define COLOR_VERDE "\x1b[32m\x1b[1m"
#define COLOR_ROJO "\x1b[91m\x1b[1m"
#define COLOR_NORMAL "\x1b[0m"

#define EXITO 0
#define ERROR -1


typedef struct pruebas pruebas_t;


pruebas_t* crear_pruebas();


void validar_prueba(pruebas_t* pruebas, char* descripcion, bool estado);


void mostrar_resultados(pruebas_t* pruebas);


void destruir_pruebas(pruebas_t* pruebas);



#endif /*__TDA_PRUEBAS_H__*/