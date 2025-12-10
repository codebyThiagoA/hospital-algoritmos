
#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <stdio.h>


typedef struct Paciente Paciente;

#ifdef __cplusplus
extern "C" {
#endif





int salvarPacientesEmArquivo(const Paciente* lista, const char* caminho);


int carregarPacientesDeArquivo(Paciente** listaOut, const char* caminho);

#ifdef __cplusplus
}
#endif

#endif 
