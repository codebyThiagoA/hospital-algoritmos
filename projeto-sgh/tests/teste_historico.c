#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/paciente.h"
#include "../include/historico.h"

int main() {
    Paciente *p1 = criarPaciente("Julia", "111.111.111-11", 30, 2);
    Paciente *p2 = criarPaciente("Sandro", "222.222.222-22", 45, 1);
    Paciente *p3 = criarPaciente("Carlos", "333.333.333-33", 60, 3);

    inicializarHistorico(&p1->historico);
    inicializarHistorico(&p2->historico);
    inicializarHistorico(&p3->historico);

    printf("\n--- Teste 1: Paciente sem historico ---\n");
    mostrarHistorico(&p1->historico);

    printf("\n--- Teste 2: Paciente com 1 atendimento ---\n");
    pushAtendimento(&p2->historico, "Consulta de rotina", "12/10/2025");
    mostrarHistorico(&p2->historico);

    printf("\n--- Teste 3: Paciente com multiplos atendimentos ---\n");
    pushAtendimento(&p3->historico, "Exame de sangue", "10/10/2025");
    pushAtendimento(&p3->historico, "Retorno de exames", "11/10/2025");
    pushAtendimento(&p3->historico, "Consulta com cardiologista", "13/10/2025");
    mostrarHistorico(&p3->historico);

    printf("\n--- Teste 4: Limpar historico ---\n");
    limparHistorico(&p3->historico);
    mostrarHistorico(&p3->historico); 

    limparHistorico(&p1->historico);
    limparHistorico(&p2->historico);
    liberarListaPacientes(&p1);
    liberarListaPacientes(&p2);
    liberarListaPacientes(&p3);

    return 0;
}
