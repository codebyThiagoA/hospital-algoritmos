#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <stdlib.h>
#include <direct.h>
#define MAX_PATH_LEN 260
#endif
#include "paciente.h"
#include "fila.h"
#include "historico.h"
#include "arquivos.h"
#include "grafo.h"
#include "heap.h"
#include "arvore.h"
#include "arvore_avl.h"


static void trim_newline(char* s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n && (s[n-1] == '\n' || s[n-1] == '\r')) { s[--n] = '\0'; }
}

static void trim_spaces(char* s) {
    if (!s) return;
    
    size_t i = 0;
    while (s[i] && (s[i] == ' ' || s[i] == '\t')) i++;
    if (i) memmove(s, s + i, strlen(s + i) + 1);
    
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
    char buf[MAX_PATH_LEN];
    if (_fullpath(buf, p, MAX_PATH_LEN)) {
        printf("Caminho absoluto: %s\n", buf);
    }
#endif
}


static Paciente* g_lista = NULL;
static Fila g_fila;
static Heap g_heap;
static int g_demo_inicializado = 0;
static Grafo* g_grafo = NULL;
static NoArvore* g_arvore = NULL;
static NoAVL* g_arvore_avl = NULL;


static void acionarCadastro(void);
static void acionarFila(void);
static void acionarAtendimento(void);
static void acionarHistorico(void);
static void acionarAtendimentoPrioridade(void);
static void acionarModificarPrioridade(void);


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
    // Iniciar heap
    inicializarHeap(&g_heap);
    // Construir grafo de setores
    g_grafo = criarGrafo(6);
    if (g_grafo) {
        definirVertice(g_grafo, 0, "Recepcao");
        definirVertice(g_grafo, 1, "Triagem");
        definirVertice(g_grafo, 2, "Raio-X");
        definirVertice(g_grafo, 3, "Laboratorio");
        definirVertice(g_grafo, 4, "Consultorio");
        definirVertice(g_grafo, 5, "Farmacia");
        
        adicionarAresta(g_grafo, 0, 1, 1); // Recepção -> Triagem
        adicionarAresta(g_grafo, 1, 4, 1); // Triagem -> Consultorio
        adicionarAresta(g_grafo, 1, 2, 2); // Triagem -> Raio-X
        adicionarAresta(g_grafo, 2, 4, 2); // Raio-X -> Consultorio
        adicionarAresta(g_grafo, 4, 3, 1); // Consultorio -> Laboratorio
        adicionarAresta(g_grafo, 3, 5, 1); // Laboratorio -> Farmacia
        adicionarAresta(g_grafo, 4, 5, 1); // Consultorio -> Farmacia
        
        adicionarAresta(g_grafo, 1, 0, 1);
        adicionarAresta(g_grafo, 4, 1, 1);
        adicionarAresta(g_grafo, 2, 1, 2);
        adicionarAresta(g_grafo, 4, 2, 2);
        adicionarAresta(g_grafo, 3, 4, 1);
        adicionarAresta(g_grafo, 5, 3, 1);
        adicionarAresta(g_grafo, 5, 4, 1);
    }
    
    Paciente* a = criarPaciente("Maria Clara", "123.456.789-00", 30, 8);
    Paciente* b = criarPaciente("Joao Silva", "987.654.321-11", 45, 5);
    if (a) adicionarPaciente(&g_lista, a);
    if (b) adicionarPaciente(&g_lista, b);
    if (a) {
        pushAtendimento(&a->historico, "Triagem inicial", "2025-10-10 09:00");
        pushAtendimento(&a->historico, "Clinico geral", "2025-10-10 10:30");
        enfileirar(&g_fila, a);
        inserirHeap(&g_heap, a);
    }
    if (b) {
        pushAtendimento(&b->historico, "Coleta exames", "2025-10-11 14:00");
        enfileirar(&g_fila, b);
        inserirHeap(&g_heap, b);
    }
}


void loopMenu(void* lista, void* fila) {
    (void)lista; (void)fila; 
    int opcao = 0;
    carregarDemoSeNecessario();
    do {
    printf("\n===== SISTEMA HOSPITALAR =====\n");
    printf("1. Cadastrar paciente\n");
    printf("2. Listar pacientes\n");
    printf("3. Adicionar paciente a fila\n");
    printf("4. Atender paciente (FIFO)\n");
    printf("5. Mostrar fila\n");
    printf("6. Historico de paciente\n");
    printf("7. Salvar em arquivo\n");
    printf("8. Carregar de arquivo\n");
    printf("9. Mostrar mapa do hospital\n");
    printf("10. Menor caminho entre setores\n");
    printf("11. Atender por prioridade (HEAP)\n");
    printf("12. Ver fila de prioridade\n");
    printf("13. Modificar prioridade\n");
    printf("14. Listar pacientes ordenados (Arvore BST)\n");
    printf("15. Listar pacientes ordenados (Arvore AVL)\n");
    printf("16. Comparar altura (BST vs AVL)\n");
    printf("17. Sair\n");
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
                        
                        // Reconstruir heap e árvores
                        inicializarHeap(&g_heap);
                        liberarArvore(g_arvore);
                        liberarAVL(g_arvore_avl);
                        g_arvore = NULL;
                        g_arvore_avl = NULL;
                        
                        Paciente* atual = g_lista;
                        while (atual) {
                            inserirHeap(&g_heap, atual);
                            g_arvore = inserirNaArvore(g_arvore, atual);
                            g_arvore_avl = inserirNaAVL(g_arvore_avl, atual);
                            atual = atual->proximo;
                        }
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
            case 11: acionarAtendimentoPrioridade(); break;
            case 12: exibirHeap(&g_heap); break;
            case 13: acionarModificarPrioridade(); break;
            case 14: 
                printf("\n--- Pacientes Ordenados por CPF (BST) ---\n");
                if (!g_arvore) {
                    printf("Nenhum paciente cadastrado na arvore BST.\n");
                } else {
                    percorrerInOrder(g_arvore);
                }
                break;
            case 15:
                printf("\n--- Pacientes Ordenados por CPF (AVL) ---\n");
                if (!g_arvore_avl) {
                    printf("Nenhum paciente cadastrado na arvore AVL.\n");
                } else {
                    percorrerInOrderAVL(g_arvore_avl);
                }
                break;
            case 16: {
                printf("\n--- Comparacao de Altura ---\n");
                int alt_bst = 0, alt_avl = 0;
                if (g_arvore) {
                    // Calcular altura BST (função auxiliar necessária)
                    alt_bst = obterAlturaArvore(g_arvore);
                }
                if (g_arvore_avl) {
                    alt_avl = obterAlturaAVL(g_arvore_avl);
                }
                printf("Altura BST: %d\n", alt_bst);
                printf("Altura AVL: %d\n", alt_avl);
                printf("Balanceamento AVL: %s\n", verificarBalanceamento(g_arvore_avl) ? "SIM" : "NAO");
                break;
            }
            case 17: printf("Encerrando...\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 17);

    liberarListaPacientes(&g_lista);
    liberarGrafo(g_grafo);
    liberarArvore(g_arvore);
    liberarAVL(g_arvore_avl);
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
    printf("Idade: "); 
    if (scanf("%d", &idade) != 1) { limparEntrada(); return; } 
    limparEntrada();
    
    
    do {
        printf("Prioridade (0-10, onde 10 = mais urgente): "); 
        if (scanf("%d", &prioridade) != 1) { 
            limparEntrada(); 
            printf("[ERRO] Valor inválido! Digite um número.\n");
            continue;
        }
        limparEntrada();
        
        if (prioridade < 0 || prioridade > 10) {
            printf("[ERRO] Prioridade deve estar entre 0 e 10!\n");
        }
    } while (prioridade < 0 || prioridade > 10);

    Paciente* p = criarPaciente(nome, cpf, idade, prioridade);
    if (p) { 
        adicionarPaciente(&g_lista, p); 
        inserirHeap(&g_heap, p);
        g_arvore = inserirNaArvore(g_arvore, p);
        g_arvore_avl = inserirNaAVL(g_arvore_avl, p);
        printf("Paciente cadastrado em ambas as arvores!\n"); 
    }
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

static void acionarAtendimentoPrioridade(void) {
    Paciente* p = removerComMaiorPrioridade(&g_heap);
    if (!p) return;
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

static void acionarModificarPrioridade(void) {
    char cpf[15];
    int novaPrioridade;
    
    printf("CPF do paciente: ");
    if (!fgets(cpf, sizeof(cpf), stdin)) return;
    trim_nl(cpf);
    
    
    do {
        printf("Nova prioridade (0-10): ");
        if (scanf("%d", &novaPrioridade) != 1) { 
            limparEntrada(); 
            printf("[ERRO] Valor inválido! Digite um número.\n");
            continue;
        }
        limparEntrada();
        
        if (novaPrioridade < 0 || novaPrioridade > 10) {
            printf("[ERRO] Prioridade deve estar entre 0 e 10!\n");
        }
    } while (novaPrioridade < 0 || novaPrioridade > 10);
    
    modificarPrioridade(&g_heap, cpf, novaPrioridade);
}