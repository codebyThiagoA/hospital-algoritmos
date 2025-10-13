#include <stdio.h>
#include <stdlib.h>
#include "menu.h"

int main() {

    ListaPacientes* lista = criarListaPacientes();
    FilaAtendimento* fila = criarFila();

    carregarPacientesDeArquivo(lista, "data/pacientes.txt");

    loopMenu(lista, fila);

    salvarPacientesEmArquivo(lista, "data/pacientes.txt");

    liberarListaPacientes(lista);
    liberarFila(fila);

    return 0;
}
