#ifndef ARVORE_H
#define ARVORE_H

#include "paciente.h"


typedef struct NoArvore {
    Paciente *paciente;
    struct NoArvore *esquerda;
    struct NoArvore *direita;
} NoArvore;


NoArvore* inserirNaArvore(NoArvore *raiz, Paciente *paciente);
Paciente* buscarNaArvore(NoArvore *raiz, const char *cpf);
NoArvore* removerDaArvore(NoArvore *raiz, const char *cpf);
void percorrerInOrder(NoArvore *raiz);
void liberarArvore(NoArvore *raiz);
int obterAlturaArvore(NoArvore *raiz);

#endif
