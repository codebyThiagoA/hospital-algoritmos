// Implementação da persistência de pacientes e históricos
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif
#include "arquivos.h"
#include "paciente.h"
#include "historico.h"

// --- Utilidades de string ---
static void str_escape(const char* src, FILE* out) {
	// Escreve em 'out' a string escapando caracteres especiais
	for (const unsigned char* p = (const unsigned char*)src; *p; ++p) {
		switch (*p) {
			case '\\': fputs("\\\\", out); break;
			case '\n': fputs("\\n", out); break;
			case '\t': fputs("\\t", out); break;
			case '|':   fputs("\\|", out); break;
			case '\r': fputs("\\r", out); break;
			default: fputc(*p, out); break;
		}
	}
}

static char* str_unescape_inplace(char* s) {
	// Converte as sequências de escape de volta, no próprio buffer
	char* r = s;
	for (char* p = s; *p; ++p) {
		if (*p == '\\') {
			++p;
			if (!*p) break;
			switch (*p) {
				case 'n': *r++ = '\n'; break;
				case 't': *r++ = '\t'; break;
				case 'r': *r++ = '\r'; break;
				case '|': *r++ = '|'; break;
				case '\\': *r++ = '\\'; break;
				default: *r++ = *p; break; // mantém caractere desconhecido
			}
		} else {
			*r++ = *p;
		}
	}
	*r = '\0';
	return s;
}

// Divide a linha por '|' em até 'max' campos, respeitando escapes. Modifica a linha.
static int split_pipe_escaped(char* line, char* fields[], int max) {
	int count = 0;
	char* p = line;
	fields[count++] = p;
	for (; *p && count < max; ++p) {
		if (*p == '\\') { // pular próximo
			if (*(p+1)) ++p;
			continue;
		}
		if (*p == '|') {
			*p = '\0';
			fields[count++] = p + 1;
		}
	}
	// Desescapar todos os campos
	for (int i = 0; i < count; ++i) {
		str_unescape_inplace(fields[i]);
	}
	return count;
}

// Limpeza completa em caso de erro (historico + pacientes)
static void libera_lista_total(Paciente* p) {
	while (p) {
		Paciente* n = p->proximo;
		limparHistorico(&p->historico);
		free(p);
		p = n;
	}
}

// Garante que os diretórios pais do caminho existam (cria recursivamente)
static int ensure_parent_dirs(const char* path) {
	if (!path) return -1;
	size_t len = strlen(path);
	if (!len) return 0;
	// Copia mutável
	char* buf = (char*)malloc(len + 1);
	if (!buf) return -1;
	memcpy(buf, path, len + 1);

	// Encontra o último separador de diretório
	char* lastSep = NULL;
	for (char* p = buf; *p; ++p) {
		if (*p == '/' || *p == '\\') lastSep = p;
	}
	if (!lastSep) { free(buf); return 0; } // sem diretórios no caminho

	// Trunca no último separador para obter o diretório pai
	*lastSep = '\0';
	if (buf[0] == '\0') { free(buf); return 0; }

	// Cria segmentos progressivamente
	for (char* p = buf; *p; ++p) {
		if (*p == '/' || *p == '\\') {
			char ch = *p; *p = '\0';
			if (buf[0] != '\0') {
#ifdef _WIN32
				_mkdir(buf);
#else
				mkdir(buf, 0777);
#endif
			}
			*p = ch;
		}
	}
	// Cria o diretório completo
#ifdef _WIN32
	_mkdir(buf);
#else
	mkdir(buf, 0777);
#endif
	free(buf);
	return 0;
}

// --- Serialização ---
int salvarPacientesEmArquivo(const Paciente* lista, const char* caminho) {
	if (!caminho) return -1;
	// Garante que diretórios necessários existam
	ensure_parent_dirs(caminho);
	FILE* f = fopen(caminho, "w");
	if (!f) return -1;

	// Contar pacientes
	int n = 0;
	for (const Paciente* p = lista; p; p = p->proximo) ++n;
	fprintf(f, "PACIENTES %d\n", n);
	fputs("# Formato: P|nome|cpf|idade|prioridade|Q\n", f);
	fputs("#          A|data|descricao (repetido Q vezes)\n", f);

	for (const Paciente* p = lista; p; p = p->proximo) {
		// Contar histórico (pilha LIFO)
		int q = 0; for (const Atendimento* a = p->historico.topo; a; a = a->prox) ++q;

		fputc('P', f); fputc('|', f);
		str_escape(p->nome, f); fputc('|', f);
		str_escape(p->cpf, f); fputc('|', f);
		fprintf(f, "%d|%d|%d\n", p->idade, p->prioridade, q);

		// Serializa pilha do topo para base (ordem LIFO)
		for (const Atendimento* a = p->historico.topo; a; a = a->prox) {
			fputc('A', f); fputc('|', f);
			str_escape(a->data, f); fputc('|', f);
			str_escape(a->descricao, f); fputc('\n', f);
		}
	}

	int rc = ferror(f) ? -1 : 0;
	fclose(f);
	return rc;
}

// --- Desserialização ---
int carregarPacientesDeArquivo(Paciente** listaOut, const char* caminho) {
	if (!listaOut || !caminho) return -1;
	*listaOut = NULL;

	FILE* f = fopen(caminho, "r");
	if (!f) return -1;

	char line[1024];
	int totalPac = 0;

	// Ler cabeçalho
	while (fgets(line, sizeof(line), f)) {
		// Remover newline
		size_t len = strlen(line); if (len && (line[len-1] == '\n' || line[len-1] == '\r')) line[--len] = '\0';
		if (!len) continue; // linha vazia
		if (line[0] == '#') continue;
		if (strncmp(line, "PACIENTES ", 10) == 0) {
			totalPac = atoi(line + 10);
			break;
		} else {
			// Se a primeira linha válida não for cabeçalho, formato inválido
			fclose(f);
			return -1;
		}
	}

	Paciente* lista = NULL;
	int lidos = 0;

	while (lidos < totalPac && fgets(line, sizeof(line), f)) {
		size_t len = strlen(line); if (len && (line[len-1] == '\n' || line[len-1] == '\r')) line[--len] = '\0';
		if (!len || line[0] == '#') continue;
		if (line[0] != 'P' || line[1] != '|') { continue; }

		// Parse P|nome|cpf|idade|prioridade|Q
		char* tmp = line + 2;
		char* fields[6] = {0};
	int nf = split_pipe_escaped(tmp, fields, 6);
	if (nf < 5) { libera_lista_total(lista); fclose(f); return -1; }
		const char* nome = fields[0];
		const char* cpf = fields[1];
		int idade = atoi(fields[2]);
		int prioridade = atoi(fields[3]);
		int q = atoi(fields[4]);

	Paciente* p = criarPaciente(nome, cpf, idade, prioridade);
	if (!p) { libera_lista_total(lista); fclose(f); return -1; }

		// Ler Q linhas A|...
		for (int i = 0; i < q; ++i) {
			if (!fgets(line, sizeof(line), f)) { libera_lista_total(lista); fclose(f); return -1; }
			len = strlen(line); if (len && (line[len-1] == '\n' || line[len-1] == '\r')) line[--len] = '\0';
			if (line[0] != 'A' || line[1] != '|') { libera_lista_total(lista); fclose(f); return -1; }
			tmp = line + 2;
			char* af[3] = {0};
			int na = split_pipe_escaped(tmp, af, 3);
			if (na < 2) { libera_lista_total(lista); fclose(f); return -1; }
			// Registra no historico do paciente
			pushAtendimento(&p->historico, af[1], af[0]);
		}
		adicionarPaciente(&lista, p);
		++lidos;
	}

	fclose(f);
	*listaOut = lista;
	return (lidos == totalPac) ? 0 : -1;
}

