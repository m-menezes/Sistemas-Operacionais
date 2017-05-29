#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/wait.h>
#include <time.h>

void bin_tree(int nivel, int nlvl){
  pid_t filho = fork();
  int status;

  if (filho == -1){
    printf("Fork Error.\n");
  }
  else if (filho == 0) {
    /* Filho */
      printf("Filho - %d -:- Pai - %d.\n", getpid(),getppid());

    if (nivel < nlvl){
      bin_tree(nivel+1, nlvl);
      bin_tree(nivel+1, nlvl);
    }
    exit(EXIT_SUCCESS);
  }
  else{
    /* Pai */
    waitpid(filho, &status, 0);
  }
}
void sequencial(int nivel, int nlvl){
  pid_t filho = fork();
  int status;
  if (filho == -1){
    printf("Fork Error.\n");
  }
  else if (filho == 0) {
    /* Filho */
      printf("Filho - %d -:- Pai - %d.\n", getpid(),getppid());
    if (nivel < nlvl){
      sequencial(nivel+1, nlvl);
    }
    exit(EXIT_SUCCESS);
  }
  else {
    /* Pai */
    waitpid(filho, &status, 0);
  }
}

int main(int argc, char *argv[]){
  clock_t t;
  double tempo;
  int nlvl = atoi(argv[1]);
  int n = (pow(2,nlvl+1))-1;
  //Criação arquivos dos clocks
  FILE* afile;
  FILE* sfile;
  afile = fopen ("arvore_clocks.csv","a+");
  sfile = fopen ("sequencial_clocks.csv","a+");
  //Se não receber dois argumentos
  if (argc != 2) {
    printf("Erro nos argumentos.");
    return EXIT_FAILURE;
  }
  if (nlvl < 0) {
    printf("Valor negativo.\n");
    return EXIT_FAILURE;
  }
  //Arvore Binaria
    t = clock();
      printf("Execução com %d niveis.\n", nlvl);
      bin_tree(0, nlvl);
    t = clock() - t;
    tempo = ((float)t)/CLOCKS_PER_SEC;
    fprintf(afile, "%f;", tempo);
    fclose (afile);

  //Sequencial
    t = clock();
      printf("Execução com %d niveis.\n", n);
      sequencial(1, n);
    t = clock() - t;
    tempo = ((float)t)/CLOCKS_PER_SEC;
    fprintf(sfile, "%f;", tempo);
    fclose (sfile);

  printf("Forks finalizados.\n");
  return EXIT_SUCCESS;
}
