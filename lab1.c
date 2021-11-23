/*Caio Gomes Monteiro DRE 120036373*/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

#define TAM_VETOR 10000

/*Estrutura que guarda os argumentos da função que cada thread processará, neste caso o vetor a modificar e 
onde começar a modificar dentro do vetor.*/
typedef struct {
    int comeco_vetor;
    long *vetor_elementos;
} t_Args;

/*Função que cada thread roda. Como o vetor tem 10000 elementos, cada thread modifica 10000/2 elementos do vetor.*/
void* Ao_Quadrado(void* arg){
    t_Args *args = (t_Args *) arg;
    for(int i = args->comeco_vetor; i < ((args->comeco_vetor) + 5000) ; i++){
        long temp = args->vetor_elementos[i];
        args->vetor_elementos[i] = temp*temp;
    }
    pthread_exit(NULL);
}

/*Thread principal.*/
int main(void){
    /*Identificadores de cada thread no sistema.*/
    pthread_t tid_sistema1;
    pthread_t tid_sistema2;
    
    /*Alocando memória para o vetor de 10000 posições.*/
    long *vetor = (long*)malloc(sizeof(double) * TAM_VETOR);
    if (vetor == NULL) {
        printf("--ERRO: malloc()\n"); exit(-1);
    }
    
    /*Alocando memória para as duas estruturas a serem passadas como argumento para as duas threads.*/
    t_Args *arg1 = malloc(sizeof(t_Args));
    if (arg1 == NULL) {
        printf("--ERRO: malloc()\n"); exit(-1);
    }
    t_Args *arg2 = malloc(sizeof(t_Args));
    if (arg2 == NULL) {
        printf("--ERRO: malloc()\n"); exit(-1);
    }

    /*Construção de um vetor arbitrário para exemplificação do programa, que funciona com qualquer vetor.*/
    for(int i =  0; i < TAM_VETOR ; i++){
        vetor[i] = i+1;
    }
    
    /*Preenchemos as estruturas para passar como argumento.*/
    arg1->comeco_vetor = 0;
    arg1->vetor_elementos = vetor;
    arg2->comeco_vetor = 5000;
    arg2->vetor_elementos = vetor;
    
    /*Criação das threads.*/
    if (pthread_create(&tid_sistema1, NULL, Ao_Quadrado, (void*) arg1)) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
    }
    if (pthread_create(&tid_sistema2, NULL, Ao_Quadrado, (void*) arg2)) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
    }

    /*Aguardar a execução das threads.*/
    if (pthread_join(tid_sistema1, NULL)) {
        printf("--ERRO: pthread_join() \n"); exit(-1); 
    }
    if (pthread_join(tid_sistema2, NULL)) {
        printf("--ERRO: pthread_join() \n"); exit(-1); 
    }

    /*Loop para a verificação automática de valores do vetor modificado.*/
    for(int i = 0; i < TAM_VETOR ; i++){
        if (vetor[i] != (i+1)*(i+1)) {
            printf("--Erro: vetor modificado de forma equivocada\n");
        }
    }

    pthread_exit(NULL);
}
