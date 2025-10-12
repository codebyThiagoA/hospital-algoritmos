// arquivos.h — Persistência em arquivo para pacientes e históricos
#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <stdio.h>

// Forward declaration para evitar dependência de headers de outros módulos
typedef struct Paciente Paciente;

#ifdef __cplusplus
extern "C" {
#endif

// Formato textual simples com escaping (\n -> \n, \t -> \t, \\ -> \\)
// Estrutura do arquivo:
//   PACIENTES <N>\n
//   Para cada paciente i em [1..N]:
//     P|<nome>|<cpf>|<idade>|<prioridade>|<Q>\n
//     Para j em [1..Q]:
//       A|<data>|<descricao>\n
// Linhas iniciadas por '#' são comentários e devem ser ignoradas no carregamento.

// Salva a lista encadeada de pacientes (e seus históricos) em 'caminho'.
// Retorna 0 em sucesso; caso contrário, um código negativo (por exemplo, -1 falha de I/O).
int salvarPacientesEmArquivo(const Paciente* lista, const char* caminho);

// Carrega pacientes de 'caminho' e devolve a nova lista por meio de '*listaOut'.
// Se '*listaOut' não for NULL, a função não libera previamente; a chamada deve garantir estado limpo.
// Retorna 0 em sucesso; caso contrário, um código negativo.
int carregarPacientesDeArquivo(Paciente** listaOut, const char* caminho);

#ifdef __cplusplus
}
#endif

#endif // ARQUIVOS_H
