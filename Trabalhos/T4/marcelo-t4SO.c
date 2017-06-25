#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

sem_t varSem;
int* array;
int numInput = 0;
int comprados = 0;
int falhas = 0;
int tenta =0;
int randNumber(int n){
	return rand()%n;
}

void* vendas(void *idcomprador){
	int nVendidos = 0;
	while(numInput != comprados){
		//gera um valor random para o numero do ingresso
		int number = randNumber(numInput);
		//lock var
		sem_wait(&varSem);
		//Se ingresso nao foi vendido ou reservado
		if(array[number] == 0){
			array[number] = -1;
			//printf("Sleeping. %d\n", number);
			usleep(10);
			//Compra aceita
			if(randNumber(20) % 2 == 0){
				array[number] = 1;
				comprados++;
				nVendidos++;
				sem_post(&varSem);
				printf("Comprador - %d\t ||\tCompra Sucess\tIngresso - %d\n", idcomprador, number);
				break;
			}
			else{
				//Compra cancelada
				falhas++;
				array[number] = 0;
				sem_post(&varSem);
				printf("Comprador - %d\t ||\tCompra Fail\tIngresso - %d\n", idcomprador, number);
				break;
			}
		}
		//Se esta reservado ou foi vendido
		else if(array[number]==1 || array[number]==-1){
			printf("Comprador - %d\t ||\tVendido\t\tIngresso - %d\n", idcomprador, number);
			//unlock var
			sem_post(&varSem);
		}
	}
}

int* criaVetor(int n){
	//aloca e zera posições vetor
	int* criaVetor = (int*) calloc(n, sizeof(int));
	//calloc zera vetor, mas para garantr o vetor é zerado novamente
	for (int i = 0; i < n; ++i){
		criaVetor[i] = 0;
	}
	return criaVetor;
}

int main(int argc, char* argv[]){
	//Cria seed para random
	srand(time(NULL));

	int tentativas = 0;
	//Verifica argumentos enviados
	if(argc != 2 || atoi(argv[1]) <= 0){
		fprintf(stderr, "Erro nos argumentos:\n--Falta de argumentos ou numero de bilhetes igual ou menor que zero.\n");
		return EXIT_FAILURE;
	}
	
	//Recebe valor do cast argv
	int n = atoi(argv[1]);
	//Random do valor de threads entre 1 e 5
	int threads = n;
	printf("Threads - %d\n", threads);
	printf("====================================================\n");

	//Salva valor em uma variavel global
	numInput = n;
	array = criaVetor(atoi(argv[1]));
	sem_init(&varSem, 0, 1);
	pthread_t comprador;

	//Cria Threads
	while(comprados < n) {
		//Atraso na chegada do comprador
		usleep(10);
		tentativas++;
		pthread_create(&comprador, NULL, vendas, (void *)(intptr_t)tentativas);
	}
	//Join Threads
	for (int i = 0; i < tentativas; i++) {
		pthread_join(comprador, NULL);
	}
	usleep(100);
	int semIngresso = (tentativas-falhas)-1;
	printf("====================================================\n");
	for (int i = 0; i < n; ++i){
		printf("Ingresso - %d\t ||\tVendido - Conteúdo\t(%d)\n", i, array[i]);
	}
	printf("====================================================\n");
	printf("Compradores totais - %d\n", tentativas - 1);
	printf("====================================================\n");
	printf("Compradores sem ingresso - %d\n", semIngresso-n);
	printf("====================================================\n");
	printf("Compras canceladas - %d\n", falhas);
	printf("====================================================\n");
	printf("Todos ingressos vendidos!\n");

	return 0;
}