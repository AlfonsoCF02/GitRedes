#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include<signal.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>


#define MSG_SIZE 250
#define MAX_CLIENTS 50
#define MAX_P_SIMULT 10
#define DATABASE "Database.txt"

typedef struct user{
    int sd;
    char user[MSG_SIZE];
    char pass[MSG_SIZE];
    int logeado; // 0 si - 1 no
    int enespera; // 0 -> no
    int turno; // 0 -> no
}user;



/*
 * El servidor ofrece el servicio de un juego 4 en raya
 */


int login(char user[], char pass[]);
int registro(char user[], char pass[]);
int existe_username(char user[]);
int find_user(user usuarios[MAX_CLIENTS], int sd_buscado);

int main ( )
{
  
    user usuarios[MAX_CLIENTS];

   usuarios[3].sd = 3;
    printf("%d\n", usuarios[3].sd );

    if(find_user(usuarios, 3)==1){
    printf("Encontrado\n");
    }

	return 0;
	
}

int find_user(user usuarios[MAX_CLIENTS], int sd_buscado){

    int encontrado = 0;

    for(int i = 0; i<MAX_CLIENTS; i++){
        if(usuarios[3].sd == sd_buscado || encontrado == 1){
            encontrado = 1;
            return 1;
        }
    }

    return 0;

}



// MAIN JUEGO


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

}