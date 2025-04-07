/*
Aluno: Leonardo de Barros Fernandes
DRE: 122117670
Código: Calculo do produto interno de maneira concorrente e verifica a variação relativa
em relação ao produto sequencial.
*/
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 

#define VERSOES

//variaveis globais
long int n;//dimensão do vetor
float *vet1;//primeiro vetor de elementos
float *vet2;//segundo vetor de elementos
int nthreads;// numero de threads;


//funcao executada pelas threads
//estrategia de divisao de tarefas: blocos de n/nthreads elementos
void *Prod_Inter (void *tid) {
    long int id = (long int) tid; //identificador da thread
    int ini, fim, bloco; //auxiliares para divisao do vetor em blocos
    float prod_int=0, *ret; //produto interno concorrente
    
    bloco = n/nthreads; //tamanho do bloco de dados de cada thread
    ini = id*bloco; //posicao inicial do vetor
    fim = ini + bloco; //posicao final do vetor
    if (id==(nthreads-1)) fim = n; //a ultima thread trata os elementos restantes no caso de divisao nao exata
  
    //realiza o calculo do produto interno
    for(int i=ini; i<fim; i++) {
       prod_int = prod_int + (vet1[i] * vet2[i]);
    }
  
    //retorna o resultado da soma
    ret = (float*) malloc(sizeof(float));
    if (ret!=NULL) *ret = prod_int;
    else printf("--ERRO: malloc() thread\n");
    pthread_exit((void*) ret);
}

//funcao principal do programa
int main(int argc, char *argv[]) {
    FILE *arq; //arquivo de entrada
    size_t ret; //retorno da funcao de leitura no arquivo de entrada
    double prod_int_seq; //produto interno registrado no arquivo
    double variancia_relativa;// calculo da variância entre sequencial e concorrente
    float prod_int_global; //resultado do produto interno concorrente
    float *prod_int_retorno_threads; //auxiliar para retorno das threads
  
    pthread_t *tid_sistema; //vetor de identificadores das threads no sistema
  
    //valida e recebe os valores de entrada
    if(argc < 3) { 
        printf("Use: %s <arquivo de entrada> <numero de threads> \n", argv[0]); 
        exit(-1); 
    }
  
    //abre o arquivo de entrada com os vetores para o calculo do produto interno
    arq = fopen(argv[1], "rb");
    if(arq==NULL) { 
        printf("--ERRO: fopen()\n"); 
        exit(-1); 
    }
  
    //le o tamanho dos vetores (primeira linha do arquivo)
    ret = fread(&n, sizeof(long int), 1, arq);
    if(!ret) {
       fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
       return 3;
    }
  
    //aloca espaco de memoria e carrega os vetores de entrada
    vet1 = (float*) malloc (sizeof(float) * n);
    vet2 = (float*) malloc (sizeof(float) * n);
    if(vet1 == NULL || vet2 == NULL) { 
        printf("--ERRO: malloc()\n"); 
        exit(-1); 
    }
    ret = fread(vet1, sizeof(float), n, arq);
    ret = fread(vet2, sizeof(float), n, arq);

    if(ret < n) {
       fprintf(stderr, "Erro de leitura dos elementos do vetor\n");
       return 4;
    }
  
    //le o numero de threads da entrada do usuario 
    nthreads = atoi(argv[2]);
    //limita o numero de threads ao tamanho do vetor
    if(nthreads>n) nthreads = n;
  
    //aloca espaco para o vetor de identificadores das threads no sistema
    tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(tid_sistema==NULL) { 
        printf("--ERRO: malloc()\n"); 
        exit(-1); 
    }
  
    //cria as threads
    for(long int i=0; i<nthreads; i++) {
      if (pthread_create(&tid_sistema[i], NULL, Prod_Inter, (void*) i)) {
         printf("--ERRO: pthread_create()\n"); exit(-1);
      }
    }
    //espera todas as threads terminarem e calcula a soma total das threads
    prod_int_global=0;
    for(int i=0; i<nthreads; i++) {
       if (pthread_join(tid_sistema[i], (void *) &prod_int_retorno_threads)) {
          printf("--ERRO: pthread_join()\n"); exit(-1);
       }
       prod_int_global += *prod_int_retorno_threads;
       free(prod_int_retorno_threads);
    }
  
    //imprime os resultados
    printf("\n");
    printf("Produto Interno Concorrente = %.26f\n", prod_int_global);
    //le o produto interno registrado no arquivo
    ret = fread(&prod_int_seq, sizeof(double), 1, arq); 
    printf("\nProduto Interno Sequencial = %.26lf\n", prod_int_seq);

    variancia_relativa = (prod_int_seq - prod_int_global) / prod_int_seq;
    printf("\nVariância Relativa = %.10lf\n", variancia_relativa);
  
    //desaloca os espacos de memoria
    free(vet1);
    free(vet2);
    free(tid_sistema);
    //fecha o arquivo
    fclose(arq);
    return 0;
  }