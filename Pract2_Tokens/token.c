#include "pilas.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Token{
  char id[20]; /* OP, NUM, ID, ID_NOVALIDO, SIM_NOVALIDO */
  char *valor; /* +, 12993, ab, ABC, * */
} Token;

void manejaError(int);

void main(){
  //Creamos variables inciales con espacio de 1 para la cadena
  char *input, cadenaTmp[100];
  int longitud = 1, numTokens = 0, i;
  char caracter;
  PILA Stack1, Stack2;
  Token *listTokens = NULL;

  printf("Introduzca la formula a analizar: ");
  //Recolectamos la cadena por cadena haciendo memoria dinamica
  while((caracter = getchar()) != '\n' && caracter != EOF){
    input = realloc(input, longitud * sizeof(char));
    if(input == NULL){
      printf("Error al asignar memoria a la cadena de lectura");
      exit(0);
    }
    input[longitud - 1] = caracter;
    input[longitud] = '\0';
    longitud++;
  }

  //Restamos uno a la longitud ya que al final no se agrega uno en la longitud
  longitud--;
  printf("De la formula: '%s' sus tokens son:\n", input);
  Stack1 = crearPila();
  Stack2 = crearPila();
  //Apilamos en forma inversa
  for (i = 0; i < longitud; i++){
    apilar(Stack1, input[i]);
  }

  //Obtenemos la forma ordenada
  while(!es_vaciaPila(Stack1)) {
    apilar(Stack2, desapilar(Stack1));
  } 

  
  //Analizamos todo, para ello creamos el arreglo de tokens que se piensa tener uno por cada caracter en los peores de los casos
  listTokens = malloc(longitud * sizeof(Token));
  while (!es_vaciaPila(Stack2)){
    i = 0;
    cadenaTmp[i] = '\0';
    caracter = desapilar(Stack2);
    //printf("%c %d: ", caracter, caracter);
    //listTokens = (Token*)malloc((numTokens+1) * sizeof(Token));
    
    //Detectar si es un caracter ya sea mayúscula y minúscula
    if((caracter >= 65  && caracter <= 90) || (caracter >= 97 && caracter <= 122)){
      cadenaTmp[i++] = caracter;
      while(!es_vaciaPila(Stack2)){
        if((elemTope(Stack2) >= 65  && elemTope(Stack2) <= 90) || (elemTope(Stack2) >= 97 && elemTope(Stack2) <= 122)){
          caracter = desapilar(Stack2);
          cadenaTmp[i++] = caracter;
        }else{
          break;
        }
      }
      cadenaTmp[i] = '\0';
      if(strlen(cadenaTmp) > 2){
        strcpy(listTokens[numTokens].id, "ID_NO_VALIDO");
        listTokens[numTokens].valor = malloc(strlen(cadenaTmp) * sizeof(char));
        strcpy(listTokens[numTokens].valor, cadenaTmp);
        numTokens++;
      }else{
        strcpy(listTokens[numTokens].id, "ID");
        listTokens[numTokens].valor = malloc(strlen(cadenaTmp) * sizeof(char));
        strcpy(listTokens[numTokens].valor, cadenaTmp);
        numTokens++;
      }
    //Detectar si es un número
    }else if(caracter >= 48  && caracter <= 57){
      cadenaTmp[i++] = caracter;
      while(!es_vaciaPila(Stack2)){
        if(elemTope(Stack2) >= 48  && elemTope(Stack2) <= 57){
          caracter = desapilar(Stack2);
          cadenaTmp[i++] = caracter;
        }else{
          break;
        }
      }
      cadenaTmp[i] = '\0';
      strcpy(listTokens[numTokens].id, "NUMERO");
      listTokens[numTokens].valor = malloc(strlen(cadenaTmp) * sizeof(char));
      strcpy(listTokens[numTokens].valor, cadenaTmp);
      numTokens++;
      // Detectar si es una operación
    }else if(caracter == '+' || caracter == '-'){
      cadenaTmp[i++] = caracter;
      cadenaTmp[i] = '\0';
      strcpy(listTokens[numTokens].id, "OPERADOR");
      listTokens[numTokens].valor = malloc(strlen(cadenaTmp) * sizeof(char));
      strcpy(listTokens[numTokens].valor, cadenaTmp);
      numTokens++;
      // Detecta un espacio, lo ignora
    }else if(caracter == 32){
    }else{
      cadenaTmp[i++] = caracter;
      cadenaTmp[i] = '\0';
      strcpy(listTokens[numTokens].id, "SIM_NO_VALIDO");
      listTokens[numTokens].valor = malloc(strlen(cadenaTmp) * sizeof(char));
      strcpy(listTokens[numTokens].valor, cadenaTmp);
      numTokens++;
    }
  }

  //Impresion de tokens
  for (i = 0; i < numTokens; i++){
    printf("< %s, %s >\n", listTokens[i].id, listTokens[i].valor);
  }
  

  //Liberamos memoria
  free(Stack1);
  free(Stack2);
  free(input);
}

void manejaError(int opcion){
	char *mensaje[] = {"No hay memoria disponible", "Pila vacia", "Liberando memoria", "Saliendo del programa", "Estacionamiento lleno"};
	printf("\n%s\n", mensaje[opcion]);
}