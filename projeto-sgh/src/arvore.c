#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"


static NoArvore* criarNo(Paciente *paciente) {
    NoArvore *novo = (NoArvore*) malloc(sizeof(NoArvore));
    if (!novo) return NULL;
    novo->paciente = paciente;
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}


NoArvore* inserirNaArvore(NoArvore *raiz, Paciente *paciente) {
    if (!paciente) return raiz;
    
    if (!raiz) {
        return criarNo(paciente);
    }
    
    int cmp = strcmp(paciente->cpf, raiz->paciente->cpf);
    
    if (cmp < 0) {
        raiz->esquerda = inserirNaArvore(raiz->esquerda, paciente);
    } else if (cmp > 0) {
        raiz->direita = inserirNaArvore(raiz->direita, paciente);
    }
   
    
    return raiz;
}


Paciente* buscarNaArvore(NoArvore *raiz, const char *cpf) {
    if (!raiz) return NULL;
    
    int cmp = strcmp(cpf, raiz->paciente->cpf);
    
    if (cmp == 0) {
        return raiz->paciente;
    } else if (cmp < 0) {
        return buscarNaArvore(raiz->esquerda, cpf);
    } else {
        return buscarNaArvore(raiz->direita, cpf);
    }
}


static NoArvore* encontrarMinimo(NoArvore *no) {
    while (no && no->esquerda) {
        no = no->esquerda;
    }
    return no;
}


NoArvore* removerDaArvore(NoArvore *raiz, const char *cpf) {
    if (!raiz) return NULL;
    
    int cmp = strcmp(cpf, raiz->paciente->cpf);
    
    if (cmp < 0) {
        raiz->esquerda = removerDaArvore(raiz->esquerda, cpf);
    } else if (cmp > 0) {
        raiz->direita = removerDaArvore(raiz->direita, cpf);
    } else {
        
        
       
        if (!raiz->esquerda) {
            NoArvore *temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (!raiz->direita) {
            NoArvore *temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
        
       
        NoArvore *temp = encontrarMinimo(raiz->direita);
        
       
        raiz->paciente = temp->paciente;
        
       
        raiz->direita = removerDaArvore(raiz->direita, temp->paciente->cpf);
    }
    
    return raiz;
}


void percorrerInOrder(NoArvore *raiz) {
    if (!raiz) return;
    
    percorrerInOrder(raiz->esquerda);
    
    printf("Nome: %s | CPF: %s | Idade: %d | Prioridade: %d\n",
           raiz->paciente->nome, raiz->paciente->cpf, 
           raiz->paciente->idade, raiz->paciente->prioridade);
    
    percorrerInOrder(raiz->direita);
}


void liberarArvore(NoArvore *raiz) {
    if (!raiz) return;
    
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

int obterAlturaArvore(NoArvore *raiz) {
    if (!raiz) return 0;
    
    int altEsq = obterAlturaArvore(raiz->esquerda);
    int altDir = obterAlturaArvore(raiz->direita);
    
    return (altEsq > altDir ? altEsq : altDir) + 1;
}
