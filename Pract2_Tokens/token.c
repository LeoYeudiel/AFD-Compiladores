/* Forma de correr el programa
  gcc mainPilas.c token.c -o eje.exe
    Forma de ejecutar el programa
  ./eje.exe 5 
 */
/* Librerías a utilizar */
#include "pilas.h" //Tad Pila
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Declaración de la estructura para guardar los tokens */
typedef struct Token{
  char id[20]; /* OP, NUM, ID, ID_NOVALIDO, SIM_NOVALIDO */
  char *valor; /* +, 12993, ab, ABC, * */
} Token;

/* Prototipos de funciones 
manejaError: En caso de presentarse errores en la asignación de memoria 
introducirToken: Tipo de token, valor, se acumula el valor total de tokens
*/
void insertarFilaEnPila(FILE *, Pila *);
void IntercambiarPilas(Pila *, Pila *);
void manejaError(int);
void introducirToken(char *, char *, Token *, int *);
Token *aumenntartamanioLista(Token *, int *);
char *vaciarPilaEnCadena(Pila *);
void main(int argc, char *argv[]){
  //Creamos variables inciales con espacio de 1 para la cadena
  char *input = NULL, *cadenaTmp;
  int longitud, numTokens, i;
  char caracter;
  PILA Stack1, Stack2, StackAux;
  Token *listTokens = NULL;
  FILE *archivo;
  //Verificamos que haya introducido el numero de cadenas a evaluar
  if(argc != 2){
    printf("Ejecucion incorrecta. Debe de proporcionar el numero de cadenas a probar.\n");
    exit(0);
  }

  archivo=fopen(argv[1], "r");
  if(archivo==NULL){
      printf("\n El archivo no se pudo abrir");
      fclose(archivo);
      exit(0);
  }
  numTokens = 0;
  int l=0;
  Stack1 = crearPila();
  Stack2 = crearPila();
  StackAux = crearPila();
  while(feof(archivo)==0){
    cadenaTmp=NULL;
    //longitud = 1; 
    //listTokens = NULL;
    //Apilamos, pero el TAD Pila es LIFO (se almacenará al revés)
    insertarFilaEnPila(archivo, Stack1);
    //Usamos Stack2 para reordenar
    IntercambiarPilas(Stack1, Stack2);
    apilar(Stack2, '\0');
    printf("\n %d", Stack2->numElem);
    int tamCaracteres=Stack2->numElem;
    printf("\n ANTES DE VACIAR LA PILA EN CADENA");
    cadenaTmp=vaciarPilaEnCadena(Stack2);
    printf("\n DESPUES DE VACIAR LA PILA EN CADENA");
    printf("\n %s", cadenaTmp);
    if(es_vaciaPila(Stack2)==1) printf("\n VACIA");
    if(es_vaciaPila(Stack2)==0) printf("\n NO VACIA");
    //input[longitud] = '\0';
    //Restamos uno a la longitud ya que al final se tiene el caracter nulo
    //longitud--;
    //printf("\nDe la formula: '%s' sus tokens son:\n\n", input);
    
    //Analizamos todo, para ello creamos el arreglo de tokens que se piensa tener uno por cada caracter en el peores de los casos
    //listTokens = (Token*)malloc(longitud * sizeof(Token));
    while (!es_vaciaPila(Stack2)){
      i = 0;
      //cadenaTmp[i] = '\0';
      caracter = desapilar(Stack2);
      //printf("%c %d: ", caracter, caracter);
      //listTokens = (Token*)malloc((numTokens+1) * sizeof(Token));
      
      //Primer caso: si es una letra mayúscula y/o minúscula
      if((caracter >= 65  && caracter <= 90) || (caracter >= 97 && caracter <= 122)){
        apilar(StackAux, caracter);
        while(!es_vaciaPila(Stack2)){
          //Ya que se detectó una letra, debemos analizar si cuenta con otra. Si tiene más de dos será un ID_NO_VALIDO
          if((elemTope(Stack2) >= 65  && elemTope(Stack2) <= 90) || (elemTope(Stack2) >= 97 && elemTope(Stack2) <= 122)){
            caracter = desapilar(Stack2);
            apilar(StackAux, caracter);
          }else{
            break;
          }
        }
        apilar(StackAux, '\0');
        if(StackAux->numElem > 3){
          numTokens++;
          cadenaTmp=vaciarPilaEnCadena(StackAux);
          printf("\n TOKEN ID NO VALIDO\n");
          introducirToken("ID_NO_VALIDO", cadenaTmp, listTokens, &numTokens);
        }else{
          numTokens++;
          printf("\n OKEN ID VALIDO\n");
          introducirToken("ID", cadenaTmp, listTokens, &numTokens);
        }

      //Segundo caso: Detectar si es un número
      }else if(caracter >= 48  && caracter <= 57){
        apilar(StackAux, caracter);
        while(!es_vaciaPila(Stack2)){
          //Aquí no importa la longitud del número, cuando deje de detectar numeros, se verificara si se trata de otro símbolo
          if(elemTope(Stack2) >= 48  && elemTope(Stack2) <= 57){
            caracter = desapilar(Stack2);
            apilar(StackAux, caracter);
          }else{
            break;
          }
        }
        apilar(StackAux, '\0');
        cadenaTmp=vaciarPilaEnCadena(StackAux);
        printf("\n TOKEN NUMERO VALIDO\n");
        numTokens++;
        introducirToken("NUMERO", cadenaTmp, listTokens, &numTokens);
        //Tercer caso: Detectar si es un símbolo de operación + o -
      }else if(caracter == '+' || caracter == '-'){
        apilar(StackAux, caracter);
        apilar(StackAux, '\0');
        cadenaTmp=vaciarPilaEnCadena(StackAux);
        printf("\n TOKEN OPERADDOR VALIDO\n");
        numTokens++;
        introducirToken("OPERADOR", cadenaTmp, listTokens, &numTokens);//EL PROBLEMA ESTA CUANDO SE INTRODUCE UN SIMBOLO
        
        //Cuarto caso: Detecta un espacio, lo ignora
      }else if(caracter == 32){
      }
        //Quinto caso: Cualquier otro caracter que no pertenezca a los casos anteriores, es INVÁLIDO
      else{
        apilar(StackAux, caracter);
        apilar(StackAux, '\0');
        cadenaTmp=vaciarPilaEnCadena(StackAux);
        printf("\n TOKEN SIM NO VALIDO\n");
        numTokens++;
        introducirToken("SIM_NO_VALIDO", cadenaTmp, listTokens, &numTokens);
      }
    }
    //Liberamos memoria
    free(cadenaTmp);
    //l++;
  }
  free(Stack1);
  free(Stack2);
  //free(listTokens);
  //printf("\n\n");
  //Impresión de los tokens
  /*for (i = 0; i < numTokens; i++){
    printf("\t< %s, %s >\n", listTokens[i].id, listTokens[i].valor);
  }*/
}

void introducirToken(char *tipoToken, char *cadenaTmp, Token *listTokens, int *numTokens){
  printf("\n antes de aumentra tamaño de lista de tokens\n");
  listTokens=aumenntartamanioLista(listTokens, numTokens);
  printf("\n DESPUES de aumentra tamaño de lista de tokens\n");
  strcpy(listTokens[(*numTokens)].id, tipoToken);
  listTokens[(*numTokens)].valor = (char*)malloc((strlen(cadenaTmp)) * sizeof(char));
  strcpy(listTokens[(*numTokens)].valor, cadenaTmp);
  printf("\n TOKEN LLENO\n");
}

void manejaError(int opcion){
	char *mensaje[] = {"No hay memoria disponible", "Pila vacia", "Liberando memoria", "Saliendo del programa", "Estacionamiento lleno"};
	printf("\n%s\n", mensaje[opcion]);
}
//Inserta cierta fila de un archivo de texto en una pila y devuelve la cantidad
//de elementos que encontró separados por una coma. Se lee hasta el salto de línea de la fila
void insertarFilaEnPila(FILE *archivo, Pila *Stack){
	int codigo=1;
	char letra;
	//Mientras no se llegue al final de la fila (Al salto de línea) metemos todos los 
	//caracteres a la pila
	while(codigo!=10){
			letra=fgetc(archivo);
			codigo=letra;
			//Se insertan todos los carecteres a la pila excepto
			//los espacios y los saltos de línea
			if(codigo!=32 && codigo!=10){
				apilar(Stack, letra);
			}			
	}
}

void IntercambiarPilas(Pila *Stack1, Pila *Stack2){
	char letra;
	int codigo, tamEstado=0, x=0;
	while(es_vaciaPila(Stack1)!=1){
		letra=desapilar(Stack1);
		codigo=letra;
		apilar(Stack2, letra);
	}
}
//Aumenta en uno el tamaño de la lista de tokens para poder introducir posteriormente uno
Token *aumenntartamanioLista(Token *listTokens, int *numTokens){
  listTokens = (Token*)realloc(listTokens, (*(numTokens+1)) * sizeof(Token));
  return listTokens;
}
char *vaciarPilaEnCadena(Pila *StackAux){
  int j=0;
  char *cadenatmp;
  //Los elementos en la pila StackAux ya vienen ordenados
  //abrimos espacio en memoria para almacenar la cadena correspondiente que tenga S1 
  cadenatmp=(char*)malloc((StackAux->numElem)*sizeof(char));
  if (cadenatmp == NULL) {
    printf("Error: No se pudo asignar memoria\n");
    return NULL;
  }
  //vaciamos StackAux en cadenatmp 
  while(!es_vaciaPila(StackAux)){
    cadenatmp[j++]=desapilar(StackAux);
  }
  return cadenatmp;
}
