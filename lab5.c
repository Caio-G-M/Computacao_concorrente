/*Caio Gomes Monteiro - DRE 120036373*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 5 //Número de threads a serem utilizadas.

/* Variáveis globais. */
int x = 0; /*Variável que servirá para checar a condição e execução das threads.*/
pthread_mutex_t x_mutex; /*Variável para sincronização por exclusão mútua.*/

/*Duas váriaveis para sincronização lógica. A primeira condição é a impressão do print 5 e 
a segunda condição é que os prints 2, 3 e 4 foram executados depois do print 5 em qualquer ordem.*/
pthread_cond_t cond_1, cond_2; 

/*Thread que imprime o print 5, ou seja, o primeiro print que não depende de nenhum outro.*/
void *print5 (void *t) {
  pthread_mutex_lock(&x_mutex);
  printf("Seja bem-vindo!\n");
  x++;
  
  /*Depois de impresso o print 5, as threads 2, 3 e 4 que dependem da condição 1 são sinalizadas.*/
  pthread_cond_broadcast(&cond_1);
  
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/*Thread que imprime o print 4 depois de o 5 já ter sido feito.*/
void *print4 (void *t) {
  pthread_mutex_lock(&x_mutex);
  if (x==0) { //para o caso da thread 5 não ter imprimido o print 5 ainda.
     pthread_cond_wait(&cond_1, &x_mutex); /*Caso ainda não haja print 5, esperamos o sinal de cond_1.*/
  }
  
  /*Fazemos o print 4 e acrescentamos x.*/
  printf("Aceita um copo d'água?\n");
  x++;

  /*Caso em que este print foi o quarto print executado, portanto devemos sinalizar a condição 2.*/
  if (x==4) { 
    pthread_cond_signal(&cond_2); 
  }
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/*Thread que imprime o print 3 depois de o 5 já ter sido feito.*/
void *print3 (void *t) {
  pthread_mutex_lock(&x_mutex);
  if (x==0) { //para o caso da thread 5 não ter imprimido o print 5 ainda.
     pthread_cond_wait(&cond_1, &x_mutex); /*Caso ainda não haja print 5, esperamos o sinal de cond_1.*/
  }

  /*Fazemos o print 3 e acrescentamos x.*/
  printf("Sente-se por favor.\n");
  x++;
  
  /*Caso em que este print foi o quarto print executado, portanto devemos sinalizar a condição 2.*/
  if (x==4) { 
    pthread_cond_signal(&cond_2); 
  }
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/*Thread que imprime o print 2 depois de o 5 já ter sido feito.*/
void *print2 (void *t) {
  pthread_mutex_lock(&x_mutex);
  if (x==0) { //para o caso da thread 5 não ter imprimido o print 5 ainda.
     pthread_cond_wait(&cond_1, &x_mutex); /*Caso ainda não haja print 5, esperamos o sinal de cond_1.*/
  }

  /*Fazemos o print 2 e acrescentamos x.*/
  printf("Fique a vontade.\n");
  x++;

  /*Caso em que este print foi o quarto print executado, portanto devemos sinalizar a condição 2.*/
  if (x==4) { 
    pthread_cond_signal(&cond_2); 
  }
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}


/*Thread que imprime o print 1 depois de os prints 2, 3, 4 e 5 já terem sido feitos.*/
void *print1 (void *t) {
  pthread_mutex_lock(&x_mutex);
  while (x<4) { //para o caso das outras quatro threads ainda não terem imprimido.
     pthread_cond_wait(&cond_2, &x_mutex); /*Caso ainda não haja esses prints, esperamos o sinal de cond_2.*/
  }

  /*Fazemos o print 1.*/
  printf("Volte sempre!\n");
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/*Thread principal (main).*/
int main(int argc, char *argv[]) {
  int i; 
  int *t1, *t2, *t3, *t4, *t5;
  pthread_t threads[NTHREADS];
  //Aloca espaço para os identificadores das threads.
  t1=malloc(sizeof(int)); 
  t2=malloc(sizeof(int)); 
  t3=malloc(sizeof(int)); 
  t4=malloc(sizeof(int)); 
  t5=malloc(sizeof(int));
  *t1=1;
  *t2=2;
  *t3=3;
  *t4=4;
  *t5=5;

  /* Inicializa o mutex (lock de exclusão mútua) e as duas variáveis de condição.*/
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&cond_1, NULL);
  pthread_cond_init (&cond_2, NULL);

  /*Cria as threads.*/
  pthread_create(&threads[0], NULL, print1, (void *)t1);
  pthread_create(&threads[1], NULL, print2, (void *)t2);
  pthread_create(&threads[2], NULL, print3, (void *)t3);
  pthread_create(&threads[3], NULL, print4, (void *)t4);
  pthread_create(&threads[4], NULL, print5, (void *)t5);

  /*Espera todas as threads executarem.*/
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  
  /*Desaloca variáveis e termina.*/
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&cond_1);
  pthread_cond_destroy(&cond_2);
}
