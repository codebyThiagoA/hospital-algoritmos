#ifndef FILA_H
#define FILA_H


#include "paciente.h"  


typedef struct NoFila {
    Paciente *paciente;      
    struct NoFila *prox;
} NoFila;


typedef struct {
    NoFila *inicio;
    NoFila *fim;
} Fila;



void filaInit(Fila *f);
int filaVazia(Fila *f);
void enfileirar(Fila *f, Paciente *p);
Paciente* desenfileirar(Fila *f);
void mostrarFila(Fila *f);


#endif


