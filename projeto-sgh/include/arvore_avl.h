#ifndef ARVORE_AVL_H
#define ARVORE_AVL_H

#include "paciente.h"


typedef struct NoAVL {
    Paciente *paciente;
    struct NoAVL *esquerda;
    struct NoAVL *direita;
    int altura;  
} NoAVL;


NoAVL* inserirNaAVL(NoAVL *raiz, Paciente *paciente);
Paciente* buscarNaAVL(NoAVL *raiz, const char *cpf);
NoAVL* removerDaAVL(NoAVL *raiz, const char *cpf);
void percorrerInOrderAVL(NoAVL *raiz);
void liberarAVL(NoAVL *raiz);


int obterAlturaAVL(NoAVL *raiz);

int verificarBalanceamento(NoAVL *raiz);

#endif
