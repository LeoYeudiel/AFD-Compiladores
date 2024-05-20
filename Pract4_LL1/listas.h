#ifndef _listaDina_
#define _listaDina_
#define TRUE 1
#define FALSE 0

typedef struct Nodo_Lista{
	char *regla;
	struct Nodo_Lista *siguiente;
}Nodo_Lista;

typedef Nodo_Lista *LISTA;

LISTA crearLista();
Nodo_Lista *crearNodoLista();
void insertar(LISTA *L, char *);
int es_vaciaLista(LISTA L);

#endif