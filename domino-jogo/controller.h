/*
 * controller.h
 * Fluxo do programa: menu principal, turnos, partida e
 * persistencia (salvar/carregar) em arquivo.
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "jogo.h"

/* REQNF05 - Ponto de entrada: inicializa o Jogo e roda o menu
 * principal em loop ate a opcao "Sair". */
void iniciarSistema(void);

/* Prepara uma nova partida e chama executarPartida(). */
void iniciarJogo(
    Jogo *jogo,
    int numeroJogadores
);

/* Laco principal da partida: joga turno a turno ate acabar, entao
 * mostra o resultado. */
void executarPartida(
    Jogo *jogo
);

/* Executa o turno da IA (jogador 2 no modo contra computador). */
void jogarTurnoComputador(Jogo *jogo);

/* Salva o Jogo em "jogo_salvo.dat". Retorna 0 em caso de erro. */
int salvarJogo(const Jogo *jogo);

/* Carrega o Jogo de "jogo_salvo.dat". Retorna 0 se nao existir ou
 * der erro de leitura. */
int recuperarJogo(Jogo *jogo);

#endif
