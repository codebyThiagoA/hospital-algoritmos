#ifndef PACIENTE_H
#define PACIENTE_H
#include "historico.h"

typedef struct Atendimento Atendimento; 

typedef struct Paciente {
    char nome[100];
    char cpf[15];
    int idade;
    int prioridade; 
    Historico historico;
    struct Paciente *proximo; 
} Paciente;


Paciente* criarPaciente(const char *nome, const char *cpf, int idade, int prioridade);
void adicionarPaciente(Paciente **lista, Paciente *novo);
Paciente* buscarPacientePorCPF(Paciente *lista, const char *cpf);
void listarPacientes(Paciente *lista);
void liberarListaPacientes(Paciente **lista);

#endif
