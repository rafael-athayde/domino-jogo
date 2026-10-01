/*
 * jogo.c
 * Regras puras do domino: pecas, jogadas, compra, fim de jogo.
 * Sem entrada/saida (isso fica em tela.c/controller.c).
 */

#include <stdlib.h>
#include "jogo.h"

/* Retira e retorna a primeira peca do monte disponivel, deslocando
 * as demais. Usada na distribuicao inicial e na compra de pecas. */
static Peca retirarPecaDisponivel(Jogo *jogo)
{
    Peca peca;
    int i;

    peca.lado1 = -1;
    peca.lado2 = -1;

    if (jogo->quantidadeDisponiveis > 0)
    {
        peca = jogo->pecas[0];

        for (i = 0; i < jogo->quantidadeDisponiveis - 1; i++)
        {
            jogo->pecas[i] = jogo->pecas[i + 1];
        }

        jogo->quantidadeDisponiveis--;
    }

    return peca;
}

/* REQ01 - Gera as 28 pecas (0-0 a 6-6, sem repeticao) no monte. */
void criarPecas(Jogo *jogo)
{
    int lado1;
    int lado2;
    int indice;

    indice = 0;

    /* lado2 comeca em lado1 para nao gerar peca duplicada (ex: 2-5 e 5-2). */
    for (lado1 = 0; lado1 <= 6; lado1++)
    {
        for (lado2 = lado1; lado2 <= 6; lado2++)
        {
            jogo->pecas[indice].lado1 = lado1;
            jogo->pecas[indice].lado2 = lado2;

            indice++;
        }
    }

    jogo->quantidadeDisponiveis = TOTAL_PECAS;
}

/* REQ02 - Embaralha o monte disponivel (Fisher-Yates). */
void embaralharPecas(Jogo *jogo)
{
    int i;
    int j;
    Peca temp;

    for (i = jogo->quantidadeDisponiveis - 1; i > 0; i--)
    {
        j = rand() % (i + 1);

        temp = jogo->pecas[i];
        jogo->pecas[i] = jogo->pecas[j];
        jogo->pecas[j] = temp;
    }
}

/* Define o primeiro jogador de acordo com a regra do maior duplo:
 * verifica 6-6, depois 5-5, 4-4, ... ate 0-0.
 * Como cada peca e unica, no maximo um jogador possui cada duplo.
 * Caso nenhum duplo esteja nas maos (situacao possivel com a distribuicao
 * atual), utiliza sorteio apenas como criterio de desempate. */
static int definirPrimeiroJogador(const Jogo *jogo)
{
    int duplo;
    int jogador;
    int peca;

    for (duplo = 6; duplo >= 0; duplo--)
    {
        for (jogador = 0; jogador < jogo->numeroJogadores; jogador++)
        {
            for (peca = 0; peca < jogo->jogadores[jogador].quantidade; peca++)
            {
                if (jogo->jogadores[jogador].mao[peca].lado1 == duplo &&
                    jogo->jogadores[jogador].mao[peca].lado2 == duplo)
                {
                    return jogador;
                }
            }
        }
    }

    return rand() % jogo->numeroJogadores;
}

/* REQ03, REQ07, REQ08, REQ09, REQ10 - Prepara uma partida nova:
 * zera mesa e maos, cria/embaralha pecas, distribui 7 por jogador
 * e define quem comeca pelo maior duplo. */
void prepararPartida(Jogo *jogo, int numeroJogadores)
{
    int i;
    int p;

    jogo->numeroJogadores = numeroJogadores;
    jogo->mesa.quantidade = 0;
    jogo->jogoAtivo = 1;
    jogo->jogoInterrompido = 0;

    for (i = 0; i < MAX_JOGADORES; i++)
    {
        jogo->jogadores[i].quantidade = 0;
    }

    criarPecas(jogo);
    embaralharPecas(jogo);

    for (i = 0; i < numeroJogadores; i++)
    {
        for (p = 0; p < PECAS_POR_JOGADOR; p++)
        {
            jogo->jogadores[i].mao[p] = retirarPecaDisponivel(jogo);
            jogo->jogadores[i].quantidade++;
        }
    }

    /* REQ10 - o maior duplo presente nas maos define quem comeca. */
    jogo->jogadorAtual = definirPrimeiroJogador(jogo);
}

/* REQ11, REQ16 - Jogador compra uma peca do monte. */
int comprarPeca(Jogo *jogo, int jogador)
{
    int indice;

    if (!jogo->jogoAtivo)
        return 0;

    if (jogador < 0 || jogador >= jogo->numeroJogadores)
        return 0;

    if (jogo->quantidadeDisponiveis <= 0)
        return 0;

    indice = jogo->jogadores[jogador].quantidade;

    jogo->jogadores[jogador].mao[indice] = retirarPecaDisponivel(jogo);
    jogo->jogadores[jogador].quantidade++;

    return 1;
}

/* REQ12 - Verifica se a peca encaixa na extremidade escolhida
 * (1 = esquerda, compara com lado1 da primeira peca da mesa;
 *  2 = direita, compara com lado2 da ultima peca da mesa).
 * Mesa vazia: qualquer peca encaixa. */
int jogadaValida(
    const Jogo *jogo,
    int jogador,
    int indicePeca,
    int ladoMesa
)
{
    Peca peca;
    int esquerda;
    int direita;

    if (!jogo->jogoAtivo)
        return 0;

    if (jogador < 0 || jogador >= jogo->numeroJogadores)
        return 0;

    if (indicePeca < 0 || indicePeca >= jogo->jogadores[jogador].quantidade)
        return 0;

    if (ladoMesa != 1 && ladoMesa != 2)
        return 0;

    if (jogo->mesa.quantidade == 0)
        return 1;

    peca = jogo->jogadores[jogador].mao[indicePeca];

    if (ladoMesa == 1)
    {
        esquerda = jogo->mesa.pecas[0].lado1;

        if (peca.lado1 == esquerda || peca.lado2 == esquerda)
            return 1;

        return 0;
    }

    direita = jogo->mesa.pecas[jogo->mesa.quantidade - 1].lado2;

    if (peca.lado1 == direita || peca.lado2 == direita)
        return 1;

    return 0;
}

/* Joga a peca na extremidade escolhida (ja validada por
 * jogadaValida). Remove da mao, vira a peca se preciso encaixar,
 * e insere na mesa. */
int jogarPeca(
    Jogo *jogo,
    int jogador,
    int indicePeca,
    int ladoMesa
)
{
    Peca peca;
    int i;
    int extremidade;
    int temp;

    if (!jogadaValida(jogo, jogador, indicePeca, ladoMesa))
    {
        return 0;
    }

    peca = jogo->jogadores[jogador].mao[indicePeca];

    /* Remove a peca da mao, deslocando as demais. */
    for (i = indicePeca; i < jogo->jogadores[jogador].quantidade - 1; i++)
    {
        jogo->jogadores[jogador].mao[i] = jogo->jogadores[jogador].mao[i + 1];
    }

    jogo->jogadores[jogador].quantidade--;

    /* Primeira peca da mesa: nao ha extremidade para comparar. */
    if (jogo->mesa.quantidade == 0)
    {
        jogo->mesa.pecas[0] = peca;
        jogo->mesa.quantidade = 1;

        return 1;
    }

    if (ladoMesa == 1)
    {
        extremidade = jogo->mesa.pecas[0].lado1;

        /* Vira a peca se o lado que encaixa estiver do lado errado. */
        if (peca.lado1 == extremidade)
        {
            temp = peca.lado1;
            peca.lado1 = peca.lado2;
            peca.lado2 = temp;
        }

        /* Abre espaco no inicio do vetor da mesa. */
        for (i = jogo->mesa.quantidade; i > 0; i--)
        {
            jogo->mesa.pecas[i] = jogo->mesa.pecas[i - 1];
        }

        jogo->mesa.pecas[0] = peca;
        jogo->mesa.quantidade++;
    }
    else
    {
        extremidade = jogo->mesa.pecas[jogo->mesa.quantidade - 1].lado2;

        if (peca.lado2 == extremidade)
        {
            temp = peca.lado1;
            peca.lado1 = peca.lado2;
            peca.lado2 = temp;
        }

        jogo->mesa.pecas[jogo->mesa.quantidade] = peca;
        jogo->mesa.quantidade++;
    }

    return 1;
}

/* Verifica se o jogador tem alguma peca jogavel em qualquer
 * extremidade da mesa atual. */
int jogadorTemJogada(
    const Jogo *jogo,
    int jogador
)
{
    int i;

    if (jogo->mesa.quantidade == 0)
        return 1;

    for (i = 0; i < jogo->jogadores[jogador].quantidade; i++)
    {
        if (jogadaValida(jogo, jogador, i, 1) ||
            jogadaValida(jogo, jogador, i, 2))
        {
            return 1;
        }
    }

    return 0;
}

/* Avanca para o proximo jogador (volta a 0 apos o ultimo). */
void proximoJogador(Jogo *jogo)
{
    jogo->jogadorAtual++;

    if (jogo->jogadorAtual >= jogo->numeroJogadores)
    {
        jogo->jogadorAtual = 0;
    }
}

/* REQ15 - Jogo terminou se esta inativo ou algum jogador venceu
 * (ficou sem pecas). */
int jogoTerminou(const Jogo *jogo)
{
    int i;

    if (!jogo->jogoAtivo)
        return 1;

    for (i = 0; i < jogo->numeroJogadores; i++)
    {
        if (jogo->jogadores[i].quantidade == 0)
            return 1;
    }

    return 0;
}

/* Encerra a partida manualmente. */
void finalizarJogo(Jogo *jogo)
{
    jogo->jogoAtivo = 0;
}
