#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
struct valueThreads{
	int inicio;
	int final;
	int array;
};
bool verificaPrimo(int n){
	int limite = sqrt(n) + 1;
	int flag = 0; 

	if (n % 2 == 0) 
		flag = 1; 
	else{ 
		for (int i = 3 ; i < limite && !flag; i+=2){ 
			if (n % i == 0)
				flag = 1; 
		}
	} 
	if (!flag) 
		return true;
	else
		return false; 
}

void sequencial(int n, int array[]){
	int nPrimos =0;
	for (int i = 0; i < n; ++i){
		if(true == verificaPrimo(array[i])){
			nPrimos++;
		}
	}
	printf("Sequencial -> %d numeros primos!\n", nPrimos);
}


//void *runThreads(int i){
//	pthread_exit(NULL);
//
//void threads(int m, int n, int array[]){
//	pthread_t nThreads[m];
//	int sliceVector = n/m;
//	for (int i = 0; i < m; i++){
//		pthread_create(&nThreads[i], NULL, runThreads, (void*)sliceVector);																																																																																																																																			
//	}
//}

void processos(int m, int n, int array[]){
	int sliceVector = n/m;
	int inicio = 0;
	int final = sliceVector;
	int i = 0;
	pid_t   childpid;
	do{
		if(i!=0){
			inicio += sliceVector;
			final += sliceVector;
		}
		if(i == m-1){
			final = n;
			childpid = fork();
		}
		else{
			childpid = fork();
		}
		i++;
	} while (i < m && childpid > 0);
	if(childpid == 0){
		int nPrimos;
		for (int i = inicio; i < final; i++){
			if (verificaPrimo(array[i]) == true){
				nPrimos++;
			}
		}
		printf("Processo PID %d -> %d numeros primos!\n", getpid(), nPrimos);
		exit(0);
	}
}





int randNumber(){
	return ((rand() / (RAND_MAX)) + rand() % 100)+3;
}

int	main(int argc, char const *argv[]){
	if(argc!=3){
		fprintf(stderr, "Estao faltando argumentos\n");
		return EXIT_FAILURE;
	}
	//Seed Random
	srand(time(NULL));
	int m = atoi(argv[1]);//threads e processos
	int n = atoi(argv[2]);//n vetores
	int array[n];
	clock_t t;
	double tempo;

	FILE* pFile;
	FILE* sFile;

	pFile = fopen ("processos.txt","a+");
	sFile = fopen ("sequencial.txt","a+");


	for (int i = 0; i < n; i++){
		array[i] = randNumber();
		printf("%d ", array[i]);
	}
	printf("\n");

	//Sequencial
	t = clock();
	sequencial(n, array);
	t = clock() - t;
	tempo = (double)(t) / CLOCKS_PER_SEC;
	printf("%f\n", tempo);
	fprintf(sFile, "%f\n", tempo);
	fclose (sFile);

	//Threads e Processos
	if(m > 0 && m < n){
		t = clock();
		processos(m, n, array);
		t = clock() - t;
		tempo = (double)(t) / CLOCKS_PER_SEC;
		printf("%f\n", tempo);
		fprintf(pFile, "%f\n", tempo);
		fclose (pFile);
		//threads(m, n, array);
	}
	return 0;
}