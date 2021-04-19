/******************************************************************************/
/*			Application: ....			              */
/******************************************************************************/
/*									      */
/*			 programme  SERVEUR 				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs :  ....						      */
/*		Date :  ....						      */
/*									      */
/******************************************************************************/	

#include <stdio.h>
#include <curses.h>

#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

#include "fon.h"     		/* Primitives de la boite a outils */

#define SERVICE_DEFAUT "1111"

#define send_error() buffer[0]=8;h_writes(id_socket_client,buffer,1);

void serveur_appli (char *service);   /* programme serveur */


/******************************************************************************/	
/*---------------- programme serveur ------------------------------*/

int main(int argc,char *argv[])
{

	char *service= SERVICE_DEFAUT; /* numero de service par defaut */


	/* Permet de passer un nombre de parametre variable a l'executable */
	switch (argc)
 	{
   	case 1:
		  printf("defaut service = %s\n", service);
		  		  break;
 	case 2:
		  service=argv[1];
            break;

   	default :
		  printf("Usage:serveur service (nom ou port) \n");
		  exit(1);
 	}

	/* service est le service (ou numero de port) auquel sera affecte
	ce serveur*/
	
	serveur_appli(service);
}


/******************************************************************************/	
void serveur_appli(char *service)

/* Procedure correspondant au traitemnt du serveur de votre application */

{
	struct sockaddr_in *a; // local
	struct sockaddr_in *info_client; //distant
	char buffer[100];
		
	int id_socket_listen = h_socket(AF_INET, SOCK_STREAM);
	adr_socket(service,"*",SOCK_STREAM,&a);
	h_bind(id_socket_listen,a);
	h_listen(id_socket_listen, 5);





	while(1) {

		int id_socket_client = h_accept(id_socket_listen,info_client);
		//srand(info_client->sin_port);
		srand(time(NULL));
		char* combinaison;
		int n = -1;

		while(1) {
			h_reads(id_socket_client,buffer,1);
			switch (buffer[0]) {
			case 1: // roll
				h_reads(id_socket_client,buffer,1);
				n = buffer[0];
				combinaison = (char*)malloc(n*sizeof(char));
				for(int i = 0; i < n ; i++) {
					*combinaison = rand() % 8;
				}
				// buffer[0] = 9;
				// h_writes(id_socket_client,buffer,1); // ack
				break;
			case 2: // ask
				if(n!=-1) {
					h_reads(id_socket_client,buffer,n);
					int blanc = 0,rouge = 0;
					for(int i = 0; i < n; i++) { // première passe (rouge)
						if(buffer[i] == combinaison [i]) {
							rouge++;
							buffer[i] = -2;
						}
					}
					for(int i = 0; i < n; i++) { // deuxième passe (blanc)
						if(buffer[i] == -2)
							break;
						for(int j = 0; j < n; j++) {
							if(combinaison[i] == buffer[j]) {
								buffer[j]= -1;
								blanc++;
								break;
							}
						}
					}
					if(rouge == n) {
						buffer[0] = 4;
							h_writes(id_socket_client,buffer,1); // win
					} else {
						buffer[0] = 4;
						buffer[1] = rouge;
						buffer[2] =	blanc;
						h_writes(id_socket_client,buffer,3); // reply
					}
				} else {
					send_error()
				}
				break;
			case 3: // stop
				// ????????????????????????????????????????????
				break;
			default:
				send_error()
				break;
			}
		


		}

		h_close(id_socket_client);
		h_close(id_socket_listen);

	}









}

/******************************************************************************/	

