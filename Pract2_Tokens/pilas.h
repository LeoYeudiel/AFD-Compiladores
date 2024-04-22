#ifndef _pilaDina_
#define _pilaDina_
#define TRUE 1
#define FALSE 0

typedef struct Nodo_Pila{
	char dato;
	struct Nodo_Pila *anterior;
}Nodo_Pila;

typedef struct{
	Nodo_Pila *tope;
	int numElem;
}Pila;

typedef Pila *PILA;

PILA crearPila();
int es_vaciaPila(PILA);
Nodo_Pila * crearNodoPila();
void apilar(PILA, char);
char desapilar(PILA);
char elemTope(PILA);

#endif