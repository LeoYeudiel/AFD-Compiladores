#include "pilas.h" //Tad Pila
#include "listas.h" //Tad Pila
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Token {
    char id[20]; /* OP, NUM, ID, ID_NOVALIDO, SIM_NOVALIDO */
    char *valor; /* +, 12993, ab, ABC, * */
} Token;
//Estructura para identificar en que columna esta cierto operador
typedef struct columnasTabla{
  char simbolo;
  int columna;
}columnasTabla;

void insertarFilaEnPila(FILE *, Pila *);
void IntercambiarPilas(Pila *, Pila *);
void manejaError(int);
void introducirToken(char *, char *, Token **, int *);
Token *aumenntartamanioLista(Token **, int *);
char *vaciarPilaEnCadena(Pila *);

//funcion para llenar la matriz de busqueda recibe los siguientes parámetros
/*la lista de las reglas de produccion, la matriz que contiene las funciones primero, la matriz
de las funciones siguiente, la estructura columnasTabla que identifica en que columna estan ciertos simbolos
la matriz que será llenada y el arreglo de caracteres que contiene a todas las reglas de produccion
*/
void llenarMatrizBusqueda(LISTA *, char **, char **, columnasTabla *, int **, char*); 
int BuscarIndiceenGramatica(char, char*);
int BuscarColumna(columnasTabla *, char);
void llenarMatrizValPredetrminado(int **, int, int);
//Verifica que la cadena sea acceptada por el analizador LL1
/*paremetros:
la cadena tokenizada a analizar, las reglas de producciones, 
la matriz en donde se buscan las reglas de produccion y la cadena que
contiene las reglas de produccion E..F
*/
int validarCadena(char *, LISTA*, int **, char*, PILA, columnasTabla *);
//Inserta la correpondiente regla de produccion en la pila
void insertarProduccionEnFila(PILA, LISTA*, int, int);
int main(int argc, char *argv[]) {
    char *cadenaTmp = NULL;
    int numTokens = 0, i, es_numero = 0, no_valida = 0;
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

    printf("COMENZANDO ANALIZADOR LEXICO\n");
    printf("================================================================\n");
    while (!feof(archivo)) {
      //printf("Paso\n\n");
      insertarFilaEnPila(archivo, Stack1);
      IntercambiarPilas(Stack1, Stack2);
      while (!es_vaciaPila(Stack2))
      {
        es_numero = 0;
        i = 0;
        char caracter = desapilar(Stack2);
        //printf("\n%d\n", caracter);
        // Primer caso: si es una letra mayuscula o minuscula
        if ((caracter >= 65 && caracter <= 90) || (caracter >= 97 && caracter <= 122)){
          apilar(StackAux, caracter);
          while (!es_vaciaPila(Stack2) && ((elemTope(Stack2) >= 65 && elemTope(Stack2) <= 90) || (elemTope(Stack2) >= 97 && elemTope(Stack2) <= 122) || (elemTope(Stack2) >= 48 && elemTope(Stack2) <= 57)))
          {
            caracter = desapilar(Stack2);
            //printf("\n%d\n", caracter);
            apilar(StackAux, caracter);
          }
            cadenaTmp = vaciarPilaEnCadena(StackAux);
            //printf("\n TOKEN ID VALIDO\n");
            introducirToken("ID", cadenaTmp, &listTokens, &numTokens);
        }
        // Segundo caso: detectar si es un número
        else if (caracter >= 48 && caracter <= 57){
          apilar(StackAux, caracter);
          while (!es_vaciaPila(Stack2) && (elemTope(Stack2) >= 48 && elemTope(Stack2) <= 57))
          {
            caracter = desapilar(Stack2);
            apilar(StackAux, caracter);
          }

          while (!es_vaciaPila(Stack2) && ((elemTope(Stack2) >= 65 && elemTope(Stack2) <= 90) || (elemTope(Stack2) >= 97 && elemTope(Stack2) <= 122) || (elemTope(Stack2) >= 48 && elemTope(Stack2) <= 57)))
          {
            es_numero = 1;
            caracter = desapilar(Stack2);
            apilar(StackAux, caracter);
          }

          if(es_numero == 1){
            cadenaTmp = vaciarPilaEnCadena(StackAux);
            introducirToken("ID", cadenaTmp, &listTokens, &numTokens);
          }else{
            cadenaTmp = vaciarPilaEnCadena(StackAux);
            introducirToken("NUMERO", cadenaTmp, &listTokens, &numTokens);
          }
          
        }
        // Tercer caso: detectar si es un símbolo de operación +, -, *, /
        else if (caracter == '+' || caracter == '-' || caracter == '*' || caracter == '-' || caracter == '/' || caracter == '(' || caracter == ')'){
          apilar(StackAux, caracter);
          cadenaTmp = vaciarPilaEnCadena(StackAux);
          //printf("\n TOKEN OPERADOR VALIDO\n");
          introducirToken("OPERADOR", cadenaTmp, &listTokens, &numTokens);
          // Cuarto caso: no detecta el salto de línea del documento y lo ignora
        }else if(caracter == 13){
          // Cuarto caso: Cualquier otro caracter que no pertenezca a los casos anteriores, es INVÁLIDO
        }
        // Cuarto caso: Cualquier otro caracter que no pertenezca a los casos anteriores, es INVÁLIDO
        else
        {
          apilar(StackAux, caracter);
          cadenaTmp = vaciarPilaEnCadena(StackAux);
          //printf("\n TOKEN SIMBOLO NO VALIDO\n");
          introducirToken("SIMBOLO_NO_VALIDO", cadenaTmp, &listTokens, &numTokens);
          no_valida = 1;
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

    if(no_valida == 1){
      printf("Cadena NO valida por el analizador lexico\n");
      printf("================================================================\n");
      printf("TERMINADO EL ANALIZADOR LEXICO");
    }else{
      printf("Cadena SI valida por el analizador lexico\n");
      printf("================================================================\n");
      printf("TERMINADO EL ANALIZADOR LEXICO");
    }
    FILE *archivo2;
    PILA pila1, pila2;
    pila1=crearPila();
    pila2=crearPila();
    char *cadenaAnalisis;
    cadenaTmp=NULL;
    archivo2 = fopen("salidaLexico.txt", "r");
    if (archivo2 == NULL) {
      printf("\n El archivo no se pudo abrir");
      exit(0);
    }
    while(!feof(archivo2)){
      insertarFilaEnPila(archivo2, pila1);
      cadenaTmp = vaciarPilaEnCadena(pila1);

      //Analizamos el token
      switch (cadenaTmp[1])
        {
        case 'N':
          apilar(pila2, '#');
          break;
        case 'I':
          apilar(pila2, 'I');
          break;
        case 'O':
          apilar(pila2, cadenaTmp[10]);
          break;
        }
    }
    cadenaAnalisis = vaciarPilaEnCadena(pila2);
    printf("\n Cadena tokenizada: %s\n", cadenaAnalisis);
    fclose(archivo2);
    //Gramatica
    char *gramatica="EeTtF"; //E, E', T, T', F
    //Reglas de producción
    LISTA *ReglasProd=(LISTA*)malloc(5*sizeof(LISTA));
    for(int i=0;i<5;i++){
      ReglasProd[i]=crearLista();
    }
    insertar(&ReglasProd[0], "Te");//E
    insertar(&ReglasProd[1], "+Te");//E'
    insertar(&ReglasProd[1], "-Te");//E'
    insertar(&ReglasProd[1], "<");//E', <=epsilon
    insertar(&ReglasProd[2], "Ft");//T
    insertar(&ReglasProd[3], "*Ft");//T'
    insertar(&ReglasProd[3], "/Ft");//T'
    insertar(&ReglasProd[3], "<");//T'
    insertar(&ReglasProd[4], "(E)");//F
    insertar(&ReglasProd[4], "I");//F
    
    //matriz de funcion primero, las filas represnetan a las reglas de produccion
    //E, E', T, T', F 
    char **primero;
    primero=(char**)malloc(5*sizeof(char*));
    if(primero==NULL){
      printf("Error al asignar memoria\n");
      exit(0);
    }
    *(primero)=(char*)malloc(3*sizeof(char)); //E
    *(primero+1)=(char*)malloc(4*sizeof(char)); //E'
    *(primero+2)=(char*)malloc(3*sizeof(char));//T
    *(primero+3)=(char*)malloc(4*sizeof(char));//T'
    *(primero+4)=(char*)malloc(3*sizeof(char));//F

    //llenado de la función primero
    strcpy(*(primero), "I(");
    strcpy(*(primero+1), "+-<");
    strcpy(*(primero+2), "I(");
    strcpy(*(primero+3), "*/<");
    strcpy(*(primero+4), "I(");

    //matriz de funcion SIGUIENTE, las filas represntan a las reglas de produccion
    //E, E', T, T', F 
    char **siguiente;
    siguiente=(char**)malloc(5*sizeof(char*));
    if(siguiente==NULL){
      printf("Error al asignar memoria\n");
      exit(0);
    }
    *(siguiente)=(char*)malloc(3*sizeof(char)); //E
    *(siguiente+1)=(char*)malloc(3*sizeof(char)); //E'
    *(siguiente+2)=(char*)malloc(5*sizeof(char));//T
    *(siguiente+3)=(char*)malloc(5*sizeof(char));//T'
    *(siguiente+4)=(char*)malloc(7*sizeof(char));//F
    //llenado de la función siguiente
    strcpy(*(siguiente), "$)");
    strcpy(*(siguiente+1), "$)");
    strcpy(*(siguiente+2), "$+-)");
    strcpy(*(siguiente+3), "$+-)");
    strcpy(*(siguiente+4), "$+-*/)");

    //Se identifica a cada simbolo terminal en cierta columna
    columnasTabla simsTerminales[8];
    simsTerminales[0].columna=0;
    simsTerminales[0].simbolo='+';
    simsTerminales[1].columna=1;
    simsTerminales[1].simbolo='-';
    simsTerminales[2].columna=2;
    simsTerminales[2].simbolo='*';
    simsTerminales[3].columna=3;
    simsTerminales[3].simbolo='/';
    simsTerminales[4].columna=4;
    simsTerminales[4].simbolo='(';
    simsTerminales[5].columna=5;
    simsTerminales[5].simbolo=')';
    simsTerminales[6].columna=6;
    simsTerminales[6].simbolo='I';
    simsTerminales[7].columna=7;
    simsTerminales[7].simbolo='$';
    //Se crea la matriz en donde se guardarán las reglas de producción:
    int **matrizBusqueda;
    matrizBusqueda=(int**)malloc(5*sizeof(int*));
    if(matrizBusqueda==NULL)
      exit(0);
    //espacio para los simbolos de las columnas
    for(int i=0;i<5;i++){
      *(matrizBusqueda+i)=(int*)malloc(8*sizeof(int));
    }
    //cada fila de la matriz representa la lista iésima, por lo cual en cada 
    //celda de la matriz solo se guardará la columna en la que se encuentra la regla de produccion
    llenarMatrizValPredetrminado(matrizBusqueda, 5, 8);//Se llena la matriz con un valor prdeterminado
    llenarMatrizBusqueda(ReglasProd, primero, siguiente, simsTerminales, matrizBusqueda, gramatica);
    for(int k=0;k<5;k++){
      for(int l=0;l<8;l++){
        printf(" %d ", matrizBusqueda[k][l]);
      }
      printf("\n");
    }

    int posCadena;
    //Se valida que la cadena sea aceptada por el analizador sintactico LL1
    PILA validacion;
    validacion=crearPila();
    //se añade el simbolo de fin de pila
    //apilar(validacion, '$');
    //se añade la regla de produccion inicial E
    apilar(validacion, 'E');
    posCadena=validarCadena(cadenaAnalisis, ReglasProd, matrizBusqueda, gramatica, validacion, simsTerminales);
    if(posCadena==strlen(cadenaAnalisis) && es_vaciaPila(validacion)==1){
      printf("=====================\n");
      printf("Cadena aceptada por el analizador sintactico LL1");
      printf("=====================\n");
    }else{
      printf("=====================\n");
      printf("Cadena NO aceptada por el analizador sintactico LL1");
      printf("Error en la posicion de la cadena %d", posCadena);
      printf("=====================\n");
    }
}

int validarCadena(char *cadenaAnalisis, LISTA*ReglasProd, int **matrizBusqueda, char*gramatica, PILA validacion, columnasTabla *simsTerminales){
  PILA aux;
  aux=crearPila();
  char coincidencia, car;
  int posCadena=0;
  int posFila, posColumna;
  while(es_vaciaPila(validacion)!=1 && posCadena<=strlen(cadenaAnalisis)){
    coincidencia=desapilar(validacion);
    //si se trata de un sim no terminal
    if(coincidencia!=cadenaAnalisis[posCadena]){
      posFila=BuscarIndiceenGramatica(coincidencia, gramatica);
      posColumna=BuscarColumna(simsTerminales, cadenaAnalisis[posCadena]);
      printf("\n %d %d\n", posFila, posColumna);
      //validamos que exista una regla d eproduccion en la celda de la matriz
      if(matrizBusqueda[posFila][posColumna]!=-1){
        insertarProduccionEnFila(validacion, ReglasProd, posFila, matrizBusqueda[posFila][posColumna]);
        /*IntercambiarPilas(validacion, aux);
        while(es_vaciaPila(aux)!=1){
          car=desapilar(aux);
          printf("%c", car);
          apilar(validacion, car);
        }
        printf("\n");*/

      }else{
        return posCadena;
      }
      //si es epsilon no hacer nada, solo desapilar
      if(elemTope(validacion)=='<')
        coincidencia=desapilar(validacion);
    }else{
      //si se trata de un simbolo terminal avanzar en la cadena 
      printf("%c", cadenaAnalisis[posCadena]);
      posCadena++;
    }
  }
  return posCadena;
}


void insertarProduccionEnFila(PILA validacion, LISTA *ReglasProd, int fila, int columna){
  PILA aux;
  aux=crearPila();
  Nodo_Lista *actual;
  int col=1, contCadena=0;
  //nos posicionamos en la fila que contiene la regla de produccion de interes
  actual=ReglasProd[fila];
  while(col<columna){
    actual=actual->siguiente;
    col++;
  }
  //apilamos esa produccion en nuestra pila de validacion
  while(contCadena<strlen(actual->regla)){
    apilar(aux, (actual->regla)[contCadena]);
    contCadena++;
  }
  //insertamos los datos en la pila original de validacion de forma invertida
  IntercambiarPilas(aux, validacion);
}


void llenarMatrizValPredetrminado(int **matrizBusqueda, int filas, int columnas){
  int val=-1;
  for(int i=0;i<filas;i++){
    for(int j=0;j<columnas;j++){
      matrizBusqueda[i][j]=val;
    }
  }
}
void llenarMatrizBusqueda(LISTA *ReglasProd, char **primero, char **siguiente, columnasTabla *simsTerminales, int **matrizBusqueda, char* gramatica){
  int pos;
  char *primeroIndividual;
  int reglaProduccion;
  for(int i=0;i<5;i++){
    Nodo_Lista *actual;
    actual=ReglasProd[i];
    //avanzar en las reglas de produccion
    reglaProduccion=0;
    while(actual!=NULL){
      if((actual->regla)[0]=='E' || (actual->regla)[0]=='e' || (actual->regla)[0]=='T' || (actual->regla)[0]=='t' || (actual->regla)[0]=='F'){
        pos=BuscarIndiceenGramatica((actual->regla)[0], gramatica);
        primeroIndividual=(*(primero+pos));
        for(int m=0;m<strlen(primeroIndividual);m++){
          //Buscamos a que columna pertenece cada uno de los simbolos dentro del conjunto primero
          pos=BuscarColumna(simsTerminales, primeroIndividual[m]);
          //indica en la fila adecuada que regla de produccion colocar
          matrizBusqueda[i][pos]=reglaProduccion+1;
          //printf("\n regla:%d\n", reglaProduccion+1);
        }
        
      }else{
        //si se trata de una produccion epsilon entonces se colocan
        //las producciones en siguiente de la produccion
        if((actual->regla)[0]=='<'){
          //Se necesita la funcion siguiente de la regla de produccion actual
          primeroIndividual=(*(siguiente+i));
          for(int m=0;m<strlen(primeroIndividual);m++){
            pos=BuscarColumna(simsTerminales, primeroIndividual[m]);
            //indica en la fila adecuada que regla de produccion colocar
            matrizBusqueda[i][pos]=reglaProduccion+1;
            //printf("\n regla:%d\n", reglaProduccion+1);
          }
        }else{
          //Se trata de un simbolo terminal, primero(terminal)= el mismo terminal
          pos=BuscarColumna(simsTerminales, (actual->regla)[0]);
          matrizBusqueda[i][pos]=reglaProduccion+1;
        }
      }
      actual=actual->siguiente;
      reglaProduccion++; //en que nodo de la lista i-esima se encuentra la regla de prod actual
    }
  }
}

int BuscarIndiceenGramatica(char letra, char* gramatica){
  int longitud;
  longitud=strlen(gramatica);
  for(int i=0;i<longitud;i++){
    if(letra==gramatica[i])
      return i;
  }
  return -1;
}

int BuscarColumna(columnasTabla *simsTerminales, char letra){
  for(int i=0;i<8;i++){
    if(simsTerminales[i].simbolo==letra){
      return i;
    }
  }
  return -1;
}

void introducirToken(char *tipoToken, char *cadenaTmp, Token **listTokens, int *numTokens) {
    //printf("\n Antes de aumentar tamaño de lista de tokens\n");
    *listTokens = aumenntartamanioLista(listTokens, numTokens);
    //printf("\n Después de aumentar tamaño de lista de tokens\n");
    strcpy((*listTokens)[(*numTokens)].id, tipoToken);
    (*listTokens)[(*numTokens)].valor = strdup(cadenaTmp);
    //printf("\n TOKEN LLENO\n");
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
