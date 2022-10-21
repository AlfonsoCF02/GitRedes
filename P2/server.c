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

#include "listas.h"
#include "juego.h"
#include "server.h"

/*
 * El servidor ofrece el servicio de un juego 4 en raya
 */

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
   	//int arrayClientes[MAX_CLIENTS];
    int numClientes = 0;
   	//contadores
    int i,j,k;
	int recibidos;
   	char identificador[MSG_SIZE];
    int on, ret;

    int pv;

    //Vectores de usuarios y partidas
    user usuarios[MAX_CLIENTS];
    partida partidas[MAX_P_SIMULT];

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
    	signal(SIGINT,manejador);
    
	/*-----------------------------------------------------------------------
		El servidor acepta una petición
	------------------------------------------------------------------------ */
		while(1){
            
            //Comprobamos que no se haya pulsado ctrl+c en el server
            if(manejador_flag == 1){
                             
                for (j = 0; j < numClientes; j++){
                    bzero(buffer, sizeof(buffer));
                    strcpy(buffer,"Desconexión servidor\n"); 
                    send(usuarios[j].sd,buffer , sizeof(buffer),0);
                    close(usuarios[j].sd);
                    FD_CLR(usuarios[j].sd,&readfds);
                }

                close(sd);
                exit(-1);              
                                
            }

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

                                    usuarios[numClientes].sd = new_sd; //A quien hay que mandar

                                    //No se puede en la estructura, quitamos valores basura
                                    //Por si acaso alguno es 0
                                    inicialzar_usuario(usuarios, numClientes);

                                    numClientes++;
                                    FD_SET(new_sd,&readfds);
                                
                                    strcpy(buffer, "+0k. Usuario conectado\n");
                                
                                    send(new_sd,buffer,sizeof(buffer),0);

                                    printf("<%i>: Conectado\n",new_sd);
                                
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
                            bzero(buffer, sizeof(buffer)); //Limpiar el buffer
                            fgets(buffer, sizeof(buffer),stdin);
                            
                            //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
                            if(strcmp(buffer,"SALIR\n") == 0){
                             
                                for (j = 0; j < numClientes; j++){
						            bzero(buffer, sizeof(buffer));
						            strcpy(buffer,"Desconexión servidor\n"); 
                                    send(usuarios[j].sd,buffer , sizeof(buffer),0);
                                    close(usuarios[j].sd);
                                    FD_CLR(usuarios[j].sd,&readfds);
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

                                //Vemos en que posicion del vector esta el socket
                                pv = find_pv(usuarios, i);
                                if(pv == -1){
                                    printf("<%i>: Error, no encontrado en el vector\n",i);
                                }

                                //Quitamos el \0 del mensaje recibido
                                buffer[strlen(buffer) - 1] = '\0';

                                //Mostramos el mensaje recibido
                                printf("<%i>: %s\n",i, buffer);

                                //Vemos la orden recibida
                                    //si comparamos usando strtok en cada if se jode
                                char orden[MSG_SIZE];
                                sprintf(orden, strtok(buffer, " "));

                                //Procesamos la orden recibida

                                if(strcmp(buffer,"SALIR") == 0){  // Cliente qiuere salir
                                    
                                    salirCliente(i,&readfds,&numClientes,usuarios);
                                    
                                }
                                else if(strcmp(orden,"USUARIO") == 0){ // Cliente manda USUARIO
                                    
                                    if(usuarios[pv].logueado != 0){  //Si no esta logueado ya
                                    
                                        //Almacenamos el username
                                        sprintf(usuarios[pv].user, strtok(NULL, " "));
                                    
                                        if(existe_username(usuarios[pv].user) == 0){
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
                                        bzero(buffer,sizeof(buffer));
                                        sprintf(buffer, "-Err. Usuario ya logueado");
                                        printf("<%i>: %s\n",i, buffer);
                                        send(i,buffer,sizeof(buffer),0);
                                    }                                    

                                }
                                else if(strcmp(orden,"PASSWORD") == 0){ // Cliente manda PASSWORD
                                    
                                    if(usuarios[pv].logueado != 0){  //Si no esta logueado ya
                                    
                                        //Almacenamos el pass del usuario
                                        sprintf(usuarios[pv].pass, strtok(NULL, " "));
                                        if(login(usuarios[pv].user, usuarios[pv].pass) == 0){
                                            bzero(buffer,sizeof(buffer));
                                            sprintf(buffer, "+Ok. Usuario validado");
                                            printf("<%i>: %s\n",i, buffer);
                                            send(i,buffer,sizeof(buffer),0);

                                            usuarios[pv].logueado = 0;

                                        }
                                        else{
                                            bzero(buffer,sizeof(buffer));
                                            sprintf(buffer, "-Err. Error en la validación");
                                            printf("<%i>: %s\n",i, buffer);
                                            send(i,buffer,sizeof(buffer),0);
                                        }
                                    }
                                    else{
                                        bzero(buffer,sizeof(buffer));
                                        sprintf(buffer, "-Err. Usuario ya logueado");
                                        printf("<%i>: %s\n",i, buffer);
                                        send(i,buffer,sizeof(buffer),0);
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
                                            if(registro(usuarios[pv].user, usuarios[pv].pass) == 0){
                                                //Devolver ok usuaruio correcto
                                                bzero(buffer,sizeof(buffer));
                                                sprintf(buffer, "+Ok. Usuario registrado");
                                                printf("<%i>: %s\n",i, buffer);
                                                send(i,buffer,sizeof(buffer),0);
                                            }
                                            else{
                                                
                                                bzero(buffer,sizeof(buffer));
                                                sprintf(buffer, "-Err. El nombre de usuario ya existe");
                                                printf("<%i>: %s\n",i, buffer);
                                                send(i,buffer,sizeof(buffer),0);

                                            }
                                        }
                                        else{
                                            bzero(buffer,sizeof(buffer));
                                            sprintf(buffer, "-Err. El formato incorrecto (-p)");
                                            printf("<%i>: %s\n",i, buffer);
                                            send(i,buffer,sizeof(buffer),0);
                                        }
                                    }
                                    else{
                                        bzero(buffer,sizeof(buffer));
                                        sprintf(buffer, "-Err. El formato incorrecto (-u)");
                                        printf("<%i>: %s\n",i, buffer);
                                        send(i,buffer,sizeof(buffer),0);
                                    }

                                }
                                else if(strcmp(orden,"COLOCAR-FICHA") == 0){ 
                                    
                                    //Almacenamos la posicion recibida
                                    sprintf(buffer, strtok(NULL, " "));

                                    //Comprobamos si la cadena es un numero y convertirlo a int
                                    char *endptr;
                                    int pos_rec = (int) strtol(buffer, &endptr, 10);

                                    if (((*buffer) != '\0') && ((*endptr) == '\0')) {
                                        // strtol tiene éxito, la cadena contiene un número
                                        // y lo almacena convertido a int en pos_rec

                                        /*  FUNCIONAMIENTO strol
                                        long strtol(const char *nptr, char **endptr, int base);
                                        Traducción: Si endptr no es NULL, strtol() almacena la dirección del primer carácter 
                                        inválido en *endptr. Si no aparece ningún dígito, strtol() almacena el valor original 
                                        de nptr en *endptr (y devuelve 0). En particular, si *nptr no es '\0' pero **endptr es '\0' 
                                        cuando la función retorna, la cadena al completo es válida.
                                        */
                                        
                                        //Implemetar logica para colocar la ficha
                                            
                                            //Cuidado controlar que si no se puede colocar
                                            //en la posicion seleccionada retornar error 
                                            //del else  

                                    }
                                    else{
                                        bzero(buffer,sizeof(buffer));
                                        sprintf(buffer, "-Err. Posicion no valida");
                                        printf("<%i>: %s\n",i, buffer);
                                        send(i,buffer,sizeof(buffer),0);  
                                    }

                                    


                                }
                                else if(strcmp(orden,"INICIAR-PARTIDA") == 0){ 
                                    
                                    //Implemetar logica para inicar partida


                                }
                                else{
                                    
                                    //Cualquier otro mensaje genera error

                                    bzero(buffer,sizeof(buffer));
                                    sprintf(buffer, "-Err. Orden no valida");
                                    printf("<%i>: %s\n",i, buffer);
                                    send(i,buffer,sizeof(buffer),0);

                                }


                                
                            }
                            //Si el cliente introdujo ctrl+c
                            if(recibidos == 0) // Si el numero de bits recibidos es 0 es xk el ciente ha salido
                                // de forma abructa
                            {
                                //Eliminar ese socket
                                salirCliente(i,&readfds,&numClientes,usuarios);
                            }
                        }
                    }
                }
            }
		}

	close(sd);
	return 0;
	
}

void salirCliente(int socket, fd_set * readfds, int * numClientes, user usuarios[]){

    char buffer[250];
    int j;

/*      PARA DEPURAR --- BORRAR ANTES DE ENTREGAR

       //Imprimir antes de borrar
       printf("ANTES DE SALIR \n");
    for (j = 0; j < *numClientes; j++){
        printf("Pos: %d, SD: %d, User: %s, Pass: %s, logueado: %d, espera: %d, turno: %d\n",
        j,usuarios[j].sd,usuarios[j].user,usuarios[j].pass,
        usuarios[j].logueado,usuarios[j].enespera,usuarios[j].turno);
    }

*/

    close(socket);
    FD_CLR(socket,readfds);
    
    //Re-estructurar el array de clientes
    for (j = 0; j < (*numClientes) - 1; j++)
        if (usuarios[j].sd == socket)
            break;
    for (; j < (*numClientes) - 1; j++)
        (usuarios[j] = usuarios[j+1]);

    //Al moverlos todos 1 posicion adelante el ultimo hay que borrarlo
    inicialzar_usuario(usuarios, *numClientes);

    (*numClientes)--;

    printf("<%i>: Desconectado\n",socket);

/*      PARA DEPURAR --- BORRAR ANTES DE ENTREGAR

    //Imprimir antes de borrar
    printf("\nDESPUES DE SALIR ");
    for (j = 0; j < *numClientes; j++){
        printf("Pos: %d, SD: %d, User: %s, Pass: %s, logueado: %d, espera: %d, turno: %d\n",
        j,usuarios[j].sd,usuarios[j].user,usuarios[j].pass,
        usuarios[j].logueado,usuarios[j].enespera,usuarios[j].turno);
    }

*/

}

void manejador (int signum){

    printf("\nSe ha recibido la señal SIGINT\n");
    printf("APAGANDO EL SISTEMA\n");
    signal(SIGINT,manejador);
    
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

    //Si no existe el fichero lo creamos
    if((f = fopen(DATABASE, "a")) == NULL)  
    {
        printf("\nErro registro, no se pudo abrir fichero <%s>", DATABASE);
        exit(-1);
    }
    fclose(f);

    //Lo abrimos para lectura (si no existe peta, mejor leer y que este vacio)
    if((f = fopen(DATABASE, "r"))==NULL){

        printf("\nError, no logionse pudo abrir fichero <%s>", DATABASE);
        exit(-1);

    }

    while (fgets(leido, MSG_SIZE, f) != NULL){ //se va leyendo el fichero

        strcpy(leido_usr, strtok(leido, ";"));
        strcpy(leido_pass, strtok(NULL, ";"));
        
        leido_pass[strlen(leido_pass) - 1] = '\0';

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
        printf("\nErro registro, no se pudo abrir fichero <%s>", DATABASE);
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

    //Si no existe el fichero lo creamos
    if((f = fopen(DATABASE, "a")) == NULL)  
    {
        printf("\nErro registro, no se pudo abrir fichero <%s>", DATABASE);
        exit(-1);
    }
    fclose(f);

    //Lo abrimos para lectura (si no existe peta, mejor leer y que este vacio)
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

void inicialzar_usuario(user usuarios[], int numClientes){
    sprintf(usuarios[numClientes].user, "");
    sprintf(usuarios[numClientes].pass, "");
    usuarios[numClientes].logueado = 1;
    usuarios[numClientes].enespera = 1;
    usuarios[numClientes].turno = -1;
}

int find_pv(user usuarios[MAX_CLIENTS], int sd_buscado){

    for(int i = 0; i<MAX_CLIENTS; i++){
        if(usuarios[i].sd == sd_buscado){
            return i;
        }
    }

    return -1;

}