#ifndef SEMANTICO_H
#define SEMANTICO_H
#include "token.h"
#include "tabela_simbolos.h"


TipoDado semantico_binaria(TipoToken operador, TipoDado esquerdo, TipoDado direito);

TipoDado semantico_unaria(TipoToken operador, TipoDado operando);

int semantico_atribuicao(TipoDado destino, TipoDado origem);

int semantico_condicao(TipoDado tipo);
#endif
