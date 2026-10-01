/*
 * controller.c
 * Fluxo do jogo: menu principal, turno de cada jogador, inicio/
 * execucao da partida e salvar/carregar em arquivo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "controller.h"
#include "tela.h"

/* Executa o turno do jogador atual. Se for a vez da IA, delega
 * para jogarTurnoComputador(); senao, mostra mesa/mao e le a
 * escolha do jogador humano. */
static void jogarTurno(
    Jogo *jogo
)
{
    int jogador;
    int opcao;
    int indice;
    int lado;

    jogador = jogo->jogadorAtual;

    if (jogo->contraComputador == 1 && jogador == 1)
    {
        jogarTurnoComputador(jogo);
        return;
    }

    printf("\n");
    printf("------------------------------------\n");
    printf("Vez do Jogador %d\n", jogador + 1);
    printf("------------------------------------\n");

    mostrarMesa(jogo);
    mostrarMao(jogo, jogador, 0);

    /* Se o jogador nao possui nenhuma jogada valida e o monte
     * esta vazio, nao ha o que comprar: ele passa automaticamente. */
    if (!jogadorTemJogada(jogo, jogador) && jogo->quantidadeDisponiveis == 0)
    {
        printf("\nNao ha nenhuma jogada valida e nao existem mais pecas para comprar.\n");
        printf("Jogador %d passou a vez!\n", jogador + 1);
        proximoJogador(jogo);
        return;
    }

    printf("\n");
    printf("1 - Jogar peca\n");
    printf("2 - Comprar peca\n");
    printf("3 - Finalizar jogo\n");
    printf("4 - Salvar jogo e sair\n");
    printf("Escolha: ");

    scanf("%d", &opcao);

    /* Jogar uma peca da mao na mesa. */
    if (opcao == 1)
    {
        printf("Escolha o numero da peca: ");
        scanf("%d", &indice);

        /* Usuario escolhe a partir de 1; vetor usa a partir de 0. */
        indice--;

        if (jogo->mesa.quantidade == 0)
        {
            /* Mesa vazia: nao ha extremidade a escolher. */
            lado = 1;
        }
        else
        {
            printf("Escolha a extremidade:\n");
            printf("1 - Esquerda\n");
            printf("2 - Direita\n");
            printf("Escolha: ");

            scanf("%d", &lado);
        }

        /* REQ12 - jogarPeca ja valida a jogada internamente. */
        if (jogarPeca(jogo, jogador, indice, lado))
        {
            printf("\nPeca jogada com sucesso!\n");

            /* REQ15 - so passa a vez se a partida nao acabou agora. */
            if (!jogoTerminou(jogo))
            {
                proximoJogador(jogo);
            }
        }
        else
        {
            printf("\nJogada invalida!\n");
            printf("A peca nao encaixa nessa extremidade.\n");
        }
    }

    /* REQ11, REQ16 - Comprar peca do monte. */
    else if (opcao == 2)
    {
        if (comprarPeca(jogo, jogador))
        {
            printf("\nPeca comprada com sucesso!\n");
            mostrarMao(jogo, jogador, 0);
        }
        else
        {
            if (jogo->quantidadeDisponiveis == 0)
            {
                printf("\nNao ha mais pecas para comprar.\n");
                printf("Jogador %d passou a vez!\n", jogador + 1);
                proximoJogador(jogo);
            }
            else
            {
                printf("\nNao foi possivel comprar uma peca.\n");
            }
        }
    }

    /* Finalizar a partida manualmente. */
    else if (opcao == 3)
    {
        finalizarJogo(jogo);
        printf("\nJogo finalizado.\n");
    }

    /* Salvar e sair: marca o jogo como interrompido para poder
     * ser recuperado depois pelo menu principal. */
    else if (opcao == 4)
    {
        if (salvarJogo(jogo))
        {
            jogo->jogoInterrompido = 1;
            jogo->jogoAtivo = 0;
            printf("\nJogo salvo com sucesso!\n");
            printf("Voce pode recuperar a partida pelo menu principal.\n");
        }
        else
        {
            printf("\nNao foi possivel salvar o jogo.\n");
        }
    }

    else
    {
        printf("\nOpcao invalida!\n");
    }
}

/* Grava o Jogo (struct completa, binario) em "jogo_salvo.dat". */
int salvarJogo(const Jogo *jogo)
{
    FILE *arquivo;

    arquivo = fopen("jogo_salvo.dat", "wb");

    if (arquivo == NULL)
    {
        return 0;
    }

    if (fwrite(jogo, sizeof(Jogo), 1, arquivo) != 1)
    {
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);
    return 1;
}

/* Le "jogo_salvo.dat" e carrega o conteudo no Jogo apontado. */
int recuperarJogo(Jogo *jogo)
{
    FILE *arquivo;

    arquivo = fopen("jogo_salvo.dat", "rb");

    if (arquivo == NULL)
    {
        return 0;
    }

    if (fread(jogo, sizeof(Jogo), 1, arquivo) != 1)
    {
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);
    return 1;
}

/* Prepara e inicia uma nova partida, mostrando as maos iniciais. */
void iniciarJogo(
    Jogo *jogo,
    int numeroJogadores
)
{
    prepararPartida(jogo, numeroJogadores);

    printf("\n");
    printf("Nova partida iniciada!\n");
    printf("Cada jogador recebeu 7 pecas.\n");
    printf("Primeira jogada: Jogador %d.\n", jogo->jogadorAtual + 1);

    if (numeroJogadores == 2)
    {
        /* Na apresentacao inicial, nenhum jogador deve ter sua mao
         * revelada antes de sabermos quem fara a primeira jogada. */
        printf("\nMaos iniciais (ocultas ate definir o jogador da vez):\n");
        mostrarMao(jogo, 0, 1);
        mostrarMao(jogo, 1, 1);
    }
    else
    {
        /* Contra o computador, somente a mao do jogador humano e exibida. */
        mostrarMao(jogo, 0, 0);
    }

    executarPartida(jogo);
}

/* Laco principal da partida: joga turno a turno ate acabar. */
void executarPartida(
    Jogo *jogo
)
{
    while (jogo->jogoAtivo && !jogoTerminou(jogo))
    {
        jogarTurno(jogo);
    }

    mostrarResultado(jogo);
}

/* REQNF05 - Controlador principal: inicializa o Jogo e roda o
 * menu principal ate a opcao "Sair". */
void iniciarSistema(void)
{
    Jogo jogo;
    int escolha;

    /* Valores neutros ate a primeira partida ser preparada. */
    jogo.quantidadeDisponiveis = 0;
    jogo.numeroJogadores = 0;
    jogo.jogadorAtual = 0;
    jogo.jogoAtivo = 0;
    jogo.mesa.quantidade = 0;
    jogo.contraComputador = 0;
    jogo.jogoInterrompido = 0;

    srand((unsigned int)time(NULL));

    do
    {
        mostrarMenu();
        printf("\nEscolha: ");
        scanf("%d", &escolha);

        switch (escolha)
        {
            /* REQ07 - Partida com 2 jogadores humanos. */
            case 1:
                jogo.contraComputador = 0;
                iniciarJogo(&jogo, 2);
                break;

            /* Partida contra o computador (jogador 2 = IA). */
            case 2:
                printf("\nModo contra computador iniciado!\n");
                jogo.contraComputador = 1;
                iniciarJogo(&jogo, 2);
                break;

            /* REQ04 - Mostra as 28 pecas e permite embaralhar
             * (demonstracao, sem iniciar partida). */
            case 3:
                criarPecas(&jogo);
                mostrarPecas(&jogo);

                {
                    char resposta;

                    printf("Deseja embaralhar as pecas? [S/N]: ");

                    /* O espaco antes de %c consome o '\n' deixado
                     * pelo scanf("%d", ...) anterior. */
                    scanf(" %c", &resposta);

                    if (resposta == 'S' || resposta == 's')
                    {
                        embaralharPecas(&jogo);
                        printf("\nPecas embaralhadas:\n");
                        mostrarPecas(&jogo);
                    }
                }

                break;

            /* Recupera a partida salva em "jogo_salvo.dat". */
            case 4:
                if (recuperarJogo(&jogo))
                {
                    jogo.jogoAtivo = 1;
                    jogo.jogoInterrompido = 0;
                    printf("\nJogo recuperado com sucesso!\n");
                    executarPartida(&jogo);
                }
                else
                {
                    printf("\nNao ha jogo salvo para recuperar.\n");
                }

                break;

            /* REQ17 - Mostra as regras do jogo. */
            case 5:
                mostrarRegras();
                break;

            /* Sai do programa (condicao de parada do do-while). */
            case 0:
                printf("\nSaindo do programa...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
                break;
        }

    } while (escolha != 0);
}
