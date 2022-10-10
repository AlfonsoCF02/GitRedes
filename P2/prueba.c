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





/*
 * El servidor ofrece el servicio de un juego 4 en raya
 */


int login(char user[], char pass[]);
int registro(char user[], char pass[]);
int existe_username(char user[]);


int main ( )
{
  
    if(existe_username("alfonso") != 0){
        printf("no existe");
    }

    if(registro("alfonso","12") != 0){
        printf("registrado");
    }

    if(login("alfonso","12") != 0){
        printf("logueado");
    }

	return 0;
	
}

int login(char user[], char pass[]){

    //Devuelve 0 si el login es correcto y 1 incorrecto

    FILE* f;
    char leido_usr[MSG_SIZE];
    char leido_pass[MSG_SIZE];

    if((f = fopen(DATABASE, "r"))==NULL){

        printf("\nError, no logionse pudo abrir fichero <%s>", DATABASE);
        exit(-1);

    }

    while (fscanf(f, "%s;%s", leido_usr, leido_pass) != EOF){ //se va leyendo el fichero
        
        if ((strcmp(leido_usr, user) == 0) && (strcmp(leido_pass, pass) == 0)){ //si el user existe

            return 0;

        }
    }
    fclose(f); /* se cierra el fichero */
    //No se ha encontrado
    return 1;

}




int registro(char user[], char pass[]){

    //Devuelve 0 si el registro es correcto o 1 si no lo es

    //Comprobamos si el username ya existe

    if(existe_username(user) == 0){
        return 1;
    }

    // En caso contrario lo registramos

    FILE* f;

    if((f = fopen(DATABASE, "a")) == NULL)  
    {
        printf("\nErroregistorr, no se pudo abrir fichero <%s>", DATABASE);
        exit(-1);
    }

    fprintf(f, "%s;%s", user, pass);
    
    //Cierre del flujo/fichero
    fclose(f);
    return(0);

}


int existe_username(char user[]){

    //Devuelve 0 si exitste el usuario y 1 si no

    FILE* f;
    char leido_usr[MSG_SIZE];
    char leido_pass[MSG_SIZE];

    if((f = fopen(DATABASE, "r"))==NULL){

        printf("\nErrexisteor, no se pudo abrir fichero <%s>", DATABASE);
        exit(-1);

    }

    while (fscanf(f, "%s;%s", leido_usr, leido_pass) != EOF){ //se va leyendo el fichero
        
        if (strcmp(leido_usr, user) == 0){ //si el user existe

            return 0;

        }
    }
    fclose(f); /* se cierra el fichero */
    //No se ha encontrado
    return 1;
} 