#include "tda_pruebas.h"

#define NO_HAY_PRUEBAS -2



struct pruebas{
	int pruebas_pasadas;
	int pruebas_totales;
};




pruebas_t* crear_pruebas(){

	return calloc(1, sizeof(pruebas_t));
}


/* Recibe el estado de la prueba descripta por descripcion
 * y la imprime junto con el resultado
 */
void validar_prueba(pruebas_t* pruebas, char* descripcion, bool estado){

	if(!pruebas)
		return;

	if(estado)
		pruebas->pruebas_pasadas++;
	
	pruebas->pruebas_totales++;
	printf(COLOR_NORMAL "PRUEBA: %s ----%s.\n",descripcion,
		   estado? COLOR_VERDE"EXITO": COLOR_ROJO"FALLA");
}


/* 
 * Dada las pruebas, calcula y devuelve la nota entera del 0 al 10
 */
int calcular_nota(pruebas_t* pruebas){

	if(pruebas->pruebas_totales == 0)
		return NO_HAY_PRUEBAS;
	
	double nota_decimal=(100*pruebas->pruebas_pasadas)
					    /pruebas->pruebas_totales; //el 100 representa el 100%

	nota_decimal/=10; // Pues quiero la nota del 0 al 10

	nota_decimal=round(nota_decimal); // redondea a un entero

	return (int)nota_decimal;
}


void mostrar_resultados(pruebas_t* pruebas){

	int nota=calcular_nota(pruebas);
	printf(COLOR_NORMAL "\n\n||PASASTE %i PRUEBAS DE %i||\n\n",pruebas->pruebas_pasadas
											 				  ,pruebas->pruebas_totales);
	if(nota>=4)
		printf(COLOR_VERDE);
	else
		printf(COLOR_ROJO);
	printf("\t\t______________________\n");
	printf("\t\t|Tu nota final es: %i|\n", nota);
	printf("\t\t----------------------\n");
	printf(COLOR_NORMAL "\n");
}


void destruir_pruebas(pruebas_t* pruebas){

	free(pruebas);
}