#include "abb.h"
#include "tda_pruebas.h"

typedef struct cosa {
	int clave;
	char contenido[5];
} cosa_t;

/*
 * Reserva meomoria para una cosa y le asigna la clave del parametro
*/
cosa_t* crear_cosa(int clave){
	cosa_t* c = (cosa_t*)malloc(sizeof(cosa_t));
	if(c)
		c->clave = clave;
	return c;
}

void destruir_cosa(cosa_t* c){
	if(c)
		free(c);
}

/*
 * Compara los elementos dados por sus claves.
 * Valor de retorno: Reglas dadas en abb.h
*/
int comparar_cosas(void* elemento1, void* elemento2){
	if(!elemento1 || !elemento2)
		return 0;

	if(((cosa_t*)elemento1)->clave>((cosa_t*)elemento2)->clave)
		return 1;
	
	if(((cosa_t*)elemento1)->clave<((cosa_t*)elemento2)->clave)
		return -1;
	
	return 0;
}

/*
 * Si tiene un elemento lo envia a destruir_cosa
*/
void destructor_de_cosas(void* elemento){
	if(!elemento)
		return;
	destruir_cosa((cosa_t*)elemento);
}

/*
 * Dado un elemento imprime su clave 
 * Valor de retorno: false para que itere todos los elementos
*/
bool mostrar_elemento(void* elemento, void* extra){
	extra=extra; //para que no se queje el compilador, gracias -Werror -Wall
	
	if(elemento)
		printf("%i ", ((cosa_t*)elemento)->clave);
	
	return false;
}

/*
 * Dado un elemento imprime su clave 
 * Valor de retorno: false hasta que encuentre un 5, ahí retorna true
*/
bool mostrar_hasta_5(void* elemento, void* extra){
	extra=extra; //para que no se queje el compilador, gracias -Werror -Wall
	
	if(elemento){
		printf("%i ", ((cosa_t*)elemento)->clave);
		if(((cosa_t*)elemento)->clave == 5)
			return true;
	}

	return false;
}

/*
 * Dado un elemento y un acumulado (extra) va sumando cada clave 
 * e imprime el acumulado 
 * Valor de retorno: false para que itere todos los elementos
*/
bool mostrar_acumulado(void* elemento, void* extra){
	if(elemento && extra){
		*(int*)extra += ((cosa_t*)elemento)->clave;
		printf("%i ", *(int*)extra);
	}
	return false;
}



void probar_arbol_insertar(pruebas_t* pruebas, abb_t* arbol){

	printf(COLOR_NORMAL "\n--PRUEBAS DE INSERTAR\n\n");

	cosa_t* c1= crear_cosa(1);
	cosa_t* c2= crear_cosa(2);
	cosa_t* c3= crear_cosa(3);
	cosa_t* c4= crear_cosa(4);
	cosa_t* c5= crear_cosa(5);
	cosa_t* aux = crear_cosa(0);

	validar_prueba(pruebas,"Se inserta un elemento correctamente",
				   arbol_insertar(arbol, c4)==EXITO);
	validar_prueba(pruebas,"Primer elemento es la raiz",
				   ((cosa_t*)arbol_raiz(arbol))->clave==4);
	validar_prueba(pruebas,"Se inserta otro elemento correctamente",
				   arbol_insertar(arbol, c2)==EXITO);

	validar_prueba(pruebas,"Se insertan 3 mas",arbol_insertar(arbol, c1)==EXITO &&
											   arbol_insertar(arbol, c3)==EXITO && 
											   arbol_insertar(arbol, c5)==EXITO);

	aux->clave = 5;
	validar_prueba(pruebas,"Raiz del arbol no se modifica",
				   ((cosa_t*)arbol_raiz(arbol))->clave==4);
	validar_prueba(pruebas,"Busco el ultimo elemento insertado.",
				   ((cosa_t*)arbol_buscar(arbol, aux))->clave==5);
	free(aux);
}


void probar_arbol_borrar(pruebas_t* pruebas, abb_t* arbol){

	printf(COLOR_NORMAL "\n--PRUEBAS DE BORRADO\n\n");

	cosa_t* c1= crear_cosa(1);          //disenio                6
	cosa_t* c2= crear_cosa(2);			//				      2     8
	cosa_t* c3= crear_cosa(3);			//				    1  3   7 9
	cosa_t* c4= crear_cosa(4);			//                      5     10
	cosa_t* c5= crear_cosa(5);			//                     4
	cosa_t* c6= crear_cosa(6);			
	cosa_t* c7= crear_cosa(7);			
	cosa_t* c8= crear_cosa(8);			
	cosa_t* c9= crear_cosa(9);		
	cosa_t* c10= crear_cosa(10);		
	cosa_t* aux = crear_cosa(0);

	validar_prueba(pruebas,"Inserto 9 elementos",arbol_insertar(arbol, c6)==EXITO &&
												 arbol_insertar(arbol, c2)==EXITO &&
												 arbol_insertar(arbol, c1)==EXITO &&
												 arbol_insertar(arbol, c3)==EXITO &&
												 arbol_insertar(arbol, c5)==EXITO &&
												 arbol_insertar(arbol, c4)==EXITO &&
												 arbol_insertar(arbol, c8)==EXITO &&
							   					 arbol_insertar(arbol, c7)==EXITO && 
							   					 arbol_insertar(arbol, c9)==EXITO && 
												 arbol_insertar(arbol, c10)==EXITO);
	
	validar_prueba(pruebas,"Elemento raiz es el esperado",
				   ((cosa_t*)arbol_raiz(arbol))->clave==6);

	aux->clave = 1;
	validar_prueba(pruebas,"Borro nodo hoja.",arbol_borrar(arbol, aux)==EXITO);
	validar_prueba(pruebas,"El elemento borrado no se encuentra en el arbol",
				   !arbol_buscar(arbol, aux));

	aux->clave = 9;
	validar_prueba(pruebas,"Borro nodo con un hijo.",arbol_borrar(arbol, aux)==EXITO);
	validar_prueba(pruebas,"El elemento borrado no se encuentra en el arbol",
				   !arbol_buscar(arbol, aux));

	aux->clave = 8;
	validar_prueba(pruebas,"Borro nodo con dos hijos(simple).",arbol_borrar(arbol, aux)==EXITO);
	validar_prueba(pruebas,"El elemento borrado no se encuentra en el arbol",
				   !arbol_buscar(arbol, aux));

		// NO ENCONTRE' LA SOLUCION A ESTA PRUEBA

	aux->clave = 6;
	validar_prueba(pruebas,"Borro nodo Raiz, cuyo predecesor inorden tiene hijo.",
				   arbol_borrar(arbol, aux)==EXITO);
	validar_prueba(pruebas,"El elemento borrado no se encuentra en el arbol",
				   !arbol_buscar(arbol, aux));
	validar_prueba(pruebas,"Nuevo elemento raiz es el esperado",
				   ((cosa_t*)arbol_raiz(arbol))->clave==5);

	aux->clave = 4;
	validar_prueba(pruebas,"Busco el elemento 4.",
				   (arbol_buscar(arbol, aux))!=NULL);
	free(aux);
}

/*
 * Dado un vector de punteros a cosas y su cantidad
 * compara la i-esima clave de cosas con el i-esimo número
 * Valor de retorno: true si todos las claves son iguales. False si no.
*/
bool claves_iguales(cosa_t* cosas[], int cant_cosas, int numeros[]){

	bool son_iguales=true;
	int i=0;
	while(son_iguales && i<cant_cosas){
		if(cosas[i]->clave!=numeros[i])
			son_iguales=false;
		i++;
	}
	return son_iguales;
}


void probar_arbol_recorridos(pruebas_t* pruebas, abb_t* arbol){

	printf(COLOR_NORMAL "\n--PRUEBAS DE RECORRIDO\n\n");

	cosa_t* c1= crear_cosa(1);
	cosa_t* c2= crear_cosa(2);
	cosa_t* c3= crear_cosa(3);
	cosa_t* c4= crear_cosa(4);
	cosa_t* c5= crear_cosa(5);

	validar_prueba(pruebas,"Inserto 5 elementos",arbol_insertar(arbol, c4)==EXITO &&
												 arbol_insertar(arbol, c2)==EXITO &&
												 arbol_insertar(arbol, c1)==EXITO &&
												 arbol_insertar(arbol, c3)==EXITO &&
												 arbol_insertar(arbol, c5)==EXITO);

	cosa_t* elementos[10];
	int cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 10);
	int numeros_esperados[5]={1,2,3,4,5};
	validar_prueba(pruebas,"Recorrido inorden es el esperado",
							claves_iguales(elementos, cantidad, numeros_esperados));

	cantidad = arbol_recorrido_preorden(arbol, (void**)elementos, 10);
	numeros_esperados[0]=4;
	numeros_esperados[1]=2;
	numeros_esperados[2]=1;
	numeros_esperados[3]=3;
	numeros_esperados[4]=5;
	validar_prueba(pruebas,"Recorrido preorden es el esperado",
							claves_iguales(elementos, cantidad, numeros_esperados));

	cantidad = arbol_recorrido_postorden(arbol, (void**)elementos, 10);
	numeros_esperados[0]=1;
	numeros_esperados[1]=3;
	numeros_esperados[2]=2;
	numeros_esperados[3]=5;
	numeros_esperados[4]=4;
	validar_prueba(pruebas,"Recorrido postorden es el esperado",
							claves_iguales(elementos, cantidad, numeros_esperados));
}


void probar_iterador_interno(pruebas_t* pruebas, abb_t* arbol){

	printf(COLOR_NORMAL "\n--PRUEBAS DE ITERADOR INTERNO(VISUALES)\n\n");

	cosa_t* c1= crear_cosa(1);
	cosa_t* c2= crear_cosa(2);
	cosa_t* c3= crear_cosa(3);
	cosa_t* c4= crear_cosa(4);
	cosa_t* c5= crear_cosa(5);
	cosa_t* c6= crear_cosa(6);
	cosa_t* c7= crear_cosa(7);

	validar_prueba(pruebas,"Inserto 7 elementos",arbol_insertar(arbol, c4)==EXITO &&
												 arbol_insertar(arbol, c2)==EXITO &&
												 arbol_insertar(arbol, c1)==EXITO &&
												 arbol_insertar(arbol, c3)==EXITO &&
												 arbol_insertar(arbol, c6)==EXITO &&
												 arbol_insertar(arbol, c7)==EXITO &&
												 arbol_insertar(arbol, c5)==EXITO);
	printf(COLOR_NORMAL "\n");
	printf("Imprimir inorden. Output expected:[1 2 3 4 5 6 7]\n Output: ");    
	abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_elemento, NULL);
	printf("\n\n");

	printf("Imprimir preorden. Output expected:[4 2 1 3 6 5 7]\n Output: ");    
	abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, mostrar_elemento, NULL);
	printf("\n\n");

	printf("Imprimir postorden. Output expected:[1 3 2 5 7 6 4]\n Output: ");    
	abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, mostrar_elemento, NULL);
	printf("\n\n");

	printf("Imprimir inorden hasta el 5. Output expected:[1 2 3 4 5]\n Output: ");    
	abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_hasta_5, NULL);
	printf("\n\n");

	printf("Imprimir preorden hasta el 5. Output expected:[4 2 1 3 6 5]\n Output: ");    
	abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, mostrar_hasta_5, NULL);
	printf("\n\n");

	printf("Imprimir postorden hasta el 5. Output expected:[1 3 2 5]\n Output: ");    
	abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, mostrar_hasta_5, NULL);
	printf("\n\n");

	int acumulador=0;
	printf("Inorden acumulando los valores. Output expected:[1 3 6 10 15 21 28]\n Output: ");    
	abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_acumulado, &acumulador);
	printf("\n\n");

	acumulador=0;
	printf("Preorden acumulando los valores. Output expected:[4 6 7 10 16 21 28]\n Output: ");    
	abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, mostrar_acumulado, &acumulador);
	printf("\n\n");

	acumulador=0;
	printf("Postorden acumulando los valores. Output expected:[1 4 6 11 18 24 28]\n Output: ");    
	abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, mostrar_acumulado, &acumulador);
	printf("\n\n");

	abb_con_cada_elemento(NULL, ABB_RECORRER_INORDEN, mostrar_elemento, NULL);
	printf("Iterador interno no hace nada con arbol NULL. Output expected:[]\n Output:");
	printf("\n\n");

	abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, NULL, NULL);
	printf("Iterador interno no hace nada con funcion NULL. Output expected:[]\n Output:");
	printf("\n\n");

	abb_con_cada_elemento(NULL, ABB_RECORRER_INORDEN, mostrar_elemento, NULL);
	printf("Iterador interno no hace nada con recorrido invalido. Output expected:[]\n Output:");
	printf("\n\n");
}


void probar_valores_null(pruebas_t* pruebas){

	printf(COLOR_NORMAL "\n--PRUEBAS CON VALORES NULL\n\n");

	validar_prueba(pruebas,"Arbol NULL es vacio", arbol_vacio(NULL));
	validar_prueba(pruebas,"Raiz de arbol NULL es NULL", !arbol_raiz(NULL));
	validar_prueba(pruebas,"Arbol no se crea con comparador NULL",
							!arbol_crear(NULL,destructor_de_cosas));
	
	abb_t* arbol=arbol_crear(comparar_cosas,NULL);
	validar_prueba(pruebas,"Arbol se crea con destructor NULL",arbol);
	validar_prueba(pruebas,"Inserto valor NULL",arbol_insertar(arbol, NULL)==EXITO);
	arbol_destruir(arbol);

	cosa_t* elementos[1];
	validar_prueba(pruebas,"Recorrido inorden de arbol NULL recorre 0 elementos",
				   arbol_recorrido_inorden(NULL, (void**)elementos, 1)==0);

	validar_prueba(pruebas,"Recorrido preorden de arbol NULL recorre 0 elementos",
				   arbol_recorrido_preorden(NULL, (void**)elementos, 1)==0);

	validar_prueba(pruebas,"Recorrido postorden de arbol NULL recorre 0 elementos",
				   arbol_recorrido_postorden(NULL, (void**)elementos, 1)==0);
}


int main(){

	pruebas_t* pruebas=crear_pruebas();
	if(!pruebas){
		perror("Fallo al crear las pruebas");
		return 0;
	}

	printf("\t\tPRUEBAS DE ABB.\n\n");

	{
		abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
	
		validar_prueba(pruebas,"Se creo el arbol correctamente.", arbol &&
					   				  arbol->comparador==comparar_cosas && 
					   				  arbol->destructor==destructor_de_cosas);
		validar_prueba(pruebas,"El arbol se crea vacio.",arbol_vacio(arbol));

		probar_arbol_insertar(pruebas, arbol);
		arbol_destruir(arbol);
	}

	{
		abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
		probar_arbol_borrar(pruebas, arbol);
		arbol_destruir(arbol);
	}

	{
		abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
		probar_arbol_recorridos(pruebas, arbol);
		arbol_destruir(arbol);
	}

	{
		abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
		probar_iterador_interno(pruebas, arbol);
		arbol_destruir(arbol);
	}

	{
		probar_valores_null(pruebas);	
	}

	mostrar_resultados(pruebas);
	destruir_pruebas(pruebas);
	
	return 0;
}