//
//  cliente.c
//  Inter2011
//
//  Created by Admin on 5/9/11.
//


/*------------------------------------------------------------
 Diego Wolff e Thiago Araujo.
 *----------------------------------------------------------
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>


#define ADDRESS     "127.0.0.1"  /* coloque aqui o IP do servidor */
#define PORTA       5193         /* coloque aqui o numero da porta do servidor*/

int main( void )
{
    int  desc_socket; /*descritor do socket do cliente*/ 
    struct sockaddr_in endereco; /* endereco do servidor (ip+porta)*/
    char buffer[100];
    char bufferServ[100];
    int sair=0;
    
    if ((desc_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[Cliente] errro na criacao do socket");
        exit(1);
    }
    
    memset( & endereco, 0, sizeof(endereco));
    endereco.sin_family = AF_INET;  
    endereco.sin_port = htons(PORTA); 
    endereco.sin_addr.s_addr=inet_addr(ADDRESS);
    
    /* pede conexao com o servidor*/
    if(connect(desc_socket, (struct sockaddr *) &endereco, sizeof(endereco)) < 0) {
        perror("[Cliente] erro na conexao!");
        exit(1);
    }
    
    printf("\n[Cliente] Bem vindo ao Projeto Inter 2011! (\"go\" inicia)  \n (\"sair\" encerra):");
    scanf("%s", buffer);
    
    while(!sair)
    {
        if (strcmp(buffer, "sair") == 0) sair = 1;
        if(strcmp(bufferServ, "200") == 0) Responder(); 
        else if(strcmp(bufferServ, "erro")) printf("\n[Cliente] Servidor respondeu com %s", bufferServ);
        
        
        write(desc_socket, buffer, strlen(buffer)+1, 0);
        
        recv(desc_socket, bufferServ, 100,0);
        printf("\n[Cliente] Servidor: [%s]",  buffer);
        scanf("%s", buffer);
    }    
    close(desc_socket);

    
    printf("\n[Cliente] Finalizado corretamente...\n");
    return 0;
}

int Responder(int desc_socket){
    printf("[Cliente] Respondendo... \n");
    write(desc_socket, "200 ok", strlen("200 ok")+1, 0);    
}






