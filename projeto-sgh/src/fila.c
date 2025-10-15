#include <stdio.h>
#include <stdlib.h>
#include "fila.h"


void filaInit(Fila *f) {
    f->inicio = NULL;
    f->fim = NULL;
}


int filaVazia(Fila *f) {
    return (f->inicio == NULL);
}


void enfileirar(Fila *f, Paciente *p) {
    NoFila *novo = (NoFila*) malloc(sizeof(NoFila));
    if (!novo) {
        printf("Erro de alocacao.\n");
        return;
    }
    novo->paciente = p;
    novo->prox = NULL;


    if (filaVazia(f)) {
        f->inicio = novo;
    } else {
        f->fim->prox = novo;
    }
    f->fim = novo;
}


Paciente* desenfileirar(Fila *f) {
    if (filaVazia(f)) {
        printf("Fila vazia!\n");
        return NULL;
    }


    NoFila *remover = f->inicio;
    Paciente *p = remover->paciente;
    f->inicio = remover->prox;


    if (f->inicio == NULL) {
        f->fim = NULL;
    }


    free(remover);
    return p;
}


void mostrarFila(Fila *f) {
    if (filaVazia(f)) {
        printf("Fila vazia.\n");
        return;
    }


    NoFila *atual = f->inicio;
    printf("\n--- Fila de Atendimento ---\n");
    while (atual != NULL) {
        printf("Paciente: %s | CPF: %s | Idade: %d | Prioridade: %d\n",
               atual->paciente->nome,
               atual->paciente->cpf,
               atual->paciente->idade,
               atual->paciente->prioridade);
        atual = atual->prox;
    }
}
