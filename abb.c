#include "abb.h"

#include <stdio.h>

#define EXITO 0
#define ERROR -1
#define DERECHA 2
#define IZQUIERDA -2

abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){

	if(!comparador)
		return NULL;

	abb_t* arbol=calloc(1, sizeof(abb_t));
	if(!arbol)
		return NULL;	
	
	arbol->comparador= comparador;
	arbol->destructor= destructor;

	return arbol;
}

/*
 * Va comparando el nodo a insertar con el actual hasta que 
 * encuentra la hoja a insertar y lo inserta
*/
void abb_insertar_recursivo(nodo_abb_t* nodo_actual, nodo_abb_t* nodo_nuevo,
						   abb_comparador comparador){
	if(!nodo_actual)
		return;

	int lado_a_insertar;

	if(comparador(nodo_actual->elemento, nodo_nuevo->elemento)<=0){   // Convencion de iguales a derecha
		abb_insertar_recursivo(nodo_actual->derecha, nodo_nuevo, comparador);
		lado_a_insertar=DERECHA;
	}else{
		abb_insertar_recursivo(nodo_actual->izquierda, nodo_nuevo, comparador);
		lado_a_insertar=IZQUIERDA;
	}

	if(lado_a_insertar==DERECHA && !nodo_actual->derecha)     //es decir que se encuentra en la hoja a insertar
		nodo_actual->derecha=nodo_nuevo;
	else if(lado_a_insertar==IZQUIERDA && !nodo_actual->izquierda)
		nodo_actual->izquierda=nodo_nuevo;
}


 int arbol_insertar(abb_t* arbol, void* elemento){

 	if(!arbol)
 		return ERROR;

 	nodo_abb_t* nodo_nuevo= calloc(1, sizeof(nodo_abb_t));
 	if(!nodo_nuevo)
 		return ERROR;

 	nodo_nuevo->elemento=elemento;
 	if(arbol_vacio(arbol))
 		arbol->nodo_raiz=nodo_nuevo;
 	else
	 	abb_insertar_recursivo(arbol->nodo_raiz, nodo_nuevo, arbol->comparador);
 	
 	return EXITO;
 }

/*
 * Dado un nodo retorna su cantidad de hijos
*/
int cantidad_hijos(nodo_abb_t* nodo_actual){

	int hijos=0;
	if(nodo_actual->izquierda)
		hijos++;
	if(nodo_actual->derecha)
		hijos++;
	return hijos;
}

/*
 * En principio recibe el hijo iquierdo del nodo a eliminar y de ahí
 * va avanzando a través de su hijo derecho hasta llegar al predecesor
 * inorden, entonces guarda el posible hijo a izquierda de este y se 
 * retorna, luego "asigna el hijo a izquierda de predecesor a su nuevo padre"
 * y luego sigue encadenando la rama recorrida
*/
nodo_abb_t* predecesor_inorden(nodo_abb_t* nodo_actual, nodo_abb_t** hijo_aux){

	if(!nodo_actual->derecha){
		*hijo_aux=nodo_actual->izquierda;  // Lo asigna bien
		return nodo_actual;
	}

	nodo_abb_t* predecesor_ind=predecesor_inorden(nodo_actual->derecha, hijo_aux);
	nodo_actual->derecha=*hijo_aux;   // Pero luego acá vale NULL, y a partir de ahí
	*hijo_aux=nodo_actual; 			 // empiezo a perder las referencias
	return predecesor_ind;
}

/*
 * Padre nuevo adopta los hijos de padre a eliminar
*/
void asignar_nuevos_hijos(nodo_abb_t* padre_nuevo, nodo_abb_t* padre_a_eliminar){

	if(padre_a_eliminar->izquierda!=padre_nuevo)
		padre_nuevo->izquierda=padre_a_eliminar->izquierda;
	padre_nuevo->derecha=padre_a_eliminar->derecha;
}

/*
 * Va comparando el nodo a eliminar con el actual, si lo encuentra
 * se fija en la cantidad de hijos y elimina segun la cantidad
*/
nodo_abb_t* abb_borrar_recursivo(abb_t* arbol, nodo_abb_t* nodo_actual,
								 void* elemento, bool* borrado){
	if(!nodo_actual)
		return NULL;

	int comparacion=arbol->comparador(nodo_actual->elemento, elemento);

	if(comparacion == 0){
		int cant_hijos=cantidad_hijos(nodo_actual);
		nodo_abb_t* nodo_aux=NULL;

		if(cant_hijos==1)
			nodo_aux=nodo_actual->izquierda? nodo_actual->izquierda : nodo_actual->derecha;
		else if(cant_hijos==2){
			nodo_abb_t* hijo_aux=NULL; 
			nodo_aux=predecesor_inorden(nodo_actual->izquierda, &hijo_aux);
			asignar_nuevos_hijos(nodo_aux, nodo_actual);
		}
		arbol->destructor(nodo_actual->elemento);
		free(nodo_actual);
		*borrado=true;
		return nodo_aux;
	}

	if(comparacion == -1)
		nodo_actual->derecha=abb_borrar_recursivo(arbol,nodo_actual->derecha,elemento,borrado);
	else
		nodo_actual->izquierda=abb_borrar_recursivo(arbol,nodo_actual->izquierda,elemento,borrado);

	return nodo_actual;
}

int arbol_borrar(abb_t* arbol, void* elemento){

	if(arbol_vacio(arbol))
		return ERROR;

	bool pudo_borrar=false;
	arbol->nodo_raiz=abb_borrar_recursivo(arbol, arbol->nodo_raiz, elemento,&pudo_borrar);

	return pudo_borrar? EXITO : ERROR;
}

/*
 * Va comparando el nodo buscado con el actual, si es menor va a izquierda
 * si es mayor a derecha y si lo encuentra lo retorna
*/
void* abb_buscar_recursivo(nodo_abb_t* nodo_actual, void* elemento,
						   abb_comparador comparador){
	if(!nodo_actual)
		return NULL;
	
	int comparacion=comparador(nodo_actual->elemento, elemento);
	
	if(comparacion==0)
		return nodo_actual->elemento;
	else if(comparacion==1)
		return abb_buscar_recursivo(nodo_actual->izquierda, elemento, comparador);
	else
		return abb_buscar_recursivo(nodo_actual->derecha, elemento, comparador);
}

void* arbol_buscar(abb_t* arbol, void* elemento){
	if(arbol_vacio(arbol))
		return NULL;
	return abb_buscar_recursivo(arbol->nodo_raiz, elemento, arbol->comparador);
}



bool arbol_vacio(abb_t* arbol){
	return (!arbol || !arbol->nodo_raiz);
}


void* arbol_raiz(abb_t* arbol){
	if(arbol_vacio(arbol))
		return  NULL;
	return arbol->nodo_raiz->elemento;
}


/*
 * Recorre el arbol inorden y va guardando los elementos en el array
 * hasta que se termina el arbol o el array
*/
void inorden_recursivo(nodo_abb_t* nodo_actual, void** array, int tamanio_array,
					   int* indice){

	if(!nodo_actual || *indice==tamanio_array)
		return;

	inorden_recursivo(nodo_actual->izquierda, array, tamanio_array, indice);
	if(*indice==tamanio_array)
		return;
	array[*indice]=nodo_actual->elemento;
	(*indice)++;
	inorden_recursivo(nodo_actual->derecha, array, tamanio_array, indice);
}

int arbol_recorrido_inorden(abb_t* arbol, void** array, int tamanio_array){

	if(arbol_vacio(arbol) || !array || tamanio_array==0)
		return 0;

	int indice=0;
	inorden_recursivo(arbol->nodo_raiz, array, tamanio_array, &indice);\

	return indice;
}

/*
 * Recorre el arbol en preorden y va guardando los elementos en el array
 * hasta que se termina el arbol o el array
*/
void preorden_recursivo(nodo_abb_t* nodo_actual, void** array, int tamanio_array,
					    int* indice){

	if(!nodo_actual || *indice==tamanio_array)
		return;

	array[*indice]=nodo_actual->elemento;
	(*indice)++;
	preorden_recursivo(nodo_actual->izquierda, array, tamanio_array, indice);
	preorden_recursivo(nodo_actual->derecha, array, tamanio_array, indice);
}

int arbol_recorrido_preorden(abb_t* arbol, void** array, int tamanio_array){
	if(arbol_vacio(arbol) || !array || tamanio_array==0)
		return 0;

	int indice=0;
	preorden_recursivo(arbol->nodo_raiz, array, tamanio_array, &indice);

	return indice;
}


/*
 * Recorre el arbol en postorden y va guardando los elementos en el array
 * hasta que se termina el arbol o el array
*/
void postorden_recursivo(nodo_abb_t* nodo_actual, void** array, int tamanio_array,
						 int* indice){

	if(!nodo_actual || *indice==tamanio_array)
		return;

	postorden_recursivo(nodo_actual->izquierda, array, tamanio_array, indice);
	postorden_recursivo(nodo_actual->derecha, array, tamanio_array, indice);
	if(*indice==tamanio_array)
		return;
	array[*indice]=nodo_actual->elemento;
	(*indice)++;
}


int arbol_recorrido_postorden(abb_t* arbol, void** array, int tamanio_array){

	if(arbol_vacio(arbol) || !array || tamanio_array==0)
		return 0;

	int indice=0;
	postorden_recursivo(arbol->nodo_raiz, array, tamanio_array, &indice);

	return indice;
}


/*
 * Recorre el arbol en postorden, destruye los elementos y los nodos
*/
void abb_destruir_recursivo(nodo_abb_t* nodo_actual, abb_liberar_elemento destructor){

	if(!nodo_actual)
		return;

	abb_destruir_recursivo(nodo_actual->izquierda, destructor);
	abb_destruir_recursivo(nodo_actual->derecha, destructor);
	if(destructor)                     
		destructor(nodo_actual->elemento);
	free(nodo_actual);
}

void arbol_destruir(abb_t* arbol){

	if(!arbol)
		return;

	abb_destruir_recursivo(arbol->nodo_raiz, arbol->destructor);
	free(arbol);
}


/*
 * Recorre el arbol recursivamente segun el recorrido solicitado aplicando a cada
 * elemento la funcion pasada por parametro hasta que dicha función devuelva true
*/
void iterador_interno(nodo_abb_t* nodo_actual, int recorrido, bool (*funcion)(void*, void*),
					  void* extra, bool* detener){

	if(!nodo_actual)
		return;

	switch(recorrido){
		case ABB_RECORRER_INORDEN:
			iterador_interno(nodo_actual->izquierda, recorrido, funcion, extra, detener);
			if(!(*detener)){
				(*detener)=funcion(nodo_actual->elemento, extra);
				if(!(*detener))
					iterador_interno(nodo_actual->derecha, recorrido, funcion, extra, detener);
				}
		break;

		case ABB_RECORRER_PREORDEN:
			(*detener)=funcion(nodo_actual->elemento, extra);
			if(!(*detener)){
				iterador_interno(nodo_actual->izquierda, recorrido, funcion, extra, detener);
				if(!(*detener))
					iterador_interno(nodo_actual->derecha, recorrido, funcion, extra, detener);
				}
		break;

		case ABB_RECORRER_POSTORDEN:
			iterador_interno(nodo_actual->izquierda, recorrido, funcion, extra, detener);
			iterador_interno(nodo_actual->derecha, recorrido, funcion, extra, detener);
			if(!(*detener))
				(*detener)=funcion(nodo_actual->elemento, extra);		
		break;
	}
}

void abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*),
						   void* extra){
	if(arbol_vacio(arbol) || !funcion)
		return;

	bool detener_iterador= false;
	iterador_interno(arbol->nodo_raiz, recorrido, funcion, extra, &detener_iterador);
}