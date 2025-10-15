#ifndef HISTORICO_H
#define HISTORICO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Atendimento {
    char descricao[200];
    char data[20];
    struct Atendimento *prox;
} Atendimento;

typedef struct {
    Atendimento *topo;
    int tamanho;
} Historico;

void inicializarHistorico(Historico *h);
void pushAtendimento(Historico *h, const char *descricao, const char *data);
void mostrarHistorico(Historico *h);
void limparHistorico(Historico *h);

#endif

