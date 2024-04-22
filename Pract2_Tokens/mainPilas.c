#include <stdlib.h>
#include "pilas.h"

void manejaError(int);

PILA crearPila(){
	PILA S;
	S=(PILA)malloc(sizeof(Pila));
	if(S==NULL){
		manejaError(0);
		exit(0);
	}
	S->tope=NULL;
}

int es_vaciaPila(PILA S){
	if(S->tope==NULL) return TRUE;
	else return FALSE;
}

Nodo_Pila * crearNodoPila(){
	Nodo_Pila * nvo;
	nvo=(Nodo_Pila*)malloc(sizeof(Nodo_Pila));
	if(nvo == NULL){
		manejaError(0);
		exit(0);
	}
	return nvo;
}

char elemTope(PILA S){
	char v;
	if(es_vaciaPila(S)){
		manejaError(1);
		exit(0);
	}
	v = S->tope->dato;
	return v;
}

char desapilar(PILA S){
	char v;
	Nodo_Pila* aux;
	if(es_vaciaPila(S)){
		manejaError(1);
		exit(0);	
	}
	aux= S->tope;
	v = aux->dato;
	S->tope = aux->anterior;
	free(aux);
	return v;
}

void apilar(PILA S, char elem){
	Nodo_Pila *nvo;
	nvo = crearNodoPila();
	nvo->dato = elem;
	nvo->anterior = S->tope;
	S->tope = nvo;
}

