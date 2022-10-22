#ifndef SERVER_H
#define SERVER_H

#define MSG_SIZE 250
#define MAX_CLIENTS 50
#define MAX_P_SIMULT 10
#define DATABASE "Database.txt"

    //Estruc    ras de manejo de usuarios y partidas

typedef struct user{
    int sd;
    char user[MSG_SIZE];
    char pass[MSG_SIZE];
    int logueado; // 1 -> no
    int enespera; // 1 -> no
    int enjuego; // 1-> no
}user;

typedef struct partida{
    int sd1;
    int sd2;
    char A[6][7];
    int turno; // socket con turno
}partida;

    //Bandera para el manejador de señal

int manejador_flag = 0;
void manejador(int signum);

    //Funcionalidades del servidor

void salirCliente(int socket, fd_set * readfds, int * numClientes, user usuarios[], int vectorEspera[], int* numEspera, partida partidas[], int* enjuego);
void enviar_mensaje(int socket_destino, char mensaje[MSG_SIZE]);

void enviar_nuevo_tablero(int sd_enviar, char A[6][7]);
void terminar_partida(partida partidas[], int socket, int* enjuego);
    //Manejo BBDD

int login(char user[], char pass[]);
int registro(char user[], char pass[]);
int existe_username(char user[]);

    //Manejo estructura usuarios
void inicialzar_estructuras(user usuarios[], partida partidas[], int vectorEspera[]);
void inicialzar_usuario(user usuarios[], int numClientes);
int find_pv(user usuarios[MAX_CLIENTS], int sd_buscado);
int find_pv_partida(partida partidas[], int sd_buscado);
    //Manejo estuctuas
void sacar_le(int vectorEspera[], int borrar, int* numEspera);
void borrar_partida(partida partidas[], int socket, int* enjuego,user usuarios[], int avisar);

#endif