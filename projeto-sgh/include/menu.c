#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"

void loopMenu(ListaPacientes* lista, FilaAtendimento* fila) {
    int opcao;

    do {
        printf("\n===== SISTEMA HOSPITALAR =====\n");
        printf("1. Cadastrar paciente\n");
        printf("2. Listar pacientes\n");
        printf("3. Adicionar paciente à fila\n");
        printf("4. Atender paciente\n");
        printf("5. Mostrar fila\n");
        printf("6. Histórico de paciente\n");
        printf("7. Salvar dados\n");
        printf("8. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                acionarCadastro(lista);
                break;
            case 2:
                listarPacientes(lista);
                break;
            case 3:
                acionarFila(lista, fila);
                break;
            case 4:
                acionarAtendimento(lista, fila);
                break;
            case 5:
                mostrarFila(fila);
                break;
            case 6:
                acionarHistorico(lista);
                break;
            case 7:
                acionarPersistencia(lista);
                break;
            case 8:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 8);
}

void acionarCadastro(ListaPacientes* lista) {
    char nome[100], cpf[15];
    int idade, prioridade;

    printf("Nome: ");
    fgets(nome, 100, stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("CPF: ");
    fgets(cpf, 15, stdin);
    cpf[strcspn(cpf, "\n")] = 0;

    printf("Idade: ");
    scanf("%d", &idade);
    printf("Prioridade (0=normal, 1=preferencial): ");
    scanf("%d", &prioridade);
    getchar();

    Paciente* p = criarPaciente(nome, cpf, idade, prioridade);
    adicionarPaciente(lista, p);

    printf("✅ Paciente cadastrado!\n");
}

void acionarFila(ListaPacientes* lista, FilaAtendimento* fila) {
    char cpf[15];
    printf("CPF do paciente: ");
    fgets(cpf, 15, stdin);
    cpf[strcspn(cpf, "\n")] = 0;

    Paciente* p = buscarPacientePorCPF(lista, cpf);
    if (p) {
        enfileirar(fila, p);
        printf("✅ %s foi adicionado à fila.\n", p->nome);
    } else {
        printf("❌ Paciente não encontrado.\n");
    }
}

void acionarAtendimento(ListaPacientes* lista, FilaAtendimento* fila) {
    Paciente* p = desenfileirar(fila);
    if (p) {
        printf("Atendendo %s...\n", p->nome);

        char descricao[200];
        printf("Descrição do atendimento: ");
        fgets(descricao, 200, stdin);
        descricao[strcspn(descricao, "\n")] = 0;

        Atendimento a;
        strcpy(a.descricao, descricao);
        a.data = time(NULL);

        pushAtendimento(p, a);

        printf("✅ Atendimento registrado.\n");
    } else {
        printf("Fila vazia.\n");
    }
}

void acionarHistorico(ListaPacientes* lista) {
    char cpf[15];
    printf("CPF do paciente: ");
    fgets(cpf, 15, stdin);
    cpf[strcspn(cpf, "\n")] = 0;

    Paciente* p = buscarPacientePorCPF(lista, cpf);
    if (p) {
        mostrarHistorico(p);
    } else {
        printf("❌ Paciente não encontrado.\n");
    }
}

void acionarPersistencia(ListaPacientes* lista) {
    salvarPacientesEmArquivo(lista, "data/pacientes.txt");
    printf("💾 Dados salvos em 'data/pacientes.txt'.\n");
}
