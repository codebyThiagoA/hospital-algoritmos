//#define MENU_H
#ifndef MENU_H

#include "pacientes.h"
#include "fila.h"
#include "historico.h"
#include "arquivos.h"
#include "paciente.h"

void loopMenu(ListaPacientes* lista, FilaAtendimento* fila);

void acionarCadastro(ListaPacientes* lista);
void acionarFila(ListaPacientes* lista, FilaAtendimento* fila);
void acionarAtendimento(ListaPacientes* lista, FilaAtendimento* fila);
void acionarHistorico(ListaPacientes* lista);
void acionarPersistencia(ListaPacientes* lista);

#endif
