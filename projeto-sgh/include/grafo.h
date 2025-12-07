#ifndef GRAFO_H
#define GRAFO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NoAdj {
    int destino;
    int peso; // 1 para não ponderado
    struct NoAdj* prox;
} NoAdj;

typedef struct {
    char nome[64];
    NoAdj* adj;
} Vertice;

typedef struct {
    int n;           // número de vértices
    Vertice* v;      // vetor de vértices
} Grafo;

// Construção e destruição
Grafo* criarGrafo(int n);
void definirVertice(Grafo* g, int idx, const char* nome);
void adicionarAresta(Grafo* g, int origem, int destino, int peso);
void liberarGrafo(Grafo* g);

// Visualização
void mostrarSetores(const Grafo* g);

// Menor caminho
// BFS para menor caminho sem pesos (ou com peso=1)
int menorCaminhoBFS(const Grafo* g, int origem, int destino, int caminho[], int max);

// Dijkstra para pesos positivos
int menorCaminhoDijkstra(const Grafo* g, int origem, int destino, int caminho[], int max);

#ifdef __cplusplus
}
#endif

#endif // GRAFO_H