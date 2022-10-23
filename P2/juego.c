#include <stdio.h>
#include <stdlib.h>

#include "juego.h"

void rellenaMatrizInicial(char A[6][7]){
   for(int i=0; i<6; i++){
      for(int j=0; j<7; j++){
         A[i][j]='-';
      }
   }
}

void imprimeMatrizActual(char A[6][7]){
   printf("|1|2|3|4|5|6|7|\n");
   for(int i=0; i<6; i++){
      for(int j=0; j<7; j++){
         printf("|");
         printf("%c", A[i][j]);
      }
      printf("|");
      printf("\n");
   }
   printf("\n");
}


int colocarFicha(int turno, char A[6][7], int sd1, int sd2, int j){

   //0=todo correcto
   //-1=columna llena
   
   //1 O;2 X

if(j<1 || j>7){
   return -1;
}

int countHuecos=5;
   for(int i=0; i<6; i++){
      if(A[i][j-1]!='-'){
         countHuecos--;
      }
   }
   if(turno==sd1){
      if(countHuecos>=0){
         A[countHuecos][j-1]='O';
      }
      else{
         return -1;
      }
   }
   else if(turno==sd2){
      if(countHuecos>=0){
         A[countHuecos][j-1]='X';
      }
      else{
         return -1;
      }
   }

   return 0;
}

int comprobarEmpate(char A[6][7]){
   //0=hay empate
   //-1= no hay empate
   int cuenta=0;
   for(int i=0; i<6; i++){
      for(int j=0; j<7; j++){
         if(A[i][j]!='-'){
            cuenta++;
         }
      }
   }
   if(cuenta==42){
      return 0;
   }
   return -1;
}

int comprobarVictoria(int turno, char A[6][7], int sd1, int sd2, int pos2){

   //Devuelve
   //1=Victoria sd1
   //2=Victoria sd2
   //-1=No hay victoria de sd1
   //-2=No hay victoria de sd2
   //-3=el turno no coincide con el id del socket

   int ganar = 0;

//CALCULAR POSICIÓN X (a partir de posición Y)
   int countHuecos=5;
   for(int i=0; i<6; i++){
      if(A[i][pos2-1]!='-'){
         countHuecos--;
      }
   }
   int pos1=countHuecos+1;
//-------------TURNO DE SD1------------------
   if(turno==sd1){

      //----------HORIZONTAL------------------
      int count=0;
      for(int j=0; j<MAX_COLUMN; j++){
         if(A[pos1][j]=='O'){
            count++;
         }
         if(A[pos1][j]=='X'||A[pos1][j]=='-'){
            count=0;
         }
         if(count==4){
            ganar=1;
            printf("Jugador 1 ha ganado\n");
            return 1;
         }
      }

      //-------------VERTICAL------------------

      count=0;
      for(int i=0; i<MAX_LINE; i++){
         if(A[i][pos2-1]=='O'){
            count++;
         }
         if(A[i][pos2-1]=='X'||A[i][pos2-1]=='-'){
            count=0;
         }
         if(count==4){
            ganar=1;
            printf("Jugador 1 ha ganado\n");
            return 1;
         }
      }

      //-----------DIAGONAL(IZQDA-DCHA)-----------

      int x1=pos1;
      int y1=pos2-1;
      while((y1>0)&&(x1>0)){
         x1--;
         y1--;
      }
      int diagonalCount=0;
      int i1=x1;
      int j1=y1;
      for(i1, j1; (i1>=0&&i1<MAX_LINE)&&(j1<=MAX_COLUMN); i1++, j1++){
         if(A[i1][j1]=='O'){
            diagonalCount++;
         }
         if(A[i1][j1]=='X'||A[i1][j1]=='-'){
            diagonalCount=0;
         }
         if(diagonalCount==4){
            ganar=1;
            printf("Jugador 1 ha ganado\n");
            return 1;
         }
      }


      //---------DIAGONAL(DCHA-IZQDA)---------------

      int x2=pos1;
      int y2=pos2-1;
      while((y2>0)&&(x2>0)){
         x2--;
         y2++;
      }
      int diagonalCount2=0;
      int i2=x2;
      int j2=y2;
      for(i2, j2; (i2>=0&&i2<MAX_LINE)&&(j2<=MAX_COLUMN); i2++, j2--){
         if(A[i2][j2]=='O'){
            diagonalCount2++;
         }
         if(A[i2][j2]=='X'||A[i2][j2]=='-'){
            diagonalCount2=0;
         }
         if(diagonalCount2==4){
            ganar=1;
            printf("Jugador 1 ha ganado\n");
            return 1;
         }
      }
      return -1;
   }
//-------------TURNO DE SD2------------------
   if(turno==sd2){

      //----------HORIZONTAL------------------
      int count=0;
      for(int j=0; j<MAX_COLUMN; j++){
         if(A[pos1][j]=='X'){
            count++;
         }
         if(A[pos1][j]=='O'||A[pos1][j]=='-'){
            count=0;
         }
         if(count==4){
            ganar=1;
            printf("Jugador 2 ha ganado\n");
            return 2;
         }
      }

      //-------------VERTICAL------------------

      count=0;
      for(int i=0; i<MAX_LINE; i++){
         if(A[i][pos2-1]=='X'){
            count++;
         }
         if(A[i][pos2-1]=='O'||A[i][pos2-1]=='-'){
            count=0;
         }
         if(count==4){
            ganar=1;
            printf("Jugador 2 ha ganado\n");
            return 2;
         }
      }

      //-----------DIAGONAL(IZQDA-DCHA)-----------

      int x1=pos1;
      int y1=pos2-1;
      while((y1>0)&&(x1>0)){
         x1--;
         y1--;
      }
      int diagonalCount=0;
      int i1=x1;
      int j1=y1;
      for(i1, j1; (i1>=0&&i1<MAX_LINE)&&(j1<=MAX_COLUMN); i1++, j1++){
         if(A[i1][j1]=='X'){
            diagonalCount++;
         }
         if(A[i1][j1]=='O'||A[i1][j1]=='-'){
            diagonalCount=0;
         }
         if(diagonalCount==4){
            ganar=1;
            printf("Jugador 1 ha ganado\n");
            return 2;
         }
      }


      //---------DIAGONAL(DCHA-IZQDA)---------------

      int x2=pos1;
      int y2=pos2-1;
      while((y2>0)&&(x2>0)){
         x2--;
         y2++;
      }
      int diagonalCount2=0;
      int i2=x2;
      int j2=y2;
      for(i2, j2; (i2>=0&&i2<MAX_LINE)&&(j2<=MAX_COLUMN); i2++, j2--){
         if(A[i2][j2]=='X'){
            diagonalCount2++;
         }
         if(A[i2][j2]=='O'||A[i2][j2]=='-'){
            diagonalCount2=0;
         }
         if(diagonalCount2==4){
            ganar=1;
            printf("Jugador 2 ha ganado\n");
            return 2;
         }
      }
      return -2;
   }
   return -3;
}



/*//Main de prueba

int main(void)
{
   char A[6][7];
   rellenaMatrizInicial(A);

/*-------COMPROBACION colocarFicha()-----------------
   int resultado=0;
   while((colocarFicha(1, A, 1, 2, 1)!=-1)&&(colocarFicha(2, A, 1, 2, 1))!=-1){
      imprimeMatrizActual(A);
   }
*/

/*-------COMPROBACION comprobarEmpate()----------
   rellenaMatriz(A);
   imprimeMatrizActual(A);
   if(comprobarEmpate(A)==0){
      printf("Hay empate\n");
   }
   else{
      printf("No hay empate\n");
   }
*/

/*------COMPROBACION HORIZONTAL sd1 comprobarVictoria()----------
   colocarFicha(1,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,1);
   colocarFicha(2,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,1);
   colocarFicha(1,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,2);
   colocarFicha(2,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,2);
   colocarFicha(1,A,1,2,3);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,3);
   colocarFicha(2,A,1,2,3);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,3);
   colocarFicha(1,A,1,2,4);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,4);
*/

/*------COMPROBACION VERTICAL sd1 comprobarVictoria()----------
   colocarFicha(1,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,1);
   colocarFicha(2,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,2);
   colocarFicha(1,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,1);
   colocarFicha(2,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,2);
   colocarFicha(1,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,1);
   colocarFicha(2,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,2);
   colocarFicha(1,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,1);
   colocarFicha(2,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,2);

*/

/*------COMPROBACION DIAGONAL(IZQDA-DCHA) sd1 comprobarVictoria()----------
   colocarFicha(1,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,1);
   colocarFicha(2,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,2);
   colocarFicha(1,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,1);
   colocarFicha(2,A,1,2,3);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,3);
   colocarFicha(1,A,1,2,3);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,3);
   colocarFicha(2,A,1,2,7);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,7);
   colocarFicha(1,A,1,2,4);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,4);
   colocarFicha(2,A,1,2,4);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,4);
   colocarFicha(1,A,1,2,4);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,4);
   colocarFicha(2,A,1,2,7);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,7);
   colocarFicha(1,A,1,2,3);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,3);
   colocarFicha(2,A,1,2,7);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,7);
   colocarFicha(1,A,1,2,4);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,4);
*/

/*------COMPROBACION DIAGONAL(DCHA-IZQDA) sd1 comprobarVictoria()----------
   colocarFicha(1,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,1);
   colocarFicha(2,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,1);
   colocarFicha(1,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,1);
   colocarFicha(2,A,1,2,7);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,7);
   colocarFicha(1,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,1);
   colocarFicha(1,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,2);
   colocarFicha(2,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,2);
   colocarFicha(1,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,2);
   colocarFicha(2,A,1,2,3);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,3);
   colocarFicha(1,A,1,2,3);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,3);
   colocarFicha(2,A,1,2,7);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,7);
   colocarFicha(1,A,1,2,4);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,4);
*/

/*------COMPROBACION HORIZONTAL sd2 comprobarVictoria()----------
   colocarFicha(2,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,1);
   colocarFicha(1,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,1);
   colocarFicha(2,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,2);
   colocarFicha(1,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,2);
   colocarFicha(2,A,1,2,3);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,3);
   colocarFicha(1,A,1,2,3);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,3);
   colocarFicha(2,A,1,2,4);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,4);
*/

/*------COMPROBACION VERTICAL sd2 comprobarVictoria()----------
   colocarFicha(2,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,1);
   colocarFicha(1,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,2);
   colocarFicha(2,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,1);
   colocarFicha(1,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,2);
   colocarFicha(2,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,1);
   colocarFicha(1,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,2);
   colocarFicha(2,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,1);
*/

/*------COMPROBACION DIAGONAL(IZQDA-DCHA) sd2 comprobarVictoria()----------
   colocarFicha(2,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,1);
   colocarFicha(1,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,2);
   colocarFicha(2,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,2);
   colocarFicha(1,A,1,2,7);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,7);
   colocarFicha(2,A,1,2,3);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,3);
   colocarFicha(1,A,1,2,3);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,3);
   colocarFicha(2,A,1,2,3);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,3);
   colocarFicha(1,A,1,2,4);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,4);
   colocarFicha(2,A,1,2,4);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,4);
   colocarFicha(1,A,1,2,4);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,4);
   colocarFicha(2,A,1,2,4);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,4);

*/

/*------COMPROBACION DIAGONAL(DCHA-IZQDA) sd2 comprobarVictoria()----------
   colocarFicha(2,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,1);
   colocarFicha(1,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,1);
   colocarFicha(2,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,1);
   colocarFicha(1,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,1);
   colocarFicha(2,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,2);
   colocarFicha(1,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,2);
   colocarFicha(2,A,1,2,7);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,7);
   colocarFicha(1,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,2);
   colocarFicha(2,A,1,2,3);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,3);
   colocarFicha(1,A,1,2,3);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,3);
   colocarFicha(2,A,1,2,7);
   imprimeMatrizActual(A);
   comprobarVictoria(2,A,1,2,7);
   colocarFicha(1,A,1,2,1);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,1);
   colocarFicha(1,A,1,2,2);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,2);
   colocarFicha(1,A,1,2,3);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,3);
   colocarFicha(1,A,1,2,4);
   imprimeMatrizActual(A);
   comprobarVictoria(1,A,1,2,4);
*/

//} cierre main

