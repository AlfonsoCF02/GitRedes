#include <stdio.h>
#include <stdlib.h>
#define MAX_LINE 6
#define MAX_COLUMN 7

#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))

char A[6][7];
int ganar=0, empate=0;
int max1=5, max2=5, max3=5, max4=5, max5=5, max6=5, max7=5;

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
int count=0;
      for(int j=0; j<MAX_COLUMN; j++){
         if(A[pos1][j]=='X'){
            count++;
         }
         if(count==4){
            ganar=1;
            printf("Jugador1 ha ganado\n");
            exit(-1);
         }
      }

count=0;
   for(int i=0; i<MAX_LINE; i++){
         if(A[i][pos2]=='X'){
            count++;
         }
         if(count==4){
            ganar=1;
            printf("Jugador1 ha ganado\n");
            exit(-1);
         }
      }

count=0;
int x=pos1;
int y=pos2;
   while((x>0&&x<5)&&(y>0&&y<6)){
      x++;
      y--;
   }

   for(int i=0; i<6; i++){
      for(int j=7; j<0; j--){
         if((A[i][j]=='X')&&(A[i+1][j-1]=='X')&&(A[i+2][j-2]=='X')&&(A[i+3][j-3]=='X')){
            ganar=1;
            printf("Jugador1 ha ganado\n");
            exit(-1);
         } 
      }
   }

}

void comprobarVictoriaO(int pos1, int pos2){

int count=0;
      for(int j=0; j<MAX_COLUMN; j++){
         if(A[pos1][j]=='O'){
            count++;
         }
         if(count==4){
            ganar=1;
            printf("Jugador2 ha ganado\n");
            exit(-1);
         }
      }

count=0;
   for(int i=0; i<MAX_LINE; i++){
         if(A[i][pos2]=='O'){
            count++;
         }
         if(count==4){
            ganar=1;
            printf("Jugador2 ha ganado\n");
            exit(-1);
         }
      }

   for(int i=6; i<0; i--){ //diagonal arriba abajo i-d
      for(int j=0; j<7; j++){
         if((A[i][j]=='O')&&(A[i-1][j+1]=='O')&&(A[i-2][j+2]=='O')&&(A[i-3][j+3]=='O')){
            ganar=1;
            printf("Jugador2 ha ganado\n");
            exit(-1);
         } 
      }
   }

   for(int i=0; i<6; i++){
      for(int j=7; j<0; j--){
         if((A[i][j]=='O')&&(A[i+1][j-1]=='O')&&(A[i+2][j-2]=='O')&&(A[i+3][j-3]=='O')){
            ganar=1;
            printf("Jugador2 ha ganado\n");
            exit(-1);
         } 
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
int pos1=0, pos2=0; //X, Y de la matriz
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
                  max1--;
                  jug1=0;
                  jug2=1;
                  pos2=0;
               }
               break;
         case 2:
               if(max2>=0){
                  A[max2][1]='X';
                  max2--;
                  jug1=0;
                  jug2=1;
                  pos2=1;
               }
               break;
         case 3:
               if(max3>=0){
                  A[max3][2]='X';
                  max3--;
                  jug1=0;
                  jug2=1;
                  pos2=2;
               }
               break;
         case 4:
               if(max4>=0){
                  A[max4][3]='X';
                  max4--;
                  jug1=0;
                  jug2=1;
                  pos2=3;
               }
               break;
         case 5:
               if(max5>=0){
                  A[max5][4]='X';
                  max5--;
                  jug1=0;
                  jug2=1;
                  pos2=4;
               }
               break;
         case 6:
               if(max6>=0){
                  A[max6][5]='X';
                  max6--;
                  jug1=0;
                  jug2=1;
                  pos2=5;
               }
               break;
         case 7:
               if(max7>=0){
                  A[max7][6]='X';
                  max7--;
                  jug1=0;
                  jug2=1;pos2=6;
               }
               break;
      };
      imprimeMatrizActual();
      comprobarEmpate(pos1, pos2);
      comprobarVictoriaX(pos1, pos2);
   }
   while(jug2==1){
      printf("Jugador2:");
      scanf("%d",&pos1);
      switch(pos1){
         case 1:
               if(max1>=0){
                  A[max1][0]='O';
                  max1--;
                  jug2=0;
                  jug1=1;
               }
               break;
         case 2:
               if(max2>=0){
                  A[max2][1]='O';
                  max2--;
                  jug2=0;
                  jug1=1;
               }
               break;
         case 3:
               if(max3>=0){
                  A[max3][2]='O';
                  max3--;
                  jug2=0;
                  jug1=1;
               }
               break;
         case 4:
               if(max4>=0){
                  A[max4][3]='O';
                  max4--;
                  jug2=0;
                  jug1=1;
               }
               break;
         case 5:
               if(max5>=0){
                  A[max5][4]='O';
                  max5--;
                  jug2=0;
                  jug1=1;
               }
               break;
         case 6:
               if(max6>=0){
                  A[max6][5]='O';
                  max6--;
                  jug2=0;
                  jug1=1;
               }
               break;
         case 7:
               if(max7>=0){
                  A[max7][6]='O';
                  max7--;
                  jug2=0;
                  jug1=1;
               }
               break;
      };
      imprimeMatrizActual();
      comprobarEmpate();
      comprobarVictoriaO(pos1, pos2);
   }
   }
}