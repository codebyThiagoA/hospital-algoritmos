#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_avl.h"


static int altura(NoAVL *no) {
    if (!no) return 0;
    return no->altura;
}


static int fatorBalanceamento(NoAVL *no) {
    if (!no) return 0;
    return altura(no->esquerda) - altura(no->direita);
}


static void atualizarAltura(NoAVL *no) {
    if (!no) return;
    int altEsq = altura(no->esquerda);
    int altDir = altura(no->direita);
    no->altura = (altEsq > altDir ? altEsq : altDir) + 1;
}


static NoAVL* rotacaoDireita(NoAVL *y) {
    NoAVL *x = y->esquerda;
    NoAVL *t2 = x->direita;

    
    x->direita = y;
    y->esquerda = t2;

   
    atualizarAltura(y);
    atualizarAltura(x);

    return x;
}


static NoAVL* rotacaoEsquerda(NoAVL *x) {
    NoAVL *y = x->direita;
    NoAVL *t2 = y->esquerda;

   
    y->esquerda = x;
    x->direita = t2;

   
    atualizarAltura(x);
    atualizarAltura(y);

    return y;
}


static NoAVL* criarNoAVL(Paciente *paciente) {
    NoAVL *novo = (NoAVL*) malloc(sizeof(NoAVL));
    if (!novo) return NULL;
    novo->paciente = paciente;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->altura = 1;
    return novo;
}


NoAVL* inserirNaAVL(NoAVL *raiz, Paciente *paciente) {
    if (!paciente) return raiz;
    
    
    if (!raiz) {
        return criarNoAVL(paciente);
    }
    
    int cmp = strcmp(paciente->cpf, raiz->paciente->cpf);
    
    if (cmp < 0) {
        raiz->esquerda = inserirNaAVL(raiz->esquerda, paciente);
    } else if (cmp > 0) {
        raiz->direita = inserirNaAVL(raiz->direita, paciente);
    } else {
        
        return raiz;
    }
    
    
    atualizarAltura(raiz);
    
   
    int fb = fatorBalanceamento(raiz);
    
   
    
    
    if (fb > 1 && fatorBalanceamento(raiz->esquerda) >= 0) {
        return rotacaoDireita(raiz);
    }
    
   
    if (fb < -1 && fatorBalanceamento(raiz->direita) <= 0) {
        return rotacaoEsquerda(raiz);
    }
    
    
    if (fb > 1 && fatorBalanceamento(raiz->esquerda) < 0) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }
    
  
    if (fb < -1 && fatorBalanceamento(raiz->direita) > 0) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }
    
    return raiz;
}


Paciente* buscarNaAVL(NoAVL *raiz, const char *cpf) {
    if (!raiz) return NULL;
    
    int cmp = strcmp(cpf, raiz->paciente->cpf);
    
    if (cmp == 0) {
        return raiz->paciente;
    } else if (cmp < 0) {
        return buscarNaAVL(raiz->esquerda, cpf);
    } else {
        return buscarNaAVL(raiz->direita, cpf);
    }
}


static NoAVL* encontrarMinimoAVL(NoAVL *no) {
    while (no && no->esquerda) {
        no = no->esquerda;
    }
    return no;
}


NoAVL* removerDaAVL(NoAVL *raiz, const char *cpf) {
    if (!raiz) return NULL;
    
    int cmp = strcmp(cpf, raiz->paciente->cpf);
    
    if (cmp < 0) {
        raiz->esquerda = removerDaAVL(raiz->esquerda, cpf);
    } else if (cmp > 0) {
        raiz->direita = removerDaAVL(raiz->direita, cpf);
    } else {
      
        
        
        if (!raiz->esquerda) {
            NoAVL *temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (!raiz->direita) {
            NoAVL *temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
        
       
        
        NoAVL *temp = encontrarMinimoAVL(raiz->direita);
        
        
        raiz->paciente = temp->paciente;
        
        
        raiz->direita = removerDaAVL(raiz->direita, temp->paciente->cpf);
    }
    
    if (!raiz) return NULL;
    
    
    atualizarAltura(raiz);
    
   
    int fb = fatorBalanceamento(raiz);
    
    
    
    
    if (fb > 1 && fatorBalanceamento(raiz->esquerda) >= 0) {
        return rotacaoDireita(raiz);
    }
    
    
    if (fb < -1 && fatorBalanceamento(raiz->direita) <= 0) {
        return rotacaoEsquerda(raiz);
    }
    
    
    if (fb > 1 && fatorBalanceamento(raiz->esquerda) < 0) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }
    
    
    if (fb < -1 && fatorBalanceamento(raiz->direita) > 0) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }
    
    return raiz;
}


void percorrerInOrderAVL(NoAVL *raiz) {
    if (!raiz) return;
    
    percorrerInOrderAVL(raiz->esquerda);
    
    printf("Nome: %s | CPF: %s | Idade: %d | Prioridade: %d\n",
           raiz->paciente->nome, raiz->paciente->cpf, 
           raiz->paciente->idade, raiz->paciente->prioridade);
    
    percorrerInOrderAVL(raiz->direita);
}


void liberarAVL(NoAVL *raiz) {
    if (!raiz) return;
    
    liberarAVL(raiz->esquerda);
    liberarAVL(raiz->direita);
    free(raiz);
}

int obterAlturaAVL(NoAVL *raiz) {
    if (!raiz) return 0;
    return raiz->altura;
}


static int verificarBalanceamentoAux(NoAVL *raiz) {
    if (!raiz) return 1;
    
    
    if (!verificarBalanceamentoAux(raiz->esquerda)) {
        return 0;
    }
    
   
    if (!verificarBalanceamentoAux(raiz->direita)) {
        return 0;
    }
    
   
    int fb = fatorBalanceamento(raiz);
    if (fb < -1 || fb > 1) {
        return 0;
    }
    
    return 1;
}


int verificarBalanceamento(NoAVL *raiz) {
    return verificarBalanceamentoAux(raiz);
}
