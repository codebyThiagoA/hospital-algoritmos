#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <stdlib.h> // _fullpath
#endif
#include "paciente.h"
#include "fila.h"
#include "historico.h"
#include "arquivos.h"
#include "grafo.h"

// Helpers locais de sanitização
static void trim_newline(char* s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n && (s[n-1] == '\n' || s[n-1] == '\r')) { s[--n] = '\0'; }
}

static void trim_spaces(char* s) {
    if (!s) return;
    // left trim
    size_t i = 0;
    while (s[i] && (s[i] == ' ' || s[i] == '\t')) i++;
    if (i) memmove(s, s + i, strlen(s + i) + 1);
    // right trim
    size_t n = strlen(s);
    while (n && (s[n-1] == ' ' || s[n-1] == '\t')) s[--n] = '\0';
}

static void trim_quotes(char* s) {
    if (!s) return;
    size_t n = strlen(s);
    if (n >= 2) {
        char a = s[0], b = s[n-1];
        if ((a == '"' && b == '"') || (a == '\'' && b == '\'')) {
            s[n-1] = '\0';
            memmove(s, s + 1, n - 1);
        }
    }
}

static void sanitize_path(char* s, const char* def) {
    trim_newline(s);
    trim_spaces(s);
    trim_quotes(s);
    if (!*s && def) {
        strcpy(s, def);
    }
}

static void print_abs_path(const char* p) {
#ifdef _WIN32
    char buf[_MAX_PATH];
    if (_fullpath(buf, p, _MAX_PATH)) {
        printf("Caminho absoluto: %s\n", buf);
    }
#endif
}

/**
 * Implementação do menu usando os tipos e funções existentes no projeto,
 * sem alterar outros arquivos. Para evitar inconsistências de tipos,
 * usamos estado interno (globais estáticos) e ignoramos os parâmetros da API.
 */

// Estado interno do módulo (não expõe para fora)
static Paciente* g_lista = NULL;
static Fila g_fila;
static int g_demo_inicializado = 0;
static Grafo* g_grafo = NULL;

// Prototipos locais para evitar declaracoes implicitas
static void acionarCadastro(void);
static void acionarFila(void);
static void acionarAtendimento(void);
static void acionarHistorico(void);
// Persistência será oferecida como duas opções diretas no menu principal

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
    // Construir grafo de setores
    g_grafo = criarGrafo(6);
    if (g_grafo) {
        definirVertice(g_grafo, 0, "Recepcao");
        definirVertice(g_grafo, 1, "Triagem");
        definirVertice(g_grafo, 2, "Raio-X");
        definirVertice(g_grafo, 3, "Laboratorio");
        definirVertice(g_grafo, 4, "Consultorio");
        definirVertice(g_grafo, 5, "Farmacia");
        // conexões (peso 1 ou mais):
        adicionarAresta(g_grafo, 0, 1, 1); // Recepção -> Triagem
        adicionarAresta(g_grafo, 1, 4, 1); // Triagem -> Consultorio
        adicionarAresta(g_grafo, 1, 2, 2); // Triagem -> Raio-X
        adicionarAresta(g_grafo, 2, 4, 2); // Raio-X -> Consultorio
        adicionarAresta(g_grafo, 4, 3, 1); // Consultorio -> Laboratorio
        adicionarAresta(g_grafo, 3, 5, 1); // Laboratorio -> Farmacia
        adicionarAresta(g_grafo, 4, 5, 1); // Consultorio -> Farmacia
        // opcionalmente bidirecionais
        adicionarAresta(g_grafo, 1, 0, 1);
        adicionarAresta(g_grafo, 4, 1, 1);
        adicionarAresta(g_grafo, 2, 1, 2);
        adicionarAresta(g_grafo, 4, 2, 2);
        adicionarAresta(g_grafo, 3, 4, 1);
        adicionarAresta(g_grafo, 5, 3, 1);
        adicionarAresta(g_grafo, 5, 4, 1);
    }
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
    printf("7. Salvar em arquivo\n");
    printf("8. Carregar de arquivo\n");
    printf("9. Mostrar mapa do hospital\n");
    printf("10. Menor caminho entre setores\n");
    printf("11. Sair\n");
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
            case 7: {
                // Salvar diretamente
                char caminho[512] = {0};
                printf("Caminho (enter= data/persistencia_exemplo.txt): ");
                if (fgets(caminho, sizeof(caminho), stdin)) {
                    sanitize_path(caminho, "data/persistencia_exemplo.txt");
                    print_abs_path(caminho);
                    int rc = salvarPacientesEmArquivo(g_lista, caminho);
                    if (rc == 0) {
                        printf("Salvo em '%s'\n", caminho);
                    } else {
                        printf("Falha ao salvar em '%s'. Verifique se a pasta existe/permissão.\n", caminho);
                    }
                }
                break;
            }
            case 8: {
                // Carregar diretamente
                char caminho[512] = {0};
                char conf[8];
                printf("Isso vai substituir a lista atual. Confirmar? (s/N): ");
                if (!fgets(conf, sizeof(conf), stdin)) break;
                trim_newline(conf);
                if (conf[0] != 's' && conf[0] != 'S') { printf("Cancelado.\n"); break; }
                printf("Caminho (enter= data/persistencia_exemplo.txt): ");
                if (fgets(caminho, sizeof(caminho), stdin)) {
                    sanitize_path(caminho, "data/persistencia_exemplo.txt");
                    print_abs_path(caminho);
                    Paciente* nova = NULL;
                    int rc = carregarPacientesDeArquivo(&nova, caminho);
                    if (rc == 0) {
                        while (g_lista) {
                            Paciente* n = g_lista->proximo;
                            limparHistorico(&g_lista->historico);
                            free(g_lista);
                            g_lista = n;
                        }
                        g_lista = nova;
                        printf("Carregado de '%s'\n", caminho);
                    } else {
                        printf("Falha ao carregar de '%s'. Arquivo não encontrado ou formato inválido.\n", caminho);
                    }
                }
                break;
            }
            case 9: {
                if (g_grafo) mostrarSetores(g_grafo);
                else printf("Grafo nao inicializado.\n");
                break;
            }
            case 10: {
                if (!g_grafo) { printf("Grafo nao inicializado.\n"); break; }
                printf("Origem (indice): ");
                int o; if (scanf("%d", &o) != 1) { limparEntrada(); break; }
                limparEntrada();
                printf("Destino (indice): ");
                int d; if (scanf("%d", &d) != 1) { limparEntrada(); break; }
                limparEntrada();
                int caminho[64];
                int len = menorCaminhoBFS(g_grafo, o, d, caminho, 64);
                if (len == 0) {
                    printf("Sem caminho via BFS. Tentando Dijkstra...\n");
                    len = menorCaminhoDijkstra(g_grafo, o, d, caminho, 64);
                }
                if (len > 0) {
                    printf("Rota (%d passos): ", len-1);
                    for (int i = 0; i < len; ++i) {
                        printf("%s%s", g_grafo->v[caminho[i]].nome, (i+1<len)?" -> ":"\n");
                    }
                } else {
                    printf("Nao foi possivel encontrar caminho.\n");
                }
                break;
            }
            case 11: printf("Encerrando...\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 11);

    liberarListaPacientes(&g_lista);
    liberarGrafo(g_grafo);
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

// acionarPersistencia removido - opções diretas no menu
