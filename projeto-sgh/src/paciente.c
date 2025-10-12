#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "paciente.h"

Paciente* criarPaciente(const char *nome, const char *cpf, int idade, int prioridade) {
    Paciente *novo = (Paciente*) malloc(sizeof(Paciente));
    if (!novo) return NULL;
    strcpy(novo->nome, nome);
    strcpy(novo->cpf, cpf);
    novo->idade = idade;
    novo->prioridade = prioridade;
    novo->historico = NULL;
    novo->proximo = NULL;
    return novo;
}

void adicionarPaciente(Paciente **lista, Paciente *novo) {
    if (!*lista) {
        *lista = novo;
    } else {
        Paciente *atual = *lista;
        while (atual->proximo) atual = atual->proximo;
        atual->proximo = novo;
    }
}

Paciente* buscarPacientePorCPF(Paciente *lista, const char *cpf) {
    while (lista) {
        if (strcmp(lista->cpf, cpf) == 0) return lista;
        lista = lista->proximo;
    }
    return NULL;
}

void listarPacientes(Paciente *lista) {
    printf("\n--- Lista de Pacientes ---\n");
    while (lista) {
        printf("Nome: %s | CPF: %s | Idade: %d | Prioridade: %d\n",
               lista->nome, lista->cpf, lista->idade, lista->prioridade);
        lista = lista->proximo;
    }
}

void liberarListaPacientes(Paciente **lista) {
    Paciente *atual = *lista;
    while (atual) {
        Paciente *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    *lista = NULL;
}
