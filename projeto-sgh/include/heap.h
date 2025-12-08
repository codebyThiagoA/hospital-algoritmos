#ifndef HEAP_H
#define HEAP_H

#include "paciente.h"

#define MAX_HEAP 200


typedef struct {
    Paciente *pacientes[MAX_HEAP];  
    int tamanho;                    
} Heap;


void inicializarHeap(Heap *h);
int inserirHeap(Heap *h, Paciente *p);
Paciente* removerComMaiorPrioridade(Heap *h);
int modificarPrioridade(Heap *h, const char *cpf, int novaPrioridade);
void exibirHeap(Heap *h);


int heapVazio(Heap *h);


int heapCheio(Heap *h);

#endif