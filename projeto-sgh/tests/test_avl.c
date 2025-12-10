#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_avl.h"
#include "paciente.h"

void test_insercao_avl() {
    printf("=== Teste de Inserção AVL ===\n");
    
    NoAVL *arvore = NULL;
    
    // Criar alguns pacientes
    Paciente *p1 = (Paciente*) malloc(sizeof(Paciente));
    strcpy(p1->cpf, "123.456.789-10");
    strcpy(p1->nome, "João Silva");
    p1->idade = 45;
    p1->prioridade = 1;
    
    Paciente *p2 = (Paciente*) malloc(sizeof(Paciente));
    strcpy(p2->cpf, "987.654.321-00");
    strcpy(p2->nome, "Maria Santos");
    p2->idade = 32;
    p2->prioridade = 2;
    
    Paciente *p3 = (Paciente*) malloc(sizeof(Paciente));
    strcpy(p3->cpf, "555.666.777-88");
    strcpy(p3->nome, "Pedro Oliveira");
    p3->idade = 28;
    p3->prioridade = 1;
    
    // Inserir pacientes
    arvore = inserirNaAVL(arvore, p1);
    arvore = inserirNaAVL(arvore, p2);
    arvore = inserirNaAVL(arvore, p3);
    
    printf("Pacientes inseridos. Altura da árvore: %d\n", obterAlturaAVL(arvore));
    printf("Árvore balanceada: %s\n", verificarBalanceamento(arvore) ? "SIM" : "NÃO");
    
    printf("\nPercurso in-order:\n");
    percorrerInOrderAVL(arvore);
    
    liberarAVL(arvore);
    printf("Teste de inserção concluído!\n\n");
}

void test_busca_avl() {
    printf("=== Teste de Busca AVL ===\n");
    
    NoAVL *arvore = NULL;
    
    Paciente *p1 = (Paciente*) malloc(sizeof(Paciente));
    strcpy(p1->cpf, "111.111.111-11");
    strcpy(p1->nome, "Alice");
    p1->idade = 25;
    p1->prioridade = 1;
    
    Paciente *p2 = (Paciente*) malloc(sizeof(Paciente));
    strcpy(p2->cpf, "222.222.222-22");
    strcpy(p2->nome, "Bob");
    p2->idade = 30;
    p2->prioridade = 2;
    
    arvore = inserirNaAVL(arvore, p1);
    arvore = inserirNaAVL(arvore, p2);
    
    // Teste de busca
    Paciente *resultado = buscarNaAVL(arvore, "111.111.111-11");
    if (resultado) {
        printf("Paciente encontrado: %s (CPF: %s)\n", resultado->nome, resultado->cpf);
    } else {
        printf("Paciente não encontrado\n");
    }
    
    resultado = buscarNaAVL(arvore, "999.999.999-99");
    if (resultado) {
        printf("Paciente encontrado: %s\n", resultado->nome);
    } else {
        printf("CPF 999.999.999-99 não encontrado (esperado)\n");
    }
    
    liberarAVL(arvore);
    printf("Teste de busca concluído!\n\n");
}

void test_remocao_avl() {
    printf("=== Teste de Remoção AVL ===\n");
    
    NoAVL *arvore = NULL;
    
    Paciente *p1 = (Paciente*) malloc(sizeof(Paciente));
    strcpy(p1->cpf, "100.100.100-00");
    strcpy(p1->nome, "Carlos");
    p1->idade = 50;
    p1->prioridade = 1;
    
    Paciente *p2 = (Paciente*) malloc(sizeof(Paciente));
    strcpy(p2->cpf, "200.200.200-00");
    strcpy(p2->nome, "Daniela");
    p2->idade = 35;
    p2->prioridade = 2;
    
    Paciente *p3 = (Paciente*) malloc(sizeof(Paciente));
    strcpy(p3->cpf, "300.300.300-00");
    strcpy(p3->nome, "Ernesto");
    p3->idade = 42;
    p3->prioridade = 1;
    
    arvore = inserirNaAVL(arvore, p1);
    arvore = inserirNaAVL(arvore, p2);
    arvore = inserirNaAVL(arvore, p3);
    
    printf("Antes da remoção:\n");
    percorrerInOrderAVL(arvore);
    printf("Altura: %d, Balanceado: %s\n\n", obterAlturaAVL(arvore), 
           verificarBalanceamento(arvore) ? "SIM" : "NÃO");
    
    // Remover um paciente
    arvore = removerDaAVL(arvore, "200.200.200-00");
    
    printf("Depois de remover 200.200.200-00:\n");
    percorrerInOrderAVL(arvore);
    printf("Altura: %d, Balanceado: %s\n\n", obterAlturaAVL(arvore), 
           verificarBalanceamento(arvore) ? "SIM" : "NÃO");
    
    liberarAVL(arvore);
    printf("Teste de remoção concluído!\n\n");
}

int main() {
    printf("============================\n");
    printf("Testes de Árvore AVL\n");
    printf("============================\n\n");
    
    test_insercao_avl();
    test_busca_avl();
    test_remocao_avl();
    
    printf("============================\n");
    printf("Todos os testes concluídos!\n");
    printf("============================\n");
    
    return 0;
}
