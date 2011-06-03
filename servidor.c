//
//  servidor.c
//  Inter2011
//

/*------------------------------------------------------------
 * Diego Wolff e Thiago Araujo
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
#define CONEXOES    5  

char *getPergunta(int n);
char **preencherPerguntas(char **perguntas, int posicoes);
void esvaziarPerguntas(char **p, int posicoes);

char **perguntas;
char *respostas; /* numero de conexoes simultaneas */

int indexPergunta = 0;

void trataConexao(int socket_cliente)
{
    
    
    int y;
	perguntas = malloc(10 * sizeof(char *));	
	char *c = malloc(101 * sizeof(char));
	char **p;
    int indiceRespPergunta;
    int score;
    
	p = preencherPerguntas(perguntas, 10);
	if(p == NULL){
		printf("Sem memoria! \n");
		exit(1);
	}

    
    char *buffer = malloc(100 * sizeof(char));
    
    int n, sair=0;
    
    while(!sair){
        n = recv(socket_cliente,buffer, 100,0);
        printf("\n[Servidor] %d bytes do Cliente, mensagem=[%s] ", n, buffer);
        fflush(stdout);
        if(strcmp(buffer,"sair") == 0){
            sair = 1;
            strcpy(buffer,"Saindo...");
        }
        else{ 
            write (socket_cliente,"200", strlen(buffer)+1,0);
            
            //Perguntar
            
            if(strcmp(buffer, "200 ok")){
                
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
    
    /* funcao serve para associar uma porta local e um endereco local a um socket. Essa
     funcao eh muito usada com outra funcao, a listen() */
    if(bind(desc_socket, (struct sockaddr *)&end_local, sizeof(end_local)) < 0){
        perror("[Servidor] erro no bind");
        exit(1);
    }
    printf("\n[Servidor] Servidor no ar...\n\n"); 
    
    /*
     * Espera uma determinada conexao em um socket.
     */
    if (listen(desc_socket, CONEXOES) < 0) {
        perror("[Servidor] erro no listen");
        exit(1);
    }
    
    
    /*
     funcao responsavel por aceitar uma conexao em um socket. Um socket cliente pede
     permissao para um socket servidor para que ambos se comuniquem.
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
    
    strcpy(buffer, "pergunta 1: \n a) \n b) \n c) \n d)");
    
    return buffer;    
}

char *getPergunta(int n)
{
    if (n > 10 || n < 0) return NULL;
    if(perguntas != NULL) return perguntas[n];
    return NULL;
}

char **preencherPerguntas(char **perguntas, int posicoes){
        printf("\n chegou aqui."); 
    if(perguntas == NULL) return NULL;
    for(int i = 0; i < 10; i++) perguntas[i] = malloc(101*sizeof(char *));
    
        printf("\n chegou aqui tbm."); 
    

    
    perguntas[0] = "1. Qual o numero de estrelas que possue nossa Via Lactea? \n a) 5 bilhoes \n b) 100 bilhoes \n c) 1 trilhao \n d) 100 trilhoes \n ";
    
    perguntas[1] = "2. Qual é a Temperatura da superficie do Sol? \n a) 5 bilhoes de graus C \n b) 100 bilhoes de graus C \n c) 1 trilhao de graus C \n d) 100 trilhoes de graus C \n";
    perguntas[2] = "3. Qual é o tamanho da Montanha K2? \n a) 5 mil metros \n b) 100 mil metros \n c) 1 1 milhao de m \n d) 100 trilhoes de m \n";
    perguntas[3] = "4. Qual é a temperatura mais baixa ja registrada? \n a) 0 oC \n b) -100 oC \n c) -200 oC \n d) -272 oC \n";
    perguntas[4] = "5. Qual é a maior profundidade dos Oceanos? \n a) 5km \n b) 100km c) 1000m \n d) 500km \n"; 
    perguntas[5] = "6. Quantas vezes o beija-flor bate as asas por segundo? \n a) 5 \n b) 10 \n c) 100 \n d) 1000 \n"; 
    perguntas[6] = "7. Qual o lugar mais quente da Terra? \n a) Saara \n b) Ecuador \n c) Salvador \n d) Rua Augusta \n";
    perguntas[7] = "8. Qual é o maior monumento construido pelo homem? \n a) Torres Gemeas \n b) Cristo Redentor \n c) Piramides do Egito \n d) A Lua \n";
    perguntas[8] = "9. Qual é o maior país do mundo? \n a) Russia \n b) EUA \n c) China \n d) Uzbequistao \n";
    perguntas[9] = "10. Qual é o animal mais rapido do mundo? \n a) Guepardo \n b) Aguia pescadora \n c) Cometa Halley \n d) Ferrari F50 \n";
    
    
    return perguntas;
}

void esvaziarPerguntas(char **p, int posicoes){
    printf("Esvaziando perguntas... \n");
    if(p != NULL){
        //como as posicoes do vetor nao foram alocadas com malloc, nao
        //posso dar free (pois estão no heap).
        /*for(int i = 0; i < posicoes; i++){
         free(p[i]);
         }
         free(*p); */
        
        free(p);
        p = NULL;
    }
}

char *preencherRespostas(char *respostas, int posicoes){
    if(respostas == NULL) return NULL;
    for(int i = 0; i < 10; i++) respostas = malloc(10 * sizeof(char));
    
    respostas[0] = 'b';
    respostas[1] = 'b';
    respostas[2] = 'a';
    respostas[3] = 'b';
    respostas[4] = 'c';
    respostas[5] = 'b';
    respostas[6] = 'a';
    respostas[7] = 'b';
    respostas[8] = 'c';
    respostas[9] = 'c';
    
    return respostas;
}



