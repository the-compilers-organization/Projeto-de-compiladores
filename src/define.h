#define TAM 6000
#define TAM_LEXEMA 100

//idenificadores e literais
#define IDENT 1
#define INTEIRO_LIT 2 //inteiros
#define REAL_LIT 3
#define CHAR_LIT 4

//operadores relacionais
#define MENOR 5 //<
#define MENOR_IGUAL 6 //<=
#define MAIOR 7//>
#define MAIOR_IGUAL 8 //>=
#define IGUAL 9 //=
#define DIFERENTE 10 // <>

//operadores aritmeticos logicos
#define MAIS 11
#define MENOS 12
#define MULTI 13
#define DIV_REAL 14
#define DIV_INT 15
#define AND 16
#define OR 17
#define NOT 18

#define ATRIBUICAO 19 // :=   atribuição

//outros simbolos
#define ABRE_PAR 20
#define FECHA_PAR 21
#define VIRGULA 22
#define PONTO_VIRGULA 23
#define PONTO 24
#define DOIS_PONTOS 25

//palavras reservadas
#define PROGRAM 26
#define IF 27
#define THEN 28
#define ELSE 29
#define WHILE 30
#define DO 31
#define REPEAT 32
#define UNTIL 33
#define INTEGER 34
#define REAL_TIPO 35
#define CHAR_TIPO 36
#define BEGIN 37
#define END 38
#define WRITE 39
#define VAR 40

#define FIM 41 //fim do arqv


extern int simbolo_lido;
extern int posicao;
extern char entrada[TAM];

extern char lexema[TAM_LEXEMA];

//FUNÇÕES DO LEXER
int proximo_token(void);
void obtenha_simbolo(void);
//... adicionar mais se necssario...

//FUNÇÕES DO "PARSER"
//adicionar...

//usar ainda? ⬇️⬇️⬇️⬇️⬇️⬇️⬇️⬇️⬇️
void erro(const char *mensagem);
void erro_lexico(char caractere);

void expr(void);
void termo(void);
void fator(void);
void primario(void);