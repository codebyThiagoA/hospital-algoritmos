#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/heap.h"
#include "../include/paciente.h"


static void trocar(Paciente **a, Paciente **b) {
    Paciente *temp = *a;
    *a = *b;
    *b = temp;
}


static int indicePai(int i) {
    return (i - 1) / 2;
}


static int indiceFilhoEsquerdo(int i) {
    return 2 * i + 1;
}


static int indiceFilhoDireito(int i) {
    return 2 * i + 2;
}


static void subir(Heap *h, int indice) {
    while (indice > 0) {
        int pai = indicePai(indice);
        
        
        if (h->pacientes[indice]->prioridade > h->pacientes[pai]->prioridade) {
            trocar(&h->pacientes[indice], &h->pacientes[pai]);
            indice = pai;
        } else {
            break;
        }
    }
}


static void descer(Heap *h, int indice) {
    int tamanho = h->tamanho;
    
    while (1) {
        int maior = indice;
        int esquerda = indiceFilhoEsquerdo(indice);
        int direita = indiceFilhoDireito(indice);
        
        
        if (esquerda < tamanho && 
            h->pacientes[esquerda]->prioridade > h->pacientes[maior]->prioridade) {
            maior = esquerda;
        }
        
        
        if (direita < tamanho && 
            h->pacientes[direita]->prioridade > h->pacientes[maior]->prioridade) {
            maior = direita;
        }
        
        
        if (maior == indice) {
            break;
        }
        
        
        trocar(&h->pacientes[indice], &h->pacientes[maior]);
        indice = maior;
    }
}


static int buscarIndicePorCPF(Heap *h, const char *cpf) {
    for (int i = 0; i < h->tamanho; i++) {
        if (strcmp(h->pacientes[i]->cpf, cpf) == 0) {
            return i;
        }
    }
    return -1; 
}


void inicializarHeap(Heap *h) {
    h->tamanho = 0;
    for (int i = 0; i < MAX_HEAP; i++) {
        h->pacientes[i] = NULL;
    }
}


int heapVazio(Heap *h) {
    return (h->tamanho == 0);
}


int heapCheio(Heap *h) {
    return (h->tamanho >= MAX_HEAP);
}


int inserirHeap(Heap *h, Paciente *p) {
    if (heapCheio(h)) {
        printf("\n[ERRO] Heap de prioridade cheio! Não é possível inserir mais pacientes.\n");
        return 0;
    }
    
    if (p == NULL) {
        printf("\n[ERRO] Paciente inválido!\n");
        return 0;
    }
    
   
    if (p->prioridade < 0 || p->prioridade > 10) {
        printf("\n[ERRO] Prioridade inválida! Deve estar entre 0 e 10.\n");
        return 0;
    }
    
    
    h->pacientes[h->tamanho] = p;
    
    
    subir(h, h->tamanho);
    
    h->tamanho++;
    
    return 1;
}


Paciente* removerComMaiorPrioridade(Heap *h) {
    if (heapVazio(h)) {
        printf("\n[AVISO] Não há pacientes aguardando atendimento prioritário.\n");
        return NULL;
    }
    
    
    Paciente *pacientePrioritario = h->pacientes[0];
    
   
    h->tamanho--;
    h->pacientes[0] = h->pacientes[h->tamanho];
    h->pacientes[h->tamanho] = NULL;
    
    
    if (h->tamanho > 0) {
        descer(h, 0);
    }
    
    printf("\n[ATENDIMENTO] Paciente %s (CPF: %s) será atendido.\n", 
           pacientePrioritario->nome, pacientePrioritario->cpf);
    printf("              Prioridade: %d | Idade: %d anos\n", 
           pacientePrioritario->prioridade, pacientePrioritario->idade);
    
    return pacientePrioritario;
}


int modificarPrioridade(Heap *h, const char *cpf, int novaPrioridade) {
    if (heapVazio(h)) {
        printf("\n[AVISO] Heap vazio! Não há pacientes para modificar.\n");
        return 0;
    }
    
    
    if (novaPrioridade < 0 || novaPrioridade > 10) {
        printf("\n[ERRO] Prioridade inválida! Deve estar entre 0 e 10.\n");
        return 0;
    }
    
   
    int indice = buscarIndicePorCPF(h, cpf);
    
    if (indice == -1) {
        printf("\n[ERRO] Paciente com CPF %s não encontrado no heap.\n", cpf);
        return 0;
    }
    
    int prioridadeAnterior = h->pacientes[indice]->prioridade;
    h->pacientes[indice]->prioridade = novaPrioridade;
    
    printf("\n[MODIFICAÇÃO] Prioridade do paciente %s alterada de %d para %d.\n",
           h->pacientes[indice]->nome, prioridadeAnterior, novaPrioridade);
    
   
    if (novaPrioridade > prioridadeAnterior) {
        
        subir(h, indice);
    } else if (novaPrioridade < prioridadeAnterior) {
        
        descer(h, indice);
    }
    
    return 1;
}


void exibirHeap(Heap *h) {
    if (heapVazio(h)) {
        printf("\n[AVISO] Nao ha pacientes no heap de prioridade.\n");
        return;
    }
    
    printf("\n================================================================\n");
    printf("          HEAP DE PRIORIDADE - PACIENTES AGUARDANDO            \n");
    printf("================================================================\n");
    printf(" Total de pacientes aguardando: %d\n", h->tamanho);
    printf("================================================================\n");
    
    for (int i = 0; i < h->tamanho; i++) {
        Paciente *p = h->pacientes[i];
        printf(" [%03d] %-30s | Prioridade: %2d\n", 
               i + 1, p->nome, p->prioridade);
        printf("       CPF: %-14s | Idade: %3d anos\n", 
               p->cpf, p->idade);
        
        if (i < h->tamanho - 1) {
            printf("----------------------------------------------------------------\n");
        }
    }
    
    printf("================================================================\n");
    
    
    if (!heapVazio(h)) {
        printf("\n[PROXIMO ATENDIMENTO] %s (Prioridade: %d)\n", 
               h->pacientes[0]->nome, h->pacientes[0]->prioridade);
    }
}

