#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * Implementação do menu usando os tipos e funções existentes no projeto,
 * sem alterar outros arquivos. Para evitar inconsistências de tipos,
 * usamos estado interno (globais estáticos) e ignoramos os parâmetros da API.
 */
#include "paciente.h"
#include "fila.h"
#include "historico.h"
#include "arquivos.h"

// Estado interno do módulo (não expõe para fora)
static Paciente* g_lista = NULL;
static Fila g_fila;
static int g_demo_inicializado = 0;

// Prototipos locais para evitar declaracoes implicitas
static void acionarCadastro(void);
static void acionarFila(void);
static void acionarAtendimento(void);
static void acionarHistorico(void);
static void acionarPersistencia(void);

static void trim_nl(char* s) {
    if (!s) return;
    size_t i = strlen(s);
    if (i && (s[i-1] == '\n' || s[i-1] == '\r')) s[i-1] = '\0';
}

static void limparEntrada(void) {
    int c; while ((c = getchar()) != '\n' && c != EOF) {}
}

static void carregarDemoSeNecessario(void) {
    if (g_demo_inicializado) return;
    g_demo_inicializado = 1;
    // Iniciar fila
    filaInit(&g_fila);
    // Popular alguns pacientes e históricos
    Paciente* a = criarPaciente("Maria Clara", "123.456.789-00", 30, 1);
    Paciente* b = criarPaciente("Joao Silva", "987.654.321-11", 45, 2);
    if (a) adicionarPaciente(&g_lista, a);
    if (b) adicionarPaciente(&g_lista, b);
    if (a) {
        pushAtendimento(&a->historico, "Triagem inicial", "2025-10-10 09:00");
        pushAtendimento(&a->historico, "Clinico geral", "2025-10-10 10:30");
        enfileirar(&g_fila, a);
    }
    if (b) {
        pushAtendimento(&b->historico, "Coleta exames", "2025-10-11 14:00");
        enfileirar(&g_fila, b);
    }
}

// Assinatura compatível com a chamada em main.c (sem depender de menu.h)
void loopMenu(void* lista, void* fila) {
    (void)lista; (void)fila; // ignorados — usamos estado interno
    int opcao = 0;
    carregarDemoSeNecessario();
    do {
    printf("\n===== SISTEMA HOSPITALAR =====\n");
    printf("1. Cadastrar paciente\n");
    printf("2. Listar pacientes\n");
    printf("3. Adicionar paciente a fila\n");
    printf("4. Atender paciente\n");
    printf("5. Mostrar fila\n");
    printf("6. Historico de paciente\n");
    printf("7. Persistencia (Salvar/Carregar)\n");
    printf("8. Sair\n");
    printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) { limparEntrada(); continue; }
        limparEntrada();

        switch (opcao) {
            case 1: acionarCadastro(); break;
            case 2: listarPacientes(g_lista); break;
            case 3: acionarFila(); break;
            case 4: acionarAtendimento(); break;
            case 5: mostrarFila(&g_fila); break;
            case 6: acionarHistorico(); break;
            case 7: acionarPersistencia(); break;
            case 8: printf("Encerrando...\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 8);

    liberarListaPacientes(&g_lista);
}

static void acionarCadastro(void) {
    char nome[100], cpf[15];
    int idade, prioridade;

    printf("Nome: ");
    if (!fgets(nome, sizeof(nome), stdin)) return;
    trim_nl(nome);
    printf("CPF: ");
    if (!fgets(cpf, sizeof(cpf), stdin)) return;
    trim_nl(cpf);
    printf("Idade: "); if (scanf("%d", &idade) != 1) { limparEntrada(); return; } limparEntrada();
    printf("Prioridade (1=alta, 2=media, 3=baixa): "); if (scanf("%d", &prioridade) != 1) { limparEntrada(); return; } limparEntrada();

    Paciente* p = criarPaciente(nome, cpf, idade, prioridade);
    if (p) { adicionarPaciente(&g_lista, p); printf("Paciente cadastrado!\n"); }
    else { printf("Falha ao cadastrar.\n"); }
}

static void acionarFila(void) {
    char cpf[15];
    printf("CPF do paciente: ");
    if (!fgets(cpf, sizeof(cpf), stdin)) return;
    trim_nl(cpf);
    Paciente* p = buscarPacientePorCPF(g_lista, cpf);
    if (p) { enfileirar(&g_fila, p); printf("%s adicionado à fila.\n", p->nome); }
    else { printf("Paciente não encontrado.\n"); }
}

static void acionarAtendimento(void) {
    Paciente* p = desenfileirar(&g_fila);
    if (!p) { printf("Fila vazia.\n"); return; }
    printf("Atendendo %s...\n", p->nome);
    char descricao[200], data[20];
    printf("Descricao do atendimento: ");
    if (!fgets(descricao, sizeof(descricao), stdin)) return;
    trim_nl(descricao);
    printf("Data (YYYY-MM-DD HH:MM): ");
    if (!fgets(data, sizeof(data), stdin)) return;
    trim_nl(data);
    pushAtendimento(&p->historico, descricao, data);
    printf("Atendimento registrado.\n");
}

static void acionarHistorico(void) {
    char cpf[15];
    printf("CPF do paciente: ");
    if (!fgets(cpf, sizeof(cpf), stdin)) return;
    trim_nl(cpf);
    Paciente* p = buscarPacientePorCPF(g_lista, cpf);
    if (p) { mostrarHistorico(&p->historico); }
    else { printf("Paciente não encontrado.\n"); }
}

static void acionarPersistencia(void) {
    int op = 0; char caminho[260];
    do {
        printf("\n=== Persistencia ===\n");
        printf("1. Salvar pacientes em arquivo\n");
        printf("2. Carregar pacientes do arquivo\n");
        printf("0. Voltar\n> ");
        if (scanf("%d", &op) != 1) { limparEntrada(); continue; }
        limparEntrada();
        if (op == 1) {
        printf("Caminho (enter= data/persistencia_exemplo.txt): ");
        if (!fgets(caminho, sizeof(caminho), stdin)) continue;
        trim_nl(caminho);
            if (!caminho[0]) strcpy(caminho, "data/persistencia_exemplo.txt");
            int rc = salvarPacientesEmArquivo(g_lista, caminho);
            printf(rc==0? "Salvo em '%s'\n" : "Falha ao salvar em '%s'\n", caminho);
        } else if (op == 2) {
            printf("Ao carregar, a lista atual sera descartada. Confirmar? (s/N): ");
            int ch = getchar(); limparEntrada();
            if (ch!='s' && ch!='S') { printf("Cancelado.\n"); continue; }
            printf("Caminho (enter= data/persistencia_exemplo.txt): ");
            if (!fgets(caminho, sizeof(caminho), stdin)) continue;
            trim_nl(caminho);
            if (!caminho[0]) strcpy(caminho, "data/persistencia_exemplo.txt");
            // Liberar lista atual e carregar nova
            liberarListaPacientes(&g_lista);
            Paciente* nova = NULL;
            int rc = carregarPacientesDeArquivo(&nova, caminho);
            if (rc==0) { g_lista = nova; printf("Carregado de '%s'\n", caminho); }
            else { printf("Falha ao carregar de '%s'\n", caminho); }
        }
    } while (op != 0);
}
