# Jogo de Dominó em C

Projeto desenvolvido durante a graduação em **Ciência da Computação**, na disciplina de **Laboratório de Programação**, em conjunto com outros três colegas.

O objetivo foi desenvolver, em linguagem C, uma versão digital do jogo de dominó tradicional executada pelo terminal, aplicando conceitos de programação, estruturas de dados, modularização, manipulação de arquivos, controle de fluxo e organização de software.

## Equipe

- Nicholas Papiani
- Henrique Azevedo
- Rafael Athayde
- Pedro Nunes

**Curso:** Ciência da Computação  
**Disciplina:** Laboratório de Programação  
**Ano:** 2026

---

## Sobre o projeto

O projeto consiste na implementação de um jogo de dominó tradicional em C, executado através do terminal.

Durante o desenvolvimento, trabalhamos desde a criação e representação das peças até o controle completo de uma partida, incluindo distribuição das peças, gerenciamento dos turnos, validação das jogadas, compra de peças, finalização da partida e interação com o usuário.

O sistema possui dois principais modos de jogo:

- Dois jogadores
- Jogador contra computador

O dominó utiliza as 28 peças tradicionais, contendo todas as combinações possíveis entre os números de 0 a 6.

---

## Funcionamento do jogo

Ao iniciar uma partida, o sistema:

1. Cria as 28 peças;
2. Embaralha as peças;
3. Distribui 7 peças para cada jogador;
4. Define quem começa;
5. Inicializa a mesa;
6. Inicia o sistema de turnos;
7. Permite que os jogadores realizem suas jogadas;
8. Verifica continuamente as condições de encerramento;
9. Determina o resultado da partida.

O jogador que possui a peça `6|6` começa. Caso nenhum jogador possua essa peça, o sistema procura a maior peça dupla disponível.

---

## A mesa

A mesa representa as peças que já foram jogadas.

Exemplo:

```text
[4|5] [5|3] [3|3] [3|0] [0|1]
```

Nesse caso, as extremidades disponíveis para a próxima jogada são `4` e `1`.

O jogador precisa escolher uma peça que possua um dos números correspondentes às extremidades disponíveis.

---

## Ações do jogador

### Jogar

O jogador escolhe uma peça da própria mão e determina em qual extremidade da mesa deseja colocá-la.

### Comprar

Caso não possua uma peça adequada, o jogador pode comprar peças do estoque enquanto houver peças disponíveis.

### Passar

A passagem é permitida quando o jogador não possui nenhuma jogada possível e não existem mais peças disponíveis para compra.

### Salvar e sair

O jogador pode interromper a partida e salvar seu progresso para continuar posteriormente.

---

## Modo contra o computador

O projeto também possui um modo em que o segundo jogador é controlado pelo computador.

O computador possui sua própria mão e não visualiza as peças do jogador nem as peças que ainda estão disponíveis no estoque.

Durante seu turno, o computador:

1. Percorre suas peças;
2. Verifica quais podem ser jogadas;
3. Tenta primeiro a extremidade esquerda;
4. Depois verifica a extremidade direita;
5. Compra uma peça caso não encontre uma jogada;
6. Continua procurando enquanto houver peças disponíveis;
7. Passa a vez caso não consiga realizar nenhuma jogada.

A estratégia implementada é uma heurística simples baseada na ordem atual das peças da mão, sem utilização de estratégias avançadas de contagem de pontos ou análise probabilística.

---

## Validação das jogadas

Uma das partes do projeto foi a implementação de um sistema responsável por verificar se uma jogada é válida antes de alterar o estado da partida.

A função `jogadaValida()` verifica:

- Se a partida está ativa;
- Se o jogador é válido;
- Se a peça escolhida pertence à mão do jogador;
- Se a extremidade escolhida é válida;
- Se a peça possui um número compatível com a extremidade selecionada.

A validação é utilizada tanto pelos jogadores humanos quanto pelo computador.

---

## Organização do projeto

O código foi dividido em módulos, cada um responsável por uma parte específica do sistema.

```text
Projeto
│
├── Peca.h
│
├── Jogo.h
├── jogo.c
│
├── Tela.h
├── tela.c
│
├── Controller.h
├── controller.c
│
├── ia.c
│
├── main.c
│
├── docs/
│   ├── Documentação Técnica
│   └── Documentação do Usuário
│
└── README.md
```

A divisão dos arquivos permite separar as responsabilidades do sistema e facilita a manutenção e evolução do código.

---

## Arquitetura

A organização do projeto segue o conceito de **Model-View-Controller (MVC)**, adaptado para uma aplicação procedural em C.

### Model

Responsável pelo estado e pelas regras do jogo.

Principais arquivos:

```text
Peca.h
Jogo.h
jogo.c
```

O modelo controla elementos como:

- Peças;
- Jogadores;
- Mesa;
- Estoque;
- Turnos;
- Validação;
- Jogadas;
- Estado da partida.

### View

Responsável pela apresentação das informações no terminal.

```text
Tela.h
tela.c
```

Entre as funções de apresentação estão:

```text
mostrarMenu()
mostrarPecas()
mostrarMesa()
mostrarMao()
mostrarRegras()
mostrarResultado()
```

### Controller

Responsável pelo fluxo da aplicação.

```text
Controller.h
controller.c
```

O controller coordena:

- Menu principal;
- Inicialização da partida;
- Turnos;
- Interação com o jogador;
- Execução da partida.

### IA

O arquivo `ia.c` concentra o comportamento do jogador controlado pelo computador.

---

## Estruturas de dados

Durante o desenvolvimento foram utilizadas `struct` para representar os principais elementos do jogo.

### Peça

```c
typedef struct {
    int lado1;
    int lado2;
} Peca;
```

Cada peça possui dois lados, com valores entre `0` e `6`.

### Jogador

```c
typedef struct {
    Peca mao[TOTAL_PECAS];
    int quantidade;
} Jogador;
```

A mão é representada por um vetor de peças e um contador indicando quantas posições estão ocupadas.

### Mesa

```c
typedef struct {
    Peca pecas[TOTAL_PECAS];
    int quantidade;
} Mesa;
```

A mesa mantém a sequência das peças que já foram jogadas.

A estrutura principal `Jogo` reúne as informações necessárias para controlar o estado completo da partida.

---

## Criação e embaralhamento das peças

O sistema gera as 28 combinações tradicionais sem duplicar peças equivalentes.

Exemplo:

```text
[0|0]
[0|1]
[0|2]
...
[6|6]
```

Uma peça como `[1|0]` não é criada novamente, pois representa a mesma peça que `[0|1]`.

Após a criação, as peças são embaralhadas antes da distribuição.

---

## Controle dos turnos

O sistema mantém o jogador atual e alterna os turnos durante a partida.

Depois de uma jogada válida, o sistema verifica se a partida terminou. Caso contrário, o próximo jogador assume o turno.

O controle também trata situações em que o jogador precisa comprar peças ou passar a vez.

---

## Salvamento e recuperação

O projeto possui um sistema para salvar uma partida em andamento.

O estado do jogo é armazenado no arquivo:

```text
jogo_salvo.dat
```

O salvamento utiliza operações de arquivo binário para armazenar as informações necessárias para reconstruir o estado da partida.

Posteriormente, o usuário pode recuperar o jogo salvo e continuar a partida.

---

## Mãos ocultas

No modo contra computador, implementamos o controle de visualização das mãos.

Quando necessário, as peças podem ser exibidas como:

```text
[?? | ??]
```

Dessa forma, as peças do outro jogador não ficam visíveis durante a partida.

---

## Finalização da partida

A partida pode terminar de diferentes maneiras.

### Jogador sem peças

Quando um jogador coloca sua última peça na mesa, ele vence a partida.

### Partida fechada

Caso nenhum jogador consiga realizar uma jogada e não existam mais peças disponíveis para compra, a partida é encerrada.

Nesse caso, a quantidade de peças restantes é utilizada para determinar o vencedor. Em caso de empate na quantidade, os pontos das peças restantes são comparados.

---

## Tecnologias e conceitos utilizados

### Linguagem

- C

### Ferramentas

- GCC
- Git
- GitHub

### Conceitos

- Structs
- Vetores
- Ponteiros
- Funções
- Modularização
- Algoritmos
- Controle de fluxo
- Validação de dados
- Manipulação de arquivos
- Geração de números aleatórios
- Separação de responsabilidades
- Arquitetura MVC
- Heurística para tomada de decisão da IA

---

## Documentação

O projeto possui documentação desenvolvida pela equipe.

### Documentação do Usuário

Apresenta o funcionamento do jogo do ponto de vista de quem irá utilizá-lo, incluindo regras, modos de jogo, ações disponíveis, menu e funcionamento da partida.

### Documentação Técnica

Apresenta detalhes da implementação, arquitetura, estruturas de dados, algoritmos, fluxo de execução, lógica do computador, salvamento e decisões técnicas utilizadas no desenvolvimento.

Os documentos estão disponíveis na pasta `docs/`.

---

## Como executar

### Pré-requisitos

É necessário possuir um compilador C, como o GCC.

### Compilação

Após clonar o repositório:

```bash
gcc *.c -o domino
```

### Execução

No Windows:

```bash
domino.exe
```

No Linux ou macOS:

```bash
./domino
```

O comando de compilação pode variar de acordo com a organização dos arquivos no repositório.

---

## Menu principal

O sistema possui um menu para acesso às principais funcionalidades:

```text
1 - Iniciar jogo (2 jogadores)
2 - Iniciar jogo (contra o computador)
3 - Mostrar peças do jogo
4 - Retornar ao jogo interrompido
5 - Regras gerais do jogo
0 - Sair do programa
```

---

## Objetivo acadêmico

O projeto foi desenvolvido como uma atividade prática da disciplina de **Laboratório de Programação**, buscando aplicar na prática conceitos estudados durante a graduação.

O desenvolvimento envolveu a divisão de responsabilidades entre os integrantes da equipe, organização do código, criação de documentação e aplicação de conceitos de programação estruturada.

Entre os principais conhecimentos aplicados estão linguagem C, estruturas de dados, modularização, algoritmos, manipulação de arquivos e lógica de programação.

---

## Equipe

| Integrante | Função |
|---|---|
| Nicholas Papiani | Desenvolvimento |
| Henrique Azevedo | Desenvolvimento |
| Rafael Athayde | Desenvolvimento |
| Pedro Nunes | Desenvolvimento |

---

## Status

**Projeto acadêmico concluído — 2026.**

Desenvolvido em conjunto durante a disciplina de **Laboratório de Programação — Ciência da Computação**.
