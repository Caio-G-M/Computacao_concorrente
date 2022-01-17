/*Caio Gomes Monteiro - 120036373*/

//Monitor que implementa a lógica do padrão de leitores/escritores e possui seção crítica.
class LE {
    private int leit, escr;
    private int int_compartilhada;
    
    //Construtor
    LE() { 
       this.leit = 0; //leitores lendo (0 ou mais)
       this.escr = 0; //escritor escrevendo (0 ou 1)
       this.int_compartilhada = 0; //variável especificada pelo problema.
    } 
    
    //Entrada para leitores
    public synchronized void EntraLeitor (int id) {
      try { 
        while (this.escr > 0) {
           System.out.println ("LeitorBloqueado("+id+")");
           wait();  //bloqueia pela condição lógica da aplicação
        }
        this.leit++;  //registra que há mais um leitor presente
        if (this.int_compartilhada%2==0){ //dependendo do valor da variável compartilhada, imprimimos seu valor
            System.out.println ("LeitorLendo("+id+"); variavel e par e igual a " + this.int_compartilhada);
        }else {
            System.out.println ("LeitorLendo("+id+"); variavel e impar e igual a " + this.int_compartilhada);
        }
        
      } catch (InterruptedException e) { }
    }
    
    //Saída para leitores
    public synchronized void SaiLeitor (int id) {
       this.leit--; //registra que um leitor saiu
       if (this.leit == 0) 
             this.notify(); //libera algum escritor (caso exista escritor bloqueado)
       System.out.println ("LeitorSaindo("+id+")");
    }
    
    //Entrada para escritores
    public synchronized void EntraEscritor (int id, int idLE) {
      try { 
        while ((this.leit > 0) || (this.escr > 0)) {
           System.out.println ("EscritorBloqueado("+id+")");
           wait();  //bloqueia pela condição lógica da aplicação 
        }
        this.escr++; //registra que há um escritor escrevendo
        System.out.println ("EscritorEscrevendo("+id+")");
        if(idLE==1){ //dependendo da variável passada como parâmetro para identificar se a thread é E ou LE, mudamos o valor de forma diferente.
            this.int_compartilhada++;
        }else{
            this.int_compartilhada = id;
        }
      } catch (InterruptedException e) { }
    }
    
    //Saída para escritores
    public synchronized void SaiEscritor (int id) {
       this.escr--; //registra que o escritor saiu
       notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
       System.out.println ("EscritorSaindo("+id+")");
    }
  }
  
  //Leitor
  class Leitor extends Thread {
    int id; //identificador da thread
    int delay; //atraso
    LE monitor;//objeto monitor para coordenar a lógica de execução das threads
  
    //Construtor
    Leitor (int id, int delayTime, LE m) {
      this.id = id;
      this.delay = delayTime;
      this.monitor = m;
    }
  
    //Método executado pela thread
    public void run () {
      double j=777777777.7, i;
      try {
        for (;;) {
          this.monitor.EntraLeitor(this.id);
          for (i=0; i<100000000; i++) {j=j/2;} //simulando alguma execução demorada
          this.monitor.SaiLeitor(this.id);
          sleep(this.delay); 
        }
      } catch (InterruptedException e) { return; }
    }
  }
  
  //Escritor
  class Escritor extends Thread {
    int id; //identificador da thread
    int delay; //atraso
    LE monitor; //objeto monitor para coordenar a lógica de execução das threads
  
    //Construtor
    Escritor (int id, int delayTime, LE m) {
      this.id = id;
      this.delay = delayTime;
      this.monitor = m;
    }
  
    //Método executado pela thread
    public void run () {
      double j=777777777.7, i;
      try {
        for (;;) {
          this.monitor.EntraEscritor(this.id, 0); 
          for (i=0; i<100000000; i++) {j=j/2;} //simulando alguma execução demorada
          this.monitor.SaiEscritor(this.id); 
          sleep(this.delay);
        }
      } catch (InterruptedException e) { return; }
    }
  }
  
  //Leitor/Escritor
  class LeitorEscritor extends Thread {
    int id; //identificador da thread
    int delay; //atraso
    LE monitor; //objeto monitor para coordenar a lógica de execução das threads
  
    //Construtor
    LeitorEscritor (int id, int delayTime, LE m) {
      this.id = id;
      this.delay = delayTime;
      this.monitor = m;
    }
  
    //Método executado pela thread
    public void run () {
      double j=777777777.7, k=777777777.7, i;
      try {
        for (;;) {
          this.monitor.EntraLeitor(id);
          for (i=0; i<100000000; i++) {j=j/2;} //simulando alguma execução demorada
          this.monitor.SaiLeitor(this.id); 
          sleep(this.delay); //atraso
          
          this.monitor.EntraEscritor(this.id, 1);
          for (i=0; i<100000000; i++) {j=j/2;} //simulando alguma execução demorada
          this.monitor.SaiEscritor(id);
          sleep(this.delay); //atraso
        }
      } catch (InterruptedException e) { return; }
    }
  }

  //Classe principal
  class Trabalho8 {
    static final int L = 6;
    static final int E = 6;
    static final int LE = 1;
  
    public static void main (String[] args) {
      int i;
      LE monitor = new LE();            //Monitor (objeto compartilhado entre leitores e escritores)
      Leitor[] l = new Leitor[L];       //Threads leitoras
      Escritor[] e = new Escritor[E];   //Threads escritoras
      LeitorEscritor[] le = new LeitorEscritor[3]; //Threads leitoras/escritoras

      for (i=0; i<L; i++) {
         l[i] = new Leitor(i+1, (i+1)*500, monitor);
         l[i].start(); 
      }
      for (i=0; i<E; i++) {
         e[i] = new Escritor(i+1, (i+1)*500, monitor);
         e[i].start(); 
      }
      for (i=0; i<LE; i++) {
        le[i] = new LeitorEscritor(i+1, (i+1)*500, monitor);
        le[i].start(); 
      }
    }
  }