/*Caio Gomes Montteiro - 120036373*/

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

/*Número de threads do problema.*/
#define NTHREADS 5

//Variáveis globais
int contador = 0;      //variável para auxiliar a ordem correta das threads.
sem_t condt2, condt3;     //semáforos para sincronizar a ordem de execução das threads

//Função executada pela thread 1
void *t1 (void *threadid) {
  sem_wait(&condt3); //Esperar pelas 4 primeiras threads imprimirem.
  printf("Volte sempre!\n");
  pthread_exit(NULL);
}

//Função executada pela thread 2
void *t2 (void *threadid) {
  sem_wait(&condt2); //Esperar pela execução da thread 1 e/ou as threads 3 ou 4
  printf("Fique a vontade.\n");
  contador++; //Atualização do contador para fazer a atualização de semáforo correta dependendo do caso.
  if (contador<3){
      sem_post(&condt2);
  } else{
      sem_post(&condt3);
  }
  pthread_exit(NULL);
}

//Função executada pela thread 3
void *t3 (void *threadid) {
  sem_wait(&condt2); //Esperar pela execução da thread 1 e/ou as threads 2 ou 4
  printf("Sente-se por favor.\n");
  contador++; //Atualização do contador para fazer a atualização de semáforo correta dependendo do caso.
  if (contador<3){
      sem_post(&condt2);
  } else{
      sem_post(&condt3);
  }
  pthread_exit(NULL);
}

//Função executada pela thread 4
void *t4 (void *threadid) {
  sem_wait(&condt2); //Esperar pela execução da thread 1 e/ou as threads 2 ou 3
  printf("Aceita um copo d'água?.\n");
  contador++; //Atualização do contador para fazer a atualização de semáforo correta dependendo do caso.
  if (contador<3){
      sem_post(&condt2);
  } else{
      sem_post(&condt3);
  }
  pthread_exit(NULL);
}

//Função executada pela thread 5
void *t5 (void *threadid) {
  printf("Seja bem-vindo!\n");
  sem_post(&condt2); //Sinalização para os prints das threads 2, 3 e 4.
  pthread_exit(NULL);
}

//Thread principal (main)
int main(int argc, char *argv[]) {
  pthread_t tid[NTHREADS];
  int *id[NTHREADS], t;

  for (t=0; t<NTHREADS; t++) {
    if ((id[t] = malloc(sizeof(int))) == NULL) {
       pthread_exit(NULL); return 1;
    }
    *id[t] = t+1;
  }

  //Inicialização dos semáforos
  sem_init(&condt2, 0, 0);
  sem_init(&condt3, 0, 0);

  //Criação das cinco threads
  if (pthread_create(&tid[0], NULL, t1, (void *)id[0])) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  if (pthread_create(&tid[1], NULL, t2, (void *)id[1])) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  if (pthread_create(&tid[2], NULL, t3, (void *)id[2])) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  if (pthread_create(&tid[3], NULL, t4, (void *)id[3])) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  if (pthread_create(&tid[4], NULL, t5, (void *)id[4])) { printf("--ERRO: pthread_create()\n"); exit(-1); }

  //Sincronização de todas as threads por pthread_join()
  for (t=0; t<NTHREADS; t++) {
    if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
    free(id[t]);
  } 
  pthread_exit(NULL);
}