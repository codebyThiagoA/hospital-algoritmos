#include "historico.h"

void inicializarHistorico(Historico *h) {
    h->topo = NULL;
    h->tamanho = 0;
}

void pushAtendimento(Historico *h, const char *descricao, const char *data) {
    Atendimento *novo = (Atendimento*) malloc(sizeof(Atendimento));
    if (!novo) {
        printf("Erro: falha ao alocar memoria para atendimento.\n");
        return;
    }

    strcpy(novo->descricao, descricao);
    strcpy(novo->data, data);

    novo->prox = h->topo;
    h->topo = novo;
    h->tamanho++;

    printf("Atendimento adicionado ao historico!\n");
}


void mostrarHistorico(Historico *h) {
    if (h->tamanho == 0) {
        printf("\n📋 Histórico vazio.\n");
        return;
    }

    printf("\n--- Historico de Atendimentos (%d registros) ---\n", h->tamanho);
    Atendimento *atual = h->topo;
    while (atual) {
        printf("- %s | Data: %s\n", atual->descricao, atual->data);
        atual = atual->prox;
    }
}

void limparHistorico(Historico *h) {
    Atendimento *atual = h->topo;
    while (atual) {
        Atendimento *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    h->topo = NULL;
    h->tamanho = 0;
}
