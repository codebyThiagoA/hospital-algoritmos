#ifndef ARVORE_H
#define ARVORE_H

#include "paciente.h"

// Estrutura do nó da árvore binária de busca
typedef struct NoArvore {
    Paciente *paciente;
    struct NoArvore *esquerda;
    struct NoArvore *direita;
} NoArvore;

// Funções da árvore binária de busca
NoArvore* inserirNaArvore(NoArvore *raiz, Paciente *paciente);
Paciente* buscarNaArvore(NoArvore *raiz, const char *cpf);
NoArvore* removerDaArvore(NoArvore *raiz, const char *cpf);
void percorrerInOrder(NoArvore *raiz);
void liberarArvore(NoArvore *raiz);

#endif
