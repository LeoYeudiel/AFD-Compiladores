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
void manejaError(int);
void introducirToken(char *, char *, Token *, int *);

void main(int argc, char *argv[]){
  //Creamos variables inciales con espacio de 1 para la cadena
  char *input = NULL, cadenaTmp[100];
  int longitud, numTokens, i, noPruebas = 0;
  char caracter;
  PILA Stack1, Stack2;
  Token *listTokens = NULL;

  //Verificamos que haya introducido el numero de cadenas a evaluar
  if(argc != 2){
    printf("Ejecucion incorrecta. Debe de proporcionar el numero de cadenas a probar.\n");
    exit(0);
  }

  noPruebas = atoi(argv[1]);

  while(noPruebas--){
    longitud = 1, numTokens = 0;
    printf("Introduzca la formula a analizar: ");
    //Recolectamos la cadena caracter por cracter usando memoria dinamica
    while((caracter = getchar()) != '\n' && caracter != EOF){
      input = (char*)realloc(input, longitud * sizeof(char));
      if(input == NULL){
        printf("Error al asignar memoria a la cadena de lectura");
        exit(0);
      }
      //Almacenamos y aseguramos que el último será siempre el caracter nulo
      input[longitud - 1] = caracter;
      input[longitud] = '\0';
      longitud++;
    }

    //Restamos uno a la longitud ya que al final se tiene el caracter nulo
    longitud--;
    printf("\nDe la formula: '%s' sus tokens son:\n\n", input);
    Stack1 = crearPila();
    Stack2 = crearPila();

    //Apilamos, pero el TAD Pila es LIFO (se almacenará al revés)
    for (i = 0; i < longitud; i++){
      apilar(Stack1, input[i]);
    }

    //Usamos Stack2 para reordenar
    while(!es_vaciaPila(Stack1)) {
      apilar(Stack2, desapilar(Stack1));
    } 
    
    //Analizamos todo, para ello creamos el arreglo de tokens que se piensa tener uno por cada caracter en el peores de los casos
    listTokens = malloc(longitud * sizeof(Token));
    while (!es_vaciaPila(Stack2)){
      i = 0;
      cadenaTmp[i] = '\0';
      caracter = desapilar(Stack2);
      //printf("%c %d: ", caracter, caracter);
      //listTokens = (Token*)malloc((numTokens+1) * sizeof(Token));
      
      //Primer caso: si es una letra mayúscula y/o minúscula
      if((caracter >= 65  && caracter <= 90) || (caracter >= 97 && caracter <= 122)){
        cadenaTmp[i++] = caracter;
        while(!es_vaciaPila(Stack2)){
          //Ya que se detectó una letra, debemos analizar si cuenta con otra. Si tiene más de dos será un ID_NO_VALIDO
          if((elemTope(Stack2) >= 65  && elemTope(Stack2) <= 90) || (elemTope(Stack2) >= 97 && elemTope(Stack2) <= 122)){
            caracter = desapilar(Stack2);
            cadenaTmp[i++] = caracter;
          }else{
            break;
          }
        }
        cadenaTmp[i] = '\0';
        if(strlen(cadenaTmp) > 2){
          introducirToken("ID_NO_VALIDO", cadenaTmp, listTokens, &numTokens);
        }else{
          introducirToken("ID", cadenaTmp, listTokens, &numTokens);
        }

      //Segundo caso: Detectar si es un número
      }else if(caracter >= 48  && caracter <= 57){
        cadenaTmp[i++] = caracter;
        while(!es_vaciaPila(Stack2)){
          //Aquí no importa la longitud del número, cuando deje de detectar numeros, se verificara si se trata de otro símbolo
          if(elemTope(Stack2) >= 48  && elemTope(Stack2) <= 57){
            caracter = desapilar(Stack2);
            cadenaTmp[i++] = caracter;
          }else{
            break;
          }
        }
        cadenaTmp[i] = '\0';
        introducirToken("NUMERO", cadenaTmp, listTokens, &numTokens);
        //Tercer caso: Detectar si es un símbolo de operación + o -
      }else if(caracter == '+' || caracter == '-'){
        cadenaTmp[i++] = caracter;
        cadenaTmp[i] = '\0';
        introducirToken("OPERADOR", cadenaTmp, listTokens, &numTokens);
        //Cuarto caso: Detecta un espacio, lo ignora
      }else if(caracter == 32){
      }
        //Quinto caso: Cualquier otro caracter que no pertenezca a los casos anteriores, es INVÁLIDO
      else{
        cadenaTmp[i++] = caracter;
        cadenaTmp[i] = '\0';
        introducirToken("SIM_NO_VALIDO", cadenaTmp, listTokens, &numTokens);
      }
    }

    //Impresión de los tokens
    for (i = 0; i < numTokens; i++){
      printf("\t< %s, %s >\n", listTokens[i].id, listTokens[i].valor);
    }

    printf("\n\n");
  }
  //Liberamos memoria
  free(Stack1);
  free(Stack2);
  free(input);
}

void introducirToken(char *tipoToken, char *cadenaTmp, Token *listTokens, int *numTokens){
  strcpy(listTokens[(*numTokens)].id, tipoToken);
  listTokens[(*numTokens)].valor = malloc(strlen(cadenaTmp) * sizeof(char));
  strcpy(listTokens[(*numTokens)].valor, cadenaTmp);
  (*numTokens)++;
}

void manejaError(int opcion){
	char *mensaje[] = {"No hay memoria disponible", "Pila vacia", "Liberando memoria", "Saliendo del programa", "Estacionamiento lleno"};
	printf("\n%s\n", mensaje[opcion]);
}