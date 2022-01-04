/*Caio Gomes Monteiro - 120036373*/

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

//Variáveis globais para o problema.
int N; //Número de threads e tamanho do vetor de inteiros aleatórios.
int* vetor_int; //Vetor com inteiros aleatórios.
int bloqueadas = 0; //Contador de threads bloqueadas para a implementação da sincronização coletiva com barreira.

//Variáveis de sincronização.
pthread_mutex_t mutex;
pthread_cond_t condicional;

//Função que implementa a sincronização coletiva com barreira para este problema. 
void barreira(int nthreads) {
    
    if (bloqueadas == (N-1)) { 
      //Caso da última thread chegar na barreira.
      pthread_cond_broadcast(&condicional);
      bloqueadas=0;
    } else {
      bloqueadas++;
      pthread_cond_wait(&condicional, &mutex);
    }
}

//Função que todas as threads irão executar.
void *tarefa (void *arg) {
  
  int *id = (int*) arg;
  int aleatorio; //Número aleatório de cada iteração.
  double *soma_local; //Variável que guarda a soma local.
  soma_local = (double*) malloc(sizeof(double));
   if(soma_local==NULL) {
      printf("ERRO-- alocacao de memoria dentro das threads\n");
      exit(1);
   }
  
  //Início da sessão crítica.
  pthread_mutex_lock(&mutex);
  for(int i = 0; i<N; i++){
      
      printf("Vetor na iteracao %d da thread[%d]:\n\n", i +1, *id);
      for(int j = 0; j<N; j++){
          *soma_local += vetor_int[j];
          printf("%d ", vetor_int[j]);   
      }
      printf("\n\n");
      
      printf("Soma local thread[%d] na iteracao %d = %f\n", *id, i+1, *soma_local);
      
      barreira(N); //Primeira barreira.
      
      aleatorio = rand()%10;
      
      vetor_int[(*id) - 1] = aleatorio;
      
      barreira(N); //Segunda barreira.

  }
  //Fim da sessão crítica.
  pthread_mutex_unlock(&mutex);
  pthread_exit((void*) soma_local);
}

//Thread principal
int main(int argc, char *argv[]) {
  long int temp = 0;
  double *valorRetorno;
  srand(time(NULL)); //Definição de semente(seed) para os números randômicos.

  //Vetor que guarda todas as somas finais das threads para que sejam feitas as comparações entre os resultados.
  double *vetor_resultados = (double*) malloc(sizeof(double)*N);
   if(vetor_resultados==NULL) {
      printf("ERRO--alocacao de memoria dentro das threads\n");
      exit(1);
   }

  if(argc < 2) {
       fprintf(stderr, "Digite: %s <dimensao do vetor>\n", argv[0]);
       return 1; 
   }
  
  N = atoi(argv[1]);
  
  //Alocação e preenchimento do vetor de inteiros de tamanho N.
  vetor_int = (int*) malloc(sizeof(int)*N);
  if(vetor_int == NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
  }
  for(int i=0; i<N; i++)
      vetor_int[i] = rand()%10;

  //Identificadores das threads.
  pthread_t tid[N];
  int id[N];

  //Inicialização das variáveis de sincronização e condição.
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&condicional, NULL);

  //Criação das threads.
  for(int i=0; i<N; i++) {
    id[i] = i+1;
    if(pthread_create(&tid[i], NULL, tarefa, (void *) &id[i])) exit(-1);
  } 

  /*Sincronização e finalização das threads, junto com o print dos resultados de cada thread
  e a comparação entre cada resultado encontrado.*/
  for(int i=0; i<N; i++) {
      if(pthread_join(*(tid+i), (void**) &valorRetorno)){
         fprintf(stderr, "ERRO--pthread_join\n");
         return 3;
      }
      
      temp = *valorRetorno;
      printf("Soma total da tread %d: %ld\n", id[i], temp);
      vetor_resultados[i] = temp;
      for(int j=0; j<i; j++){
          if(vetor_resultados[j] != temp){
              printf("Soma total da thread %d diferente da thread %d\n", i, j);
          }
      }
   }

  pthread_exit(NULL);
  return 0;
}