README


SECCIÓN 1. DESARROLLO PRÁCTICO


	INTRODUCCIÓN:

		En el siguiente trabajo se desarrolló la implementación del TDA Arbol Bina-
	rio de Busqueda(de ahora en adelante ABB )y además un iterador interno con
	condición de corte.

		Aunado a esto también se reutilizó el TDA "tda_pruebas" que fue implementado
	para el anterior(lista). Este TDA es usado para testear todas las funciones
	implementadas en el ABB y mostrar los resultados obtenidos al final de estas.


	COMPILACIÓN Y EJECUCIÓN:

		La linea de ejecución es igual que el ejemplo brindado por la cátedra:

			-Ejecución: valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./abb

		Ahora, la linea de compilación es el ejemplo brindado por la cátedra + el 
		flag "-lm" para que reconozca la función round() usada en "tda_pruebas":

			-Compilación: gcc *.c -o abb -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0 -lm



SECCIÓN 2. DESARROLLO TEÓRICO


	P1-Explicar qué es un ABB y cómo se diferencia de un Árbol Binario.

	Respuesta: Un ABB es un tipo de dato abstracto sub-conjunto del arbol binario
	, el cual es utilizado para almacenar y buscar datos de forma más óptima. La
	diferencia principal entre estos dos TDA´s es el criterio de ordenamiento, en
	el cual, cada nodo del ABB tiene los nodos menores de un lado y los mayores 
	del otro, mientras que arbol binario no se ordena, lo cual se ve reflejado al
	analizar la complejidad algorítmica de las primitivas de estas.


	P2-Explicar cuál es el objetivo de tener una función de destrucción en el TDA 
	y qué implicaría para el usuario no tenerla.

	Respuesta: El objetivo del destructor es poder abstraerse más de los datos 
	contenidos al momento de borrar el nodo o el arbol, delegandole al usuario
	la tarea de destruir lo que sea que haya almacenado, donde lo único que debe- 
	mos hacer como programador del ABB es llamar dicha función pasándole el dato
	contenido antes de eliminar cada nodo.
		Por su parte, si el usuario no provee el destructor, tendrá que limitarse a
	almacenar datos en memoria estática. 


	P3-¿Cuál es la complejidad de las diferentes operaciones del ABB? Justifique.

	Respuesta: 

	Aclaración: Para este cálculo tomaré "n" como el número de elementos y:
			*O(a) la complejidad algorítmica de la función comparador()
			*O(b) la complejidad algorítmica de la función destructor()
			*O(c) la complejidad algorítmica de la función calloc()
			*O(d) la complejidad algorítmica de la función pasada al iterador interno
	Ya que son funciones externas donde no conozco la implementación.

		-arbol_crear: O(c). Lo cual surge de sumar las complejidades de cada linea,
	que resulta O(1) + O(c), por lo tanto al sumar queda O(c).

		-arbol_insertar: O(a).O(log(n)). Lo cual surge de sumar las complejidades de
	cada linea, que resulta O(1) + O(c) +( O(a).O(log(n)) ) donde este último 
	término es la complejidad de abb_insertar_recursivo, por lo tanto al sumar 
	queda el máximo entre los tres terminos O(a).O(log(n)), porque tardaría lo 
	que tarda el comparar, log(n) veces.

		-arbol_borrar: Máximo entre O(a).O(log(n)) y O(b). Lo cual surge de sumar 
	las complejidades de cada linea, que resulta O(1) + ( O(a).O(log(n)) + O(b)) 
	donde este último término es la complejidad de abb_borrar_recursivo, por lo 
	tanto al sumar queda el máximo entre O(a).O(log(n)) y O(b), porque tardaría 
	lo que tarda el comparar, log(n) veces y a eso se le suma el destructor una 
	vez.

		-arbol_buscar: O(a).O(log(n)). Porque tardaría lo que tarda el comparar
	log(n) veces.

		-arbol_raiz: O(1). Porque solo retorna el contenido de la raiz del arbol
	accediendo al propio struct.

		-arbol_vacio: O(1). Porque solo verifica que el árbol o su raiz sean NULL
	accediendo al propio struct.
	
		-arbol_recorridos:O(n). Todos los recorridos recorren(valga la redundancia)
	todos los elementos del árbol.

		-arbol_destruir: n.O(b). Porque recorre todo el arbol, aplicándole el des-
	tructor a cada elemento

		-abb_con_cada_elemento: n.O(b). Porque recorre todo el arbol, aplicándole la 
	función pasada por parametro a cada elemento.

	
	OBSERVACIÓN: Todas estas complejidades están basadas en arboles promedio, pero
	este árbol puede degenerar en lista, entonces ahí las funciones insertar, 
	borrar y buscar tenderían a O(n).



NOTA ADICIONAL: Al momento de plantear una prueba en "probar_arbol_borrar" del 
archivo abb_pruebas.c, especificamente la de la linea 173, tenía perdida de 
memoria, lo revisé con gdb y encontré que el problema está con la función de 
predecesor_innorden, donde yo paso un nodo_abb_t* hijo_aux, para guardar el 
posible hijo a izquierda del predecesor inorden y luego seguir concadenando los
nodos, y observé que cuando encuentra el predecesor, efectivamente guardo el 
hijo izquierdo de él en hijo_aux(linea 92 de abb.c), pero cuando vuelvo al ám-
bito anterior hijo_aux es apunta a NULL, y a partir de ahí empieo a perder las
referencias.

	Quiero acotar que pude haber saltado esta prueba, ya que igual pasé todas las
de kwijibo, pero la verdad quiero entender que está pasando en ese caso.

Muchas gracias y saludos.

Luis Escalante.