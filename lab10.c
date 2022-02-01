/*Caio Gomes Montteiro - 120036373*/

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include<unistd.h>

//Tamanho do vetor de inteiros
#define N 10

/*Número de threads do problema.*/
int n_threads_consumidoras;
int n_threads_produtoras;
int n_threads;

//Variáveis globais
sem_t slot_cheio, vetor_vazio, mutex; //Semáforos e exclusão mútua.
int* vetor_int; //Vetor de inteiros a ser consumido e ser preenchido constantemente.
int quant_leituras = 0; //Variável que ajuda nas condições do problema.

//Função para os consumidores.
void Retirar(int* id){
    int item;
    static int out=0;
    
    //Espera por um slot cheio para consumir
    sem_wait(&slot_cheio);
    sem_wait(&mutex);
    sleep(1); //tempo para poder observar os prints melhor
    item = vetor_int[out];
    printf("Thread consumidora %d consumiu vetor_int[%d] = %d\n", *id, out, item);
    vetor_int[out] = 0; //atualização do consumo no vetor (representado por 0)
    quant_leituras++;
    out = (out+1)% N;
    
    //se esta for a última leitura possível do vetor, o vetor está vazio e pronto para o produtor.
    if(quant_leituras==N){
        sem_post(&vetor_vazio);
        quant_leituras=0;
    }
    sem_post(&mutex);
    sleep(0.5);
    
}

//Função para os produtores.
void Inserir(int* id){
    sem_wait(&vetor_vazio);
    sem_wait(&mutex);
    
    //preenche o vetor com elementos aleatórios.
    for(int i=0; i<N ; i++){
        vetor_int[i] = (rand()%10)+1;
    }
    
    sleep(0.5);
    printf("Thread produtora %d escreveu o vetor: ", *id);
    for(int i=0; i<N; i++){
      printf("%d/ ", vetor_int[i]);
    }
    printf("\n");
    
    sem_post(&mutex);
    
    //avisa que existem N slot cheios para consumo.
    for(int i=0; i<N ; i++){
        sem_post(&slot_cheio);
    }
}

//Função para as threads produtoras.
void *thread_produtora(void* arg){
    int *id = (int *) arg;
    while(1){
        Inserir(id);
    }
    pthread_exit(NULL);
}

//Função para as threads consumidoras.
void *thread_consumidora(void* arg){
    int *id = (int *) arg;
    while(1){
        Retirar(id);
    }
    pthread_exit(NULL);
}

//Thread principal (main)
int main(int argc, char *argv[]) {
   srand(time(NULL));
  
   //Recebimento e validação dos parâmetros de entrada.
   if(argc < 3) {
       fprintf(stderr, "Digite: %s <numero de threads consumidoras> <numero de threads produtoras>\n", argv[0]);
       return 1; 
   }
   
   //Atribuição das variáveis presentes em argv[].
   n_threads_consumidoras = atoi(argv[1]);
   n_threads_produtoras = atoi(argv[2]);
   n_threads = n_threads_consumidoras + n_threads_produtoras;
   
   //Alocação de memória para o vetor de inteiros.
   vetor_int = (int*) malloc(sizeof(int)*N);
   if(vetor_int == NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   
   pthread_t tid[n_threads];
   int id[n_threads];

   //Inicialização dos semáforos.
   sem_init(&slot_cheio, 0, 0);
   sem_init(&vetor_vazio, 0, 1);
   sem_init(&mutex, 0, 1);
  
   //Criação das threads consumidoras e produtoras.
   for(int i=0; i<n_threads_consumidoras; i++) {
     id[i] = i+1;
     if(pthread_create(&tid[i], NULL, thread_consumidora, (void *) &id[i])) exit(-1);
   } 

   for(int i=0; i<n_threads_produtoras; i++) {
     id[i+n_threads_consumidoras] = i+1;
     if(pthread_create(&tid[i+n_threads_consumidoras], NULL, thread_produtora, (void *) &id[i+n_threads_consumidoras])) exit(-1);
   }

  pthread_exit(NULL);
}