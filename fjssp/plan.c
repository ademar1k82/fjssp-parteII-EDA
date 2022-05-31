/**
Descrição:          Ficheiro com todas as funções aos planos de escalonamento - funcionalidade 8
					FJSSP - parte 2
Desenvolvedor(es):   Ademar Valente (23155)
e-mail: a23155@alunos.ipca.pt
github: ademar1k82
Última atualização:  31/05/2022
Criação:             28/05/2022
*/

#include <stdio.h>
#include <stdlib.h>
#include "header.h"


/**
* @brief	Inicia um novo plano, com todos os campos vazios
* @param	jid		Identificador do trabalho
* @param	oid		Identificador da operação
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool startPlan(Cell plan[][MAX_TIME], int jid, int oid)
{
	for (int i = 0; i < NUMBER_MACHINES; i++)
	{
		for (int j = 0; j < MAX_TIME; j++)
		{
			plan[i][j].jobID = jid;
			plan[i][j].operationID = oid;
			plan[i][j].initialTime = 0;
			plan[i][j].finalTime = 0;
		}
	}
}


/**
* @brief	Preenche uma célula do plano
* @param	mid		Identificador da máquina
* @param	jid		Identificador do trabalho
* @param	oid		Identificador da operação
* @param	it		Tempo inicial desta célula
* @param	ft		Tempo final desta célula
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool fillCells(Cell p[][MAX_TIME], int mid, int jid, int oid, int it, int ft)
{
	// não permite ativar células que já estão ativas
	if (searchActiveCells(p, mid, it, ft))
	{
		return false;
	}

	// preenche o intervalo de células disponíveis para este escalonamento
	for (int i = it; i < ft; i++)
	{
		// mid - 1 porque os IDs das máquinas começam em 1 e a matriz do plano começa em 0
		p[mid - 1][i].jobID = jid;
		p[mid - 1][i].operationID = oid;
		p[mid - 1][i].initialTime = it;
		p[mid - 1][i].finalTime = ft;
	}

	return true;
}


/**
* @brief	Procurar em um intervalo de células do plano, se existem células ativas
* @param	mid		Identificador da máquina
* @param	it		Tempo inicial desta célula
* @param	ft		Tempo final desta célula
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool searchActiveCells(Cell p[][MAX_TIME], int mid, int it, int ft)
{
	for (int i = it; i < ft; i++)
	{
		if (p[mid-1][i].jobID > -1 || p[mid-1][i].operationID > -1)
		{
			return true;
		}
	}

	return false;
}


/**
* @brief	Mostrar plano de escalonamento na consola
* @param	p		Plano a ser mostrado
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool displayPlan(Cell p[][MAX_TIME])
{
	printf("\n");
	for (int i = 0; i < NUMBER_MACHINES; i++)
	{
		for (int j = 0; j < MAX_TIME; j++)
		{
			if (p[i][j].jobID > -1 && p[i][j].operationID > -1)
			{
				printf("|j%do%d", p[i][j].jobID, p[i][j].operationID);
			}
			else
			{
				printf("|    ");
			}
		}
		printf("|\n");
	}
	printf("\n");
}