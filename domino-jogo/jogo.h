/*
 * jogo.h
 * Estruturas e regras do jogo de domino (criar/embaralhar pecas,
 * jogadas, compra, fim de jogo). Nao faz entrada/saida.
 */

#ifndef JOGO_H
#define JOGO_H

#include "peca.h"

#define TOTAL_PECAS 28
#define MAX_JOGADORES 2
#define PECAS_POR_JOGADOR 7

/* Mao de um jogador. */
typedef struct {
    Peca mao[TOTAL_PECAS];
    int quantidade;
} Jogador;

/* Pecas jogadas na mesa, em ordem (esquerda -> direita). */
typedef struct {
    Peca pecas[TOTAL_PECAS];
    int quantidade;
} Mesa;

/* Estado completo de uma partida (tambem usado para salvar/carregar). */
typedef struct {
    Peca pecas[TOTAL_PECAS];       /* monte disponivel para compra */

    int quantidadeDisponiveis;

    Jogador jogadores[MAX_JOGADORES];

    Mesa mesa;

    int numeroJogadores;
    int jogadorAtual;      /* indice (0 ou 1) do jogador da vez */
    int jogoAtivo;         /* 1 = partida em andamento */
    int contraComputador;  /* 1 = jogador 2 e a IA */
    int jogoInterrompido;  /* 1 = jogo foi salvo e interrompido */

} Jogo;

/* Gera as 28 pecas do domino no monte disponivel. */
void criarPecas(Jogo *jogo);

/* Embaralha as pecas do monte disponivel (Fisher-Yates). */
void embaralharPecas(Jogo *jogo);

/* Prepara uma nova partida: cria, embaralha e distribui as pecas,
 * e sorteia quem comeca. */
void prepararPartida(Jogo *jogo, int numeroJogadores);

/* Jogador compra uma peca do monte. Retorna 0 se nao houver pecas
 * disponiveis ou o jogo/jogador for invalido. */
int comprarPeca(Jogo *jogo, int jogador);

/* Verifica (sem alterar nada) se a peca "indicePeca" da mao do
 * jogador encaixa na extremidade "ladoMesa" (1 = esq, 2 = dir). */
int jogadaValida(
    const Jogo *jogo,
    int jogador,
    int indicePeca,
    int ladoMesa
);

/* Joga a peca "indicePeca" na extremidade "ladoMesa", virando-a se
 * necessario. Retorna 0 se a jogada for invalida. */
int jogarPeca(
    Jogo *jogo,
    int jogador,
    int indicePeca,
    int ladoMesa
);

/* Verifica se o jogador tem alguma peca jogavel na mesa atual. */
int jogadorTemJogada(
    const Jogo *jogo,
    int jogador
);

/* Passa a vez para o proximo jogador (circular). */
void proximoJogador(Jogo *jogo);

/* Verifica se o jogo terminou (inativo ou algum jogador sem pecas). */
int jogoTerminou(const Jogo *jogo);

/* Encerra a partida manualmente. */
void finalizarJogo(Jogo *jogo);

#endif
