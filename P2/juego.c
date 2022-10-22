#include <stdio.h>
#include <stdlib.h>
#define MAX_LINE 6
#define MAX_COLUMN 7

#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))

char A[6][7];
int ganar=0, empate=0;
int max1=5, max2=5, max3=5, max4=5, max5=5, max6=5, max7=5;
int num1=0, num2=0, num3=0, num4=0, num5=0, num6=0, num7=0;

void rellenaMatrizInicial(){
   for(int i=0; i<6; i++){
      for(int j=0; j<7; j++){
         A[i][j]='-';
      }
   }
}

void comprobarEmpate(){
   if((max1==-1)&&(max2==-1)&&(max3==-1)&&(max4==-1)&&(max5==-1)&&(max6==-1)&&(max7==-1)){
      printf("Hay un empate\n");
      empate=1;
   }
}

void comprobarVictoriaX(int pos1, int pos2){

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
            printf("Jugador 1 ha ganado\n");
            exit(-1);
         }
      }

//-------------VERTICAL------------------

count=0;
   for(int i=0; i<MAX_LINE; i++){
         if(A[i][pos2]=='X'){
            count++;
         }
         if(A[i][pos2]=='O'||A[i][pos2]=='-'){
            count=0;
         }
         if(count==4){
            ganar=1;
            printf("Jugador 1 ha ganado\n");
            exit(-1);
         }
      }

//-----------DIAGONAL(IZQDA-DCHA)-----------

int x1=pos1;
int y1=pos2;
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
            exit(-1);
         }
   }


//---------DIAGONAL(DCHA-IZQDA)---------------

int x2=pos1;
int y2=pos2;
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
            printf("Jugador 1 ha ganado\n");
            exit(-1);
         }
   }

}

void comprobarVictoriaO(int pos1, int pos2){

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
            printf("Jugador 2 ha ganado\n");
            exit(-1);
         }
      }

//-------------VERTICAL------------------

int count2=0;
   for(int i=0; i<MAX_LINE; i++){
         if(A[i][pos2]=='O'){
            count2++;
         }
         if(A[i][pos2]=='X'||A[i][pos2]=='-'){
            count2=0;
         }
         if(count2==4){
            ganar=1;
            printf("Jugador 2 ha ganado\n");
            exit(-1);
         }
      }

//-----------DIAGONAL(IZQDA-DCHA)-----------

int x1=pos1;
int y1=pos2;
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
         if(A[i1][j1]=='X'){
            diagonalCount=0;
         }
         if(diagonalCount==4){
            ganar=1;
            printf("Jugador 2 ha ganado\n");
            exit(-1);
         }
   }

//---------DIAGONAL(DCHA-IZQDA)---------------

int x2=pos1;
int y2=pos2;
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
            printf("Jugador 2 ha ganado\n");
            exit(-1);
         }
   }

}

void imprimeMatrizActual(){
   printf("|1|2|3|4|5|6|7|\n");
   for(int i=0; i<6; i++){
      for(int j=0; j<7; j++){
         printf("|");
         printf("%c", A[i][j]);
      }
      printf("|");
      printf("\n");
   }
}

int main(void)
{
int pos1=0, pos2=0, x=0, y=0; //X, Y de la matriz
int jug1=1, jug2=0; //Para los turnos

rellenaMatrizInicial();
imprimeMatrizActual();

while(ganar==0&&empate==0){
   while(jug1==1){
      printf("Jugador1:");
      scanf("%d",&pos1);
      switch(pos1){
         case 1:
               if(max1>=0){
                  A[max1][0]='X';
                  x=max1;
                  max1--;
                  jug1=0;
                  jug2=1;
                  pos2=0;
                  num1++;
               }
               break;
         case 2:
               if(max2>=0){
                  A[max2][1]='X';
                  x=max2;
                  max2--;
                  jug1=0;
                  jug2=1;
                  pos2=1;
                  num2++;
               }
               break;
         case 3:
               if(max3>=0){
                  A[max3][2]='X';
                  x=max3;
                  max3--;
                  jug1=0;
                  jug2=1;
                  pos2=2;
                  num3++;
               }
               break;
         case 4:
               if(max4>=0){
                  A[max4][3]='X';
                  x=max4;
                  max4--;
                  jug1=0;
                  jug2=1;
                  pos2=3;
                  num4++;
               }
               break;
         case 5:
               if(max5>=0){
                  A[max5][4]='X';
                  x=max5;
                  max5--;
                  jug1=0;
                  jug2=1;
                  pos2=4;
                  num5++;
               }
               break;
         case 6:
               if(max6>=0){
                  A[max6][5]='X';
                  x=max6;
                  max6--;
                  jug1=0;
                  jug2=1;
                  pos2=5;
                  num6++;
               }
               break;
         case 7:
               if(max7>=0){
                  A[max7][6]='X';
                  x=max7;
                  max7--;
                  jug1=0;
                  jug2=1;
                  pos2=6;
                  num7++;
               }
               break;
      };
      imprimeMatrizActual();
      comprobarEmpate(x, pos2);
      comprobarVictoriaX(x, pos2);
   }
   while(jug2==1){
      printf("Jugador2:");
      scanf("%d",&pos1);
      switch(pos1){
         case 1:
               if(max1>=0){
                  A[max1][0]='O';
                  x=max1;
                  pos2=0;
                  max1--;
                  jug2=0;
                  jug1=1;
                  num1++;
               }
               break;
         case 2:
               if(max2>=0){
                  A[max2][1]='O';
                  x=max2;
                  pos2=1;
                  max2--;
                  jug2=0;
                  jug1=1;
                  num2++;
               }
               break;
         case 3:
               if(max3>=0){
                  A[max3][2]='O';
                  x=max3;
                  pos2=2;
                  max3--;
                  jug2=0;
                  jug1=1;
                  num3++;
               }
               break;
         case 4:
               if(max4>=0){
                  A[max4][3]='O';
                  pos2=3;
                  x=max4;
                  max4--;
                  jug2=0;
                  jug1=1;
                  num4++;
               }
               break;
         case 5:
               if(max5>=0){
                  A[max5][4]='O';
                  pos2=4;
                  x=max5;
                  max5--;
                  jug2=0;
                  jug1=1;
                  num5++;
               }
               break;
         case 6:
               if(max6>=0){
                  A[max6][5]='O';
                  pos2=5;
                  x=max6;
                  max6--;
                  jug2=0;
                  jug1=1;
                  num6++;
               }
               break;
         case 7:
               if(max7>=0){
                  A[max7][6]='O';
                  x=max7;
                  pos2=6;
                  max7--;
                  jug2=0;
                  jug1=1;
                  num7++;
               }
               break;
      };
      imprimeMatrizActual();
      comprobarEmpate();
      comprobarVictoriaO(x, pos2);
   }
   }
}