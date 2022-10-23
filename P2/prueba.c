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