#include<stdio.h>
#include<stdlib.h>
#include "listas.h"

LISTA crearLista();
Nodo_Lista *crearNodoLista();
void insertar(LISTA *L, char *);
int es_vaciaLista(LISTA L);

//Creación de una lista vacía:
LISTA crearLista(){
    LISTA L;
    L=NULL;
    return L;
}

//Verifica si la lista está vacía
int es_vaciaLista(LISTA L){
    if(L==NULL)
     return TRUE;
    return FALSE;
}

//Crea un elemento de la lista
Nodo_Lista *crearNodoLista(){
    Nodo_Lista *nvo;
    nvo=(Nodo_Lista*)malloc(sizeof(Nodo_Lista));
    if(nvo==NULL){
       // manejaError(0);
        exit(0);
    }
    return nvo;
}
//Inserta un nuevo nodo a la lista
void insertar(LISTA *L, char *produccion){
    Nodo_Lista *nvo;
    Nodo_Lista *actual;
    nvo =crearNodoLista();
    nvo->regla=produccion;
    if(es_vaciaLista(*L)==TRUE){
        nvo->siguiente=NULL;
        *L=nvo;
    }else{
        actual=*L;
        //recorre toda la lista para inserrtar un nuevo elemento
        while(actual->siguiente!=NULL){
            actual=actual->siguiente;
        }
        nvo->siguiente=NULL;
        actual->siguiente=nvo;
    }
}