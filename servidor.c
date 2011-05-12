//
//  servidor.c
//  Inter2011
//
//  Created by Admin on 5/9/11.
//  Copyright 2011 worf. All rights reserved.
//

/*------------------------------------------------------------
 * Programa: servidor.c 
 *           codigo do programa servidor utilizando TCP 
 *
 * Autor...: Fabio Lubacheski
 *----------------------------------------------------------
 */

#include <sched.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>

#define ADDRESS     "127.0.0.1" /* coloque aqui o IP do servidor */
#define PORTA       5193        /* coloque aqui o numero da porta do servidor*/
#define CONEXOES    5           /* numero de conexoes simultaneas */

int indexPergunta = 0;

void trataConexao(int socket_cliente)
{
    
    char *buffer = malloc(100 * sizeof(char));
    
    int n, sair=0;
    
    while(!sair){
        n=recv(socket_cliente,buffer, 100,0);
        printf("\n[Servidor] %d bytes do Cliente, mensagem=[%s] ", n,buffer);
        fflush(stdout);
        if(strcmp(buffer,"sair")==0){
            sair=1;
            strcpy(buffer,"Saindo...");
        }
        else{
            strcpy(buffer,"200");            
            if(strcpy(buffer,"200 ok")){
                
                printf("\n[Servidor] Perguntando... \n"); 
                perguntar(buffer);
            }
            
        }
        write (socket_cliente,buffer, strlen(buffer)+1,0);
    }
    close(socket_cliente);
}



int main(void)
{
    int  desc_socket, /* descritor do socket do servidor*/
    newsocket;   /* descritor do socket do cliente*/
    
    
    struct sockaddr_in end_local;  /* endereco do servidor (ip+porta)*/
    struct sockaddr_in end_remoto; /* endereco do cliente  (ip+porta)*/
    socklen_t len_end_remoto;                 /* comprimento do endereco		  */
    
    
    if ((desc_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Servidor: falha na criacao do socket do servidor");
        exit(1);
    }
    
    /*
     * Especifica o endereco para aceitar conexoes (ip+porta)
     */
    memset( &end_local, 0, sizeof(end_local));
    end_local.sin_family = AF_INET;  
    end_local.sin_port = htons(PORTA); 
    end_local.sin_addr.s_addr=inet_addr(ADDRESS);
    
    /* funÁ„o serve para associar uma porta local e um endereco local a um socket. Essa
     funÁ„o È muito usada com outra funÁ„o, a listen()*/
    if(bind(desc_socket, (struct sockaddr *)&end_local, sizeof(end_local)) < 0){
        perror("[Servidor] erro no bind");
        exit(1);
    }
    printf("\n[Servidor] Servidor no ar...\n\n"); 
    
    /*
     * Espera uma determinada conex„o em um socket.
     */
    if (listen(desc_socket, CONEXOES) < 0) {
        perror("[Servidor] erro no listen");
        exit(1);
    }
    
    
    /*
     funÁ„o respons·vel por aceitar uma conex„o em um socket. Um socket cliente pede
     permiss„o para um socket servidor para que ambos se comuniquem.
     */
    len_end_remoto = sizeof(struct sockaddr_in);
    newsocket = accept(desc_socket, (struct sockaddr *) &end_remoto, &len_end_remoto);
    if (newsocket < 0) {
        perror("[Servidor] erro no accept");
        exit(1);
    }

    trataConexao(newsocket);
    
    // Fecha socket servidor
    close(desc_socket);
    printf("\n[Servidor] Finalizado corretamente...\n");
    return 0;
}

void perguntar(char *buffer){
    
    //Posso perguntar?
    printf("\n[Servidor] Pergunta %d \n", indexPergunta++);
    
    strcpy(buffer, "pergunta 1");
    
    return buffer;    
}







