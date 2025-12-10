# Análise de Eficiência do Sistema

## 1. Definições e premissas

- **n**: número de pacientes cadastrados na lista.  
- **f**: número de elementos atualmente na fila de atendimento.  
- **k**: número de atendimentos registrados no histórico de um paciente (no pior caso, histórico máximo).  
- **K_total**: soma total de atendimentos de todos os pacientes.  

As análises consideram o uso de listas encadeadas, fila com ponteiros `head`/`tail`, pilha para histórico e operações de E/S (salvar/carregar) que percorrem todas as estruturas.  

---

## 2. Complexidade por módulo e função

| Função / Operação                          | Operação Básica                           | Melhor caso | Médio caso | Pior caso  | Espaço Extra |
|-------------------------------------------|------------------------------------------|-------------|------------|------------|--------------|
| `criarPaciente(...)`                      | alocar + copiar dados fixos              | O(1)        | O(1)       | O(1)       | O(1)         |
| `adicionarPaciente` (inserção no início)  | ajustar ponteiros                        | O(1)        | O(1)       | O(1)       | O(1)         |
| `adicionarPaciente` (inserção no fim)*    | percorrer lista ou usar `tail`           | O(1) / O(n) | O(n/2)     | O(n)       | O(1)         |
| `listarPacientes`                         | percorrer todos pacientes                | Θ(n)        | Θ(n)       | Θ(n)       | O(1)         |
| `buscarPacientePorCPF`                    | comparar CPF em cada nó                  | O(1)        | O(n/2)     | O(n)       | O(1)         |
| `filaInit()`                              | inicialização da estrutura               | O(1)        | O(1)       | O(1)       | O(1)         |
| `enfileirar(fila, paciente)`              | ajustar tail e head se necessário        | O(1)        | O(1)       | O(1)       | O(1)         |
| `desenfileirar(fila)`                     | ajustar head, possivelmente tail         | O(1)        | O(1)       | O(1)       | O(1)         |
| `mostrarFila(fila)`                       | percorrer todos os elementos da fila     | Θ(f)        | Θ(f)       | Θ(f)       | O(1)         |
| `pushAtendimento(paciente, atendimento)`  | alocar + ajustar ponteiros da pilha      | O(1)        | O(1)       | O(1)       | O(1)         |
| `mostrarHistorico(paciente)`              | percorrer a pilha de atendimentos        | Θ(k)        | Θ(k)       | Θ(k)       | O(1)         |
| `limparHistorico(paciente)`               | liberar todos os nós da pilha            | Θ(k)        | Θ(k)       | Θ(k)       | O(1)         |
| `salvarPacientesEmArquivo(...)`           | serializar + escrever todos dados        | Θ(n + K_total) | Θ(n + K_total) | Θ(n + K_total) | O(1) ou O(n+K) |
| `carregarPacientesDeArquivo(...)`         | ler + alocar nós + reconstruir estruturas| Θ(n + K_total + f) | Θ(n + K_total + f) | Θ(n + K_total + f) | O(n + K_total + f) |

\* Se a lista mantiver ponteiro `tail`, a inserção no fim também é O(1). Caso contrário, é O(n).  

---

## 3. Complexidade global típica e casos de uso

- **Cadastro de paciente:** O(1) por paciente.  
- **Listagem de pacientes:** O(n).  
- **Inserção na fila / atendimento de paciente / registro de histórico:** O(1).  
- **Visualização de fila ou histórico:** O(f) ou O(k).  
- **Salvamento completo (persistência):** O(n + K_total).  
- **Carregamento completo:** O(n + K_total + f).  

Assim, as operações que percorrem coleções inteiras (lista, fila, histórico, salvar/carregar) têm custo linear em relação ao número de elementos. Operações isoladas (inserção, enfileirar, desenfileirar, push histórico) têm custo constante, o que garante boa escalabilidade para manipulação individual de dados.  

Se buscas por CPF forem frequentes, o custo pode se tornar elevado (O(n) por busca). Nesse caso, recomenda-se usar uma estrutura de indexação (hashmap) para reduzir a busca para O(1) em média.  

---

## 4. Uso de memória (heap) — Espaço dinâmico

- Cada paciente cadastrado consome memória proporcional ao tamanho fixo da `struct Paciente`.  
- A pilha de histórico usa memória proporcional ao número de atendimentos registrados.  
- A fila consome memória proporcional à quantidade de pacientes enfileirados.  
- No total, o uso de memória cresce como **O(n + K_total + f)**.  

---

## 5. Considerações de I/O (arquivo)

- O custo de E/S depende da quantidade de dados (n e K_total).  
- Usar **gravação em streaming** (paciente por paciente) ajuda a manter o consumo de memória extra baixo.  
- Em caso de grande volume de dados, tempo de leitura/escrita pode dominar a execução — o custo será linear em n + K_total.  

---

## 6. Sugestões de melhorias e otimizações

- Manter ponteiro `tail` na lista — inserções no fim passam a ser O(1).  
- Usar uma **tabela hash (mapa CPF → paciente)** para acelerar buscas por CPF, reduzindo custo de O(n) para O(1) médio.  
- Limitar listagens completas ou oferecer paginação quando houver muitos pacientes — evita travamentos/perda de usabilidade.  
- Ao salvar/carregar, usar **modo streaming**, e **evitar carregar tudo de uma vez** em memória se os dados forem grandes.  
- Para relatórios ou operações em lote (listar + salvar todos), documentar que o custo será proporcional ao tamanho da base.  

---

## 7. Como apresentar essa análise no repositório

Recomenda-se incluir este arquivo como `ANALISE_EFICIENCIA.md` na raiz do projeto ou dentro de uma pasta `docs/`. Assim, quem quiser revisar o código ou avaliar escala poderá consultar a análise teórica da eficiência.  

Também é possível implementar um pequeno **benchmark** para validar empiricamente os tempos assintóticos — por exemplo, medir quanto tempo leva para cadastrar 10.000 pacientes, buscar por CPF, enfileirar/desenfileirar, salvar e carregar.  

---

## 8. Conclusão

A estrutura atual do sistema está bem pensada e oferece boas garantias de desempenho para operações comuns (inserção, enfileirar, atender, registrar histórico). A principal limitação de eficiência está nas buscas em lista encadeada e nas operações que varrem toda a base (listagem, salvamento, carregamento) — mas isso é esperado em sistemas com listas simples.  

Com as melhorias sugeridas, especialmente indexação por CPF e uso de ponteiro `tail`, o sistema pode manter boa performance mesmo com grande volume de dados.  

--- 