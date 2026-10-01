/*
 * main.c
 * Ponto de entrada: inicializa o gerador de numeros aleatorios e
 * entrega o controle para iniciarSistema() (controller.c).
 *
 * Nomes: Nicholas Papiani, Henrique Azevedo, Rafael Athayde, Pedro Nunes
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "controller.h"

int main(void)
{
    srand((unsigned int)time(NULL));

    iniciarSistema();

    return 0;
}
