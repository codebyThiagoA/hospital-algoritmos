#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_avl.h"

// Função auxiliar para obter a altura de um nó
static int altura(NoAVL *no) {
    if (!no) return 0;
    return no->altura;
}

// Função auxiliar para calcular o fator de balanceamento
static int fatorBalanceamento(NoAVL *no) {
    if (!no) return 0;
    return altura(no->esquerda) - altura(no->direita);
}

// Função auxiliar para atualizar a altura de um nó
static void atualizarAltura(NoAVL *no) {
    if (!no) return;
    int altEsq = altura(no->esquerda);
    int altDir = altura(no->direita);
    no->altura = (altEsq > altDir ? altEsq : altDir) + 1;
}

// Rotação simples à direita
static NoAVL* rotacaoDireita(NoAVL *y) {
    NoAVL *x = y->esquerda;
    NoAVL *t2 = x->direita;

    // Realiza rotação
    x->direita = y;
    y->esquerda = t2;

    // Atualiza alturas
    atualizarAltura(y);
    atualizarAltura(x);

    return x;
}

// Rotação simples à esquerda
static NoAVL* rotacaoEsquerda(NoAVL *x) {
    NoAVL *y = x->direita;
    NoAVL *t2 = y->esquerda;

    // Realiza rotação
    y->esquerda = x;
    x->direita = t2;

    // Atualiza alturas
    atualizarAltura(x);
    atualizarAltura(y);

    return y;
}

// Função auxiliar para criar um novo nó
static NoAVL* criarNoAVL(Paciente *paciente) {
    NoAVL *novo = (NoAVL*) malloc(sizeof(NoAVL));
    if (!novo) return NULL;
    novo->paciente = paciente;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->altura = 1;
    return novo;
}

// Inserir um paciente na árvore AVL
NoAVL* inserirNaAVL(NoAVL *raiz, Paciente *paciente) {
    if (!paciente) return raiz;
    
    // Passo 1: Inserção normal na BST
    if (!raiz) {
        return criarNoAVL(paciente);
    }
    
    int cmp = strcmp(paciente->cpf, raiz->paciente->cpf);
    
    if (cmp < 0) {
        raiz->esquerda = inserirNaAVL(raiz->esquerda, paciente);
    } else if (cmp > 0) {
        raiz->direita = inserirNaAVL(raiz->direita, paciente);
    } else {
        // CPF já existe, não insere duplicado
        return raiz;
    }
    
    // Passo 2: Atualiza altura do nó
    atualizarAltura(raiz);
    
    // Passo 3: Obtém fator de balanceamento
    int fb = fatorBalanceamento(raiz);
    
    // Passo 4: Se desbalanceado, existem 4 casos
    
    // Caso 1: Esquerda-Esquerda
    if (fb > 1 && fatorBalanceamento(raiz->esquerda) >= 0) {
        return rotacaoDireita(raiz);
    }
    
    // Caso 2: Direita-Direita
    if (fb < -1 && fatorBalanceamento(raiz->direita) <= 0) {
        return rotacaoEsquerda(raiz);
    }
    
    // Caso 3: Esquerda-Direita
    if (fb > 1 && fatorBalanceamento(raiz->esquerda) < 0) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }
    
    // Caso 4: Direita-Esquerda
    if (fb < -1 && fatorBalanceamento(raiz->direita) > 0) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }
    
    return raiz;
}

// Buscar um paciente na árvore AVL por CPF
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

// Função auxiliar para encontrar o nó com o menor valor
static NoAVL* encontrarMinimoAVL(NoAVL *no) {
    while (no && no->esquerda) {
        no = no->esquerda;
    }
    return no;
}

// Remover um paciente da árvore AVL por CPF
NoAVL* removerDaAVL(NoAVL *raiz, const char *cpf) {
    if (!raiz) return NULL;
    
    int cmp = strcmp(cpf, raiz->paciente->cpf);
    
    if (cmp < 0) {
        raiz->esquerda = removerDaAVL(raiz->esquerda, cpf);
    } else if (cmp > 0) {
        raiz->direita = removerDaAVL(raiz->direita, cpf);
    } else {
        // Nó encontrado - casos de remoção
        
        // Caso 1: Nó folha ou com apenas um filho
        if (!raiz->esquerda) {
            NoAVL *temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (!raiz->direita) {
            NoAVL *temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
        
        // Caso 2: Nó com dois filhos
        // Encontra o sucessor in-order (menor nó da subárvore direita)
        NoAVL *temp = encontrarMinimoAVL(raiz->direita);
        
        // Copia o conteúdo do sucessor para este nó
        raiz->paciente = temp->paciente;
        
        // Remove o sucessor
        raiz->direita = removerDaAVL(raiz->direita, temp->paciente->cpf);
    }
    
    if (!raiz) return NULL;
    
    // Passo 2: Atualiza altura do nó
    atualizarAltura(raiz);
    
    // Passo 3: Obtém fator de balanceamento
    int fb = fatorBalanceamento(raiz);
    
    // Passo 4: Se desbalanceado, existem 4 casos
    
    // Caso 1: Esquerda-Esquerda
    if (fb > 1 && fatorBalanceamento(raiz->esquerda) >= 0) {
        return rotacaoDireita(raiz);
    }
    
    // Caso 2: Direita-Direita
    if (fb < -1 && fatorBalanceamento(raiz->direita) <= 0) {
        return rotacaoEsquerda(raiz);
    }
    
    // Caso 3: Esquerda-Direita
    if (fb > 1 && fatorBalanceamento(raiz->esquerda) < 0) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }
    
    // Caso 4: Direita-Esquerda
    if (fb < -1 && fatorBalanceamento(raiz->direita) > 0) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }
    
    return raiz;
}

// Percorrer a árvore AVL em ordem (in-order) e listar pacientes ordenados por CPF
void percorrerInOrderAVL(NoAVL *raiz) {
    if (!raiz) return;
    
    percorrerInOrderAVL(raiz->esquerda);
    
    printf("Nome: %s | CPF: %s | Idade: %d | Prioridade: %d\n",
           raiz->paciente->nome, raiz->paciente->cpf, 
           raiz->paciente->idade, raiz->paciente->prioridade);
    
    percorrerInOrderAVL(raiz->direita);
}

// Liberar toda a árvore AVL
void liberarAVL(NoAVL *raiz) {
    if (!raiz) return;
    
    liberarAVL(raiz->esquerda);
    liberarAVL(raiz->direita);
    free(raiz);
}

// Obter a altura da árvore AVL
int obterAlturaAVL(NoAVL *raiz) {
    if (!raiz) return 0;
    return raiz->altura;
}

// Função auxiliar recursiva para verificar balanceamento
static int verificarBalanceamentoAux(NoAVL *raiz) {
    if (!raiz) return 1;
    
    // Verifica se esquerda está balanceada
    if (!verificarBalanceamentoAux(raiz->esquerda)) {
        return 0;
    }
    
    // Verifica se direita está balanceada
    if (!verificarBalanceamentoAux(raiz->direita)) {
        return 0;
    }
    
    // Verifica o fator de balanceamento do nó atual
    int fb = fatorBalanceamento(raiz);
    if (fb < -1 || fb > 1) {
        return 0;
    }
    
    return 1;
}

// Verificar se a árvore está balanceada
int verificarBalanceamento(NoAVL *raiz) {
    return verificarBalanceamentoAux(raiz);
}
