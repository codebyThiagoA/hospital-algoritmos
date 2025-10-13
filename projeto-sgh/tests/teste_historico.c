#include "../include/historico.h"

int main() {
    Historico h;
    inicializarHistorico(&h);

    pushAtendimento(&h, "Consulta com clínico geral", "10/10/2025");
    pushAtendimento(&h, "Retorno de exames", "11/10/2025");

    mostrarHistorico(&h);
    limparHistorico(&h);

    return 0;
}
