#ifndef SERVER_H
#define SERVER_H

#define MSG_SIZE 250
#define MAX_CLIENTS 30
#define MAX_P_SIMULT 10
#define DATABASE "Database.txt"

    //Estrctura de usuario

typedef struct user{
    int sd; //Socket del usuario
    char user[MSG_SIZE]; //Username del usuario
    char pass[MSG_SIZE]; //Passwor del usuario
    int logueado; // 1 -> no esta logueado
    int enespera; // 1 -> no esta en espera 
    int enjuego; // 1-> si esta en juego
}user;

    //Estrctura de partida

typedef struct partida{
    int sd1; //Socket del jugador 1
    int sd2; //Socket del jugador 2
    char A[6][7]; //Tablero de juego
    int turno; //Valor de sd1 o sd2 al que le corresponde el turno
}partida;

    //Bandera y cabecera para el manejador de señal

int manejador_flag = 0;
void manejador(int signum);

    //Funcionalidades del servidor

        //Manejo de clientes
void salirCliente(int socket, fd_set * readfds, int * numClientes, user usuarios[], int vectorEspera[], int* numEspera, partida partidas[], int* enjuego);
void enviar_mensaje(int socket_destino, char mensaje[MSG_SIZE]);

        //Manejo de partidas
void borrar_partida(partida partidas[], int socket, int* enjuego,user usuarios[], int avisar);
int find_pv_partida(partida partidas[], int sd_buscado);
void generar_msg(char msg[MSG_SIZE], char A[6][7]);

        //Manejo BBDD
int login(char user[], char pass[]);
int registro(char user[], char pass[]);
int existe_username(char user[]);

    //Manejo estructura usuarios
void inicialzar_estructuras(partida partidas[], int vectorEspera[]);
void inicialzar_usuario(user usuarios[], int numClientes);
int find_pv(user usuarios[MAX_CLIENTS], int sd_buscado);

    //Manejo vector lista de espera (lista FIFO)
void sacar_le(int vectorEspera[], int borrar, int* numEspera);

#endif