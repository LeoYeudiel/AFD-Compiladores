/* Librerías a utilizar */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Declaración de Estructuras.

Observando la descripción del autómata en forma de tabla
La estructura Estados representa un estado asociado a una fila
En cada fila se encuentra un array de elementos que describen al autómata*/

typedef struct Estados{
    char *estado; //puntero a una cadena de caracteres
    int *fila;
	int aceptacion;
	char **filaEst;
}Estados;

typedef struct Nodo_Pila{
    char letra;
    struct Nodo_Pila *anterior;
}Nodo_Pila;

typedef struct{
	Nodo_Pila *tope;
}Pila;

/* Prototipos de funciones */
int menuAfd(FILE *, int, Estados **, char ***);
void navegar(FILE *, int *, Estados **, char ***);
void comments(FILE *, int *, char);
int operaciones(FILE *, char, Estados **, char ***);
int deleteLines(FILE *, char);
char saltoComentarios(FILE *, char);
int es_vaciaPila(Pila *);
Nodo_Pila *crearNodoPila();
char elemTope(Pila *);
void apilar(Pila *, char);
char desapilar(Pila *);

int insertarFilaEnPila(FILE *, Pila *);
int IntercambiarPilas(Pila *, Pila *);
int compararCadenas(char *, char *);
int busquedaEstado(Estados **, char *, int);
void AbrirEspacioFilas(Estados **, int, int);
int IntercambiarPilasDelta(Pila *, Pila *);
int busquedaSimbolo(char ***, char *, int);
Pila *CrearPila();
int RecorridoAFD(Pila *, Estados *, char **);

int main(int argc, char *argv[]){
    /* Verificamos de que nos haya proprocionado el archivo donde viene el autómata con sus específicaciones  */
    if (argc != 2) {
        printf("Uso incorrecto. Debe proporcionar un nombre de archivo como argumento.\n");
        return 1;
    }
    /* Abrimos el archivo en modo lectura */
    FILE *archivo;
    archivo=fopen(argv[1], "r");
    if(archivo==NULL){
        printf("\n El archivo no se pudo abrir");
        fclose(archivo);
        return 1;
    }
      
    /* Declaramos variables para poder ir guardando información  */
    char letra;
    int codigo, i, NumEstados, numSimbolos, j, EstadoPos;
    letra = fgetc(archivo);
    codigo=letra;
    Estados *columnaEstados=NULL;
    //SIMBOLOS VA A SER UN ARREGLO DE PUNTEROS A CARACTERES
    char **Simbolos=NULL;
    navegar(archivo, &codigo, &columnaEstados, &Simbolos); //Pasamos la dirección de memoria de los arrays
    NumEstados = *(columnaEstados->fila);
    numSimbolos = *((columnaEstados+1)->fila);
    printf("\n === REPRESENTACION DEL AFD ===\n \n");
    printf("       |   ");
    for(i=0;i<numSimbolos;i++){
      printf("%s    ", *(Simbolos+i));
    }
    printf("\n_ _ _ _ _ _ _ _ _ _ _ ");
    for(i=0;i<NumEstados;i++){
      printf("\n");
      if(i==0){
        if((columnaEstados+i)->aceptacion==1){
          printf("-> *%s |", columnaEstados->estado);
        }else{
          printf("->  %s |", columnaEstados->estado);
        }
      }else{
        if((columnaEstados+i)->aceptacion==1){
          printf("   *%s |", (columnaEstados+i)->estado);
        }else{
          printf("   %s |", (columnaEstados+i)->estado);
        }
        
      }
      for(j=0;j<numSimbolos;j++){
        printf("  %s", *(((columnaEstados+i)->filaEst)+j));
      }
      
    }
    printf("\n_ _ _ _ _ _ _ _ _ _ _ \n");
    Pila *Stack, *Stack2;
    Stack=CrearPila();
    Stack2=CrearPila();
    
    printf("\n Inserta la cadena W: \n");
    while((letra=getchar()) != '\n'){
      apilar(Stack, letra);
    }
    IntercambiarPilas(Stack, Stack2);//OBTENEMOS LA CADENA ORIGINAL EN EL ORDEN CORRECTO
    EstadoPos=0;
    EstadoPos=RecorridoAFD(Stack2, columnaEstados, Simbolos);
    if((columnaEstados+EstadoPos)->aceptacion==1){
      printf("\n LA CADENA W SI PERTENECE AL LENGUAJE\n");
    }else{
      printf("\n LA CADENA W NO PERTENECE AL LENGUAJE\n");
    }

    /* Liberamos memoria */
    free(columnaEstados->fila);
    free((columnaEstados+1)->fila);
    free(Stack);
    free(Stack2);
    free(columnaEstados);
    free(Simbolos);
}

/* 
  Función para la detección de un comentario del archivo de lectura
  Argumentos:
  - archivo: descriptor del archivo de lectura
  - flag: bandera de que se haya puesto un comentario de bloque completo, tanto abierto y cerrado
  - letra:
 */
void comments(FILE *archivo, int *flag, char letra){
  /* Detectamos el patron de un comentario de bloque */
	if(letra=='/'){
		letra=fgetc(archivo);
    		if(letra=='*'){
          /* Leemos hasta detectar el cierre del comentario */
    			while(feof(archivo)==0){
    				letra=fgetc(archivo);
    				if(letra=='*'){
    					letra=fgetc(archivo);
    					if(letra=='/'){
                /* Una vez detectado, alzamos la bandera y volvemos a la función que llamo esta función */
    						letra=fgetc(archivo);
    						*flag=1;
    						return;
    					}
    				}
    			}
    		}
    }
    /* En caso de no haber detectado ninguna de las condiciones anteriores, volvemos a la función que llamo esta función */
    	return;
}

/* 
  Función para poder leer después de un salto de línea.
  Argumentos:
    - archivo: descriptor de archivo de lectura
    - letra: carácter a analizar del archivo
 */
int deleteLines(FILE *archivo, char letra){
	int codigo;
	codigo=letra;
  /* Leemos hasta leer un carácter diferente al salto de línea */
	while ((letra = fgetc(archivo)) != EOF){
    	codigo=letra;
    	if(letra!=10)
    		return codigo;
    }
	codigo=letra;
	return codigo;
}

/* 
  Función para...
  Argumentos:
    - archivo: descriptor de archivo de lectura
    - letra: carácter a analizar
    - columnaEstados:
    - Simbolos
*/
int operaciones(FILE *archivo, char letra, Estados **columnaEstados, char ***Simbolos){
	int codigo, i;
	codigo=letra;//Ya se tiene la primer letra leida
	for(i=0;i<2;i++){
		letra=fgetc(archivo);
		codigo+=letra;
	}
	for(i=0;i<2;i++){
		letra=fgetc(archivo);
	}

	codigo=menuAfd(archivo, codigo, columnaEstados, Simbolos);//Se espera que menuAfd lea el primer elemento de los conjuntos
	return codigo;
}

/* 
  Función para seguir la lectura en caso de la detección de un comentario
  Argumentos:
  - archivo: descriptor del archivo a leer
  - letra: carácter del cual se está leyendo
 */
char saltoComentarios(FILE *archivo, char letra){
	int flag, codigo;
	flag=0;
    comments(archivo, &flag, letra);
    if(flag==1){//Si se encontro con un comentario, empezamos a leer después de él
    	letra=fgetc(archivo);
    	flag=0;
    }
    codigo=letra;
	return codigo;
}

/* 
  Función para la lectura del archivo
  Argumentos:
  - archivo: descriptor del archivo a leer
  - código:
  - columnaEstados:
  - Símbolos:
 */
void navegar(FILE *archivo, int *codigo, Estados **columnaEstados, char ***Simbolos){
    char letra;
	  letra=(char)(*codigo);
    /* Realizamos un ciclo para la lectura de todo el archivo */
    while(feof(archivo)==0){
      /* Actualizamos la posición de lectura en caso de la lectura de un comentario en el archivo */
      *codigo=saltoComentarios(archivo, letra);
      letra=(char)(*codigo);
      /* En caso de detectar una barra diagonal volvemos a verificar si hay otro comentario */
      if(*codigo==47){
        navegar(archivo, codigo, columnaEstados, Simbolos);
      }

      /* Si no hemos recibido un carácter, retornamos valor */
      if(*codigo<0)
        return;
      
      /* En caso de detectar saltos de línea */
      if(*codigo==10){
        letra=(char)(*codigo);
        *codigo=deleteLines(archivo, letra);//recibo el nuevo codigo, lo que haya después de los saltos de linea
        /* Volvemos a leer para poder verificar más del archivo */
        navegar(archivo, codigo, columnaEstados, Simbolos);
        /* En caso si ya no hemos recibido un carácter, retornamos valor */
        if(*codigo<0)
          return;
          /* Caso contrario identificamos que parte del autómata es */
      }else{
        *codigo=operaciones(archivo, letra, columnaEstados, Simbolos);//Se le manda la letra con la que comienza la siguiente operacion
        /* En caso si ya no hemos recibido un carácter, retornamos valor */
        if(*codigo<=0)
          return;
      }	
      /* Volvemos a leer para la detección de algún otro carácter que se nos haya pasado */
      navegar(archivo, codigo, columnaEstados, Simbolos);
      /* En caso si ya no hemos recibido un carácter, retornamos valor */
      if(*codigo<=0)
          return;
    }
}

/* Función para...
  Argumentos:
    - archivo: descriptor del archivo de lectura
    - codigo: detección del tipo de información a considerar
    - columnaEstados: estructura que contiene la tabla de transciones
    - Simbolos: Guarda todo el alfabeto aqui
 */
int menuAfd(FILE *archivo, int codigo, Estados **columnaEstados, char ***Simbolos){
	char letra;
	int i;
	Pila *Stack, *Stack2;
	int numEstados, x=0, tamEstado, numSimbolos, tamSimbolo;
  char *estadoIni, *aux;
  int EstadoBuscado;
	char *estadoAceptacion;
  char *EstadoAct;
	Pila *Stack3, *Stack4;
	int iterador, posSimbolo, posEstado, contador;
	long posicionInicial;
	int numLineas;
  int caracterActual;
	switch (codigo){
	case 207: //Estados
		Stack=CrearPila();
		Stack2=CrearPila();
		//Se cuenta el número de estados del autómata
		numEstados=insertarFilaEnPila(archivo, Stack);
		//Se abre espacio en memoria para la estructura que representa la columna de estados
		*columnaEstados=(Estados*)malloc(numEstados*sizeof(Estados));
		//En el primer elemento del array de estados se asocia el tamaño total del arreglo
		(*columnaEstados)->fila=(int*)malloc(sizeof(int));
		*((*columnaEstados)->fila)=numEstados;
		//Desapilamos stack para apilar stack2 y saber el tamaño de cada estado (creación de la matriz)
		x=0;
		while(es_vaciaPila(Stack)!=1){
			//Que cantidad de caracteres hay en un estado
			tamEstado=IntercambiarPilas(Stack, Stack2);
			((*columnaEstados)+((numEstados-x)-1))->estado=(char*)malloc((tamEstado+1)*sizeof(char));
			x++;
		}
		
		//En este punto ya se apiló a stack2 que esta en el orden adecuado
		//Se comienzan a guardar los caracteres
		//x=0 iterador sobre la cadena
		//numEstados=0 Iterador sobre structs
		numEstados=0;
		x=0;
		while(es_vaciaPila(Stack2)!=1){
			letra=desapilar(Stack2);
			if(letra==44){
				//Se comienza otra cadena
				*((((*columnaEstados)+numEstados)->estado)+(x))='\0';
				(((*columnaEstados)+numEstados)->aceptacion)=0;
				numEstados++;
				x=0;
			}else{
				*((((*columnaEstados)+numEstados)->estado)+(x))=letra;
				x++;
			}
		}
    /* Liberamos memoria de las estructuras utilizadas */
		free(Stack);
		free(Stack2);
		break;
	case 249: //Alfabeto
		numSimbolos=1, tamSimbolo;
		Stack=CrearPila();
		Stack2=CrearPila();
		//Se inserta toda la fila en la pila Stack
		numSimbolos=insertarFilaEnPila(archivo, Stack);
		//Se abre espacio en memoria para la estructura que representa la columna
		*Simbolos=(char**)malloc(numSimbolos*sizeof(char*));
		//En el segundo elemento del array de estados se asocia el tamño total del arreglo del simbolos
		((*columnaEstados+1))->fila=(int*)malloc(sizeof(int));
		*(((*columnaEstados)+1)->fila)=numSimbolos;
		//Desapilamos stack para apilar stack2 y saber el tamaño de cada estado
		x=0; 
		while(es_vaciaPila(Stack)!=1){
			//Que cantidad de carácteres hay un estado
			tamSimbolo=IntercambiarPilas(Stack, Stack2);
			(*((*Simbolos)+((numSimbolos-x)-1)))=(char*)malloc((tamSimbolo+1)*sizeof(char));
			x++;
		}
		//En este punto ya se apilo a stack2 que esta en el orden adecuado
		//Se comienza a guardar los caracteres
		//Dos iteradores: uno que opere sobre el arreglo de estructuras y el otro sobre las cadena de caracteres individuales de cada struct
		numSimbolos=0;
		x=0;
		while(es_vaciaPila(Stack2)!=1){
			letra=desapilar(Stack2);
			if(letra==44){
				//Si detecta una , se comienza otra cadena
				*((*((*Simbolos)+numSimbolos))+x)='\0';
				x=0;
				numSimbolos++;
			}else{
        /* Se agrega el caracter al simbolo */
				*((*((*Simbolos)+numSimbolos))+x)=letra;
				x++;
			}
      /* Se agrega caracter nulo al final de lo que nos quedamos */
			if(es_vaciaPila(Stack2)==1)
				*((*((*Simbolos)+numSimbolos))+x)='\0';
		}

    /* Liberamos memoria de las estructuras utilizadas */
		free(Stack);
		free(Stack2);
		break;
	case 219: //Estados iniciales
		Stack=CrearPila();
		Stack2=CrearPila();
		//Se inserta el estado en la pila Stack
		insertarFilaEnPila(archivo, Stack);
		//DESAPILAMOS STACK PARA APILAR STACK2 Y SABER EL TAMAÑO DEL ESTADO
		//CANTIDAD DE CARACTERES EN EL ESTADO
		tamEstado=IntercambiarPilas(Stack, Stack2);
		estadoIni=(char*)malloc((tamEstado+1)*sizeof(char));
		//SE DESAPILA STACK2 Y LOS CARACTERES SON GUARDADOS EN estadoIni
		x=0; 
		while(es_vaciaPila(Stack2)!=1){
			letra=desapilar(Stack2);
			*(estadoIni+x)=letra;
			x++;
			
		}
		*(estadoIni+x)='\0';
		x=busquedaEstado(columnaEstados, estadoIni, *((*columnaEstados)->fila));
		//ES INTERCAMBIADO EL ESTADO DE LA PRIMERA FILA CON EL ESTADO INICIAL CORREPONDIENTE
		aux=(*columnaEstados)->estado;
		(*columnaEstados)->estado=((*columnaEstados)+x)->estado;
		((*columnaEstados)+x)->estado=aux;
    /* Liberamos memoria de las estructuras utilizadas */
		free(Stack);
		free(Stack2);
		free(estadoIni);
		break;
	case 195: //Estados de acepatcion
		Stack=CrearPila();
		Stack2=CrearPila();
		//Se inserta toda la fila de estados de aceptación en la pila Stack
		insertarFilaEnPila(archivo, Stack);
		
		while(es_vaciaPila(Stack)!=1){
			//QUE CANTIDAD DE CARACTERES HAY EN UN ESTADO ESPECIFICO
			//DESAPILAMOS STACK PARA APILAR STACK2 Y SABER EL TAMAÑO DE CADA ESTADO
			tamEstado=IntercambiarPilas(Stack, Stack2);
			estadoAceptacion=(char*)malloc((tamEstado+1)*sizeof(char));
			x=0;
			//EN STACK2 SE ENCUENTRA EL ESTADO ACTUAL
			while(es_vaciaPila(Stack2)!=1){
				letra=desapilar(Stack2);
				if(letra!=44){
					*(estadoAceptacion+x)=letra;
					x++;
				}
				if(es_vaciaPila(Stack2)==1){
					//SE TIENE A UN ESTADO COMPLETO, AHORA SE BUSCA PARA SEÑALAR QUE ES ESTADO DE ACEPTACION
					*(estadoAceptacion+x)='\0';
					EstadoBuscado=busquedaEstado(columnaEstados, estadoAceptacion, *((*columnaEstados)->fila));
					((*columnaEstados)+EstadoBuscado)->aceptacion=1;
				}
			}

		}
		AbrirEspacioFilas(columnaEstados, *(((*columnaEstados)+1)->fila), *(((*columnaEstados))->fila));
    /* Liberamos memoria de las estructuras utilizadas */
		free(Stack);
		free(Stack2);
		free(estadoAceptacion);
		break;
	case 204: //Función delta
	contador=0;
	posicionInicial = ftell(archivo);
	numLineas = 0;

    // Usa un bucle para contar las líneas restantes sin avanzar la posición
    while ((caracterActual = fgetc(archivo)) != EOF) {
        if (caracterActual == '\n') {
            numLineas++;
        }
    }
	numLineas=numLineas/2;
    // Restaura la posición original
    fseek(archivo, posicionInicial, SEEK_SET);
	while(contador<numLineas){
		Stack=CrearPila();
		Stack2=CrearPila();
		Stack3=CrearPila();
		Stack4=CrearPila();
		insertarFilaEnPila(archivo, Stack3);
		tamEstado=IntercambiarPilas(Stack3, Stack4);
		EstadoAct=(char*)malloc(tamEstado*sizeof(char));
		x=0; 
		while(es_vaciaPila(Stack4)!=1){
			letra=desapilar(Stack4);
			codigo=letra;
			if(codigo!=58){
				*(EstadoAct+x)=letra;
				x++;
			}
		}
		*(EstadoAct+x)='\0';
		//posEstado es posicion en la que se encuentra el estado buscado
		posEstado=busquedaEstado(columnaEstados, EstadoAct, *((*columnaEstados)->fila));
		free(EstadoAct);
		//SE LEE LA SIGUIENTE LINEA QUE CORRESPONDE A LAS TRANSICIONES
		insertarFilaEnPila(archivo, Stack3);
		//STACK 3 CONTIENE LA INFORMACION DE TRANSICION DE UN ESTADO EN PARTICULAR
		while(es_vaciaPila(Stack3)!=1){
				//SE ELIMINAN PARENTESIS (SON LAS FUNCIONES DELTA)
				IntercambiarPilas(Stack3, Stack4);//STACK4 OBTIENE LAS FUNCIONES TRANSICION
				tamSimbolo=IntercambiarPilasDelta(Stack4, Stack);//SE QUITA LA PARTE DEL SIMBOLO DE LA FUNCION DELTA
				//(EN STACK4 SE QUEDA EL ESTADO, EN STACK SE QUEDA EL SIMBOLO)
				
				//CADENA DE CARACTERES QUE CONTIENE EL SIMBOLO CORRESPONDIENTE
				EstadoAct=(char*)malloc((tamSimbolo+1)*sizeof(char));
				IntercambiarPilas(Stack, Stack2);//STACK QUEDA VACÍA Y STACK2 CONTIENE EL ORDEN CORRECTO DE LA CADENA(simbolo)
				//SE LLENA LA CADENA DE CARACTERES
				iterador=0;
				while(es_vaciaPila(Stack2)!=1){
					letra=desapilar(Stack2);
					*(EstadoAct+iterador)=letra;
					iterador++;
				}
				*(EstadoAct+iterador)='\0';
				//SE BUSCA UN SIMBOLO IGUAL AL QUE TIENE EstadoAct
				posSimbolo=busquedaSimbolo(Simbolos, EstadoAct, *(((*columnaEstados)+1)->fila));
				//printf("\n Posicion del simbolo: %d \n", posSimbolo);
				//SE LEE EL ESTADO AL QUE DEBE DIRIGIRSE QUE SE ENCUENTRA EN STACK4
				free(EstadoAct);
				iterador=0;
				while(es_vaciaPila(Stack4)!=1){
					letra=desapilar(Stack4);
					codigo=letra;
					if(codigo != 41){
						apilar(Stack, letra);
					}
					iterador++;
				}
				//Stack4 QUEDA VACIA (SE ACABA DE TERMINAR UNA FUNCION TRANSICION)
				*((((*columnaEstados)+posEstado)->filaEst)+posSimbolo)=(char*)malloc(iterador*sizeof(char));
				IntercambiarPilas(Stack, Stack2);
				iterador=0;
				while(es_vaciaPila(Stack2)!=1){
					letra=desapilar(Stack2);
					*(*((((*columnaEstados)+posEstado)->filaEst)+posSimbolo)+iterador)=letra;
					iterador++;
				}
				*(*((((*columnaEstados)+posEstado)->filaEst)+posSimbolo)+iterador)='\0';
		}
		contador++;
    /* Liberamos memoria de las estructuras utilizadas */
		free(Stack);
		free(Stack2);
		free(Stack3);
		free(Stack4);
	}	
		break;
	default:
		break;
	}
	letra=fgetc(archivo);//Se lee lo que está depués del salto de línea
	codigo=letra;
	return codigo;
}



//OPERACIONES CON LA PILA:
// ESTA VACÍA LA PILA
int es_vaciaPila(Pila *S){
	if(S->tope == NULL)
		return 1;
	return 0;
}

// CREA UN ELEMENTO DE LA PILA
Nodo_Pila * crearNodoPila(){
   Nodo_Pila * nvo;
   nvo = (Nodo_Pila *)malloc(sizeof(Nodo_Pila));
   if(nvo == NULL){
      exit(0); 
    }
   return nvo;
}

// AGREGAR ELEMENTOS A LA PILA
void apilar(Pila *S, char caracter){
        Nodo_Pila * nvo;
        nvo = crearNodoPila();	
        nvo->letra = caracter;
        nvo ->anterior = S->tope;
        S-> tope = nvo;
}

// ELIMINAR UN ELEMENTO DE LA PILA
char desapilar(Pila *S){
	char caracter;
    Nodo_Pila *aux;
	if( es_vaciaPila(S)==1){
		exit(0);
	}
	aux = S->tope;
    caracter=aux->letra;
	S->tope = aux -> anterior;
    free(aux);
	return caracter;
}

/* 
  Función para guardar temporalmente información en una pila
  Argumentos:
    - archivo: descriptor del archivo de lectura
    - Stack: adjuntar información de línea
 */
int insertarFilaEnPila(FILE *archivo, Pila *Stack){
	int codigo=1, numEstados=1;
	char letra;
  /* Mientra no sea una salto de línea se lee la información */
	while(codigo!=10){
			letra=fgetc(archivo);
			codigo=letra;
			if(codigo!=32 && codigo!=10){
				apilar(Stack, letra);
			}
      /* En caso de leer una , se añade el contador del número de estados a considerar en cuenta */
			if(codigo==44){
				numEstados++;
			}
	}
	return numEstados;
}

/*
  Función que regresa el tamaño de un estado particular. Lo hace cuando encuentra una , o se ha vaciado la pila
  Argumentos:
    - Stack1:
    - Stack2:
  */
int IntercambiarPilas(Pila *Stack1, Pila *Stack2){
	char letra;
	int codigo, tamEstado=0, x=0;
	while(es_vaciaPila(Stack1)!=1){
			letra=desapilar(Stack1);
			codigo=letra;
			apilar(Stack2, letra);
			if(codigo!=44){
				tamEstado++;
			}else{
				return tamEstado;
			}
			//El caso en el que se inserta el primer estado
			if(es_vaciaPila(Stack1)==1){
				return tamEstado;
			}
			
	}
}

int IntercambiarPilasDelta(Pila *Stack1, Pila *Stack2){
	char letra;
	int codigo, tamEstado=0, x=0;
	while(es_vaciaPila(Stack1)!=1){
			letra=desapilar(Stack1);
			codigo=letra;
			if(codigo != 44 && codigo !=40 && codigo !=41 && codigo !=45){
				apilar(Stack2, letra);
				tamEstado++;
			}else if(codigo==45){
				return tamEstado;
			}
			//El caso en el que se inserta el primer estado
			if(es_vaciaPila(Stack1)==1){
				return tamEstado;
			}
			
	}
}

int compararCadenas(char *cadena1, char *cadena2){
	char letra1=cadena1[0], letra2=cadena2[0];
	int x=0;
	while(letra1 != '\0'){
		letra1=cadena1[x];
		letra2=cadena2[x];
		if(letra1 != letra2)
			return 0;
		x++;
	}
	if (letra1==letra2)
		return 1;
}

int busquedaEstado(Estados **columnaEstados, char *cadena, int NumEstados){
	int i, res;
	for(i=0;i<NumEstados;i++){
		res=compararCadenas((((*columnaEstados)+i)->estado), cadena);
		if(res==1)
			return i;
	}
	//SI NO SE ENCUENTRA ALGUN ESTADO QUE COINCIDA	
	printf("\n ERROR: No coinciden los estados \n");
	exit(0);
}

int busquedaSimbolo(char ***Simbolos, char *cadena, int NumSimbolos){
	int i, res;
	for(i=0;i<NumSimbolos;i++){
		res=compararCadenas(*((*Simbolos)+i), cadena);
		if(res==1)
			return i;
	}
	//SI NO SE ENCUENTRA ALGUN ESTADO QUE COINCIDA	
	printf("\n ERROR: No coinciden los simbolos \n");
	exit(0);
}
void AbrirEspacioFilas(Estados **columnaEstados, int numSimbolos, int CantEstados){
	int i;
	for(i=0;i<CantEstados;i++){
		((*columnaEstados)+i)->filaEst=(char**)malloc(numSimbolos*sizeof(char*));
	}
}
Pila *CrearPila(){
	Pila *S;
	S=(Pila*)malloc(sizeof(Pila));
	if(S==NULL){
		exit(0);
	}
	S->tope=NULL;
	return S;
	
}
int RecorridoAFD(Pila *Stack, Estados *columnaEstados, char **Simbolos){
	char *aux, *aux2;
	int fila=0;
	aux2=(char*)malloc(2*sizeof(char));
	while(es_vaciaPila(Stack)!=1){
		*aux2=desapilar(Stack);
		*(aux2+1)='\0';
		aux=*(((columnaEstados+fila)->filaEst)+busquedaSimbolo(&Simbolos, aux2, *((columnaEstados+1)->fila)));
		fila=busquedaEstado(&columnaEstados, aux, *(columnaEstados->fila));
	}
	return fila;
}