// FUNCOES AUXILIARES DO PARSER
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



