#pragma once
#include "Simulated_Annealing.h"
#include <time.h>
void GreedyRandomizedConstruction(ponto *solucao_parcial, node *lista_ponto, int facilidades);
void GRASP_cobrindo_ponto(node *lista_ponto, int x, int y);
void GRASP_descobrindo_ponto(node *lista_ponto, int x, int y);
int GRASP_demanda_coberta(node *lista_ponto, ponto *solucao);
int GRASP_demanda_coberta(node *lista_ponto, int x, int y);
void LocalSearch(ponto *solucao_parcial, node *lista_ponto);
void Update_Soluction(ponto *solucao_parcial, node *lista_pontos, ponto *solucao);

void GRASP(node *lista_pontos, int facilidades, int max_iter) {

	srand(time(NULL));

	/*solu��o*/
	ponto *solucao;
	solucao = (ponto *)malloc(sizeof(ponto));

	inicia_pontos(solucao);

	/*solu��o parcial - modificada a cada itera��o do GRASP*/
	ponto *solucao_parcial;
	solucao_parcial = (ponto *)malloc(sizeof(ponto));

	inicia_pontos(solucao_parcial);

	/*inicio grasp*/
	for (int i = 0; i < max_iter; i++) {
		
		/*gera a solu��o inicial*/
		GreedyRandomizedConstruction(solucao_parcial ,lista_pontos, facilidades);
		/*busca local na solu��o afim de buscar o melhor local*/
		LocalSearch(solucao_parcial, lista_pontos);
		/*atualiza a solu��o se ela for melhor, se nao mantem*/
		Update_Soluction(solucao_parcial, lista_pontos, solucao);
		
		zera_pontos_cobertos(lista_pontos);
		libera_pontos(solucao_parcial);
		inicia_pontos(solucao_parcial);
	}

	printf("Solucao Final:\n");
	exibe_pontos(solucao);
	printf("Demanda Coberta: %d\n", GRASP_demanda_coberta(lista_pontos, solucao));
}

/*Gera a solu��o inicial aleatoria*/
void GreedyRandomizedConstruction(ponto *solucao_parcial, node *lista_ponto, int facilidades) {

	int id = 0;

	for (int i = 0; i < facilidades; i++) {
		/*sorteia um id que ainda nao foi sorteado*/
		do {
			id = rand() % lista_ponto->tam;
			/*enquanto conter na lista ponto um ponto ja sorteado � sorteado outro id*/
		} while (contens_pontos(solucao_parcial, numPos(lista_ponto, id).cord_x, numPos(lista_ponto, id).cord_y));
		/*insiro na lista o novo ponto*/
		insereFim_pontos(solucao_parcial, numPos(lista_ponto, id).cord_x, numPos(lista_ponto, id).cord_y);
		/*manda cobrir os postos q estao no raio*/
		GRASP_cobrindo_ponto(lista_ponto, numPos(lista_ponto, id).cord_x, numPos(lista_ponto, id).cord_y);
	}
	/*solu��o inicial gerada com o tamanho igual ao numero de facilidades disponiveis*/

	//printf("Solu��o inicial\n");

	//exibe_pontos(solucao_parcial);

	/*vejo quais os pontos foram cobertos por essa solu��o inicial e a demanda coberta*/
	//printf("Demanda coberta: %d\n", GRASP_demanda_coberta(lista_ponto, solucao_parcial));
}

int GRASP_demanda_coberta(node *lista_ponto, ponto *solucao) {
	
	int demanda_coberta = 0;
	int id = 0;
	int aux_id;
	/*calculando a demanda coberta*/
	for (int i = 0; i < solucao->tam; i++) {
		id = procura(lista_ponto, numPos_pontos(solucao, i).cord_x, numPos_pontos(solucao, i).cord_y);
		for (int j = 0; j < numPos(lista_ponto, id).raio->tam; j++) {

			aux_id = procura(lista_ponto, numPos_pontos(numPos(lista_ponto, id).raio, j).cord_x, numPos_pontos(numPos(lista_ponto, id).raio, j).cord_y);

			demanda_coberta += numPos(lista_ponto, aux_id).demanda;
		}		
	}
	return demanda_coberta;
}

int GRASP_demanda_coberta(node *lista_ponto, int x, int y) {

	/*calculando a demanda coberta*/
	return numPos(lista_ponto, procura(lista_ponto, x, y)).demanda;
}

void GRASP_cobrindo_ponto(node *lista_ponto, int x, int y) {

	int id = 0;
	int idx = 0;

	/*procura na lista de pontos o ponto que tem a facilidade, esse ponto eh da solu��o*/
	id = procura(lista_ponto, x, y);

	/*iterajo em todos os pontos que estao dentro do raio*/
	for (int j = 0; j < numPos(lista_ponto, id).raio->tam; j++) {
		/*procuro o ponto coberto na lista de pontos*/
		idx = procura(lista_ponto, (numPos_pontos((numPos(lista_ponto, id).raio), j)).cord_x, (numPos_pontos((numPos(lista_ponto, id).raio), j)).cord_y);
		/*incremento falando que ele foi coberto -  isso me da a quantidade de vezes que ele foi coberto*/
		incrementa_coberto(lista_ponto, idx);
	}	
}

void GRASP_descobrindo_ponto(node *lista_ponto, int x, int y) {

	int id = 0;
	int idx = 0;

	/*procura na lista de pontos o ponto que tem a facilidade, esse ponto eh da solu��o*/
	id = procura(lista_ponto, x, y);

	/*iterajo em todos os pontos que estao dentro do raio*/
	for (int j = 0; j < numPos(lista_ponto, id).raio->tam; j++) {
		/*procuro o ponto coberto na lista de pontos*/
		idx = procura(lista_ponto, (numPos_pontos((numPos(lista_ponto, id).raio), j)).cord_x, (numPos_pontos((numPos(lista_ponto, id).raio), j)).cord_y);
		/*incremento falando que ele foi coberto -  isso me da a quantidade de vezes que ele foi coberto*/
		decrementa_coberto(lista_ponto, idx);
	}
}

void LocalSearch(ponto *solucao_parcial, node *lista_ponto) {

	int aux_id = 0;

	int max_demanda = GRASP_demanda_coberta(lista_ponto, solucao_parcial);

	/*sorteio um ponto da sulu��o inicial*/
	int id = rand() % solucao_parcial->tam;

	/*descobre os pontos que ele cobre*/
	GRASP_descobrindo_ponto(lista_ponto, numPos_pontos(solucao_parcial, id).cord_x, numPos_pontos(solucao_parcial, id).cord_y);

	/*retiro esse ponto da solu��o e decremento os pontos que ela cobria*/
	retira_pontos(solucao_parcial, id);
	
	int demanda = GRASP_demanda_coberta(lista_ponto, solucao_parcial);
	int aux_demanda = 0;
	/*procurar o melhor ponto a se colocar a facilidade com a solu��o parcial*/
	for (int i = 0; i < lista_ponto->tam; i++) {
		aux_demanda = demanda + GRASP_demanda_coberta(lista_ponto, numPos(lista_ponto, i).cord_x, numPos(lista_ponto, i).cord_y);
		if (max_demanda < aux_demanda ) {
			aux_id = i;
			max_demanda = aux_demanda;
		}
	}

	insereFim_pontos(solucao_parcial, numPos(lista_ponto, aux_id).cord_x, numPos(lista_ponto, aux_id).cord_y);
	GRASP_cobrindo_ponto(lista_ponto, numPos(lista_ponto, aux_id).cord_x, numPos(lista_ponto, aux_id).cord_y);

	//printf("Solu��o depois da busca local\n");

	//exibe_pontos(solucao_parcial);

	/*vejo quais os pontos foram cobertos por essa solu��o inicial e a demanda coberta*/
	//printf("Demanda coberta: %d\n", GRASP_demanda_coberta(lista_ponto, solucao_parcial));
}

void Update_Soluction(ponto *solucao_parcial, node *lista_pontos, ponto *solucao) {

	int max_demanda = GRASP_demanda_coberta(lista_pontos, solucao_parcial);
	zera_pontos_cobertos(lista_pontos);
	for (int i = 0; i < solucao_parcial->tam;i++) {
		GRASP_cobrindo_ponto(lista_pontos,numPos_pontos(solucao_parcial, i).cord_x, numPos_pontos(solucao_parcial, i).cord_y);
	}
	int demanda = GRASP_demanda_coberta(lista_pontos, solucao);
	if (max_demanda > demanda){
		libera_pontos(solucao);
		inicia_pontos(solucao);
		for (int i = 0; i < solucao_parcial->tam; i++) {
			insereFim_pontos(solucao, numPos_pontos(solucao_parcial, i).cord_x, numPos_pontos(solucao_parcial, i).cord_y);
		}
	}
}
