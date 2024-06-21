//Gramatica
var pos = 0
//S -> Algoritmo id CONTENIDO FinAlgoritmo
function S(tokens) {
  let posIni = pos;
  if (TokenType.ALGORITMO == tokens[pos].type) {
    pos++;
    if (TokenType.IDENTIFICADOR == tokens[pos].type) {
      pos++;
      if (CONTENIDO(tokens))
        if (TokenType.FINALGORITMO == tokens[pos].type) {
          pos++;
          return 1;
        }
    }
  }

  console.log(tokens[pos])
  return 0;
}

//CONTENIDO -> INSTRUCCION CONTENIDO | e
function CONTENIDO(tokens) {
  let posIni = pos;

  //En caso de CONTENIDO -> INSTRUCCION CONTENIDO
  if (INSTRUCCION(tokens)) {
    if (CONTENIDO(tokens))
      return 1;
  }
  
  pos = posIni;
  //En caso de CONTENIDO -> e
  return 1;
}

//INSTRUCCION -> DECLARACION | ASIGNACION | FUNCION | RETORNARVALOR | CONDICION | CICLO | FUNCIONPREDER | INSTANCIAFUNCION
function INSTRUCCION(tokens) {
  let posIni = pos;

  //En caso de INSTRUCCION -> DECLARACION
  if (DECLARACION(tokens))
    return 1;

  //En caso de INSTRUCCION -> ASIGNACION
  if (ASIGNACION(tokens))
    return 1;

  //En caso de INSTRUCCION -> FUNCION
  if (FUNCION(tokens))
    return 1;

  //En caso de INSTRUCCION -> RETORNARVALOR
  if (RETORNARVALOR(tokens))
    return 1;

  //En caso de INSTRUCCION -> CONDICION
  if (CONDICION(tokens))
    return 1;

  //En caso de INSTRUCCION -> CICLO
  if (CICLO(tokens))
    return 1;

  //En caso de INSTRUCCION -> FUNCIONPREDER
  if (FUNCIONPREDER(tokens))
    return 1;

  //En caso de INSTRUCCION -> INSTANCIAFUNCION
  if (INSTANCIAFUNCION(tokens))
    return 1;

  return 0;
}

//DECLARACION -> Definir REPETIRVARIABLE Como TIPODATO
function DECLARACION(tokens) {
  let posIni = pos;

  if (TokenType.DEFINIR == tokens[pos].type) {
    pos++;
    if (REPETIRVARIABLE(tokens))
      if (TokenType.COMO == tokens[pos].type) {
        pos++;
        if (TIPODATO(tokens))
          return 1;
      }
  }

  return 0;
}

//VARIABLES -> REPETIRVARIABLE | e
function VARIABLES(tokens) {
  let posIni = pos;

  //En caso de VARIABLES -> REPETIRVARIABLE
  if (REPETIRVARIABLE(tokens))
    return 1;
  
  pos = posIni;
  //En caso de VARIABLES -> e
  return 1;
}

//REPETIRVARIABLE -> id REPETIRVARIABLE'
function REPETIRVARIABLE(tokens) { 
  let posIni = pos;

  if (TokenType.IDENTIFICADOR == tokens[pos].type) {
    pos++;
    if (REPETIRVARIABLE1(tokens))
      return 1;
  }

  return 0;
}

//REPETIRVARIABLE' -> , REPETIRVARIABLE | e
function REPETIRVARIABLE1(tokens) {
  let posIni = pos;

  //En caso de REPETIRVARIABLE1 -> , REPETIRVARIABLE
  if (TokenType.COMA == tokens[pos].type) {
    pos++;
    if (REPETIRVARIABLE(tokens))
      return 1;
    
    pos = posIni;
  }
    
  pos = posIni;
  //En caso de REPETIRVARIABLE1 -> e
  return 1;
}

//TIPODATO -> Entero | Real | Cadena | Logico
function TIPODATO(tokens) {
  let posIni = pos

  //En caso de TIPODATO -> Entero
  if (TokenType.ENTERO == tokens[pos].type) {
    pos++;
    return 1;
  }

  pos = posIni;
  //En caso de TIPODATO -> Real
  if (TokenType.REAL == tokens[pos].type) {
    pos++;
    return 1;
  }

  pos = posIni;
  //En caso de TIPODATO -> Cadena
  if (TokenType.CADENA == tokens[pos].type) {
    pos++;
    return 1;
  }

  pos = posIni;
  //En caso de TIPODATO -> Logico
  if (TokenType.LOGICO == tokens[pos].type) {
    pos++;
    return 1;
  }

  return 0;
}

//ASIGNACION -> id = VALOR
function ASIGNACION(tokens) {
  let posIni = pos

  if (TokenType.IDENTIFICADOR == tokens[pos].type) {
    pos++;
    if (TokenType.ASIGNACION == tokens[pos].type) {
      pos++;
      if (VALOR(tokens))
        return 1;
    }
  }
    
  return 0;
}

//VALOR -> Verdadero | Falso | String | Numero | Flotante | EXPRESION
function VALOR(tokens) {
  let posIni = pos;

  //En caso de VALOR -> Verdadero
  if (TokenType.VERDADERO == tokens[pos].type) {
    pos++;
    return 1;
  }

  pos = posIni;
  //En caso de VALOR -> Falso
  if (TokenType.FALSO == tokens[pos].type) { 
    pos++;
    return 1;
  }

  pos = posIni;
  //En caso de VALOR -> String
  if (TokenType.STRING == tokens[pos].type) { 
    pos++;
    return 1;
  }

  pos = posIni;
  //En caso de VALOR -> Numero
  if (TokenType.NUMERO == tokens[pos].type) { 
    pos++;
    return 1;
  }

  pos = posIni;
  //En caso de VALOR -> Flotante
  if (TokenType.FLOTANTE == tokens[pos].type) { 
    pos++;
    return 1;
  }

  pos = posIni;
  //En caso de VALOR -> EXPRESION
  if (EXPRESION(tokens))
    return 1;

  return 0;
}

//EXPRESION -> TERMINO + EXPRESION | TERMINO - EXPRESION | TERMINO
function EXPRESION(tokens) {
  let posIni = pos;

  //En caso de EXPRESION -> TERMINO + EXPRESION
  if (TERMINO(tokens)) 
    if (TokenType.SUMA == tokens[pos].type) {
      pos++;
      if (EXPRESION(tokens))
        return 1;
    }

  pos = posIni;
  //En caso de EXPRESION -> TERMINO - EXPRESION
  if (TERMINO(tokens))
    if (TokenType.RESTA == tokens[pos].type) {
      pos++;
      if (EXPRESION(tokens))
        return 1;
    }
  
  pos = posIni;
  //En caso de EXPRESION -> TERMINO
  if(TERMINO(tokens))
    return 1;

  return 0;
}

//TERMINO -> FACTOR * TERMINO | FACTOR / TERMINO | FACTOR
function TERMINO(tokens) { 
  let posIni = pos;

  //En caso de TERMINO -> FACTOR * TERMINO
  if (FACTOR(tokens))
    if (TokenType.MULTIPLICACION == tokens[pos].type) {
      pos++;
      if (TERMINO(tokens))
        return 1;
    }
  
  pos = posIni;
  //En caso de TERMINO -> FACTOR / TERMINO
  if (FACTOR(tokens))
    if (TokenType.DIVISION == tokens[pos].type) {
      pos++;
      if (TERMINO(tokens))
        return 1;
    }
  
  pos = posIni;
  //En caso de TERMINO -> FACTOR
  if (FACTOR(tokens))
    return 1;
  
  return 0;
}

//FACTOR -> id | Numero | Flotante | (EXPRESION)
function FACTOR(tokens) { 
  let posIni = pos;

  //En caso de FACTOR -> id
  if (TokenType.IDENTIFICADOR == tokens[pos].type) {
    pos++;
    return 1;
  }

  pos = posIni;
  //En caso de FACTOR -> Numero
  if (TokenType.NUMERO == tokens[pos].type) { 
    pos++;
    return 1;
  }

  pos = posIni;
  //En caso de FACTOR -> Flotante
  if (TokenType.FLOTANTE == tokens[pos].type) { 
    pos++;
    return 1;
  }


  pos = posIni;
  //En caso de FACTOR -> (EXPRESION)
  if (TokenType.IZQPAREN == tokens[pos].type) {
    pos++;
    if (EXPRESION(tokens))
      if (TokenType.RIGPAREN == tokens[pos].type) {
        pos++;
        return 1;
      }
  }

  return 0;
}

//FUNCION -> Funcion id(VARIABLES) CONTENIDOFUNCION FinFuncion
function FUNCION(tokens) {
  let posIni = pos;

  if (TokenType.FUNCION == tokens[pos].type) {
    pos++;
    if (TokenType.IDENTIFICADOR == tokens[pos].type) {
      pos++;
      if (TokenType.IZQPAREN == tokens[pos].type) { 
        pos++;
        if (VARIABLES(tokens))
          if (TokenType.RIGPAREN == tokens[pos].type) {
            pos++;
            if (CONTENIDOFUNCION(tokens))
              if (TokenType.FINFUNCION == tokens[pos].type) {
                pos++;
                return 1;
              }
          }
      }
    }
  }

  return 0;
}

//CONTENIDOFUNCION -> INSTRUCCIONFUNCION CONTENIDOFUNCION | e
function CONTENIDOFUNCION(tokens) {
  let posIni = pos;

  //En caso de CONTENIDOFUNCION -> INSTRUCCIONFUNCION CONTENIDOFUNCION
  if (INSTRUCCIONFUNCION(tokens)) {
    if (CONTENIDOFUNCION(tokens))
      return 1;
    
    pos = posIni;
  }

  pos = posIni;
  //En caso de CONTENIDOFUNCION -> e
  return 1;
}

//INSTRUCCIONFUNCION -> DECLARACION | ASIGNACION | RETORNARVALOR | CONDICION | CICLO | FUNCIONPREDER | INSTANCIAFUNCION
function INSTRUCCIONFUNCION(tokens) {
  let posIni = pos;

  //En caso de INSTRUCCIONFUNCION -> DECLARACION
  if (DECLARACION(tokens))
    return 1;

  pos = posIni;
  //En caso de INSTRUCCIONFUNCION -> ASIGNACION
  if (ASIGNACION(tokens))
    return 1;

  pos = posIni;
  //En caso de INSTRUCCIONFUNCION -> RETORNARVALOR
  if (RETORNARVALOR(tokens))
    return 1;


  pos = posIni;
  //En caso de INSTRUCCIONFUNCION -> CONDICION
  if (CONDICION(tokens))
    return 1;

  pos = posIni;
  //En caso de INSTRUCCIONFUNCION -> CICLO
  if (CICLO(tokens))
    return 1;

  pos = posIni;
  //En caso de INSTRUCCIONFUNCION -> FUNCIONPREDER
  if (FUNCIONPREDER(tokens))
    return 1;

  pos = posIni;
  //En caso de INSTRUCCIONFUNCION -> INSTANCIAFUNCION
  if (INSTANCIAFUNCION(tokens))
    return 1;

  return 0;
}

//RETORNARVALOR -> Retornar id
function RETORNARVALOR(tokens) { 
  let posIni = pos;

  if (TokenType.RETORNAR == tokens[pos].type) {
    pos++;
    if (TokenType.IDENTIFICADOR == tokens[pos].type) { 
      pos++;
      return 1;
    }
  }

  return 0;
}

//CONDICION -> Si (EXPRESIONCONDICION) Entonces CONTENIDOFUNCION Sino CONTENIDOFUNCION FinSi | Si (EXPRESIONCONDICION) ENTONCES CONTENIDOFUNCION FinSi
function CONDICION(tokens) {
  let posIni = pos;

  //En caso de CONDICION -> Si (EXPRESIONCONDICION) Entonces CONTENIDOFUNCION Sino CONTENIDOFUNCION FinSi
  if (TokenType.SI == tokens[pos].type) {
    pos++;
    if (TokenType.IZQPAREN == tokens[pos].type) { 
      pos++;
      if (EXPRESIONCONDICION(tokens))
        if (TokenType.RIGPAREN == tokens[pos].type) {
          pos++;
          if (TokenType.ENTONCES == tokens[pos].type) {
            pos++;
            if (CONTENIDOFUNCION(tokens))
              if (TokenType.SINO == tokens[pos].type) {
                pos++;
                if (CONTENIDOFUNCION(tokens))
                  if (TokenType.FINSI == tokens[pos].type) {
                    pos++;
                    return 1;
                  }
              }
          }
        }
    }
  }

  pos = posIni;
  //En caso de CONDICION -> Si (EXPRESIONCONDICION) Entonces CONTENIDOFUNCION FinSi
  if (TokenType.SI == tokens[pos].type) {
    pos++;
    if (TokenType.IZQPAREN == tokens[pos].type) {
      if (EXPRESIONCONDICION(tokens))
        if (TokenType.RIGPAREN == tokens[pos].type) {
          pos++;
          if (TokenType.ENTONCES == tokens[pos].type) {
            pos++;
            if (CONTENIDOFUNCION(tokens))
              if (TokenType.FINSI == tokens[pos].type) {
                pos++;
                return 1;
              }
          }
        }
    }
  }

  return 0;
}

//EXPRESIONCONDICION -> EXPRESION' < EXPRESION' | EXPRESION' > EXPRESION' | EXPRESION' <= EXPRESION' | EXPRESION' >= EXPRESION' | EXPRESION' == EXPRESION' | NO EXPRESION' | EXPRESION' O EXPRESION' | EXPRESION' Y EXPRESION' | EXPRESION'
function EXPRESIONCONDICION(tokens) {
  let posIni = pos;

  //En caso de EXPRESIONCONDICION -> EXPRESION' < EXPRESION'
  if (EXPRESION1(tokens))
    if (TokenType.MENORQUE == tokens[pos].type) {
      pos++;
      if (EXPRESION1(tokens))
        return 1;
    }
  
  pos = posIni;
  //En caso de EXPRESIONCONDICION -> EXPRESION' > EXPRESION'
  if (EXPRESION1(tokens))
    if (TokenType.MAYORQUE == tokens[pos].type) {
      pos++;
      if (EXPRESION1(tokens))
        return 1;
    }
  
  pos = posIni;
  //En caso de EXPRESIONCONDICION -> EXPRESION' >= EXPRESION'
  if (EXPRESION1(tokens))
    if (TokenType.MAYORIGUAL == tokens[pos].type) {
      pos++;
      if (EXPRESION1(tokens))
        return 1;
    }
  
  pos = posIni;
  //En caso de EXPRESIONCONDICION -> EXPRESION' <= EXPRESION'
  if (EXPRESION1(tokens))
    if (TokenType.MENORIGUAL == tokens[pos].type) {
      pos++;
      if (EXPRESION1(tokens))
        return 1;
    }
  
  pos = posIni;
  //En caso de EXPRESIONCONDICION -> EXPRESION' == EXPRESION'
  if (EXPRESION1(tokens))
    if (TokenType.IGUAL == tokens[pos].type) {
      pos++;
      if (EXPRESION1(tokens))
        return 1;
    }
  
  pos = posIni;
  //En caso de EXPRESIONCONDICION -> NO EXPRESION'
  if (TokenType.NO == tokens[pos].type) {
    pos++;
    if (EXPRESION1(tokens))
      return 1;
  }
  
  pos = posIni;
  //En caso de EXPRESIONCONDICION -> EXPRESION' O EXPRESION'
  if (EXPRESION1(tokens))
    if (TokenType.O == tokens[pos].type) {
      pos++;
      if (EXPRESION1(tokens))
        return 1;
    }
  
  pos = posIni;
  //En caso de EXPRESIONCONDICION -> EXPRESION' Y EXPRESION'
  if (EXPRESION1(tokens))
    if (TokenType.Y == tokens[pos].type) {
      pos++;
      if (EXPRESION1(tokens))
        return 1;
    }
  
  pos = posIni;
  //En caso de EXPRESIONCONDICION -> EXPRESION'
  if (EXPRESION1(tokens))
    return 1;

  return 0;
}


//EXPRESION' -> TERMINO' + EXPRESION' | TERMINO' - EXPRESION' | TERMINO'
function EXPRESION1(tokens) {
  let posIni = pos;

  //En caso de EXPRESION' -> TERMINO' + EXPRESION'
  if (TERMINO1(tokens))
    if (TokenType.SUMA == tokens[pos].type) {
      pos++;
      if (EXPRESION1(tokens))
        return 1;
    }
  
  pos = posIni;
  //En caso de EXPRESION' -> TERMINO' - EXPRESION'
  if (TERMINO1(tokens))
    if (TokenType.RESTA == tokens[pos].type) {
      pos++;
      if (EXPRESION1(tokens))
        return 1;
    }
      
  pos = posIni;
  //En caso de EXPRESION' -> TERMINO'
  if (TERMINO1(tokens))
    return 1;

  return 0;
}

//TERMINO' -> FACTOR' * TERMINO' | FACTOR' / TERMINO' | FACTOR'
function TERMINO1(tokens) { 
  let posIni = pos;

  //En caso de TERMINO' -> FACTOR' * TERMINO'
  if (FACTOR1(tokens))
    if (TokenType.MULTIPLICACION == tokens[pos].type) {
      pos++;
      if (TERMINO1(tokens))
        return 1;
    }

  pos = posIni;
  //En caso de TERMINO' -> FACTOR' / TERMINO'
  if (FACTOR1(tokens))
    if (TokenType.DIVISION == tokens[pos].type) {
      pos++;
      if (TERMINO1(tokens))
        return 1;
    }

  pos = posIni;
  //En caso de TERMINO' -> FACTOR'
  if (FACTOR1(tokens))
    return 1;

  return 0;
}

//FACTOR' -> id | Numero | (EXPRESION') | Verdadero | Falso
function FACTOR1(tokens) {
  let posIni = pos;

  //En caso de FACTOR' -> id
  if (TokenType.IDENTIFICADOR == tokens[pos].type) {
    pos++;
    return 1;
  }
    
  pos = posIni;
  //En caso de FACTOR' -> Numero
  if (TokenType.NUMERO == tokens[pos].type) { 
    pos++;
    return 1;
  }

  pos = posIni;
  //En caso de FACTOR' -> (EXPRESION')
  if (TokenType.IZQPAREN == tokens[pos].type) {
    pos++;
    if (EXPRESION1(tokens))
      if (TokenType.RIGPAREN == tokens[pos].type) {
        pos++;
        return 1;
      }
  }
  
  pos = posIni;
  //En caso de FACTOR' -> Verdadero
  if (TokenType.VERDADERO == tokens[pos].type) {
    pos++;
    return 1;
  }

  pos = posIni;
  //En caso de FACTOR' -> Falso
  if (TokenType.FALSO == tokens[pos].type) { 
    pos++;
    return 1;
  }

  return 0;
}

//CICLO -> Mientras (EXPRESION) Hacer CONTENIDOFUNCION FinMientras
function CICLO(tokens) {
  let posIni = pos;

  if (TokenType.MIENTRAS == tokens[pos].type) {
    pos++;
    if (TokenType.IZQPAREN == tokens[pos].type) {
      pos++;
      if (EXPRESION(tokens))
        if (TokenType.RIGPAREN == tokens[pos].type) {
          pos++;
          if (TokenType.HACER == tokens[pos].type) {
            pos++;
            if (CONTENIDOFUNCION(tokens))
              if (TokenType.FINMIENTRAS == tokens[pos].type) {
                pos++;
                return 1;
              }
          }
        }
    }
  }

  return 0;
}

//FUNCIONPREDER -> Leer id | Escribir id
function FUNCIONPREDER(tokens) {
  let posIni = pos;
  
  //En caso de FUNCIONPREDER -> Leer id
  if (TokenType.LEER == tokens[pos].type) {
    pos++;
    if (TokenType.IDENTIFICADOR == tokens[pos].type) { 
      pos++;
      return 1;
    }
  }

  pos = posIni;
  //En caso de FUNCIONPREDER -> Escribir  id
  if (TokenType.ESCRIBIR == tokens[pos].type) { 
    pos++;
    if (TokenType.IDENTIFICADOR == tokens[pos].type) { 
      pos++;
      return 1;
    }
  }
  
  return 0;
}

//INSTANCIAFUNCION -> id (VARIABLES)
function INSTANCIAFUNCION(tokens) {
  let posIni = pos;

  if (TokenType.IDENTIFICADOR == tokens[pos].type) {
    pos++;
    if (TokenType.IZQPAREN == tokens[pos].type) { 
      pos++;
      if (VARIABLES(tokens))
        if (TokenType.RIGPAREN == tokens[pos].type) {
          pos++;
          return 1;
        }
    }
  }

  return 0;
}

