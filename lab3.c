/*Caio Gomes Monteiro - 120036373*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include "timer.h"
#include<time.h>

long long int N; //Tamanho N do vetor.
int nthreads; //Quantidade de threads a serem utilizadas.
float *vetor; //Vetor a ser utilizado pelo código. 
float Li, Ls; //Limiares inferiores e superiores, respectivamente.

/*Função que avalia a corretude do método concorrente ao compará-lo com o método sequencial.*/
void avaliarCorretude(long int quantRepetidasSeq, long int quantRepetidasConc){
   if(quantRepetidasConc!=quantRepetidasSeq){
      printf("Os resultados sequenciais e concorrentes sao diferentes!!!\n");
   }
}

/*Função a ser rodada por cada thread.*/
void *funcaoThreads(void* arg) {
   long int identificador = (long int) arg; //Identificador local da thread.
   double *somaLocal; //Variável local a ser retornada pela thread.
   somaLocal = (double*) malloc(sizeof(double));
   if(somaLocal==NULL) {
      printf("ERRO--alocacao de memoria dentro das threads\n");
      exit(1);
   }
   long int tamBloco = N/nthreads;  //Tamanho do bloco da thread.
   long int inicioIteracao = identificador * tamBloco; //Elemento inicial do bloco da thread a ser lido e processado.
   long int fimIteracao; //Elemento imediatamente depois do último processado pela thread.
   if(identificador == nthreads-1) fimIteracao = N;
   else fimIteracao = inicioIteracao + tamBloco;
   //Compara os valores no vetor com o intervalo das limiares.
   for(long int i=inicioIteracao; i<fimIteracao; i++){
      float x = vetor[i];
      if(x<Ls && x>Li){
          *somaLocal = *somaLocal + 1;
      }
   }
   //Retorna a quantidade de resultados apenas calculados por uma thread.
   pthread_exit((void *) somaLocal); 
}

//Thread principal (main)
int main(int argc, char *argv[]) {
   double tempo_inicio, tempo_fim; //Variáveis para tomada de tempo e avaliação de desempenho.
   pthread_t *idThreads; //Identificadores das threads no computador.
   double *valorRetorno; //Valor de retorno das threads.
   float limite_maximo_RNG = 1000.0; //Limite superior para a geração de floats randômicos.
   long int quantRepetidasSeq = 0; //Variável que guarda o resultado obtido sequencialmente.
   long int quantRepetidasConc = 0; //Variável que guarda o resultado obtido concorrentemente.
   srand((unsigned int)time(NULL)); //Definição de seed para os números randômicos baseado no tempo do sistema.

   //Recebimento e validação dos parâmetros de entrada.
   if(argc < 5) {
       fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads> <limiar inferior> <limiar superior>\n", argv[0]);
       return 1; 
   }
   
   N = atoll(argv[1]);
   nthreads = atoi(argv[2]);
   Li = atof(argv[3]);
   Ls = atof(argv[4]);
   
   //Alocação de memória para o vetor principal.
   vetor = (float*) malloc(sizeof(float)*N);
   if(vetor == NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   
   //Inicialização dos valores do vetor principal com floats aleatórios.
   for(long int i=0; i<N; i++)
      vetor[i] = ((float)rand()/(float)(RAND_MAX)) * limite_maximo_RNG;
  
   //Cálculo da quantidade de elementos do vetor no intervalo das limiares feito de forma sequencial.
   GET_TIME(tempo_inicio);
   for(long int i=0; i<N; i++){
      float x = vetor[i];
      if(x<Ls && x>Li){
          quantRepetidasSeq++;
      } 
   }  
   GET_TIME(tempo_fim);
   printf("Tempo sequencial:  %lf\n", tempo_fim-tempo_inicio);
   
   //Cálculo da quantidade de elementos do vetor no intervalo das limiares feito de forma concorrente.
   GET_TIME(tempo_inicio);
   idThreads = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
   if(idThreads==NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   
   //Criação das threads.
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(idThreads+i, NULL, funcaoThreads, (void*) i)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
   }
   
   //Sincronização e finalização das threads.
   for(long int i=0; i<nthreads; i++) {
      if(pthread_join(*(idThreads+i), (void**) &valorRetorno)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
      //O valor retornado pelos threads é aproveitado.
      quantRepetidasConc += *valorRetorno;
   }
   GET_TIME(tempo_fim);
   printf("Tempo concorrente:  %lf\n", tempo_fim-tempo_inicio);
   //Avaliação da corretude.
   avaliarCorretude(quantRepetidasConc,quantRepetidasSeq);

   //Liberação de memória alocada.
   free(vetor);
   free(idThreads);
   return 0;
}