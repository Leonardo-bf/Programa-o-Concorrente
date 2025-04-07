/*
Aluno: Leonardo de Barros Fernandes
DRE: 122117670
Código: Calculo do produto interno de maneira sequencial
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100 //valor maximo de um elemento do vetor
#define TEXTO 

int main(int argc, char*argv[]){
    float *vet1, *vet2; //vetores de entrada
    long int n; //qtd de elementos do vetor
    float elem; //valor gerado para incluir no vetor
    double prod_int; //resultado do calculo do produto dos vetores
    int fator = 1; //fator multiplicador para gerar números negativos
    FILE * descritorArquivo; //descritor do arquivo de saída
    size_t ret;

    if(argc < 3){
        fprintf(stderr, "Digite: %s <dimensao> <nome do arquivo de saida>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);// pega da entrada a dimensão dos vetores

    vet1 = (float *) malloc(n * sizeof(float));// aloca memória para o vetor 1
    vet2 = (float *) malloc(n * sizeof(float));// aloca memória para o vetor 2
   
    if(!vet1 || !vet2) {
        fprintf(stderr, "Erro de alocao da memoria do vetor\n");
        return 2;
    }

    //preenche o vetor com valores float aleatorios
   srand(time(NULL));
   for(long int i=0; i<n; i++) {
        elem = (rand() % MAX)/3.0 * fator;
        vet1[i] = elem;
        fator*=-1;// garante que alguns valores sejam negativos
   }

   for(long int i=0; i<n; i++) {
    elem = (rand() % MAX)/3.0 * fator;
    vet2[i] = elem;
    }

    for(long int i=0; i<n; i++) {
        prod_int += (vet1[i] * vet2[i]);
        // realiza a soma após a multiplicação
   }

    //imprimir na saida padrao o vetor gerado
    #ifdef TEXTO
    fprintf(stdout, "%ld\n", n);
    for(long int i=0; i<n; i++) {
        fprintf(stdout, "%f ",vet1[i]);
    }
    fprintf(stdout, "\n");
    for(long int i=0; i<n; i++) {
        fprintf(stdout, "%f ",vet2[i]);
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "%lf\n", prod_int);
    #endif

    //escreve o vetor no arquivo
    //abre o arquivo para escrita binaria
    descritorArquivo = fopen(argv[2], "wb");
    if(!descritorArquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 3;
    }
    //escreve a dimensao
    ret = fwrite(&n, sizeof(long int), 1, descritorArquivo);
    //escreve os elementos do vetor1
    ret = fwrite(vet1, sizeof(float), n, descritorArquivo);
    //escreve os elementos do vetor2
    ret = fwrite(vet2, sizeof(float), n, descritorArquivo);
    if(ret < n) {
        fprintf(stderr, "Erro de escrita no  arquivo\n");
        return 4;
    }
    //escreve o somatorio
    ret = fwrite(&prod_int, sizeof(double), 1, descritorArquivo);

    //finaliza o uso das variaveis
    fclose(descritorArquivo);
    free(vet1);
    free(vet2);
    return 0;


}



