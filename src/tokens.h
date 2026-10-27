#define TAM 10000
#define TAM_LEXEMA 100 //caracteres que podem ser armazenados  num lexema reconhecido pelo Lexer.


#define IDENTIFICADOR 1 //Nome de variável ou identificador.
#define INTEIRO_LITERAL 2 //Número inteiro
#define REAL_LITERAL 3 //Número real
#define CHAR_LITERAL 4 //Caractere literal, por exemplo: 'a', '5' ou '\n'.


// OPERADORES RELACIONAIS

#define MENOR 5 
#define MAIOR 6 
#define MENOR_IGUAL 7
#define MAIOR_IGUAL 8 
#define IGUAL 9 
#define DIFERENTE 10 //Operador <>


// OPERADORES ARITMETICOS

#define MAIS 11 
#define MENOS 12 
#define MULT 13 
#define DIV_REAL 14


// OPERADORES REPRESENTADOS POR PALAVRAS RESERVADAS

#define DIV 15 
#define AND 16 
#define OR 17 
#define NOT 18


// OPERADOR DE ATRIBUICAO

#define ATRIBUICAO 19 //Operador :=


// SIMBOLOS ESPECIAIS

#define ABRE_PAR 20 
#define FECHA_PAR 21
#define VIRGULA 22 
#define PONTO_VIRGULA 23
#define PONTO 24 
#define DOIS_PONTOS 25


// PALAVRAS RESERVADAS

#define PROGRAM 26
#define IF 27
#define THEN 28
#define ELSE 29
#define WHILE 30
#define DO 31
#define REPEAT 32
#define UNTIL 33
#define INTEGER 34
#define REAL 35
#define CHAR 36
#define BEGIN_TOKEN 37
#define END_TOKEN 38
#define WRITE 39 
#define VAR 40 
#define FIM 41 //final da entrada, quando lexer encontra \0, simbolo_lido recebe FIM



// ------------------------------------------- VARIAVEIS GLOBAIS COMPARTILHADAS: 

extern char entrada[TAM];
extern char lexema[TAM_LEXEMA];
extern int posicao; //Indica a posição atual do Lexer dentro do vetor entrada.
extern int simbolo_lido; //Armazena o token atualmente reconhecido pelo Lexer.


// ------------------------------------------- FUNCOES DO LEXER

void obtenha_simbolo(void);
int palavra_reservada(char palavra[]);
void erro_lexico(char caractere);


// ------------------------------------------- FUNCOES DO PARSER

void consumir(int token);
void erro_sintatico(void);


// Cada função abaixo representa um símbolo não terminal da gramática utilizada pelo Parser.

void programa(void); //Reconhece a estrutura completa de um programa.
void secao_var(void); //Reconhece a seção de declaração de variáveis.
void decl_var(void); //Reconhece uma declaração de variável.
void tipo(void); //Reconhece os tipos integer, real ou char.
void bloco(void); //Reconhece um bloco delimitado por begin e end.
void lista_comandos(void); //Reconhece uma sequência de zero ou mais comandos.
void comando(void); //Identifica e encaminha o tipo de comando encontrado.
void atribuicao(void); //Reconhece um comando de atribuição.
void iteracao(void); //Reconhece comandos de repetição while e repeat.
void decisao(void); //Reconhece estruturas condicionais if/then/else.
void escrita(void); //Reconhece o comando write.


// ------------------------------------------- FUNCOES PARA EXPRESSOES: 

void expressao(void); //Inicia a análise de uma expressão.
void expr_logica(void); //Reconhece operadores lógicos: and e or.
void expr_relacional(void); //Reconhece operadores relacionais.
void expr_aditiva(void); //Reconhece operações de adição e subtração.
void expr_multiplicativa(void); //Reconhece multiplicação e divisões.
void expr_basica(void); //Reconhece os elementos básicos de uma expressão.