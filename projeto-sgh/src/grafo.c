#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "grafo.h"

Grafo* criarGrafo(int n) {
    if (n <= 0) return NULL;
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    if (!g) return NULL;
    g->n = n;
    g->v = (Vertice*)calloc(n, sizeof(Vertice));
    if (!g->v) { free(g); return NULL; }
    for (int i = 0; i < n; ++i) {
        g->v[i].adj = NULL;
        g->v[i].nome[0] = '\0';
    }
    return g;
}

void definirVertice(Grafo* g, int idx, const char* nome) {
    if (!g || idx < 0 || idx >= g->n) return;
    strncpy(g->v[idx].nome, nome ? nome : "", sizeof(g->v[idx].nome)-1);
    g->v[idx].nome[sizeof(g->v[idx].nome)-1] = '\0';
}

void adicionarAresta(Grafo* g, int origem, int destino, int peso) {
    if (!g || origem < 0 || origem >= g->n || destino < 0 || destino >= g->n) return;
    NoAdj* no = (NoAdj*)malloc(sizeof(NoAdj));
    if (!no) return;
    no->destino = destino;
    no->peso = (peso <= 0) ? 1 : peso;
    no->prox = g->v[origem].adj;
    g->v[origem].adj = no;
}

void liberarGrafo(Grafo* g) {
    if (!g) return;
    for (int i = 0; i < g->n; ++i) {
        NoAdj* p = g->v[i].adj;
        while (p) { NoAdj* nx = p->prox; free(p); p = nx; }
    }
    free(g->v);
    free(g);
}

void mostrarSetores(const Grafo* g) {
    if (!g) return;
    printf("\n=== Mapa do Hospital ===\n");
    for (int i = 0; i < g->n; ++i) {
        printf("%d) %s -> ", i, g->v[i].nome);
        NoAdj* p = g->v[i].adj;
        if (!p) { printf("(sem conexões)\n"); continue; }
        while (p) {
            printf("%s(p=%d)%s", g->v[p->destino].nome, p->peso, p->prox?", ":"\n");
            p = p->prox;
        }
    }
}

// Reconstrói caminho usando predecessor
static int reconstruir(int origem, int destino, const int* prev, int caminho[], int max) {
    int stack[256]; int top = 0;
    for (int v = destino; v != -1; v = prev[v]) {
        if (top >= 256) break;
        stack[top++] = v;
        if (v == origem) break;
    }
    if (top == 0 || stack[top-1] != origem) return 0; // sem caminho
    int len = (top < max) ? top : max;
    for (int i = 0; i < len; ++i) caminho[i] = stack[top-1-i];
    return len;
}

int menorCaminhoBFS(const Grafo* g, int origem, int destino, int caminho[], int max) {
    if (!g || origem<0 || destino<0 || origem>=g->n || destino>=g->n) return 0;
    int* dist = (int*)malloc(sizeof(int)*g->n);
    int* prev = (int*)malloc(sizeof(int)*g->n);
    int* q = (int*)malloc(sizeof(int)*g->n);
    if (!dist||!prev||!q) { free(dist); free(prev); free(q); return 0; }
    for (int i=0;i<g->n;++i){ dist[i]=INT_MAX; prev[i]=-1; }
    int h=0,t=0;
    dist[origem]=0; q[t++]=origem;
    while (h<t) {
        int u = q[h++];
        for (NoAdj* p = g->v[u].adj; p; p=p->prox) {
            int v = p->destino;
            int w = 1; // BFS assume peso 1
            if (dist[v] == INT_MAX) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                q[t++] = v;
            }
        }
    }
    int len = reconstruir(origem, destino, prev, caminho, max);
    free(dist); free(prev); free(q);
    return len;
}

int menorCaminhoDijkstra(const Grafo* g, int origem, int destino, int caminho[], int max) {
    if (!g || origem<0 || destino<0 || origem>=g->n || destino>=g->n) return 0;
    int* dist = (int*)malloc(sizeof(int)*g->n);
    int* prev = (int*)malloc(sizeof(int)*g->n);
    int* vis = (int*)calloc(g->n, sizeof(int));
    if (!dist||!prev||!vis) { free(dist); free(prev); free(vis); return 0; }
    for (int i=0;i<g->n;++i){ dist[i]=INT_MAX; prev[i]=-1; }
    dist[origem]=0;
    for (int it=0; it<g->n; ++it) {
        int u=-1, best=INT_MAX;
        for (int i=0;i<g->n;++i) if (!vis[i] && dist[i] < best) { best=dist[i]; u=i; }
        if (u==-1) break;
        vis[u]=1;
        for (NoAdj* p = g->v[u].adj; p; p=p->prox) {
            int v=p->destino; int w=p->peso;
            if (w<0) w=1;
            if (dist[u]!=INT_MAX && dist[u]+w < dist[v]) {
                dist[v] = dist[u]+w;
                prev[v] = u;
            }
        }
    }
    int len = reconstruir(origem, destino, prev, caminho, max);
    free(dist); free(prev); free(vis);
    return len;
}
