#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

#include "server.h"
#include "juego.h"

/*
 * El servidor ofrece el servicio de un juego 4 en raya.
 */

int main ( )
{

    //Se captura la señal SIGINT (Ctrl+c) en el server y se le asocia un manejador
    signal(SIGINT,manejador);

	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int sd, new_sd;
	struct sockaddr_in sockname, from;
	char buffer[MSG_SIZE];
	socklen_t from_len;
    fd_set readfds, auxfds;
   	int salida;

   	//Vectores de usuarios y partidas
    user usuarios[MAX_CLIENTS];
    partida partidas[MAX_P_SIMULT];

    //Vector para implementar una lista de espera FIFO
    int vectorEspera[MAX_CLIENTS];

    //Contadores empleados en el programa
    int i,j,k;
	int recibidos;

    int numClientes = 0;
    int numEspera = 0;
    int enjuego = 0;

    int pv;


    //Inicializamos las estructuas (vectores) utilizadas para evitar
    //Valores basura que puedan interferir en comprobaciones criticas
    //La estructura de usuarios se gestiona de forma individual

    inicialzar_estructuras(partidas, vectorEspera);

	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1){

		perror("No se puede abrir el socket cliente\n");
    	exit (1);

	}
    
    // Activaremos una propiedad del socket para permitir que otros
    // sockets puedan reutilizar cualquier puerto al que nos enlacemos.
    // Esto permite en protocolos como el TCP, poder ejecutar un
    // mismo programa varias veces seguidas y enlazarlo siempre al
    // mismo puerto. De lo contrario habrÌa que esperar a que el puerto
    // quedase disponible (TIME_WAIT en el caso de TCP).

    int on = 1;
    int ret = setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2060);
	sockname.sin_addr.s_addr =  INADDR_ANY;

    // Comprobar si se asocia puerto e ip
	if (bind (sd, (struct sockaddr *) &sockname, sizeof (sockname)) == -1){

		perror("Error en la operación bind");
		exit(1);
	}

   	/*---------------------------------------------------------------------
		Del las peticiones que vamos a aceptar sólo necesitamos el 
		tamaño de su estructura, el resto de información (familia, puerto, 
		ip), nos la proporcionará el método que recibe las peticiones.
   	----------------------------------------------------------------------*/

	from_len = sizeof (from);

	if(listen(sd,1) == -1){
		perror("Error en la operación de listen");
		exit(1);
	}

	//Inicializamos los conjuntos fd_set
    	FD_ZERO(&readfds);
    	FD_ZERO(&auxfds);
    	FD_SET(sd,&readfds);
    	FD_SET(0,&readfds);
   	
	/*-----------------------------------------------------------------------
		El servidor acepta una petición
	------------------------------------------------------------------------ */
		while(1){
            
            //Comprobamos que no se haya pulsado ctrl+c en el server
            if(manejador_flag == 1){

                //Si se ha pulsado se desconecta a los usuarios conectados
                //Se cierra el socket y apaga el servidor

                for (j = 0; j < numClientes; j++){
                    enviar_mensaje(usuarios[j].sd, "Desconexión servidor\n");
                    close(usuarios[j].sd);
                    FD_CLR(usuarios[j].sd,&readfds);
                }

                close(sd);

                printf("APAGADO COMPLETADO\n");

                exit(-1);              
                                
            }

            //Esperamos recibir mensajes de los clientes 
            // (nuevas conexiones o mensajes de los clientes ya conectados)
            
            auxfds = readfds;  // Para evitar perderlo por el select
            
            salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
            
            if(salida > 0){ //Tenemos una peticion
                
                for(i=0; i<FD_SETSIZE; i++){  //Se recorre el set

                    if(FD_ISSET(i, &auxfds)) {  //Mira si el descriptor de socket dado por fd se encuentra en el conjunto especificado por set.
                        
                        if( i == sd ){  //Si el socket que ha establecido la comunicacion es examinado
                            
                            //Se recibe la conexion del cliente
                            if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1){
                                perror("Error aceptando peticiones");
                            }
                            else
                            {
                                //Se comprueba que se puedan aceptar mas clientes
                                if(numClientes < MAX_CLIENTS){

                                    usuarios[numClientes].sd = new_sd; //Se introduce en la lista de usuarios
                                    
                                    //Se inicializan los datos del cliente, por si alguno es 0
                                    //No se puede en la estructura
                                    inicialzar_usuario(usuarios, numClientes);

                                    numClientes++;

                                    FD_SET(new_sd,&readfds);
                                
                                    enviar_mensaje(new_sd, "+0k. Usuario conectado\n");
                                
                                }
                                else{   //Si no se pueden recibir mas conexiones
                                    enviar_mensaje(new_sd, "Demasiados clientes conectados\n");
                                    close(new_sd);
                                }  
                            }  
                        }
                        else if (i == 0){ // Se ha escrito algo en la terminal del servidor

                        //Por ahora solo se reconoce salir, pero puede haber más como saber quien hay o cuanto llevan conectados
        
                            //Se ha introducido información de teclado
                            bzero(buffer, sizeof(buffer)); //Limpiar el buffer
                            fgets(buffer, sizeof(buffer), stdin); //Se guarda lo que se ha introducido en el buffer
                            
                            //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor.

                            if(strcmp(buffer,"SALIR\n") == 0){

                                printf("APAGANDO EL SISTEMA\n");
                                printf("Deconectando usuarios\n");

                                for (j = 0; j < numClientes; j++){
						            enviar_mensaje(usuarios[j].sd, "Desconexión servidor\n");
                                    close(usuarios[j].sd);
                                    FD_CLR(usuarios[j].sd,&readfds);
                                }

                                printf("APAGADO COMPLETADO\n");

                                close(sd);
                                exit(-1);
                                
                                
                            }
                            
                        } 
                        else{ //El cliente ha mandado cualquier mensaje
                            
                            bzero(buffer,sizeof(buffer));   //Se limpia el buffer
                            
                            //Se almacena el mensaje en el buffer y se recupera el tamaño
                            recibidos = recv(i,buffer,sizeof(buffer),0);
                            
                            if(recibidos > 0){  //Si ha escrito algo

                                //Vemos en que posicion del vector usuarios esta el socket
                                pv = find_pv(usuarios, i);
                                if(pv == -1){
                                    printf("<%i>: Error, no encontrado en el vector\n",i);
                                }

                                //Quitamos el \n del mensaje recibido si lo tiene
                                if(buffer[strlen(buffer)-1]=='\n'){
                                    buffer[strlen(buffer)-1] = '\0';
                                }

                                //Mostramos el mensaje recibido
                                printf("<%i>: %s\n",i, buffer);

                                //Vemos la orden recibida para poder procesarlas
                                // Nota: si comparamos usando strtok en cada if ERROR
                                char orden[MSG_SIZE];
                                sprintf(orden, strtok(buffer, " "));

                                //Procesamos la orden recibida

                                if(strcmp(buffer,"SALIR") == 0){  // Cliente qiuere salir

                                    /*
                                        salirCliente() se encarga de realizar todo lo pertinente para terminar
                                        la conexion de un cliente y reflejar los cambios en todas las estructuras
                                        necesarias para garantizar la consistencia del serdivor y el juego.
                                    */
                                    
                                    salirCliente(i,&readfds,&numClientes,usuarios,vectorEspera, &numEspera, partidas, &enjuego);
                                    
                                }
                                else if(strcmp(orden,"USUARIO") == 0){ // Cliente manda USUARIO
                                    
                                    if(usuarios[pv].logueado != 0){  //Si no esta logueado ya
                                    
                                        //Almacenamos el username
                                        sprintf(usuarios[pv].user, strtok(NULL, " "));
                                    
                                        if(existe_username(usuarios[pv].user) == 0){

                                            enviar_mensaje(i, "+Ok. Usuario correcto");
                                            
                                        }
                                        else{
                                            enviar_mensaje(i, "-Err. Usuario incorrecto");
                                        }
                                    }
                                    else{

                                        enviar_mensaje(i, "-Err. Usuario ya logueado");
                                    }                                    

                                }
                                else if(strcmp(orden,"PASSWORD") == 0){ // Cliente manda PASSWORD
                                    
                                    if(usuarios[pv].logueado != 0){  //Si no esta logueado ya
                                    
                                        //Nota: Un usuario logueado se puede volver a loguear en otra terminal
                                        //      Se permite que juegue varias partidas simultaneas.

                                        //Almacenamos el pass del usuario
                                        sprintf(usuarios[pv].pass, strtok(NULL, " "));

                                        if(login(usuarios[pv].user, usuarios[pv].pass) == 0){

                                            enviar_mensaje(i, "+Ok. Usuario validado");

                                            //Se activa la bandera de logueado
                                            usuarios[pv].logueado = 0;

                                        }
                                        else{
                                            enviar_mensaje(i, "-Err. Error en la validación");
                                        }
                                    }
                                    else{
                                        enviar_mensaje(i, "-Err. Usuario ya logueado");
                                    }                   

                                }
                                else if(strcmp(orden,"REGISTRO") == 0){ // Cliente manda REGISTRO

                                    //Comprobamos el formato

                                    char aux[MSG_SIZE];
                                    sprintf(aux, strtok(NULL, " "));
                                    if( strcmp(aux,"-u") == 0 ){
                                        //Almacenamos el user
                                        sprintf(usuarios[pv].user, strtok(NULL, " "));
                                        sprintf(aux, strtok(NULL, " "));
                                        if(strcmp(aux,"-p") == 0){
                                            //Almacenamos la pass
                                            sprintf(usuarios[pv].pass, strtok(NULL, " "));

                                            //Tratamos de registrar al usuario con los datos introducidos
                                            //registro() comprueba que no exista ya en la BD
                                            if(registro(usuarios[pv].user, usuarios[pv].pass) == 0){

                                                enviar_mensaje(i, "+Ok. Usuario registrado");

                                            }
                                            else{
                                                enviar_mensaje(i, "-Err. El nombre de usuario ya existe");
                                            }
                                        }
                                        else{
                                            enviar_mensaje(i, "-Err. El formato incorrecto (-p)");
                                        }
                                    }
                                    else{
                                        enviar_mensaje(i, "-Err. El formato incorrecto (-u)");
                                    }

                                }
                                else if(strcmp(orden,"COLOCAR-FICHA") == 0){ 
                                    
                                    if(usuarios[pv].logueado == 0){ //Si el user esta logueado

                                        if(usuarios[pv].enjuego == 0){  //Tiene que estar jugando una partida

                                            //Localizamos el indice de la partida en el vector
                                            int p = find_pv_partida(partidas, i);

                                            if(partidas[p].turno == i){ //Si es el turno del jugador

                                                //Almacenamos la posicion recibida
                                                sprintf(buffer, strtok(NULL, " "));

                                                //Comprobamos si la cadena es un numero y convertirlo a int
                                                char *endptr;
                                                int pos_rec = (int) strtol(buffer, &endptr, 10);

                                                if (((*buffer) != '\0') && ((*endptr) == '\0')) {
                                                    // Si strtol tiene éxito, la cadena contiene un número
                                                    // y lo almacena convertido a int en pos_rec

                                                    /*  FUNCIONAMIENTO strol
                                                    long strtol(const char *nptr, char **endptr, int base);
                                                    Traducción: Si endptr no es NULL, strtol() almacena la dirección del primer carácter 
                                                    inválido en *endptr. Si no aparece ningún dígito, strtol() almacena el valor original 
                                                    de nptr en *endptr (y devuelve 0). En particular, si *nptr no es '\0' pero **endptr es '\0' 
                                                    cuando la función retorna, la cadena al completo es válida.
                                                    */

                                                    //Se coloca la ficha con las comprobaciones pertinentes

                                                    if(colocarFicha(i, partidas[p].A, partidas[p].sd1, partidas[p].sd2, pos_rec) == 0){
                                                        
                                                        //Se comprueba si hay victoria

                                                        int vflag = comprobarVictoria(i, partidas[p].A, partidas[p].sd1, partidas[p].sd2, pos_rec);
                                                        
                                                        if( vflag == 1 ||  vflag == 2 ){ //Si el jugador ha ganado
                                                            
                                                            //Se envia un mensaje informando de la victoria
                                                            char msg_vict[MSG_SIZE];
                                                            bzero(msg_vict, sizeof(msg_vict));
                                                            sprintf(msg_vict, "+Ok. Jugador <%s> ha ganado la partida", usuarios[pv].user);

                                                            enviar_mensaje(partidas[p].sd1, msg_vict);
                                                            enviar_mensaje(partidas[p].sd2, msg_vict);

                                                            //Se termina la partida
                                                            /*
                                                                Nota: borrar_partida() se encarga de dejar todo consistente
                                                            */
                                                            borrar_partida(partidas, i, &enjuego, usuarios, 0);

                                                        }
                                                        else{ //Si no hay victoria
                                                        
                                                            //Se comprueba si hay empate

                                                            if(comprobarEmpate(partidas[p].A) == 0){

                                                                //Se informa del empate
                                                                enviar_mensaje(partidas[p].sd1, "+Ok. Se ha producido un empate en la partida");
                                                                enviar_mensaje(partidas[p].sd2, "+Ok. Se ha producido un empate en la partida");

                                                                //Se termina la partida
                                                                borrar_partida(partidas, i, &enjuego, usuarios, 0);

                                                            }
                                                            else{ //Si no hay empate

                                                                //Se envia el nuevo tablero
                                                                char msg[MSG_SIZE];
                                                                bzero(msg, sizeof(msg));
                                                                generar_msg(msg, partidas[p].A);

                                                                printf("Mensaje a enviar:\n%s\n", msg);
                                                                
                                                                enviar_mensaje(partidas[p].sd1, msg);
                                                                enviar_mensaje(partidas[p].sd2, msg);
                                                                
                                                                // Se cambia el turno 
                                                                if(partidas[p].turno == partidas[p].sd1){
                                                                    partidas[p].turno = partidas[p].sd2;
                                                                }
                                                                else{
                                                                    partidas[p].turno = partidas[p].sd1;
                                                                }
                                                                
                                                                enviar_mensaje(partidas[p].turno, "+Ok. Turno de partida");

                                                            }
                                                        }
                                                    }
                                                    else{
                                                        enviar_mensaje(i, "-Err. Debe seleccionar otra columna que tenga alguna casilla disponible");
                                                    }
                                                }
                                                else{
                                                    enviar_mensaje(i, "-Err. la posicion debe ser un numero");
                                                }
                                            }
                                            else{
                                                enviar_mensaje(i, "-Err. Debe esperar su turno");
                                            }
                                        }    
                                        else{
                                            enviar_mensaje(i, "-Err. Usted no esta jugando ninguna partida");
                                        }
                                    }
                                    else{
                                        enviar_mensaje(i, "-Err. debe estar logueado para colocar ficha");
                                    }
                                }
                                else if(strcmp(orden,"INICIAR-PARTIDA") == 0){ 
                                    
                                    if(usuarios[pv].logueado == 0){ //Si el user esta logueado

                                        if(enjuego < MAX_P_SIMULT){ //Comprobamos si se puede iniciar otra partida
                                            
                                            if(usuarios[pv].enjuego == 1){ //No esta en una partida ya

                                                if(usuarios[pv].enespera == 1){ //No esta en espera ya

                                                    usuarios[pv].enespera = 0; //Se pone en espera

                                                    vectorEspera[numEspera] = i; //Se añade a listaespera
                                                    numEspera++;
    
                                                    //Si hay dos personas en espera
                                                    if((vectorEspera[0] != -1) && (vectorEspera[1] != -1)){

                                                        //Se crea la partida
                                                        partidas[enjuego].sd1 = vectorEspera[0];
                                                        partidas[enjuego].sd2 = vectorEspera[1];
                                                        partidas[enjuego].turno = vectorEspera[0];
                                                        enjuego++;

                                                        //Activamos bandera de en juego de los jugadores
                                                        usuarios[find_pv(usuarios, vectorEspera[0])].enjuego = 0;
                                                        usuarios[find_pv(usuarios, vectorEspera[1])].enjuego = 0;

                                                        //Desactivamos bandera en espera de los jugadores
                                                        usuarios[find_pv(usuarios, vectorEspera[0])].enespera = 1;
                                                        usuarios[find_pv(usuarios, vectorEspera[1])].enespera = 1;

                                                        //Sacamos a los dos del vector de espera a los  nuevos jugadores
                                                        //(movemos a los demas adelante)
                                                        sacar_le(vectorEspera, vectorEspera[0], &numEspera);
                                                        sacar_le(vectorEspera, vectorEspera[1], &numEspera);

                                                        //Inicializamos la matriz de la partida con -
                                                        rellenaMatrizInicial(partidas[enjuego - 1].A);

                                                        //Se envia a los dos +Ok empieza y el tablero en blanco
                                                        enviar_mensaje(partidas[enjuego - 1].sd1 ,"+Ok. Empieza la partida. -,-,-,-,-,-,-; -,-,-,-,-,-,-; -,-,-,-,-,-,-; -,-,-,-,-,-,-; -,-,-,-,-,-,-;-,-,-,-,-,-,-;");
                                                        enviar_mensaje(partidas[enjuego - 1].sd2 ,"+Ok. Empieza la partida. -,-,-,-,-,-,-; -,-,-,-,-,-,-; -,-,-,-,-,-,-; -,-,-,-,-,-,-; -,-,-,-,-,-,-;-,-,-,-,-,-,-;");

                                                        //Se le envia a quien tiene que empezar el turno
                                                        enviar_mensaje(partidas[enjuego - 1].turno, "Ok. Turno de partida");

                                                    }
                                                    else{
                                                        enviar_mensaje(i, "+Ok. Esperando jugadores");
                                                    }
                                                }
                                                else{
                                                    enviar_mensaje(i, "-Err. Ya esta en espera");
                                                }
                                            }
                                            else{
                                                enviar_mensaje(i, "-Err. Usted ya esta jugando una pertida");
                                            }
                                        }
                                        else{
                                            enviar_mensaje(i, "-Err. Max de partidas alcanzado, intentelo mas tarde");
                                        } 
                                    }
                                    else{
                                        enviar_mensaje(i, "-Err. debe loguearse para buscar partida");
                                    }
                                }
                                else{
                                    
                                    //Cualquier otro tipo de mensaje recibido genera error

                                    enviar_mensaje(i, "-Err. Orden no valida");

                                }


                                
                            }
                            //Si el cliente ha introducido ctrl+c (bits recibidos 0)
                            if(recibidos == 0){
                                //Eliminar ese socket
                                salirCliente(i,&readfds,&numClientes,usuarios,vectorEspera, &numEspera, partidas, &enjuego);
                            }
                        }
                    }
                }
            }
		}

	close(sd);  //Se cierra el socket
	return 0;
	
}

void salirCliente(int socket, fd_set * readfds, int * numClientes, user usuarios[], int vectorEspera[], int* numEspera, partida partidas[], int* enjuego){

    //Se cierra el socket del ciente que quiere salir y se elimina del set
    close(socket);
    FD_CLR(socket,readfds);

    //Se busca indice en el vector de usurios
    int pos_usr = find_pv(usuarios, socket);

        //Si esta en espera se saca de la lista de espera

    if(usuarios[pos_usr].enespera == 0){
        sacar_le(vectorEspera, socket, numEspera);
    }
    
        //Si el usuario está jugando
    
    if(usuarios[pos_usr].enjuego == 0){

        //Si lo ultimo de borrar partida es 1 se avisa al jugador
        // de que el oponente ha abandonado la partida
        int avisar_salida = 1;

        borrar_partida(partidas, socket, enjuego, usuarios, avisar_salida); 

    }

    //Re-estructurar el array de clientes

    for (; pos_usr < (*numClientes) - 1; pos_usr++){
        (usuarios[pos_usr] = usuarios[pos_usr+1]);
    }

    //Al moverlos todos 1 posicion adelante el ultimo hay que borrarlo
    inicialzar_usuario(usuarios, *numClientes);

    (*numClientes)--;

    printf("<%i>: Desconectado\n",socket);

}

void manejador (int signum){

    printf("\nSe ha recibido la señal SIGINT\n");
    printf("APAGANDO EL SISTEMA\n");
    printf("Deconectando usuarios\n");
    
    //Se activa la flag y en el while del main se cierra el server
    //Si no habría que poner muchas variables globales para poder pasarlas
    //al manejador.
    manejador_flag = 1;

}


int login(char user[], char pass[]){

    //Devuelve 0 si el login es correcto y 1 incorrecto

    FILE* f;
    char leido[MSG_SIZE];
    char leido_usr[MSG_SIZE];
    char leido_pass[MSG_SIZE];

    //Si no existe el fichero DATABASE lo creamos
    if((f = fopen(DATABASE, "a")) == NULL)  {

        printf("\nError, no se pudo crear el fichero <%s>", DATABASE);
        exit(-1);
    }
    fclose(f);

    //Lo abrimos para lectura (si no existe da error, por eso
    //se ha creado anteriormente).
    if((f = fopen(DATABASE, "r"))==NULL){

        printf("\nError, no pudo abrir fichero <%s>", DATABASE);
        exit(-1);

    }

    while (fgets(leido, MSG_SIZE, f) != NULL){ //Se va leyendo el fichero

        //Se procesan los datos leido
        strcpy(leido_usr, strtok(leido, ";"));
        strcpy(leido_pass, strtok(NULL, ";"));
        
        //Se quita el \n
        leido_pass[strlen(leido_pass) - 1] = '\0';

        if ((strcmp(leido_usr, user) == 0) && (strcmp(leido_pass, pass) == 0)){ 
        
            return 0; //Se devuelve logueado Ok

        }
    }
    fclose(f); // se cierra el fichero

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

    if((f = fopen(DATABASE, "a")) == NULL){ //Se abre el fichero para añadir
        printf("\nError, no se pudo abrir fichero <%s>", DATABASE);
        exit(-1);
    }

    fprintf(f, "%s;%s\n", user, pass);  //Se escriben los datos
    
    //Se cierra el fichero
    fclose(f);
    return(0);

}


int existe_username(char user[]){

    //Devuelve 0 si exitste el usuario y 1 si no

    FILE* f;
    char leido[MSG_SIZE];
    char leido_usr[MSG_SIZE];
    char leido_pass[MSG_SIZE];

    //Si no existe el fichero lo creamos
    if((f = fopen(DATABASE, "a")) == NULL)  
    {
        printf("\nErro, no se pudo crear el fichero <%s>", DATABASE);
        exit(-1);
    }
    fclose(f);

    //Lo abrimos para lectura (si no existe da error, por eso
    //se ha creado anteriormente).
    if((f = fopen(DATABASE, "r"))==NULL){
        printf("\nError, no se pudo abrir fichero <%s>", DATABASE);
        exit(-1);
    }

    while (fgets(leido, MSG_SIZE, f) != NULL){ //Se va leyendo el fichero

        strcpy(leido_usr,strtok(leido, ";"));

        if (strcmp(leido_usr, user) == 0){ //Si el user existe

            return 0;

        }
    }
    fclose(f); // se cierra el fichero

    //No se ha encontrado
    return 1;
} 

void inicialzar_usuario(user usuarios[], int numClientes){

    //Inicializa un inico usuario a sus valores default

    sprintf(usuarios[numClientes].user, "");
    sprintf(usuarios[numClientes].pass, "");
    usuarios[numClientes].logueado = 1;
    usuarios[numClientes].enespera = 1;
    usuarios[numClientes].enjuego = 1;

}

int find_pv(user usuarios[MAX_CLIENTS], int sd_buscado){

    //Devuelve el indicide de un usuario buscado en el 
    //vector de usuarios

    for(int i = 0; i<MAX_CLIENTS; i++){
        if(usuarios[i].sd == sd_buscado){
            return i;
        }
    }

    return -1;

}

void enviar_mensaje(int socket_destino, char mensaje[MSG_SIZE]){

    //Envia un mensaje a un socket seleccionado y lo muestra por
    //consola en el server

    char buffer[MSG_SIZE];

    bzero(buffer, sizeof(buffer));
    sprintf(buffer, mensaje);
    send(socket_destino, buffer, sizeof(buffer), 0);

    //Mostramos por terminal el mensaje enviado

    //Si tiene \n se la  (para que salga mas limpio en la terminal)
    if(buffer[strlen(buffer)-1]=='\n'){
        buffer[strlen(buffer)-1] = '\0';
    }
    printf("<%i>: %s\n", socket_destino, buffer);

}


void inicialzar_estructuras(partida partidas[], int vectorEspera[]){

    //Inicializa todas las de partidas y vectorEspera

    int z = 0;

    for(z=0 ; z< MAX_CLIENTS; z++){
        partidas[z].sd1 = -1;
        partidas[z].sd2 = -1;
        partidas[z].turno = -1;
        //La matriz ya vemos luego en el de borrar tmb
    }
    
    z = 0;

    for(z=0 ; z< MAX_CLIENTS; z++){
        vectorEspera[z] = -1;
    }

}

void sacar_le(int vectorEspera[], int borrar, int* numEspera){

    //Elimina a un usuario seleccionado de la lista de espera
    //y la deja consistente.

    int j;
    
    //Encontramos el indice del usuario
    for (j = 0; j < *(numEspera) - 1; j++)
        if (vectorEspera[j] == borrar)
            break;
    
    //Lo borramos y reestructuramos
    for (; j < *(numEspera) - 1; j++)
        (vectorEspera[j] = vectorEspera[j+1]);

    //Al moverlos todos 1 posicion adelante el ultimo hay que borrarlo
    vectorEspera[*(numEspera)] = -1;

    (*numEspera)--;

}

void enviar_nuevo_tablero(int sd_enviar, char A[6][7]){

    //IMPLEMENTAR EL PASAR LA MATRIZ


}

void borrar_partida(partida partidas[], int socket, int* enjuego, user usuarios[], int avisar){

    //Elimina una partida y deja todo consistente

    int j;
    
    //Encontrar la partida a terminar
    for (j = 0; j < *(enjuego) - 1; j++)
        if ((partidas[j].sd1 == socket) || (partidas[j].sd2== socket))
            break;

    //Poner los valores de los usuarios disponibles de nuevo
        int user1 = find_pv(usuarios, partidas[j].sd1);
        int user2 = find_pv(usuarios, partidas[j].sd2);
        usuarios[user1].enespera = 1; usuarios[user1].enjuego = 1;
        usuarios[user2].enespera = 1; usuarios[user2].enjuego = 1;

    //Decir al otro que el compañero se ha desconectado
    if(avisar == 1){
        if (partidas[j].sd1 == socket){
            enviar_mensaje(partidas[j].sd2, "+Ok. Tu oponente ha salido de la partida");
        }
        else{
            enviar_mensaje(partidas[j].sd1, "+Ok. Tu oponente ha salido de la partida");
        }
    }

    //Reestructurar el vector de partidas
    for (; j < *(enjuego) - 1; j++)
        (partidas[j] = partidas[j+1]);

    //Al moverlos todos 1 posicion adelante el ultimo hay que borrarlo
    partidas[*(enjuego)].sd1 = -1;
    partidas[*(enjuego)].sd2 = -1;
    partidas[*(enjuego)].turno = -1;
    //poner la matriz a 0

    (*enjuego)--;

}

int find_pv_partida(partida partidas[], int sd_buscado){

    //Encuenta el indice de partida que pertenece al socket seleccionado

    int j;

    for (j = 0; j < MAX_P_SIMULT - 1; j++){
        if ((partidas[j].sd1 == sd_buscado) || (partidas[j].sd2 == sd_buscado)){
            return j;
        }
    }

    return -1;

}

void generar_msg(char msg[MSG_SIZE], char A[6][7]){
    char msg_inicial[]="+Ok. Nuevo tablero. ";
    strcat(msg, msg_inicial);
    int count_msg = strlen(msg_inicial);
    for(int i=0; i<6; i++){
        for(int j=0; j<7; j++){
            msg[count_msg]=A[i][j];
            count_msg++;
            if(j<6){
                msg[count_msg]=',';
                count_msg++;
            }
            else{
                msg[count_msg]=';';
                count_msg++;
                msg[count_msg]=' ';
                count_msg++;
            }
        }
    }
}
