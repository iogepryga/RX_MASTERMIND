/******************************************************************************/
/*			Application: ...					*/
/******************************************************************************/
/*									      */
/*			 programme  CLIENT				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs : ... 					*/
/*									      */
/******************************************************************************/	


#include <stdio.h>
#include <curses.h> 		/* Primitives de gestion d'ecran */
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

#include "fon.h"   		/* primitives de la boite a outils */

#define SERVICE_DEFAUT "1111"
#define SERVEUR_DEFAUT "127.0.0.1"

void client_appli (char *serveur, char *service);

int string_length(char *s) {
  int result = 0;
  for(int i = 0; *(s+i) != 0 ; i++)
    result++;
  return result;
}


/*****************************************************************************/
/*--------------- programme client -----------------------*/

int main(int argc, char *argv[])
{

	char *serveur= SERVEUR_DEFAUT; /* serveur par defaut */
	char *service= SERVICE_DEFAUT; /* numero de service par defaut (no de port) */


	/* Permet de passer un nombre de parametre variable a l'executable */
	switch(argc)
	{
 	case 1 :		/* arguments par defaut */
		  printf("serveur par defaut: %s\n",serveur);
		  printf("service par defaut: %s\n",service);
		  break;
  	case 2 :		/* serveur renseigne  */
		  serveur=argv[1];
		  printf("service par defaut: %s\n",service);
		  break;
  	case 3 :		/* serveur, service renseignes */
		  serveur=argv[1];
		  service=argv[2];
		  break;
    default:
		  printf("Usage:client serveur(nom ou @IP)  service (nom ou port) \n");
		  exit(1);
	}

	/* serveur est le nom (ou l'adresse IP) auquel le client va acceder */
	/* service le numero de port sur le serveur correspondant au  */
	/* service desire par le client */
	
	client_appli(serveur,service);
}

/*****************************************************************************/
void client_appli (char *serveur,char *service)

/* procedure correspondant au traitement du client de votre application */

{
	struct sockaddr_in *a,*b;
		
	int id_socket = h_socket(AF_INET, SOCK_STREAM);

	adr_socket(service,serveur,SOCK_STREAM,&a); // distant

	adr_socket(service,"*",SOCK_STREAM,&b); // local
	b->sin_port=0;

	h_bind(id_socket,b);
	h_connect(id_socket,a);

	char buffer[100];

	int n = 10;

	buffer[0] = 1; buffer[1] = n;
	h_writes(id_socket,buffer,2);


	sleep(4);
	buffer[0] = 2;
	srand(time(NULL));
	for(int i = 1 ; i <= n ; i++) {
		buffer[i] = rand() % 8;
		printf("%d ",buffer[i]);
	}
	h_writes(id_socket,buffer,n+1);
	printf("\n");

	h_reads(id_socket,buffer,1);
	if(buffer[0] == 4) {
		h_reads(id_socket,buffer,2);
		printf("%d %d\n",buffer[0],buffer[1]);
	}

	h_close(id_socket);
}

/*****************************************************************************/

