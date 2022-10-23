#ifndef JUEGO_H
#define JUEGO_H

#define MAX_LINE 6
#define MAX_COLUMN 7

#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))

void rellenaMatrizInicial(char A[6][7]);
void imprimeMatrizActual(char A[6][7]);
int colocarFicha(int turno, char A[6][7], int sd1, int sd2, int j);
int comprobarVictoria(int turno, char A[6][7], int sd1, int sd2, int pos2);
int comprobarEmpate(char A[6][7]);

//int main(void); main de prueba para el juego

#endif
