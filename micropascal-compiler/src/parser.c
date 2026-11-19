#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/tabela_simbolos.h"
#include "../include/semantico.h"

/*
 * ROTEIRO PARA A EQUIPE - SEGUNDA ETAPA
 *
 * OBJETIVO:
 * Integrar a analise semantica ao analisador sintatico
 * descendente recursivo do compilador MicroPascal.
 *
 * Este arquivo ainda realiza APENAS analise sintatica.
 * Os comentarios TODO orientam a implementacao futura.
 *
 * REQUISITOS DO PROFESSOR:
 *
 * 1. VERIFICACAO DE ESCOPO:
 *    - Nao permitir duas variaveis com o mesmo nome.
 *    - Nao permitir o uso de variaveis nao declaradas.
 *    - Utilizar uma tabela de simbolos implementada
 *      por tabela hash.
 *    - Armazenar pelo menos nome e tipo da variavel.
 *
 * 2. VERIFICACAO DE TIPOS EM EXPRESSOES:
 *    - Verificar os tipos dos operandos.
 *    - Determinar o tipo resultante de cada expressao.
 *    - Rejeitar operacoes entre tipos incompativeis.
 *
 * 3. VERIFICACAO DE TIPOS EM COMANDOS:
 *    - Atribuicoes exigem tipos iguais.
 *    - Condicoes de if, while e repeat/until
 *      devem ser booleanas.
 *
 * ORGANIZACAO:
 *
 * tabela_simbolos.c:
 *   Armazena, busca e gerencia os identificadores.
 *
 * semantico.c:
 *   Verifica compatibilidade de tipos.
 *
 * parser.c:
 *   Reconhece a estrutura sintatica, chama as funcoes
 *   semanticas e informa os erros encontrados.
 *
 * TODO GERAL:
 *   1. Integrar tabela_simbolos.h e semantico.h,
 *      diretamente ou por meio de parser.h.
 *   2. Adicionar uma TabelaSimbolos ao Parser.
 *   3. Alterar as funcoes de expressoes para
 *      retornarem TipoDado.
 *   4. Integrar as verificacoes semanticas.
 *   5. Criar tratamento separado de erros semanticos.
 *   6. Inicializar e liberar a tabela de simbolos.
 *
 * IMPORTANTE:
 *   Nao utilizar AST.
 *   Propagar os tipos durante a descida recursiva.
 *   Preservar a gramatica e a analise sintatica.
 */


/*
 * AVANCAR
 *
 * Avanca para o proximo token fornecido pelo Lexer.
 *
 * Esta funcao deve continuar responsavel apenas
 * pela leitura dos tokens.
 *
 * Nao realizar verificacoes semanticas aqui.
 */
static void avancar(Parser *p) {
    p->token_atual = lexer_proximo_token(p->lexer);
}


/*
 * ERRO SINTATICO
 *
 * Informa um erro quando a sequencia de tokens
 * nao corresponde a estrutura da gramatica.
 *
 * TODO:
 *   Criar uma rotina separada para erros semanticos.
 *
 * A rotina de erro semantico deve permitir informar:
 *   - Linha em que ocorreu o problema.
 *   - Nome do identificador, quando existir.
 *   - Motivo do erro.
 *
 * EXEMPLOS:
 *   - Variavel nao declarada.
 *   - Declaracao duplicada.
 *   - Tipos incompativeis.
 *   - Condicao nao booleana.
 *
 * IMPORTANTE:
 *   Nao confundir erros sintaticos com semanticos.
 *   Definir como liberar a tabela de simbolos
 *   antes de encerrar a execucao em caso de erro.
 */
static void erro(Parser *p) {
    if (p->token_atual.tipo != TOKEN_ERRO)
        fprintf(stderr, "Erro de sintaxe no token [%s]\n",
                p->token_atual.lexema);

    exit(EXIT_FAILURE);
}


/*
 * ESPERAR
 *
 * Verifica se o token atual corresponde ao esperado.
 *
 * Se nao corresponder, informa erro sintatico.
 * Caso contrario, consome o token.
 *
 * Preservar esta responsabilidade.
 */
static void esperar(Parser *p, TipoToken tipo) {
    if (p->token_atual.tipo != tipo)
        erro(p);

    avancar(p);
}


/*
 * EM
 *
 * Verifica se um token pertence a um conjunto.
 *
 * Utilizada para identificar operadores e
 * comandos reconhecidos pela gramatica.
 *
 * RETORNO:
 *   1 se o token pertence ao conjunto.
 *   0 caso contrario.
 */
static int em(TipoToken t, const TipoToken *ts, size_t n) {
    for (size_t i = 0; i < n; ++i)
        if (ts[i] == t)
            return 1;

    return 0;
}


/*
 * CONJUNTOS DE TOKENS
 *
 * Utilizados para reconhecer comandos e operadores.
 *
 * Preservar a precedencia definida pela gramatica.
 *
 * Ao integrar a analise semantica, guardar o operador
 * antes de chamar avancar(), pois o token atual muda.
 */
static const TipoToken inicio[] = {
    TOKEN_BEGIN,
    TOKEN_IDENTIFICADOR,
    TOKEN_WHILE,
    TOKEN_REPEAT,
    TOKEN_IF,
    TOKEN_WRITE
};

static const TipoToken logicos[] = {
    TOKEN_OR,
    TOKEN_AND
};

static const TipoToken relacionais[] = {
    TOKEN_IGUAL,
    TOKEN_DIFERENTE,
    TOKEN_MENOR,
    TOKEN_MAIOR,
    TOKEN_MENOR_IGUAL,
    TOKEN_MAIOR_IGUAL
};

static const TipoToken aditivos[] = {
    TOKEN_MAIS,
    TOKEN_MENOS
};

static const TipoToken multiplicativos[] = {
    TOKEN_MULT,
    TOKEN_DIV_REAL,
    TOKEN_DIV_INT
};


/*
 * TODO:
 * Alterar a declaracao de expressao() para TipoDado
 * quando a analise semantica for implementada.
 *
 * Atualizar tambem as chamadas correspondentes.
 */
// Declaração antecipada
static TipoDado expressao(Parser *p);
static void comando(Parser *p);
static void bloco(Parser *p);

/*
 * FATOR
 *
 * TODO: alterar o retorno de void para TipoDado.
 *
 * OBJETIVO:
 * Identificar o tipo basico de uma expressao.
 *
 * ROTEIRO:
 *
 * 1. LITERAL INTEIRO:
 *    - Reconhecer TOKEN_INTEIRO_LITERAL.
 *    - Consumir o token.
 *    - Retornar TIPO_INTEIRO.
 *
 * 2. LITERAL REAL:
 *    - Reconhecer TOKEN_REAL_LITERAL.
 *    - Consumir o token.
 *    - Retornar TIPO_REAL.
 *
 * 3. LITERAL CARACTERE:
 *    - Reconhecer TOKEN_CHAR_LITERAL.
 *    - Consumir o token.
 *    - Retornar TIPO_CHAR.
 *
 * 4. IDENTIFICADOR:
 *    a) Guardar o nome e a linha antes de avancar().
 *    b) Consultar tabela_buscar().
 *    c) Se retornar NULL, informar erro semantico
 *       de variavel nao declarada.
 *    d) Se encontrar, recuperar o tipo do simbolo.
 *    e) Consumir o identificador.
 *    f) Retornar o tipo encontrado.
 *
 * 5. EXPRESSAO ENTRE PARENTESES:
 *    a) Consumir TOKEN_ABRE_PAREN.
 *    b) Chamar expressao() e guardar seu tipo.
 *    c) Consumir TOKEN_FECHA_PAREN.
 *    d) Retornar o tipo da expressao.
 *
 * 6. OPERADOR NOT:
 *    a) Guardar o operador.
 *    b) Consumir TOKEN_NOT.
 *    c) Analisar o operando chamando fator().
 *    d) Chamar semantico_unaria().
 *    e) Exigir operando booleano.
 *    f) Retornar TIPO_BOOLEANO se for valido.
 *
 *    OBSERVACAO:
 *    A regra not booleano -> booleano foi adotada
 *    por consistencia com os operadores logicos.
 *    Ela nao esta explicita no enunciado fornecido.
 *
 * 7. MENOS UNARIO:
 *    a) Reconhecer TOKEN_MENOS no inicio do fator.
 *    b) Guardar o operador.
 *    c) Consumir TOKEN_MENOS.
 *    d) Analisar o operando.
 *    e) Chamar semantico_unaria().
 *    f) Aceitar somente integer ou real.
 *    g) Retornar o tipo do operando.
 *
 * REGRAS:
 *   -integer -> integer
 *   -real    -> real
 *   -char    -> erro semantico
 *   -boolean -> erro semantico
 *
 * IMPORTANTE:
 *   Se uma verificacao retornar TIPO_INVALIDO,
 *   informar erro semantico.
 *
 *   Se o token nao corresponder a um fator
 *   reconhecido, informar erro sintatico.
 *
 *   Nao construir AST.
 */
static TipoDado fator(Parser *p) {

    TipoDado tipo;
    TipoToken op;
    Token id;
    Simbolo *s;

    switch (p->token_atual.tipo) {

        case TOKEN_INTEIRO_LITERAL:
            avancar(p);
            return TIPO_INTEIRO;

        case TOKEN_REAL_LITERAL:
            avancar(p);
            return TIPO_REAL;
        
        case TOKEN_CHAR_LITERAL:
            avancar(p);
            return TIPO_CHAR;

        case TOKEN_IDENTIFICADOR: 
            id = p->token_atual;
            s = tabela_buscar(&p->tabela, id.lexema);
            if(s == NULL){
                erro_semantico(p, id.lexema, "variavel nao declarada");
                return TIPO_INVALIDO;
            }
            tipo = s->tipo;
            avancar(p);
            return tipo;

        case TOKEN_ABRE_PAREN:
            avancar(p);
            tipo = expressao(p);
            esperar(p, TOKEN_FECHA_PAREN);
            return tipo;

        case TOKEN_NOT:
            op = p->token_atual.tipo;
            avancar(p);
            tipo = fator(p);
            tipo = semantico_unaria(op, tipo);
            if(tipo == TIPO_INVALIDO){
                erro_semantico(p, NULL, "o operador not exige operando booleano");
                return TIPO_INVALIDO;
            }
            return tipo;

        case TOKEN_MENOS:
            op = p->token_atual.tipo;
            avancar(p);
            tipo = fator(p);
            tipo = semantico_unaria(op, tipo);
            if(tipo == TIPO_INVALIDO){
                erro_semantico(p, NULL, "o operador menos unario exige operando integer ou real");
                return TIPO_INVALIDO;
            }
            return tipo;

        default:
            erro(p);
            return TIPO_INVALIDO;
    }
}


/*
 * TERMO
 *
 * TODO: alterar o retorno de void para TipoDado.
 *
 * OBJETIVO:
 * Verificar as operacoes de multiplicacao e divisao.
 *
 * OPERADORES:
 *   *
 *   /
 *   div
 *
 * ROTEIRO:
 *   1. Chamar fator() e guardar o tipo retornado.
 *   2. Enquanto encontrar um operador multiplicativo:
 *      a) Guardar o operador antes de avancar().
 *      b) Consumir o operador.
 *      c) Chamar fator() para obter o tipo direito.
 *      d) Chamar semantico_binaria().
 *      e) Verificar se o resultado e TIPO_INVALIDO.
 *      f) Se for invalido, informar erro semantico.
 *      g) Atualizar o tipo acumulado.
 *   3. Retornar o tipo final.
 *
 * REGRAS:
 *   integer * integer   -> integer
 *   real * real         -> real
 *   integer div integer -> integer
 *   real / real         -> real
 *
 *   Qualquer outra combinacao e invalida.
 *
 * IMPORTANTE:
 *   Nao permitir conversao implicita de tipos.
 *   Preservar a associatividade a esquerda.
 */
// static void termo(Parser *p) {
//     fator(p);

//     while (em(p->token_atual.tipo, multiplicativos, 3)) {
//         avancar(p);
//         fator(p);
//     }
// }

static TipoDado termo(Parser *p) { //devolve TipoDado (n e mais void)
    TipoDado tipo_esq = fator(p); // guarda o tipo da esquerda
    
    while (em(p->token_atual.tipo, multiplicativos, 3)) { // achar *, / ou div
        Token op = p->token_atual; // salva qual é o operador antes de pular
        avancar(p);
        TipoDado tipo_dir = fator(p); // guarda o tipo da direita
        TipoDado resultado = semantico_binaria(op.tipo, tipo_esq, tipo_dir); //pergunta qual o tipo do resultado

        if (resultado == TIPO_INVALIDO) { //se não combinar (ex: 'a' * 5) é erro
            erro_semantico(p, op.linha, op.lexema, "tipos incompativeis na operacao");//Chamada da função de erro semântico da equipe
        }

        tipo_esq = resultado;
    }

    return tipo_esq; //devolver o tipo final da conta
}


/*
 * SOMA
 *
 * TODO: alterar o retorno de void para TipoDado.
 *
 * OBJETIVO:
 * Verificar as operacoes de adicao e subtracao.
 *
 * ROTEIRO:
 *   1. Chamar termo() e guardar o tipo retornado.
 *   2. Enquanto encontrar + ou -:
 *      a) Guardar o operador antes de avancar().
 *      b) Consumir o operador.
 *      c) Chamar termo() para obter o tipo direito.
 *      d) Chamar semantico_binaria().
 *      e) Verificar se o resultado e TIPO_INVALIDO.
 *      f) Se for invalido, informar erro semantico.
 *      g) Atualizar o tipo acumulado.
 *   3. Retornar o tipo final.
 *
 * REGRAS:
 *   integer + integer -> integer
 *   integer - integer -> integer
 *   real + real       -> real
 *   real - real       -> real
 *
 *   char + integer    -> char
 *   char - integer    -> char
 *
 * ATENCAO:
 *   A ordem dos operandos deve ser respeitada.
 *
 *   integer + char e invalido.
 *   integer - char e invalido.
 *
 *   Nao permitir conversao implicita entre
 *   integer e real.
 *
 *   Preservar a associatividade a esquerda.
 */
// static void soma(Parser *p) {
//     termo(p);

//     while (em(p->token_atual.tipo, aditivos, 2)) {
//         avancar(p);
//         termo(p);
//     }
// }

static TipoDado soma(Parser *p) {
    TipoDado tipo_esq = termo(p);
    
    while (em(p->token_atual.tipo, aditivos, 2)){
        Token op = p->token_atual;
        avancar(p);
        TipoDado tipo_dir = termo(p);
        TipoDado resultado = semantico_binaria(op.tipo, tipo_esq, tipo_dir);
        
        if (resultado == TIPO_INVALIDO) {
            erro_semantico(p, op.linha, op.lexema, "tipos incompativeis na operacao");
        }

        tipo_esq = resultado;
    }

    return tipo_esq;
}


/*
 * RELACIONAL
 *
 * TODO: alterar o retorno de void para TipoDado.
 *
 * OBJETIVO:
 * Verificar as operacoes de comparacao.
 *
 * OPERADORES:
 *   <
 *   <=
 *   >
 *   >=
 *   =
 *   <>
 *
 * ROTEIRO:
 *   1. Chamar soma() e guardar o tipo retornado.
 *   2. Enquanto encontrar um operador relacional:
 *      a) Guardar o operador.
 *      b) Consumir o operador.
 *      c) Chamar soma() para obter o tipo direito.
 *      d) Chamar semantico_binaria().
 *      e) Verificar se o resultado e TIPO_INVALIDO.
 *      f) Se for invalido, informar erro semantico.
 *      g) Atualizar o tipo acumulado.
 *   3. Retornar o tipo final.
 *
 * REGRAS:
 *
 * 1. <, <=, >, >=:
 *    integer com integer -> booleano
 *    real com real       -> booleano
 *
 *    Nao permitem tipos diferentes.
 *    Nao permitem char nem booleano.
 *
 * 2. = e <>:
 *    integer com integer   -> booleano
 *    real com real         -> booleano
 *    char com char         -> booleano
 *    booleano com booleano -> booleano
 *
 *    Nao permitem operandos de tipos diferentes.
 *
 * IMPORTANTE:
 *   Comparacoes validas retornam TIPO_BOOLEANO.
 *
 *   Em comparacoes encadeadas, cada nova operacao
 *   deve verificar o tipo resultante da anterior.
 *
 *   Preservar a gramatica e a precedencia atuais.
 */
// static void relacional(Parser *p) {
//     soma(p);

//     while (em(p->token_atual.tipo, relacionais, 6)) {
//         avancar(p);
//         soma(p);
//     }
// }

static TipoDado relacional(Parser *p) {
    TipoDado tipo_esq = soma(p);

    while (em(p->token_atual.tipo, relacionais, 6)) {
        Token op = p->token_atual;
        avancar(p);
        TipoDado tipo_dir = soma(p);
        TipoDado resultado = semantico_binaria(op.tipo, tipo_esq, tipo_dir);

        if (resultado == TIPO_INVALIDO) {
            erro_semantico(p, op.linha, op.lexema, "tipos incompativeis na comparacao");
        }

        tipo_esq = resultado;
    }

    return tipo_esq;
}


/*
 * EXPRESSAO
 *
 * TODO: alterar o retorno de void para TipoDado.
 *
 * OBJETIVO:
 * Determinar o tipo final de uma expressao.
 *
 * ROTEIRO:
 *   1. Chamar relacional() e guardar o tipo.
 *   2. Enquanto encontrar and ou or:
 *      a) Guardar o operador.
 *      b) Consumir o operador.
 *      c) Chamar relacional() para obter o tipo direito.
 *      d) Chamar semantico_binaria().
 *      e) Verificar se o resultado e TIPO_INVALIDO.
 *      f) Se for invalido, informar erro semantico.
 *      g) Atualizar o tipo acumulado.
 *   3. Retornar o tipo final.
 *
 * REGRAS:
 *   booleano and booleano -> booleano
 *   booleano or booleano  -> booleano
 *
 *   Qualquer outra combinacao e invalida.
 *
 * IMPORTANTE:
 *   Nao construir AST.
 *   Propagar apenas os tipos das subexpressoes.
 *
 *   Preservar a precedencia definida pela gramatica.
 */
// static void expressao(Parser *p) {
//     relacional(p);

//     while (em(p->token_atual.tipo, logicos, 2)) {
//         avancar(p);
//         relacional(p);
//     }
// }

static TipoDado expressao(Parser *p) {
    TipoDado tipo_esq = relacional(p);

    while (em(p->token_atual.tipo, logicos, 2)) {
        Token op = p->token_atual;
        avancar(p);
        TipoDado tipo_dir = relacional(p);
        TipoDado resultado = semantico_binaria(op.tipo, tipo_esq, tipo_dir);

        if (resultado == TIPO_INVALIDO) {
            erro_semantico(p, op.linha, op.lexema, "tipos incompativeis na operacao logica");
        }

        tipo_esq = resultado;
    }

    return tipo_esq;
}


static void declaracao(Parser *p) {
    typedef struct {
        char nome[TAM_NOME];
        int linha;
    } IdentificadorPendente;

    size_t quantidade = 0;
    size_t capacidade = 4;
    IdentificadorPendente *ids = malloc(capacidade * sizeof *ids);
    if (ids == NULL) {
        fprintf(stderr, "Erro semantico: memoria insuficiente ao ler declaracao.\n");
        tabela_liberar(&p->tabela);
        exit(EXIT_FAILURE);
    }

    for (;;) {
        if (p->token_atual.tipo != TOKEN_IDENTIFICADOR) {
            free(ids);
            erro(p);
        }

        if (quantidade == capacidade) {
            capacidade *= 2;
            IdentificadorPendente *novo = realloc(ids, capacidade * sizeof *ids);
            if (novo == NULL) {
                fprintf(stderr, "Erro semantico: memoria insuficiente ao ler declaracao.\n");
                free(ids);
                tabela_liberar(&p->tabela);
                exit(EXIT_FAILURE);
            }
            ids = novo;
        }

        memcpy(ids[quantidade].nome, p->token_atual.lexema, TAM_NOME);
        ids[quantidade].nome[TAM_NOME - 1] = '\0';
        ids[quantidade].linha = p->token_atual.linha;
        ++quantidade;
        avancar(p);

        if (p->token_atual.tipo != TOKEN_VIRGULA)
            break;
        avancar(p);
    }

    esperar(p, TOKEN_DOISPONTOS);

    TipoDado tipo;
    switch (p->token_atual.tipo) {
        case TOKEN_INTEGER: tipo = TIPO_INTEIRO; break;
        case TOKEN_REAL:    tipo = TIPO_REAL;    break;
        case TOKEN_CHAR:    tipo = TIPO_CHAR;    break;
        default:
            free(ids);
            erro(p);
            return;
    }
    avancar(p);
    esperar(p, TOKEN_PONTOVIRG);

    for (size_t i = 0; i < quantidade; ++i) {
        int resultado = tabela_inserir(&p->tabela, ids[i].nome, tipo, ids[i].linha);
        if (resultado != 1) {
            if (resultado == 0) {
                fprintf(stderr,
                        "Erro semantico na linha %d: identificador '%s' ja declarado.\n",
                        ids[i].linha, ids[i].nome);
            } else {
                fprintf(stderr,
                        "Erro semantico na linha %d: nao foi possivel registrar '%s'.\n",
                        ids[i].linha, ids[i].nome);
            }
            free(ids);
            tabela_liberar(&p->tabela);
            exit(EXIT_FAILURE);
        }
    }

    free(ids);
}


/*
 * BLOCO
 *
 * Reconhece comandos delimitados por begin e end.
 *
 * ROTEIRO:
 *   1. Consumir TOKEN_BEGIN.
 *   2. Reconhecer os comandos internos.
 *   3. Consumir TOKEN_END.
 *
 * IMPORTANTE:
 *   A gramatica atual declara variaveis apenas
 *   na secao var do programa.
 *
 *   Nao criar automaticamente novos escopos
 *   para cada begin/end.
 *
 *   As verificacoes semanticas dos comandos
 *   internos serao realizadas por comando().
 */
static void bloco(Parser *p) {
    esperar(p, TOKEN_BEGIN);

    while (em(p->token_atual.tipo, inicio, 6))
        comando(p);

    esperar(p, TOKEN_END);
}


/*
 * COMANDO
 *
 * TODO: integrar as verificacoes semanticas
 * dos comandos reconhecidos pelo Parser.
 *
 * 1. ATRIBUICAO:
 *    a) Guardar nome e linha da variavel destino.
 *    b) Consultar tabela_buscar().
 *    c) Se nao existir, informar erro semantico.
 *    d) Guardar o tipo da variavel destino.
 *    e) Consumir TOKEN_ATRIBUICAO.
 *    f) Analisar a expressao do lado direito.
 *    g) Guardar o tipo retornado por expressao().
 *    h) Chamar semantico_atribuicao().
 *    i) Se retornar 0, informar erro semantico
 *       de tipos incompativeis.
 *
 *    REGRAS:
 *      integer := integer -> valido
 *      real := real       -> valido
 *      char := char       -> valido
 *
 *      integer := real    -> invalido
 *      real := integer    -> invalido
 *      char := integer    -> invalido
 *
 *    Nao permitir conversao implicita de tipos.
 *
 * 2. WHILE:
 *    a) Consumir TOKEN_WHILE.
 *    b) Chamar expressao() e guardar o tipo.
 *    c) Chamar semantico_condicao().
 *    d) Se retornar 0, informar erro semantico.
 *    e) Consumir TOKEN_DO.
 *    f) Analisar o comando do corpo.
 *
 *    A condicao deve ser TIPO_BOOLEANO.
 *
 * 3. REPEAT/UNTIL:
 *    a) Consumir TOKEN_REPEAT.
 *    b) Analisar os comandos do corpo conforme
 *       a gramatica atual.
 *    c) Consumir TOKEN_UNTIL.
 *    d) Chamar expressao() e guardar o tipo.
 *    e) Chamar semantico_condicao().
 *    f) Se retornar 0, informar erro semantico.
 *
 *    A condicao deve ser TIPO_BOOLEANO.
 *
 * 4. IF:
 *    a) Consumir TOKEN_IF.
 *    b) Chamar expressao() e guardar o tipo.
 *    c) Chamar semantico_condicao().
 *    d) Se retornar 0, informar erro semantico.
 *    e) Consumir TOKEN_THEN.
 *    f) Analisar o comando apos then.
 *    g) Se existir else, analisar seu comando.
 *
 *    A condicao deve ser TIPO_BOOLEANO.
 *
 * 5. WRITE:
 *    a) Consumir TOKEN_WRITE.
 *    b) Consumir TOKEN_ABRE_PAREN.
 *    c) Chamar expressao().
 *    d) Verificar os tipos da expressao.
 *    e) Verificar se todos os identificadores
 *       utilizados foram declarados.
 *    f) Consumir TOKEN_FECHA_PAREN.
 *
 *    O enunciado nao exige tipo especifico
 *    para a expressao de write.
 *
 * 6. BEGIN/END:
 *    a) Chamar bloco().
 *    b) Analisar os comandos internos.
 *
 * IMPORTANTE:
 *   Preservar a pontuacao e as regras sintaticas
 *   ja existentes.
 *
 *   Nao construir AST.
 */
static void comando(Parser *p) {
    switch (p->token_atual.tipo) {
        case TOKEN_BEGIN:
            bloco(p);
            esperar(p, TOKEN_PONTOVIRG);
            return;

        case TOKEN_IDENTIFICADOR:
            avancar(p);
            esperar(p, TOKEN_ATRIBUICAO);
            expressao(p);
            esperar(p, TOKEN_PONTOVIRG);
            return;

        case TOKEN_WHILE:
            avancar(p);
            expressao(p);
            esperar(p, TOKEN_DO);
            comando(p);
            return;

        case TOKEN_REPEAT:
            avancar(p);
            comando(p);
            esperar(p, TOKEN_UNTIL);
            expressao(p);
            esperar(p, TOKEN_PONTOVIRG);
            return;

        case TOKEN_IF:
            avancar(p);
            expressao(p);
            esperar(p, TOKEN_THEN);
            comando(p);

            if (p->token_atual.tipo == TOKEN_ELSE) {
                avancar(p);
                comando(p);
            }
            return;

        case TOKEN_WRITE:
            avancar(p);
            esperar(p, TOKEN_ABRE_PAREN);
            expressao(p);
            esperar(p, TOKEN_FECHA_PAREN);
            esperar(p, TOKEN_PONTOVIRG);
            return;

        default:
            erro(p);
    }
}


/*
 * PARSER_INICIALIZAR
 *
 * TODO: inicializar a tabela de simbolos.
 *
 * ROTEIRO:
 *   1. Associar o Lexer ao Parser.
 *   2. Inicializar a tabela hash chamando
 *      tabela_inicializar().
 *   3. Ler o primeiro token usando avancar().
 *
 * IMPORTANTE:
 *   A estrutura Parser em parser.h deve possuir
 *   um campo para armazenar a TabelaSimbolos,
 *   ou outra referencia gerenciada de forma
 *   consistente.
 *
 *   A tabela deve estar inicializada antes
 *   de registrar ou consultar identificadores.
 *
 *   Definir tambem uma rotina de finalizacao
 *   para chamar tabela_liberar().
 */
void parser_inicializar(Parser *p, Lexer *lexer) {
    p->lexer = lexer;
    tabela_inicializar(&p->tabela);
    avancar(p);
}


/*
 * PARSER_ANALISAR_PROGRAMA
 *
 * TODO: integrar a tabela hash e a analise semantica
 * ao reconhecimento completo do programa.
 *
 * GRAMATICA:
 *   program IDENT;
 *   var
 *       declaracoes
 *   begin
 *       comandos
 *   end.
 *
 * ROTEIRO:
 *   1. Reconhecer TOKEN_PROGRAM.
 *   2. Reconhecer o identificador do programa.
 *   3. Consumir TOKEN_PONTOVIRG.
 *   4. Reconhecer TOKEN_VAR.
 *   5. Processar as declaracoes chamando declaracao().
 *   6. Registrar as variaveis na tabela hash.
 *   7. Analisar os comandos chamando bloco().
 *   8. Aplicar as verificacoes semanticas
 *      durante a analise dos comandos.
 *   9. Consumir TOKEN_PONTO.
 *  10. Verificar se chegou a TOKEN_EOF.
 *
 * REGRAS:
 *   - O nome do programa nao deve ser registrado
 *     como declaracao de variavel.
 *   - As declaracoes alimentam a tabela hash.
 *   - As expressoes consultam os tipos registrados.
 *   - Os comandos aplicam as regras semanticas.
 *
 * FINALIZACAO:
 *   Definir uma rotina parser_finalizar() para
 *   liberar a tabela de simbolos quando a analise
 *   terminar.
 *
 *   Atualizar parser.h e main.c para utilizar
 *   essa rotina de maneira consistente.
 *
 * IMPORTANTE:
 *   Preservar a gramatica existente.
 *   Nao construir AST.
 */
void parser_analisar_programa(Parser *p) {
    esperar(p, TOKEN_PROGRAM);
    esperar(p, TOKEN_IDENTIFICADOR);
    esperar(p, TOKEN_PONTOVIRG);
    esperar(p, TOKEN_VAR);

    while (p->token_atual.tipo == TOKEN_IDENTIFICADOR)
        declaracao(p);

    bloco(p);
    esperar(p, TOKEN_PONTO);

    if (p->token_atual.tipo != TOKEN_EOF)
        erro(p);

    tabela_liberar(&p->tabela);
}
