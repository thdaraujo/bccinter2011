//
//  cliente.c
//  Inter2011
//
//  Created by Admin on 5/9/11.
//  Copyright 2011 worf. All rights reserved.
//


/*------------------------------------------------------------
 * Programa: cliente.c 
 *           codigo do programa cliente utilizando TCP 
 *
 * Objetivo: alocar um socket, conectar ao servidor e enviar
 *		       uma mensagem de ola e aguardar a resposta do servidor.
 * Autor...: Fabio Lubacheski
 *----------------------------------------------------------
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
//isso aparece no git?
#define ADDRESS     "127.0.0.1"  /* coloque aqui o IP do servidor */
#define PORTA       5193         /* coloque aqui o numero da porta do servidor*/

int main( void )
{
    int  desc_socket; /*descritor do socket do cliente*/ 
    struct sockaddr_in endereco; /* endereco do servidor (ip+porta)*/
    char buffer[100];
    int sair=0;
    
    /*
     Criacao de um Socket
     
     A criacao de um socket eh feita atraves da seguinte funcao (declarada em <sys/socket.h>):
     
     int socket(int familia, int type, int protocol);
     
     Esta funcao cria o socket a ser usado numa das "pontas" da comunicacao, jah que o 
     socket serah utilizado para comunicao entre dois processos (que podem estar sendo
     executados em maquinas diferentes).
     Os parametros sao os seguintes:
     
     * familia: especifica a "familia" de protocolos de rede a ser utilizado. Os valores 
     podem ser:
     + AF_INET      (ARPA INTERNET PROTOCOLS) - "A mais usada socket usa internet IPv4"
     + AF_UNIX      (UNIX INTERNET PROTOCOLS)
     + AF_ISO       (ISO PROTOCOLS)
     + AF_NS        (XEROX NETWORK SYSTEM PROTOCOLS)
     Os nossos exemplos irao se restringir aos protocolos "internet", especificados por AF_INET. 
     * type: define a categoria do  protocolo ("stream" ou "datagrama"). Os nossos 
     exemplos irao usar "stream", definido pela constante SOCK_STREAM. 
     * protocol: indica o protocolo a ser usado, dentro da "familia" definida por domain.
     Nos nossos exemplos, o valor utilizado para este parametro sera¥ igual a zero,
     indicando o uso do protocolo 
     "default" para a fami≠lia. No caso de PF_INET, o protocolo default sera¥ o IP
     
     
     
     Essa funcao retorna um inteiro correspondente ao descritor do socket criado ou o valor -1, 
     indicando que o socket nao o pode ser criado por alguma razao. A forma tipica de chamada
     dessa funcao, 
     nos nossos exemplos, sera¥ algo como
     */
    if ((desc_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[Cliente] errro na criacao do socket");
        exit(1);
    }
    
    /*
     A funcao connect estabelece a conexao com o servidor, mas antes deve 
     colocar informacoes adicionais na variavel endereco (struct sockaddr_in).
     
     As informacoes adicionais sao:	
     
     No campo endereco.sin_family eh informado que o endereco do servidor eh da familia
     AF_INET, ou seja um numero IP com 4 decimais separados por pontos (Ex 192.168.0.2) 
     
     htons significa "host to network short", como È short trabalha com 2 bytes,mas ha ainda
     outros tipos como: htonl("host to network long",4 bytes), ntÛs("network to host
     short",2 bytes), ntÛl("network to host long", 4 bytes).
     */
    memset( & endereco, 0, sizeof(endereco));
    endereco.sin_family = AF_INET;  
    endereco.sin_port = htons(PORTA); 
    endereco.sin_addr.s_addr=inet_addr(ADDRESS);
    
    /*
     funcao converte a string em ADDRESS para um endereco de rede em funcao de sua familia
     especificada por AF_INET, e armazena na variavel endereco.sin_addr,  
     converte endereco IP para a estrutura da AF_INET 
     */
    //inet_pton(AF_INET, ADDRESS, &endereco.sin_addr); 
    
    /* pede conexao com o servidor*/
    if(connect(desc_socket, (struct sockaddr *) &endereco, sizeof(endereco)) < 0) {
        perror("[Cliente] erro na conexao");
        exit(1);
    }
    
    char *ping = ("ping %s:%d", endereco, PORTA);
    system(ping);
    while(!sair)
    {
        printf("\n[Cliente] Bem vindo ao Projeto Inter 2011! (\"sair\" encerra):");
        scanf("%s", buffer);
        if (strcmp(buffer, "sair") == 0){
            Sair(desc_socket);
            sair = 1;
        }
        
        if (strcmp(buffer, "handshake") == 0) HandShake();
        if(strcmp(buffer, "200") == 0) Responder();
        
        
        write(desc_socket, buffer, strlen(buffer)+1, 0);
        printf("\n[Cliente] ...");
        
        recv(desc_socket, buffer, 100,0);
        printf("\n[Cliente] Servidor: [%s]",  buffer);
    }
    close(desc_socket);
    printf("\n[Cliente] Finalizado corretamente...\n");
    
    return 0;
}

void Sair(int desc_socket){
    char resposta[100];
    write(desc_socket, "bye", strlen("bye")+1, 0);
    
    recv(desc_socket, resposta, 100,0);
    printf("\n[Cliente] Servidor: [%s]",  resposta);
    
    write(desc_socket, "bye", strlen("bye")+1, 0);
    wait(5);
            
    close(desc_socket);
    printf("Conexão terminada. \n");
}

int HandShake(int desc_socket){
    char resposta[100];
    
    printf("\n[Cliente] Handshake. \n");
    
    recv(desc_socket, resposta, 100,0);
    printf("\n[Cliente] Servidor: [%s]",  resposta);
    if(strcmp(resposta, "200") == 0) Responder(desc_socket); //entra em modo de resposta
    else{
        printf("\n[Cliente] Nao entendi!");    
        write(desc_socket, "-1", strlen("-1 nao entendi")+1, 0); //senao, nao entendi
    }
    return 0;
}

int Responder(int desc_socket){
    printf("[Cliente] Respondendo... \n");
    write(desc_socket, "200 ok", strlen("200 ok")+1, 0);
    
}






