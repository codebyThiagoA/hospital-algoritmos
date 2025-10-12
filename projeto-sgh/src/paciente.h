#ifndef PACIENTE_H
#define PACIENTE_H

typedef struct Atendimento Atendimento; // declaração antecipada

typedef struct Paciente {
    char nome[100];
    char cpf[15];
    int idade;
    int prioridade; // 1 = alta, 2 = média, 3 = baixa
    Atendimento *historico; // ponteiro para pilha de atendimentos
    struct Paciente *proximo; // lista encadeada de pacientes
} Paciente;

// Funções principais
Paciente* criarPaciente(const char *nome, const char *cpf, int idade, int prioridade);
void adicionarPaciente(Paciente **lista, Paciente *novo);
Paciente* buscarPacientePorCPF(Paciente *lista, const char *cpf);
void listarPacientes(Paciente *lista);
void liberarListaPacientes(Paciente **lista);

#endif
