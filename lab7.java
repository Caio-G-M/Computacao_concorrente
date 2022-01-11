/*Caio Gomes Monteiro - 120036373*/

//Classe da estrutura de dados (recurso) compartilhado entre as threads.
class S {
    //Recurso compartilhado
    private int[] vetor_int;
    private int soma_vetor_int = 0;
    
    //Construtor
    public S(int[] vetor_int) { 
       this.vetor_int = vetor_int;
       this.soma_vetor_int = 0;
    }
  
    //Função que soma os elementos do vetor em um intervalo passado como argumento.
    public synchronized void somar_vetor_int(int inicioIteracao, int fimIteracao) { 
       for(int i=inicioIteracao; i<fimIteracao; i++){
         this.soma_vetor_int = this.soma_vetor_int + this.vetor_int[i];
       }
    }
    
    //Getter para a soma total dos elementos no vetor.
    public int get() { 
        return this.soma_vetor_int; 
    }
  }
  
  //Classe que estende Thread e implementa a tarefa de cada thread do programa. 
  class T extends Thread {
     //Identificador da thread
     private int id;

     //Objeto compartilhado com outras threads
     S s;
     
     //Definição de variáveis para dividir o vetor de inteiros em blocos iguais para dividir igualmente a tarefa.
     int tamBloco;
     int inicioIteracao;
     int fimIteracao;
     
     //Construtor
     public T(int tid, S s, int N, int tamanho_vetor) { 
        this.id = tid; 
        this.s = s;
        
        this.tamBloco = tamanho_vetor/N;
        this.inicioIteracao = tid * (tamanho_vetor/N);
        if(tid == N-1){
            this.fimIteracao = tamanho_vetor;
        }else{ 
            this.fimIteracao = (tid * (tamanho_vetor/N)) + (tamanho_vetor/N);
        }
     }
  
     //Tarefa em si que as threads realizam.
     public void run() {
        this.s.somar_vetor_int(this.inicioIteracao, this.fimIteracao);
     }
  }
  
  //Classe que possui a função main.
  class Soma_vetor_int {
     //Número de threads e tamanho do vetor.
     static final int N = 10;
     static final int tamanho_vetor = 50;
     static int soma_teste = 0;
     
     
     public static void main (String[] args) {
        //inicializando e preenchendo o vetor de inteiros.
        int [] vetor_int = new  int[tamanho_vetor];
        for (int i=0; i<vetor_int.length; i++){
            vetor_int[i] = i;
        }
        
        //Reserva espaço para o vetor das threads.
        Thread[] threads = new Thread[N];
  
        //Cria uma instância do recurso compartilhado entre as threads.
        S s = new S(vetor_int);
  
        //Cria as threads da aplicação.
        for (int i=0; i<threads.length; i++) {
           threads[i] = new T(i, s, N, tamanho_vetor);
        }
  
        //Inicialização das threads.
        for (int i=0; i<threads.length; i++) {
           threads[i].start();
        }
  
        //Término de todas as threads.
        for (int i=0; i<threads.length; i++) {
           try { threads[i].join(); } catch (InterruptedException e) { return; }
        }
  
        //Teste realizado para verificar se o resultado encontrado está correto.
        for(int i=0; i<vetor_int.length; i++){
            soma_teste = soma_teste + i;
        }
        if(soma_teste != s.get()){
            System.out.println("ERRO: Resultado incorreto."); 
        }

        //Impressão do resultado final que está na instância de s.
        System.out.println("Valor da soma dos inteiros no vetor = " + s.get()); 
     }
  }
  