#ifndef FILA_H
#define FILA_H


#include "paciente.h"  // Para usar struct Paciente


typedef struct NoFila {
    Paciente *paciente;       // Aponta para um paciente já cadastrado
    struct NoFila *prox;
} NoFila;


typedef struct {
    NoFila *inicio;
    NoFila *fim;
} Fila;


// Funções
void filaInit(Fila *f);
int filaVazia(Fila *f);
void enfileirar(Fila *f, Paciente *p);
Paciente* desenfileirar(Fila *f);
void mostrarFila(Fila *f);


#endif


