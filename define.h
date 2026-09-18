#define TAM 100

#define IDENT 1
#define NUMERO 2
#define MAIS 3
#define MULT 4
#define POTENCIA 5
#define ABRE_PAR 6
#define FECHA_PAR 7
#define FIM 8

extern int simbolo_lido;
extern int posicao;
extern char entrada[TAM];

int proximo_token(void);

void obtenha_simbolo(void);

void erro(const char *mensagem);
void erro_lexico(char caractere);

void expr(void);
void termo(void);
void fator(void);
void primario(void);