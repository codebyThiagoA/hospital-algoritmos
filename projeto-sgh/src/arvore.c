#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

// Função auxiliar para criar um novo nó
static NoArvore* criarNo(Paciente *paciente) {
    NoArvore *novo = (NoArvore*) malloc(sizeof(NoArvore));
    if (!novo) return NULL;
    novo->paciente = paciente;
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Inserir um paciente na árvore binária de busca por CPF
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
    // Se cmp == 0, o CPF já existe, não insere duplicado
    
    return raiz;
}

// Buscar um paciente na árvore por CPF
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

// Função auxiliar para encontrar o nó com o menor valor (mais à esquerda)
static NoArvore* encontrarMinimo(NoArvore *no) {
    while (no && no->esquerda) {
        no = no->esquerda;
    }
    return no;
}

// Remover um paciente da árvore por CPF
NoArvore* removerDaArvore(NoArvore *raiz, const char *cpf) {
    if (!raiz) return NULL;
    
    int cmp = strcmp(cpf, raiz->paciente->cpf);
    
    if (cmp < 0) {
        raiz->esquerda = removerDaArvore(raiz->esquerda, cpf);
    } else if (cmp > 0) {
        raiz->direita = removerDaArvore(raiz->direita, cpf);
    } else {
        // Nó encontrado - casos de remoção
        
        // Caso 1: Nó folha ou com apenas um filho
        if (!raiz->esquerda) {
            NoArvore *temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (!raiz->direita) {
            NoArvore *temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
        
        // Caso 2: Nó com dois filhos
        // Encontra o sucessor in-order (menor nó da subárvore direita)
        NoArvore *temp = encontrarMinimo(raiz->direita);
        
        // Copia o conteúdo do sucessor para este nó
        raiz->paciente = temp->paciente;
        
        // Remove o sucessor
        raiz->direita = removerDaArvore(raiz->direita, temp->paciente->cpf);
    }
    
    return raiz;
}

// Percorrer a árvore em ordem (in-order) e listar pacientes ordenados por CPF
void percorrerInOrder(NoArvore *raiz) {
    if (!raiz) return;
    
    percorrerInOrder(raiz->esquerda);
    
    printf("Nome: %s | CPF: %s | Idade: %d | Prioridade: %d\n",
           raiz->paciente->nome, raiz->paciente->cpf, 
           raiz->paciente->idade, raiz->paciente->prioridade);
    
    percorrerInOrder(raiz->direita);
}

// Liberar toda a árvore
void liberarArvore(NoArvore *raiz) {
    if (!raiz) return;
    
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

// Obter altura da árvore binária de busca
int obterAlturaArvore(NoArvore *raiz) {
    if (!raiz) return 0;
    
    int altEsq = obterAlturaArvore(raiz->esquerda);
    int altDir = obterAlturaArvore(raiz->direita);
    
    return (altEsq > altDir ? altEsq : altDir) + 1;
}
