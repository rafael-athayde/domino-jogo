/*
 * tela.h
 * Funcoes de exibicao (console). So leem o Jogo, nunca alteram.
 */

#ifndef TELA_H
#define TELA_H

#include "jogo.h"

/* Imprime o menu principal. */
void mostrarMenu(void);

/* Lista as pecas disponiveis no monte, numeradas a partir de 1. */
void mostrarPecas(
    const Jogo *jogo
);

/* Mostra as pecas na mesa, da esquerda para a direita. */
void mostrarMesa(
    const Jogo *jogo
);

/* Mostra a mao de um jogador. esconder=1 exibe "[?? | ??]" no
 * lugar dos valores (usado para nao revelar a mao do oponente). */
void mostrarMao(
    const Jogo *jogo,
    int jogador,
    int esconder
);

/* Imprime as regras do jogo. */
void mostrarRegras(void);

/* Imprime o resultado final (vencedor, ou motivo do fim de jogo). */
void mostrarResultado(
    const Jogo *jogo
);

#endif
