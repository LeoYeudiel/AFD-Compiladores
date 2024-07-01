//Definición de Tokens
const TokenType = {
  //Palabras reservadas
    //Cabecera y final del algoritmo
  ALGORITMO: 'ALGORITMO',
  FINALGORITMO: 'FINALGORITMO',
    
    //Declaración de variables
  DEFINIR: 'DEFINIR',
  COMO: 'COMO',

    //Funciones nativas para lectura y escritura
  LEER: 'LEER',
  ESCRIBIR: 'ESCRIBIR',

    //Para la declaración de una función
  FUNCION: 'FUNCION',
  FINFUNCION: 'FINFUNCION',
  RETORNAR: 'RETORNAR',

    //Estructura condicional If
  SI: 'SI',
  ENTONCES: 'ENTONCES',
  SINO: 'SINO',
  FINSI: 'FINSI',

    //Estructura ciclicas while o for
  MIENTRAS: 'MIENTRAS',
  HACER: 'HACER',
  FINMIENTRAS: 'FINMIENTRAS',
  PARA: 'PARA',
  HASTA: 'HASTA',
  CON: 'CON',
  PASO: 'PASO',
  FINPARA: 'FINPARA',

    //Valores lógicos
  VERDADERO: 'VERDADERO',
  FALSO: 'FALSO',
  
    //Tipo de datos
  ENTERO: 'ENTERO',
  REAL: 'REAL',
  CADENA: 'CADENA',
  LOGICO: 'LOGICO',

    //Operadores lógicos
  Y: 'Y',
  O: 'O',
  NO: 'NO',
  
  //Identificadores y literales
  IDENTIFICADOR: 'IDENTIFICADOR',  // Para identificar cualquier otro identificador
  FLOTANTE: 'FLOTANTE',  // Para números
  NUMERO: 'NUMBER',  // Para números
  STRING: 'STRING',  // Para cadenas de texto

  //Operadores
  SUMA: 'SUMA',
  RESTA: 'RESTA',
  MULTIPLICACION: 'MULTIPLICACION',
  DIVISION: 'DIVISION',

  //Operadores Lógicos
  IGUAL: 'IGUAL',
  MENORQUE: 'MENORQUE',
  MAYORQUE: 'MAYORQUE',
  MAYORIGUAL: 'MAYORIGUAL',
  MENORIGUAL: 'MENORIGUAL',

  //Delimitadores
  IZQPAREN: 'IZQPAREN', //(
  RIGPAREN: 'RIGPAREN', //)
  ASIGNACION: 'ASIGNACION', //=
  COMA: 'COMA', //,

  //Fin de línea o Input
  SALTO: 'SALTO', //Fin de la línea
  EOF: 'EOF', //Fin de la línea
}

const TokenPatterns = [
  { type: TokenType.ALGORITMO, pattern: /^Algoritmo\b/ },
  { type: TokenType.DEFINIR, pattern: /^Definir\b/ },
  { type: TokenType.LEER, pattern: /^Leer\b/ },
  { type: TokenType.ESCRIBIR, pattern: /^Escribir\b/ },
  { type: TokenType.FINALGORITMO, pattern: /^FinAlgoritmo\b/ },
  { type: TokenType.COMO, pattern: /^Como\b/ },
  { type: TokenType.ENTERO, pattern: /^Entero\b/ },
  { type: TokenType.REAL, pattern: /^Real\b/ },
  { type: TokenType.CADENA, pattern: /^Cadena\b/ },
  { type: TokenType.LOGICO, pattern: /^Logico\b/ },
  { type: TokenType.FUNCION, pattern: /^Funcion\b/ },
  { type: TokenType.FINFUNCION, pattern: /^FinFuncion\b/ },
  { type: TokenType.SI, pattern: /^Si\b/ },
  { type: TokenType.ENTONCES, pattern: /^Entonces\b/ },
  { type: TokenType.SINO, pattern: /^Sino\b/ },
  { type: TokenType.FINSI, pattern: /^FinSi\b/ },
  { type: TokenType.MIENTRAS, pattern: /^Mientras\b/ },
  { type: TokenType.HACER, pattern: /^Hacer\b/ },
  { type: TokenType.FINMIENTRAS, pattern: /^FinMientras\b/ },
  { type: TokenType.PARA, pattern: /^Para\b/ },
  { type: TokenType.HASTA, pattern: /^Hasta\b/ },
  { type: TokenType.CON, pattern: /^Con\b/ },
  { type: TokenType.PASO, pattern: /^Paso\b/ },
  { type: TokenType.FINPARA, pattern: /^FinPara\b/ },
  { type: TokenType.RETORNAR, pattern: /^Retornar\b/ },
  { type: TokenType.VERDADERO, pattern: /^Verdadero\b/ },
  { type: TokenType.FALSO, pattern: /^Falso\b/ },
  { type: TokenType.Y, pattern: /^Y\b/ },
  { type: TokenType.O, pattern: /^O\b/ },
  { type: TokenType.NO, pattern: /^No\b/ },
  { type: TokenType.SUMA, pattern: /^\+/},
  { type: TokenType.RESTA, pattern: /^\-/},
  { type: TokenType.MULTIPLICACION, pattern: /^\*/},
  { type: TokenType.DIVISION, pattern: /^\//},
  { type: TokenType.IGUAL, pattern: /^==/},
  { type: TokenType.ASIGNACION, pattern: /^=/},
  { type: TokenType.MAYORIGUAL, pattern: /^>=/},
  { type: TokenType.MENORIGUAL, pattern: /^<=/},
  { type: TokenType.MENORQUE, pattern: /^</},
  { type: TokenType.MAYORQUE, pattern: /^>/},
  { type: TokenType.IZQPAREN, pattern: /^\(/},
  { type: TokenType.RIGPAREN, pattern: /^\)/ },
  { type: TokenType.COMA, pattern: /^,/ },
  { type: TokenType.SALTO, pattern: /^\n/ },
  { type: TokenType.IDENTIFICADOR, pattern: /^[a-zA-Z_]\w*\b/ },  // Para otros identificadores
  { type: TokenType.NUMERO, pattern: /^-?\d+(\.\d+)?\b$/ },  // Para números flotantes
  /* { type: TokenType.NUMERO, pattern: /^-?\d+\b/ },  // Para números enteros */
  // { type: TokenType.STRING, pattern: /"([^"\\]|\\.)*"/ },  // Para cadenas de texto
  { type: TokenType.STRING, pattern: /^"([^"\\]|\\.)*"$/},
]


class Token{
  constructor(type, value, line) {
    this.type = type;
    this.value = value;
    this.line = line
  }

  toString() { 
    return `Token (${this.type}, ${this.value}, ${this.line})`;
  }
}



//Análizador léxico
class Lexer {
  constructor(input) {
    this.input = input;
    this.pos = 0;
    this.line = 0;
  }

  advance() {
    this.pos++;
    if (this.pos >= this.input.length) {
      this.currentChar = null; // End of input
    } else {
      this.currentChar = this.input[this.pos];
    }
  }

  skipWhitespace() {
    while (this.currentChar !== null && /\s/.test(this.currentChar)) {
      this.advance();
    }
  }

  getNextToken(index) {
    if (this.pos >= this.input.length) {
      return new Token(TokenType.EOF, null, index);
    }
    this.currentChar = this.input[this.pos];
    this.skipWhitespace();
    const remainingInput = this.input.slice(this.pos);
    let value;
    /* console.log(remainingInput) */
    for (const { type, pattern } of TokenPatterns) {
      const match = remainingInput.match(pattern);
      if (match) {
        value = match[0];
        this.pos += value.length;
        const newToken = new Token(type, value, index);
        if (type == "SALTO")
          this.line++;
        return newToken;
      }
    }
    return new Token("UNDEFINED", value, index);
    this.line++;
  }

}

//Prueba

function compilar() {
  let input = document.getElementById("progra").value;
  const lineas = input.split('\n');
  console.log(lineas)
  
  let tokens = [];
  let token;
  //let i = 50;
  for (let i = 0; i < lineas.length; i++){
    const lexer = new Lexer(lineas[i].trim());
    do {
      token = lexer.getNextToken(i);
      if (token.type != TokenType.EOF) {
        console.log(token);
        tokens.push(token);
      }
      //i--;
    } while (token.type !== TokenType.EOF && token.type !== TokenType.UNDEFINED);
  }
  
  console.log(tokens)
  document.getElementById('conso').value = "Analizador lexico pasado"
  analizadorSintactico(tokens)
}


function analizadorSintactico(tokens) {
  pos = 0;
  errores = []
  if (S(tokens)) {
    console.log(tokens[pos])
    document.getElementById('conso').value += `
Analizador sintactico pasado, y correcto`
    console.log("Si es correcto")
  } else {
    document.getElementById('conso').value += `
Analizador sintactico pasado, y NO correcto
    Se quedó en la palabra ${errores[errores.length - 1].value} en la línea ${errores[errores.length - 1].line + 1}`
    console.log(errores[errores.length - 1])
    console.log("No es correcto")
  }
}