

// Este arquivo possui as definições utilizadas em todo o projeto. Nele ficam:
// - constantes de tamanho;
// - códigos dos tokens reconhecidos pelo Lexer;
// - declaração das variáveis globais compartilhadas;
// - protótipos das funções do Lexer;
// - protótipos das funções do Parser.

// Quantidade máxima de caracteres que podem ser armazenados no código-fonte recebido pelo programa.
#define TAM 10000

// Quantidade máxima de caracteres que podem ser armazenados m um lexema reconhecido pelo Lexer.
#define TAM_LEXEMA 100


// TOKENS: Cada constante abaixo representa um tipo de token. O Lexer identifica um lexema da entrada e armazena em
// simbolo_lido uma dessas constantes.


/*
 * IDENTIFICADORES E LITERAIS
 */

/* Nome de variável ou identificador. */
#define IDENTIFICADOR 1

/* Número inteiro, por exemplo: 10, 25, 100. */
#define INTEIRO_LITERAL 2

/* Número real, por exemplo: 10.5 ou .5. */
#define REAL_LITERAL 3

/* Caractere literal, por exemplo: 'a', '5' ou '\n'. */
#define CHAR_LITERAL 4


/*
 * OPERADORES RELACIONAIS
 */

/* Operador < */
#define MENOR 5

/* Operador > */
#define MAIOR 6

/* Operador <= */
#define MENOR_IGUAL 7

/* Operador >= */
#define MAIOR_IGUAL 8

/* Operador = */
#define IGUAL 9

/* Operador <> */
#define DIFERENTE 10


/*
 * OPERADORES ARITMETICOS
 */

/* Operador de adição + */
#define MAIS 11

/* Operador de subtração - */
#define MENOS 12

/* Operador de multiplicação * */
#define MULT 13

/* Operador de divisão real / */
#define DIV_REAL 14


/*
 * OPERADORES REPRESENTADOS POR PALAVRAS RESERVADAS
 */

/* Operador de divisão inteira div */
#define DIV 15

/* Operador lógico and */
#define AND 16

/* Operador lógico or */
#define OR 17

/* Operador lógico not */
#define NOT 18


/*
 * OPERADOR DE ATRIBUICAO
 */

/* Operador := */
#define ATRIBUICAO 19


/*
 * SIMBOLOS ESPECIAIS
 */

/* Símbolo ( */
#define ABRE_PAR 20

/* Símbolo ) */
#define FECHA_PAR 21

/* Símbolo , */
#define VIRGULA 22

/* Símbolo ; */
#define PONTO_VIRGULA 23

/* Símbolo . */
#define PONTO 24

/* Símbolo : */
#define DOIS_PONTOS 25


/*
 * PALAVRAS RESERVADAS
 */

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

/*
 * Foi utilizado BEGIN_TOKEN em vez de BEGIN para deixar
 * explícito que a constante representa o token da palavra
 * reservada "begin".
 */
#define BEGIN_TOKEN 37

/* Token correspondente à palavra reservada "end". */
#define END_TOKEN 38

/* Token correspondente à palavra reservada "write". */
#define WRITE 39

/* Token correspondente à palavra reservada "var". */
#define VAR 40


/*
 * Representa o final da entrada.
 *
 * Quando o Lexer encontra '\0', simbolo_lido recebe FIM.
 */
#define FIM 41


/*
// VARIAVEIS GLOBAIS COMPARTILHADAS: O modificador extern informa que essas variáveis existem
 * em outro arquivo. A definição real será feita em main.c.
 */

/*
 * Armazena todo o código-fonte que será analisado.
 */
extern char entrada[TAM];

/*
 * Armazena o lexema correspondente ao token atual.
 *
 * Exemplo:
 *
 * entrada: soma := 10;
 *
 * quando o token atual for IDENTIFICADOR:
 *
 * lexema = "soma"
 */
extern char lexema[TAM_LEXEMA];

/*
 * Indica a posição atual do Lexer dentro do vetor entrada.
 */
extern int posicao;

/*
 * Armazena o token atualmente reconhecido pelo Lexer.
 */
extern int simbolo_lido;


// FUNCOES DO LEXER
/*
 * Analisa a entrada a partir da posição atual e reconhece
 * o próximo token.
 *
 * Ao terminar:
 *
 * - simbolo_lido recebe o tipo do token;
 * - lexema recebe o texto encontrado;
 * - posicao avança na entrada.
 */
void obtenha_simbolo(void);


/*
 * Recebe uma palavra reconhecida pelo Lexer e verifica
 * se ela é uma palavra reservada da linguagem.
 *
 * Caso seja, retorna o token correspondente.
 *
 * Caso contrário, retorna IDENTIFICADOR.
 */
int palavra_reservada(
        char palavra[]
);


/*
 * Exibe uma mensagem de erro quando o Lexer encontra
 * um caractere que não pode formar um token válido.
 */
void erro_lexico(
        char caractere
);


// FUNCOES DO PARSER


/*
 * Verifica se o token atual é o token esperado.
 *
 * Se estiver correto, solicita ao Lexer o próximo token.
 * Caso contrário, gera erro sintático.
 */
void consumir(
        int token
);


/*
 * Exibe uma mensagem informando que o token atual
 * não era esperado pela gramática.
 */
void erro_sintatico(void);


/*
 * Cada função abaixo representa um símbolo não terminal
 * da gramática utilizada pelo Parser.
 */

/* Reconhece a estrutura completa de um programa. */
void programa(void);

/* Reconhece a seção de declaração de variáveis. */
void secao_var(void);

/* Reconhece uma declaração de variável. */
void decl_var(void);

/* Reconhece os tipos integer, real ou char. */
void tipo(void);

/* Reconhece um bloco delimitado por begin e end. */
void bloco(void);

/* Reconhece uma sequência de zero ou mais comandos. */
void lista_comandos(void);

/* Identifica e encaminha o tipo de comando encontrado. */
void comando(void);

/* Reconhece um comando de atribuição. */
void atribuicao(void);

/* Reconhece comandos de repetição while e repeat. */
void iteracao(void);

/* Reconhece estruturas condicionais if/then/else. */
void decisao(void);

/* Reconhece o comando write. */
void escrita(void);


// FUNCOES PARA EXPRESSOES: As expressões foram divididas em níveis para respeitar
// precedência dos operadores da linguagem.


/* Inicia a análise de uma expressão. */
void expressao(void);

/* Reconhece operadores lógicos: and e or. */
void expr_logica(void);

/* Reconhece operadores relacionais. */
void expr_relacional(void);

/* Reconhece operações de adição e subtração. */
void expr_aditiva(void);

/* Reconhece multiplicação e divisões. */
void expr_multiplicativa(void);

/* Reconhece os elementos básicos de uma expressão. */
void expr_basica(void);
