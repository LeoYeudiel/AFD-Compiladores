#include "pilas.h" //Tad Pila
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Token {
    char id[20]; /* OP, NUM, ID, ID_NOVALIDO, SIM_NOVALIDO */
    char *valor; /* +, 12993, ab, ABC, * */
} Token;

void insertarFilaEnPila(FILE *, Pila *);
void IntercambiarPilas(Pila *, Pila *);
void manejaError(int);
void introducirToken(char *, char *, Token **, int *);
Token *aumenntartamanioLista(Token **, int *);
char *vaciarPilaEnCadena(Pila *);

int main(int argc, char *argv[]) {
    char *cadenaTmp = NULL;
    int numTokens = 0, i;
    PILA Stack1, Stack2, StackAux;
    Token *listTokens = NULL;
    FILE *archivo, *archivoSalida;

    if (argc != 2) {
        printf("Ejecucion incorrecta. Debe de proporcionar el nombre del archivo.\n");
        exit(0);
    }

    archivo = fopen(argv[1], "r");
    if (archivo == NULL) {
        printf("\n El archivo no se pudo abrir");
        exit(0);
    }

    Stack1 = crearPila();
    Stack2 = crearPila();
    StackAux = crearPila();

    while (!feof(archivo)) {
        insertarFilaEnPila(archivo, Stack1);
        IntercambiarPilas(Stack1, Stack2);

        while (!es_vaciaPila(Stack2)) {
            i = 0;
            char caracter = desapilar(Stack2);
            printf("\n%d\n", caracter);
            //Primer caso: si es una letra mayuscula o minuscula
            if ((caracter >= 65 && caracter <= 90) || (caracter >= 97 && caracter <= 122)) {
                apilar(StackAux, caracter);
                while (!es_vaciaPila(Stack2) && ((elemTope(Stack2) >= 65 && elemTope(Stack2) <= 90) || (elemTope(Stack2) >= 97 && elemTope(Stack2) <= 122))) {
                    caracter = desapilar(Stack2);
                    printf("\n%d\n", caracter);
                    apilar(StackAux, caracter);
                }
                if (StackAux->numElem > 3) {
                    cadenaTmp = vaciarPilaEnCadena(StackAux);
                    printf("\n TOKEN ID NO VALIDO\n");
                    introducirToken("ID_NO_VALIDO", cadenaTmp, &listTokens, &numTokens);
                } else {
                   cadenaTmp = vaciarPilaEnCadena(StackAux);
                    printf("\n TOKEN ID VALIDO\n");
                    introducirToken("ID", cadenaTmp, &listTokens, &numTokens);
                }
            //Segundo caso: detectar si es un número
            }else if (caracter >= 48 && caracter <= 57) {
                apilar(StackAux, caracter);
                while (!es_vaciaPila(Stack2) && (elemTope(Stack2) >= 48 && elemTope(Stack2) <= 57)) {
                    caracter = desapilar(Stack2);
                    printf("\n%d\n", caracter);
                    apilar(StackAux, caracter);
                }
                cadenaTmp = vaciarPilaEnCadena(StackAux);
                printf("\n TOKEN NUMERO VALIDO\n");
                introducirToken("NUMERO", cadenaTmp, &listTokens, &numTokens);
            //Tercer caso: detectar si es un símbolo de operación + o -
            } else if (caracter == '+' || caracter == '-') {
                apilar(StackAux, caracter);
                cadenaTmp = vaciarPilaEnCadena(StackAux);
                printf("\n TOKEN OPERADOR VALIDO\n");
                introducirToken("OPERADOR", cadenaTmp, &listTokens, &numTokens);
            //Cuarto caso: Cualquier otro caracter que no pertenezca a los casos anteriores, es INVÁLIDO
            } else {
                apilar(StackAux, caracter);
                cadenaTmp = vaciarPilaEnCadena(StackAux);
                printf("\n TOKEN SIMBOLO NO VALIDO\n");
                introducirToken("SIMBOLO_NO_VALIDO", cadenaTmp, &listTokens, &numTokens);
            }
        }
    }

    fclose(archivo);
    free(Stack1);
    free(Stack2);
    free(StackAux);
    archivoSalida=fopen("salidaLexico.txt", "w");
    if(archivoSalida==NULL){
      printf("Error al abrir el archivo  \n");
      return 1;
    }
  printf("\n %d", numTokens);
    // Impresión de los tokens
    for (i = 0; i < numTokens; i++) {
        fprintf(archivoSalida, "< %s, %s >\n", listTokens[i].id, listTokens[i].valor);
        free(listTokens[i].valor);
    }
    free(listTokens);
    fclose(archivoSalida);
}

void introducirToken(char *tipoToken, char *cadenaTmp, Token **listTokens, int *numTokens) {
    printf("\n Antes de aumentar tamaño de lista de tokens\n");
    *listTokens = aumenntartamanioLista(listTokens, numTokens);
    printf("\n Después de aumentar tamaño de lista de tokens\n");
    strcpy((*listTokens)[(*numTokens)].id, tipoToken);
    (*listTokens)[(*numTokens)].valor = strdup(cadenaTmp);
    printf("\n TOKEN LLENO\n");
    (*numTokens)++;
}

Token *aumenntartamanioLista(Token **listTokens, int *numTokens) {
    *listTokens = (Token *)realloc(*listTokens, ((*numTokens)+1) * sizeof(Token));
    return *listTokens;
}

void manejaError(int opcion) {
    char *mensaje[] = {"No hay memoria disponible", "Pila vacia", "Liberando memoria", "Saliendo del programa", "Estacionamiento lleno"};
    printf("\n%s\n", mensaje[opcion]);
}

void insertarFilaEnPila(FILE *archivo, Pila *Stack) {
    int codigo = 1;
    char letra;

    while (codigo != 10 && !feof(archivo)) {
        letra = fgetc(archivo);
        codigo = letra;

        if (codigo != 32 && codigo != 10 && codigo>0) {
            apilar(Stack, letra);
        }
    }
}

void IntercambiarPilas(Pila *Stack1, Pila *Stack2) {
    char letra;

    while (!es_vaciaPila(Stack1)) {
        letra = desapilar(Stack1);
        apilar(Stack2, letra);
    }
}

char *vaciarPilaEnCadena(Pila *StackAux) {
    int j = 0;
    Pila *stackAux2;
    stackAux2=crearPila();
    char *cadenatmp = (char *)malloc((StackAux->numElem + 1) * sizeof(char));
    if (cadenatmp == NULL) {
        printf("Error: No se pudo asignar memoria\n");
        return NULL;
    }
    //StackAux viene en desorden, intercambiamos pilas para tenere los elementos ordenados
    IntercambiarPilas(StackAux, stackAux2);
    while (!es_vaciaPila(stackAux2)) {
        cadenatmp[j++] = desapilar(stackAux2);
    }

    cadenatmp[j] = '\0'; // Agregar el carácter nulo al final de la cadena
    return cadenatmp;
}
