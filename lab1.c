/*Caio Gomes Monteiro DRE 120036373*/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

#define TAM_VETOR 10000

typedef struct {
    int comeco_vetor;
    long *vetor_elementos;
} t_Args;

void* Ao_Quadrado(void* arg){
    t_Args *args = (t_Args *) arg;
    for(int i = args->comeco_vetor; i < ((args->comeco_vetor) + 5000) ; i++){
        long temp = args->vetor_elementos[i];
        args->vetor_elementos[i] = temp*temp;
    }
    pthread_exit(NULL);
}

int main(void){
    pthread_t tid_sistema1;
    pthread_t tid_sistema2;
    
    long *vetor = (long*)malloc(sizeof(double) * TAM_VETOR);
    if (vetor == NULL) {
        printf("--ERRO: malloc()\n"); exit(-1);
    }
    
    t_Args *arg1 = malloc(sizeof(t_Args));
    if (arg1 == NULL) {
        printf("--ERRO: malloc()\n"); exit(-1);
    }
    
    t_Args *arg2 = malloc(sizeof(t_Args));
    if (arg2 == NULL) {
        printf("--ERRO: malloc()\n"); exit(-1);
    }

    for(int i =  0; i < TAM_VETOR ; i++){
        vetor[i] = 1+i;
    }
    
    arg1->comeco_vetor = 0;
    arg1->vetor_elementos = vetor;
    arg2->comeco_vetor = 5000;
    arg2->vetor_elementos = vetor;
    
    if (pthread_create(&tid_sistema1, NULL, Ao_Quadrado, (void*) arg1)) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
    }

    if (pthread_create(&tid_sistema2, NULL, Ao_Quadrado, (void*) arg2)) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
    }

    if (pthread_join(tid_sistema1, NULL)) {
        printf("--ERRO: pthread_join() \n"); exit(-1); 
    }

    if (pthread_join(tid_sistema2, NULL)) {
        printf("--ERRO: pthread_join() \n"); exit(-1); 
    }

    for(int i =  0; i < TAM_VETOR ; i++){
        printf("%ld\n", vetor[i]);
    }

    pthread_exit(NULL);
}