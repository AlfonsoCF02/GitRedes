#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

void imprimeMatrizVacia();
void imprimeMatrizActual(char A[6][7]);
void pasarMensajeAMatriz(char buffer[250], char A[6][7]);

int main ( )
{
  
	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int sd;
	struct sockaddr_in sockname;
	char buffer[250];
	socklen_t len_sockname;
    	fd_set readfds, auxfds;
    	int salida;
    	int fin = 0;
	
    
	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    		exit (1);	
	}

   
    
	/* ------------------------------------------------------------------
		Se rellenan los campos de la estructura con la IP del 
		servidor y el puerto del servicio que solicitamos
	-------------------------------------------------------------------*/
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2060);
	sockname.sin_addr.s_addr =  inet_addr("127.0.0.1");

	/* ------------------------------------------------------------------
		Se solicita la conexión con el servidor
	-------------------------------------------------------------------*/
	len_sockname = sizeof(sockname);
	
	if (connect(sd, (struct sockaddr *)&sockname, len_sockname) == -1)
	{
		perror ("Error de conexión");
		exit(1);
	}
    
    //Inicializamos las estructuras
    FD_ZERO(&auxfds);
    FD_ZERO(&readfds);
    
    FD_SET(0,&readfds);
    FD_SET(sd,&readfds);

	//Mostramos mensajes de información del cliente

    printf("\nSi no está registrado introduzca: REGISTRO -u usuario -p password\n");
	printf("Si está registrado introduzca: USUARIO usuario\n");
	printf("Si su usuario es validado introduzca: PASSWORD password\n");


	/* ------------------------------------------------------------------
		Se transmite la información
	-------------------------------------------------------------------*/
	do
	{
        auxfds = readfds; // xk el select se lo carga
        salida = select(sd+1,&auxfds,NULL,NULL,NULL);
        
        //Tengo mensaje desde el servidor
        if(FD_ISSET(sd, &auxfds)){
            
            bzero(buffer,sizeof(buffer));
            recv(sd,buffer,sizeof(buffer),0); //sizeof xk siempre mandadmos el tamaño max

            if(strcmp(buffer,"Demasiados clientes conectados\n") == 0){
				printf("\n%s\n",buffer);
                fin =1;
			}
            else if(strcmp(buffer,"Desconexión servidor\n") == 0){
				printf("\n%s\n",buffer);
                fin =1;
			}
			//Se comprueba si se ha recibido +Ok. Empieza la partida
			else if (strstr(buffer, "+Ok. Empieza la partida. ") != NULL) {
				//Si comienza la partida se imprime la matriz llena de -
				imprimeMatrizVacia();
			}
			else if (strstr(buffer, "+Ok. Nuevo tablero. ") != NULL) {
				//Si se procesa e imprime la nueva matriz
				char A[6][7];
				pasarMensajeAMatriz(buffer, A);
				imprimeMatrizActual(A);

			}
			else{
				//Si no es ninguna de esas opciones, se muestra el mensaje del server
				printf("\n%s\n",buffer);
			}
            
        }
        else
        {
            
            //He introducido información por teclado (salir del servidor)
            if(FD_ISSET(0,&auxfds)){
                bzero(buffer,sizeof(buffer));
                
                fgets(buffer,sizeof(buffer),stdin);
                
                if(strcmp(buffer,"SALIR\n") == 0){
                        fin = 1;
                
                }
                
                send(sd,buffer,sizeof(buffer),0);
                
            }
            
            
        }
        
        
				
    }while(fin == 0);
		
    close(sd);

    return 0;
		
}

void imprimeMatrizVacia(){
   printf("|1|2|3|4|5|6|7|\n");
   for(int i=0; i<6; i++){
      for(int j=0; j<7; j++){
         printf("|-");
      }
      printf("|");
      printf("\n");
   }
   printf("\n");
}

void imprimeMatrizActual(char A[6][7]){
   printf("|1|2|3|4|5|6|7|\n");
   for(int i=0; i<6; i++){
      for(int j=0; j<7; j++){
         printf("|");
         printf("%c", A[i][j]);
      }
      printf("|");
      printf("\n");
   }
   printf("\n");
}

void pasarMensajeAMatriz(char buffer[250], char A[6][7]){
	int countBuffer=21;
	for(int i=0; i<6; i++){
		for(int j=0; j<7; j++){
			buffer[countBuffer]=A[i][j];
			countBuffer++;
			if(j<6){
				countBuffer++;
			}
			else{
				countBuffer++;
				countBuffer++;
			}
		}
	}
}
