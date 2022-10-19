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

typedef struct partida{
    int sd1;
    int sd2;
    char A[6][7];
    int enjuego; // 0 -> no
}partida;

user usuarios[MAX_CLIENTS];
partida partidas[MAX_P_SIMULT];

/*
 * El servidor ofrece el servicio de un juego 4 en raya
 */

//void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);
int login(char user[], char pass[]);
int registro(char user[], char pass[]);
int existe_username(char user[]);


int main ( )
{
  
	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int sd, new_sd;//socket descriptor
	struct sockaddr_in sockname, from;
	char buffer[MSG_SIZE];
	socklen_t from_len;
    fd_set readfds, auxfds;
   	int salida;
   	int arrayClientes[MAX_CLIENTS];
    int numClientes = 0;
   	//contadores
    int i,j,k;
	int recibidos;
   	char identificador[MSG_SIZE];
    int on, ret;

    
	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    		exit (1);	
	}
    
    	// Activaremos una propiedad del socket para permitir· que otros
    	// sockets puedan reutilizar cualquier puerto al que nos enlacemos.
    	// Esto permite· en protocolos como el TCP, poder ejecutar un
    	// mismo programa varias veces seguidas y enlazarlo siempre al
        // mismo puerto. De lo contrario habrÌa que esperar a que el puerto
    	// quedase disponible (TIME_WAIT en el caso de TCP).

    	on = 1;
    	ret = setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));


	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2060);
	sockname.sin_addr.s_addr =  INADDR_ANY;

    // Ver si se asocia puerto e ip
	if (bind (sd, (struct sockaddr *) &sockname, sizeof (sockname)) == -1)
	{
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
    
	//Inicializar los conjuntos fd_set
    	FD_ZERO(&readfds);
    	FD_ZERO(&auxfds);
    	FD_SET(sd,&readfds);
    	FD_SET(0,&readfds);
   	
    //Capturamos la señal SIGINT (Ctrl+c)
    	//signal(SIGINT,manejador);
    
	/*-----------------------------------------------------------------------
		El servidor acepta una petición
	------------------------------------------------------------------------ */
		while(1){
            
            //Esperamos recibir mensajes de los clientes 
            // (nuevas conexiones o mensajes de los clientes ya conectados)
            
            auxfds = readfds;  // xk el select se lo carga
            
            salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
            
            if(salida > 0){
                
                
                for(i=0; i<FD_SETSIZE; i++){ 
                // Mas eficiente meter variable para ver que no sea mayor que el maximo que 
                // tengo mas 1 (no comprobar que es mayor que el maximo --> ver si vale nota hacerlo xd)
                    
                    //Buscamos el socket por el que se ha establecido la comunicación
                    if(FD_ISSET(i, &auxfds)) {
                        
                        if( i == sd ){
                            
                            if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1){
                                perror("Error aceptando peticiones");
                            }
                            else
                            {
                                if(numClientes < MAX_CLIENTS){
                                    arrayClientes[numClientes] = new_sd; //A quien hay que mandar
                                    numClientes++;
                                    FD_SET(new_sd,&readfds);
                                
                                    strcpy(buffer, "+0k. Usuario conectado\n");
                                
                                    send(new_sd,buffer,sizeof(buffer),0);
                                
                                    for(j=0; j<(numClientes-1);j++){
                                    
                                        bzero(buffer,sizeof(buffer));
                                        sprintf(buffer, "Nuevo Cliente conectado: %d\n",new_sd);
                                        //Se manda a cada cliente conectado (array)
                                        send(arrayClientes[j],buffer,sizeof(buffer),0);
                                    }
                                }
                                else
                                {   //Si hay muchos a la puta calle
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,"Demasiados clientes conectados\n");
                                    send(new_sd,buffer,sizeof(buffer),0);
                                    close(new_sd);
                                }
                                
                            }
                            
                            
                        }
                        else if (i == 0){ // Se ha escrito algo en la terminal del servidor
                                //Por ahora solo se reconoce salir, pero puede haber más como saber quien hay
                                // o cuanto llevan conectados
        
                            //Se ha introducido información de teclado
                            bzero(buffer, sizeof(buffer)); //Limpiar el buffer??¿?¿?¿?¿
                            fgets(buffer, sizeof(buffer),stdin);
                            
                            //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
                            if(strcmp(buffer,"SALIR\n") == 0){
                             
                                for (j = 0; j < numClientes; j++){
						   bzero(buffer, sizeof(buffer));
						   strcpy(buffer,"Desconexión servidor\n"); 
                                    send(arrayClientes[j],buffer , sizeof(buffer),0);
                                    close(arrayClientes[j]);
                                    FD_CLR(arrayClientes[j],&readfds);
                                }
                                    close(sd);
                                    exit(-1);
                                
                                
                            }
                            //Mensajes que se quieran mandar a los clientes (implementar)
                            
                        } 
                        else{ //Esto ya no es ningun caso especial como los otros
                            //Recibir lo que manda cualquier cliente
                            bzero(buffer,sizeof(buffer));
                            
                            recibidos = recv(i,buffer,sizeof(buffer),0);
                            
                            if(recibidos > 0){
                                if(strcmp(buffer,"SALIR\n") == 0){  // Cliente qiuere salir
                                    
                                    salirCliente(i,&readfds,&numClientes,arrayClientes);
                                    
                                }
                                else if(strcmp(strtok(buffer, " "),"USUARIO") == 0){ // Cliente manda USUARIO
                                    char * user_tmp = strtok(NULL, " ");
                                    //quitarmos el \n
                                    user_tmp[strlen(user_tmp) - 1] = '\0';
                                    printf("<%i>: %s %s\n",i, buffer, user_tmp);
                                    
                                    if(existe_username(user_tmp) == 0){
                                        //Devolver ok usuaruio correcto
                                        bzero(buffer,sizeof(buffer));
                                        sprintf(buffer, "+Ok. Usuario correcto");
                                        printf("<%i>: %s\n",i, buffer);
                                        send(i,buffer,sizeof(buffer),0);
                                    }
                                    else{
                                        bzero(buffer,sizeof(buffer));
                                        sprintf(buffer, "-Err. Usuario incorrecto");
                                        printf("<%i>: %s\n",i, buffer);
                                        send(i,buffer,sizeof(buffer),0);
                                    }

                                }
                                else{
                                    
                                    sprintf(identificador,"<%d>: %s",i,buffer);
                                    bzero(buffer,sizeof(buffer));

                                    strcpy(buffer,identificador);

                                    printf("%s\n", buffer);

                                    for(j=0; j<numClientes; j++)
                                        if(arrayClientes[j] != i)//Al que manda el mensaje no se lee envia salir xd
                                            send(arrayClientes[j],buffer,sizeof(buffer),0);

                                    
                                }


                                
                            }
                            //Si el cliente introdujo ctrl+c
                            if(recibidos == 0) // Si el numero de bits recibidos es 0 es xk el ciente ha salido
                                // de forma abructa
                            {
                                printf("El socket %d, ha introducido ctrl+c\n", i);
                                //Eliminar ese socket
                                salirCliente(i,&readfds,&numClientes,arrayClientes);
                            }
                        }
                    }
                }
            }
		}

	close(sd);
	return 0;
	
}

void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]){
  
    char buffer[250];
    int j;
    
    close(socket);
    FD_CLR(socket,readfds);
    
    //Re-estructurar el array de clientes
    for (j = 0; j < (*numClientes) - 1; j++)
        if (arrayClientes[j] == socket)
            break;
    for (; j < (*numClientes) - 1; j++)
        (arrayClientes[j] = arrayClientes[j+1]);
    
    (*numClientes)--;
    
    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"Desconexión del cliente: %d\n",socket);
    
    for(j=0; j<(*numClientes); j++)
        if(arrayClientes[j] != socket)
            send(arrayClientes[j],buffer,sizeof(buffer),0);


}

/**

void manejador (int signum){

    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);
    
    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor

    //seria como salir cliente o algo  así
}

*/

int login(char user[], char pass[]){

    //Devuelve 0 si el login es correcto y 1 incorrecto

    FILE* f;
    char leido[MSG_SIZE];
    char leido_usr[MSG_SIZE];
    char leido_pass[MSG_SIZE];

    if((f = fopen(DATABASE, "r"))==NULL){

        printf("\nError, no logionse pudo abrir fichero <%s>", DATABASE);
        exit(-1);

    }

    while (fgets(leido, MSG_SIZE, f) != NULL){ //se va leyendo el fichero

        strcpy(leido_usr, strtok(leido, ";"));
        strcpy(leido_pass, strtok(NULL, ";"));
        
        leido_pass[strlen(leido_pass) - 1] = '\0';

        printf("Leido <%s> y <%s>\n", leido_usr, leido_pass);
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

    fprintf(f, "%s;%s\n", user, pass);
    
    //Cierre del flujo/fichero
    fclose(f);
    return(0);

}


int existe_username(char user[]){

    //Devuelve 0 si exitste el usuario y 1 si no

    FILE* f;
    char leido[MSG_SIZE];
    char leido_usr[MSG_SIZE];
    char leido_pass[MSG_SIZE];

    if((f = fopen(DATABASE, "r"))==NULL){

        printf("\nErrexisteor, no se pudo abrir fichero <%s>", DATABASE);
        exit(-1);

    }

    while (fgets(leido, MSG_SIZE, f) != NULL){ //se va leyendo el fichero

        strcpy(leido_usr,strtok(leido, ";"));

        if (strcmp(leido_usr, user) == 0){ //si el user existe

            return 0;

        }
    }
    fclose(f); /* se cierra el fichero */
    //No se ha encontrado
    return 1;
} 