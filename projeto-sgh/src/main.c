#include <stdio.h>
#include <stdlib.h>

// Declaração direta para evitar dependência de tipos opacos em menu.h
void loopMenu(void* lista, void* fila);

int main(void) {
    loopMenu(NULL, NULL);
    return 0;
}
