#ifndef GRAFO_H
#define GRAFO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NoAdj {
    int destino;
    int peso; 
    struct NoAdj* prox;
} NoAdj;

typedef struct {
    char nome[64];
    NoAdj* adj;
} Vertice;

typedef struct {
    int n;           
    Vertice* v;      
} Grafo;


Grafo* criarGrafo(int n);
void definirVertice(Grafo* g, int idx, const char* nome);
void adicionarAresta(Grafo* g, int origem, int destino, int peso);
void liberarGrafo(Grafo* g);


void mostrarSetores(const Grafo* g);


int menorCaminhoBFS(const Grafo* g, int origem, int destino, int caminho[], int max);


int menorCaminhoDijkstra(const Grafo* g, int origem, int destino, int caminho[], int max);

#ifdef __cplusplus
}
#endif

#endif 