/*Caio Gomes Monteiro - 120036373*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"
#include<time.h>

int *matrizA; //Matriz de entrada A.
int *matrizB; //Matriz de entrada B.
int *resultado_seq; //Matriz que guardará os resultados calculados de forma sequencial.
int *resultado_conc; //Matriz que guardará os resultados calculados de forma concorrente.
int nthreads; //Número de threads utilizadas.

//Estrutura que guarda os parâmetros a serem passados para cada thread.
typedef struct{
   int id; //Identifica o elemento que a thread irá processar.
   int dim; //Dimensão das duas matrizes de entrada A e B.
} tArgs;

//Função que todas as threads executarão.
void * tarefa(void *arg) {
   int temp;
   tArgs *args = (tArgs*) arg;
   for(int i=args->id; i<args->dim; i+=nthreads){
      for(int j=0; j<args->dim; j++){ 
        temp = 0;
        for (int k=0 ; k<args->dim ; k++){
            temp += matrizA[i*(args->dim) + k] * matrizB[k*(args->dim) + j];
        }
        resultado_conc[i*(args->dim) + j] = temp;
      }
   }
   pthread_exit(NULL);
}

//Função que compara todos os elementos de duas matrizes. Será utilizada para avaliar a corretude.
void comparaMatriz(int* matriz1, int* matriz2, int dimensao){
    for(int i=0; i<dimensao; i++) {
        for(int j=0; j<dimensao; j++){
            if(matriz1[i*dimensao+j] != matriz2[i*dimensao+j]){
                printf("--ERRO: Elemento da matriz calculado de forma equivocada\n");
            }
        }
    }
}

//Thread principal.
int main(int argc, char* argv[]) {
   int dim; //Dimensão das matrizes utilizadas.
   pthread_t *tid; //Números de identificação das threads no sistema.
   tArgs *args; //Vetor de estruturas que contém os parâmetros para as threads.
   double inicio_seq, fim_seq, inicio_conc, fim_conc, delta_seq, delta_conc; //Variáveis para avaliação de desempenho.
   int soma; //Variável temporária usada no cálculo sequencial.
   srand(time(0)); //Definição da seed para ser usado pelo RNG baseado no tempo do computador.

   //Leitura e avaliação dos parâmetros de entrada.
   if(argc<3) {
      printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
      return 1;
   }
   dim = atoi(argv[1]);
   nthreads = atoi(argv[2]);
   if (nthreads > dim) nthreads=dim;

   //Alocação de memória para as matrizes.
   matrizA = (int *) malloc(sizeof(int) * dim * dim);
   if (matrizA == NULL) {printf("ERRO--malloc\n"); return 2;}
   
   matrizB = (int *) malloc(sizeof(int) * dim * dim);
   if (matrizB == NULL) {printf("ERRO--malloc\n"); return 2;}
   
   resultado_conc = (int *) malloc(sizeof(int) * dim * dim);
   if (resultado_conc == NULL) {printf("ERRO--malloc\n"); return 2;}
   
   resultado_seq = (int *) malloc(sizeof(int) * dim * dim);
   if (resultado_seq == NULL) {printf("ERRO--malloc\n"); return 2;}

   //Inicialização das matrizes com elementos randômicos, que têm um alcance máximo de até 10000.
   for(int i=0; i<dim; i++) {
      for(int j=0; j<dim; j++){
         matrizA[i*dim + j] = rand()%10000;    
         matrizB[i*dim + j] = rand()%10000; 
         resultado_conc[i*dim + j] = 0;
         resultado_conc[i*dim + j] = 0;
      }
   }

   GET_TIME(inicio_seq);
   
   //Algoritmo de multiplicação de matrizes de forma sequencialmente.
   for(int i=0; i<dim; i++){
      for(int j=0; j<dim; j++){ 
        soma = 0;
        for (int k=0 ; k<dim ; k++){
            soma += matrizA[i*dim + k] * matrizB[k*dim + j];
        }
        resultado_seq[i*dim + j] = soma;
      }
   }
   GET_TIME(fim_seq);

   //Alocação de memória para os identificadores de sistema e para as estruturas com os parâmetros.
   tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}
   args = (tArgs*) malloc(sizeof(tArgs) * nthreads);
   if(args==NULL) {puts("ERRO--malloc"); return 2;}
   
   GET_TIME(inicio_conc);
   //Criação das threads com pthread_create().
   for(int i=0; i<nthreads; i++) {
      (args+i)->id = i;
      (args+i)->dim = dim;
      if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
         puts("ERRO--pthread_create"); return 3;
      }
   } 
   
   //Sincronização das threads, ou seja, esperar que todas terminem suas respectivas execuções.
   for(int i=0; i<nthreads; i++) {
      pthread_join(*(tid+i), NULL);
   }
   GET_TIME(fim_conc);   
   
   //Avaliação da corretude do resultado obtido.
   comparaMatriz(resultado_conc,resultado_seq,dim);
   
   //Cálculo dos tempos de multiplicações sequenciais e concorrentes.
   delta_seq = fim_seq - inicio_seq;
   delta_conc = fim_conc - inicio_conc;
   
   printf("Tempo multiplicacao sequencial: %lf\n", delta_seq);
   printf("Tempo multiplicacao concorrente: %lf\n", delta_conc);
   printf("Desempenho: %lf\n",delta_seq/delta_conc);

   //Desalocação de memória utilizada.
   free(matrizA);
   free(matrizB);
   free(resultado_conc);
   free(resultado_seq);
   free(args);
   free(tid);  
   return 0;
}