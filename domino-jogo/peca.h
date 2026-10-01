/*
 * peca.h
 * Estrutura basica de uma peca de domino.
 */

#ifndef PECA_H
#define PECA_H

/* Uma peca de domino: dois lados, cada um de 0 a 6. */
typedef struct {
    int lado1;
    int lado2;
} Peca;

#endif
