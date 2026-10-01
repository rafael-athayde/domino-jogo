/*
 * ia.c
 * Turno do jogador controlado pelo computador: joga a primeira
 * peca valida (tenta esquerda, depois direita); se nao houver,
 * compra pecas ate conseguir jogar ou o monte esgotar.
 */

#include <stdio.h>
#include <stdlib.h> /* system("pause") */
#include "controller.h" /* minusculo para compilar em Linux tambem */
#include "jogo.h"
#include "tela.h"

void jogarTurnoComputador(Jogo *jogo)
{
    int jogador = jogo->jogadorAtual;
    int comprou = 1; /* 1 so para entrar no laco pela primeira vez */
    int i;

    printf("\n");
    printf("====================================\n");
    printf("    VEZ DO COMPUTADOR (Jogador 2)   \n");
    printf("====================================\n");

    mostrarMesa(jogo);

    while (comprou)
    {
        /* Procura, na mao, a primeira peca que encaixe em alguma
         * extremidade. */
        for (i = 0; i < jogo->jogadores[jogador].quantidade; i++)
        {
            if (jogadaValida(jogo, jogador, i, 1))
            {
                jogarPeca(jogo, jogador, i, 1);
                printf("\n>>> O COMPUTADOR JOGOU UMA PECA NA ESQUERDA! <<<\n\n");

                system("pause");

                if (!jogoTerminou(jogo)) {
                    proximoJogador(jogo);
                }
                return;
            }

            if (jogadaValida(jogo, jogador, i, 2))
            {
                jogarPeca(jogo, jogador, i, 2);
                printf("\n>>> O COMPUTADOR JOGOU UMA PECA NA DIREITA! <<<\n\n");

                system("pause");

                if (!jogoTerminou(jogo)) {
                    proximoJogador(jogo);
                }
                return;
            }
        }

        /* Nao achou peca jogavel: tenta comprar e repete a busca. */
        comprou = comprarPeca(jogo, jogador);
        if (comprou)
        {
            printf("\n>>> O computador comprou uma peca do monte...\n");
        }
    }

    /* Monte vazio e nenhuma peca jogavel: passa a vez. */
    printf("\n>>> O computador nao tem pecas validas e o monte esta vazio. Passou a vez!\n\n");
    system("pause");
    proximoJogador(jogo);
}
