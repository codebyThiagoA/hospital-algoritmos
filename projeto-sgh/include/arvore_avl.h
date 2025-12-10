#ifndef ARVORE_AVL_H
#define ARVORE_AVL_H

#include "paciente.h"

// Estrutura do nó da árvore AVL
typedef struct NoAVL {
    Paciente *paciente;
    struct NoAVL *esquerda;
    struct NoAVL *direita;
    int altura;  // Altura do nó
} NoAVL;

// Funções da árvore AVL
NoAVL* inserirNaAVL(NoAVL *raiz, Paciente *paciente);
Paciente* buscarNaAVL(NoAVL *raiz, const char *cpf);
NoAVL* removerDaAVL(NoAVL *raiz, const char *cpf);
void percorrerInOrderAVL(NoAVL *raiz);
void liberarAVL(NoAVL *raiz);

// Função para obter a altura da árvore (útil para análise)
int obterAlturaAVL(NoAVL *raiz);

// Função para verificar se a árvore está balanceada
int verificarBalanceamento(NoAVL *raiz);

#endif
