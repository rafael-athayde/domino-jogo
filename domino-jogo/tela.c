/*
 * tela.c
 * Implementa a exibicao no console (menu, pecas, mesa, mao,
 * regras, resultado). So le o Jogo, nunca altera.
 */

#include <stdio.h>
#include "tela.h"

/* Imprime o menu principal. */
void mostrarMenu(void)
{
    printf("\n");
    printf("====================================\n");
    printf("========== Jogo de domino ==========\n");
    printf("====================================\n");
    printf("\n");

    printf("1 - Iniciar jogo (2 jogadores)\n");
    printf("2 - Iniciar jogo (contra o computador)\n");
    printf("3 - Mostrar pecas do jogo\n");
    printf("4 - Recuperar jogo salvo/interrompido\n");
    printf("5 - Regras do jogo\n");
    printf("0 - Sair do programa\n");
}

/* REQ04 - Lista as pecas disponiveis no monte. */
void mostrarPecas(
    const Jogo *jogo
)
{
    int i;

    printf("\n");
    printf(
        "Pecas disponiveis (%d):\n",
        jogo->quantidadeDisponiveis
    );

    printf("\n");

    for (i = 0; i < jogo->quantidadeDisponiveis; i++)
    {
        printf(
            "Peca %2d: [%d | %d]\n",
            i + 1,
            jogo->pecas[i].lado1,
            jogo->pecas[i].lado2
        );
    }

    printf("\n");
}

/* REQ09 - Mostra as pecas na mesa, em ordem. */
void mostrarMesa(
    const Jogo *jogo
)
{
    int i;

    printf("\nMesa: ");

    if (jogo->mesa.quantidade == 0)
    {
        printf("[vazia]\n");
        return;
    }

    for (i = 0; i < jogo->mesa.quantidade; i++)
    {
        printf(
            "[%d | %d] ",
            jogo->mesa.pecas[i].lado1,
            jogo->mesa.pecas[i].lado2
        );
    }

    printf("\n");
}

/* REQ13, REQ14 - Mostra a mao do jogador. esconder=1 oculta os
 * valores (usado para nao revelar a mao do outro jogador humano). */
void mostrarMao(
    const Jogo *jogo,
    int jogador,
    int esconder
)
{
    int i;

    printf(
        "\nJogador %d - %d pecas:\n",
        jogador + 1,
        jogo->jogadores[jogador].quantidade
    );

    for (i = 0; i < jogo->jogadores[jogador].quantidade; i++)
    {
        if (esconder)
        {
            printf("[%d] [?? | ??]\n", i + 1);
        }
        else
        {
            printf(
                "[%d] [%d | %d]\n",
                i + 1,
                jogo->jogadores[jogador].mao[i].lado1,
                jogo->jogadores[jogador].mao[i].lado2
            );
        }
    }
}

/* REQ17 - Imprime as regras do jogo. */
void mostrarRegras(void)
{
    printf("\n");
    printf("========== REGRAS ==========\n");

    printf("- O domino possui 28 pecas.\n");
    printf("- Cada jogador recebe 7 pecas.\n");
    printf("- A mesa comeca vazia.\n");
    printf("- O jogador com o maior duplo comeca (6-6, 5-5, 4-4, etc.).\n");
    printf("- A peca deve encaixar em uma extremidade da mesa.\n");
    printf("- O jogador pode comprar uma peca quando necessario.\n");
    printf("- Se nao houver jogada valida e o estoque estiver vazio, o jogador passa a vez.\n");
    printf("- Vence quem ficar sem pecas primeiro.\n");

    printf("============================\n");
    printf("\n");
}

/* Mostra o resultado da partida: vencedor (sem pecas na mao), ou,
 * se ninguem venceu, se o jogo foi salvo/interrompido ou
 * finalizado manualmente. */
void mostrarResultado(
    const Jogo *jogo
)
{
    int i;

    printf("\n");
    printf("========== FIM DE JOGO ==========\n");

    for (i = 0; i < jogo->numeroJogadores; i++)
    {
        if (jogo->jogadores[i].quantidade == 0)
        {
            printf("Jogador %d venceu!\n", i + 1);
            printf("=================================\n");
            printf("\n");

            return;
        }
    }

    if (jogo->jogoInterrompido)
    {
        printf("O jogo foi interrompido e salvo.\n");
    }
    else
    {
        printf("O jogo foi finalizado pelo usuario.\n");
    }

    printf("=================================\n");
    printf("\n");
}
