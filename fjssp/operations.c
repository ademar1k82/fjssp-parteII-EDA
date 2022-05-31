/**
Descrição:          Ficheiro com todas as funções relativas às operações
   				    FJSSP - parte 2
Desenvolvedor(es):   Ademar Valente (23155)
e-mail: a23155@alunos.ipca.pt
github: ademar1k82
Última atualização:  31/05/2022
Criação:             22/05/2022
*/

#include <stdio.h>
#include <stdlib.h>
#include "header.h"


/**
 * @brief Carrega dados das operações de um ficheiro CSV para uma lista em memória
 * @param fileName		Nome do ficheiro
 * @return A lista de operações do ficheiro CSV
 */
Operation* loadOperations(char* fileName)
{
	char line[FILE_LINE_SIZE];
	int a = 0, b = 0, c = 0;

	Operation* operation = NULL;
	Operation* operations = NULL;

	FILE* file = fopen(fileName, "r");
	if (file == NULL)
	{
		return NULL;
	}

	while (!feof(file))
	{
		if (fgets(line, FILE_LINE_SIZE, file) != NULL)
		{
			// exemplo: 1;1;1
			sscanf(line, "%d;%d;%d", &a, &b, &c);
			operation = newOperation(a, b, c);
			operations = insertOperation_AtStart(operations, operation);
		}
	}

	fclose(file);

	return operations;
}


/**
* @brief	Criar nova operação
* @param	id			Identificador da operação
* @param	jobID		Identificador do trabalho
* @param	position	Posição da operação relativamente ao trabalho
* @return	Nova operação
*/
Operation* newOperation(int id, int jobID, int position)
{
	Operation* new = (Operation*)malloc(sizeof(Operation));
	if (new == NULL) // se não houver memória para alocar
	{
		return NULL;
	}

	new->id = id;
	new->jobID = jobID;
	new->position = position;
	new->next = NULL;

	return new;
}


/**
* @brief	Inserir nova operação no início da lista de operações
* @param	head	Lista de operações
* @param	new		Nova operação
* @return	Lista de operações atualizada
*/
Operation* insertOperation_AtStart(Operation* head, Operation* new)
{
	if (searchOperation(head, new->id)) // não permitir inserir uma nova com o mesmo ID
	{
		return NULL;
	}

	if (head == NULL) // se a lista estiver vazia
	{
		head = new;
	}
	else // se existir algum elemento na lista
	{
		new->next = head;
		head = new;
	}

	return head;
}


/**
* @brief	Atualizar a posição de uma operação X pela posição de uma operação Y, e vice-versa
* @param	head			Apontador para a lista de execuções
* @param	xOperationID	Identificador de uma operação qualquer X
* @param	yOperationID	Identificador de uma operação qualquer Y
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool updatePosition(Operation** head, int xOperationID, int yOperationID)
{
	if (*head == NULL) // se lista está vazia
	{
		return false;
	}

	if (xOperationID == yOperationID) // se forem iguais
	{
		return false;
	}

	Operation* xOperation = NULL;
	Operation* yOperation = NULL;

	xOperation = getOperation(*head, xOperationID);
	yOperation = getOperation(*head, yOperationID);

	if (xOperation == NULL || yOperation == NULL) // se as operações não foram encontradas
	{
		return false;
	}

	Operation* current = *head;

	while (current != NULL)
	{
		if (current->id == xOperation->id) // trocar a posição da operação X pela posição da operação Y
		{
			current->position = yOperation->position;
		}
		if (current->id == yOperation->id) // trocar a posição da operação Y pela posição da operação X
		{
			current->position = xOperation->position;
		}

		current = current->next;
	}

	return true;
}


/**
* @brief	Remover uma operação da lista de operações
* @param	head	Apontador para a lista de operações
* @param	id		Identificador da operação
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool deleteOperation(Operation** head, int id)
{
	if (*head == NULL)
	{
		return false;
	}

	Operation* current = *head;
	Operation* previous = NULL;

	if (current != NULL && current->id == id) { // se o elemento que será apagado é o primeiro da lista
		*head = current->next;
		free(current);
		return true;
	}

	while (current != NULL && current->id != id) // procurar o elemento a ser apagado
	{
		previous = current;
		current = current->next;
	}

	if (current == NULL) // se o elemento não foi encontrado
	{
		return false;
	}

	previous->next = current->next; // desassociar o elemento da lista
	free(current);

	return true;
}


/**
* @brief	Remover operação pelo identificador do trabalho
* @param	head			Apontador para a lista de operações
* @param	jobID			Identificador do trabalho
* @return	Inteiro com o identificador da operação removida
*/
int deleteOperation_ByJob(Operation** head, int jobID)
{
	if (*head == NULL) // se a lista estiver vazia
	{
		return -1;
	}

	Operation* current = *head;
	Operation* previous = NULL;
	int operationDeleted = 0;

	if (current != NULL && current->jobID == jobID) { // se o elemento que será apagado é o primeiro da lista
		operationDeleted = current->id;
		*head = current->next;
		free(current);

		return operationDeleted;
	}

	while (current != NULL && current->jobID != jobID) // procurar o elemento a ser apagado
	{
		previous = current;
		current = current->next;
	}

	if (current == NULL) // se o elemento não foi encontrado
	{
		return -1;
	}

	operationDeleted = current->id;
	previous->next = current->next; // desassociar o elemento da lista
	free(current);

	return operationDeleted;
}


/**
* @brief	Armazenar lista de operações em ficheiro binário
* @param	fileName	Nome do ficheiro para armazenar a lista
* @param	head		Lista de operações
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool writeOperations(char fileName[], Operation* head)
{
	if (head == NULL) // se lista está vazia
	{
		return false;
	}

	FILE* file = NULL;

	if ((file = fopen(fileName, "wb")) == NULL) // erro ao abrir o ficheiro
	{
		return false;
	}

	Operation* current = head;
	FileOperation currentInFile; // é a mesma estrutura mas sem o campo *next, uma vez que esse campo não é armazenado no ficheiro

	while (current != NULL)
	{
		currentInFile.id = current->id;
		currentInFile.jobID = current->jobID;
		currentInFile.position = current->position;
		fwrite(&currentInFile, sizeof(FileOperation), 1, file); // guarda cada registo da lista no ficheiro

		current = current->next;
	}

	fclose(file);

	return true;
}


/**
* @brief	Ler lista de operações de ficheiro binário
* @param	fileName	Nome do ficheiro para ler a lista
* @return	Lista de operações
*/
Operation* readOperations(char fileName[])
{
	FILE* file = NULL;

	if ((file = fopen(fileName, "rb")) == NULL) // erro ao abrir o ficheiro
	{
		return NULL;
	}

	Operation* head = NULL;
	Operation* current = NULL;
	FileOperation currentInFile; // é a mesma estrutura mas sem o campo *next, uma vez que esse campo não é armazenado no ficheiro

	while (fread(&currentInFile, sizeof(FileOperation), 1, file)) // lê todos os registos do ficheiro e guarda na lista
	{
		current = newOperation(currentInFile.id, currentInFile.jobID, currentInFile.position);
		head = insertOperation_AtStart(head, current);
	}

	fclose(file);

	return head;
}


/**
* @brief	Mostrar a lista de operações na consola
* @param	head	Lista de operações
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool displayOperations(Operation* head)
{
	if (head == NULL) // se a lista estiver vazia
	{
		return false;
	}

	Operation* current = head;

	while (current != NULL)
	{
		printf("ID: %d, ID Trabalho: %d, Posição: %d;\n", current->id, current->jobID, current->position);
		current = current->next;
	}

	return true;
}


/**
* @brief	Procurar por uma operação na lista de operações
* @param	head	Lista de operações
* @param	id		Identificador da operação
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool searchOperation(Operation* head, int id)
{
	if (head == NULL) // se lista está vazia
	{
		return false;
	}

	Operation* current = head;

	while (current != NULL)
	{
		if (current->id == id)
		{
			return true;
		}
		current = current->next;
	}

	return false;
}


/**
* @brief	Procurar por uma operação através do identificador do trabalho, na lista de operações
* @param	head			Lista de operações
* @param	jobID			Identificador do trabalho
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool searchOperation_ByJob(Operation* head, int jobID)
{
	if (head == NULL) // se a lista estiver vazia
	{
		return false;
	}

	Operation* current = head;

	while (current != NULL)
	{
		if (current->jobID == jobID)
		{
			return true;
		}
		current = current->next;
	}

	return false;
}


/**
* @brief	Obter uma operação da lista de operações
* @param	head	Lista de operações
* @param	id		Identificador da operação
* @return	Operação encontrada (ou NULL se não encontrou)
*/
Operation* getOperation(Operation* head, int id)
{
	if (head == NULL) // se lista está vazia
	{
		return NULL;
	}

	Operation* current = head;

	while (current != NULL)
	{
		if (current->id == id)
		{
			Operation* operation = newOperation(current->id, current->jobID, current->position); // criar cópia da operação
			return operation;
		}
		current = current->next;
	}

	return NULL;
}


/**
* @brief	Obter o mínimo de tempo necessário para completo um trabalho e as respetivas execuções
* @param	operations		Lista de operações
* @param	executions		Lista de execuções
* @param	jobID			Identificador do trabalho
* @param	minExecutions	Apontador para a lista de execuções a ser devolvida, relativamente ao tempo mínimo
* @return	Quantidade de tempo
*/
int getMinTime_ToCompleteJob(Operation* operations, Execution* executions, int jobID, Execution** minExecutions)
{
	if (operations == NULL || executions == NULL) // se as listas estiverem vazias
	{
		return -1;
	}

	int time = 999;
	int counter = 0;

	Operation* currentOperation = operations;
	Execution* currentExecution = executions;
	Execution* minExecution = NULL;

	while (currentOperation != NULL) // percorrer lista de operações
	{
		if (currentOperation->jobID == jobID) // se encontrar o job relativo à operação
		{
			while (currentExecution != NULL) // percorrer lista de execução de operações
			{
				if (currentExecution->operationID == currentOperation->id) // se encontrar a execução de operação relativa à operação
				{
					// guardar execução de operação com menor tempo de utilização
					if (currentExecution->runtime < time)
					{
						time = currentExecution->runtime;
						minExecution = newExecution(currentExecution->operationID, currentExecution->machineID, currentExecution->runtime);
					}
				}

				currentExecution = currentExecution->next;
			}

			*minExecutions = insertExecution_AtStart_AtList(*minExecutions, minExecution);

			// repor lista percorrida (currentExecution), para que se for necessário voltar a percorrer o while da execução de operações de novo
			currentExecution = NULL;
			currentExecution = executions;
			counter += time; // acumular o tempo de utilização de cada execução de operação
			time = 99990; // resetar tempo para a próxima iteração
		}

		currentOperation = currentOperation->next;
	}

	return counter;
}


/**
* @brief	Obter o máximo de tempo necessário para completo um trabalho e as respetivas execuções
* @param	operations		Lista de operações
* @param	executions		Lista de execuções
* @param	jobID			Identificador do trabalho
* @param	maxExecutions	Apontador para a lista de execuções a ser devolvida, relativamente ao tempo máximo
* @return	Quantidade de tempo
*/
int getMaxTime_ToCompleteJob(Operation* operations, Execution* executions, int jobID, Execution** maxExecutions)
{
	if (operations == NULL || executions == NULL) // se as listas estiverem vazias
	{
		return -1;
	}

	int time = 0;
	int counter = 0;

	Operation* currentOperation = operations;
	Execution* currentExecution = executions;
	Execution* maxExecution = NULL;

	while (currentOperation != NULL) // percorrer lista de operações
	{
		if (currentOperation->jobID == jobID) // se encontrar o job relativo à operação
		{
			while (currentExecution != NULL) // percorrer lista de execução de operações
			{
				if (currentExecution->operationID == currentOperation->id) // se encontrar a execução de operação relativa à operação
				{
					// guardar execução de operação com maior tempo de utilização
					if (currentExecution->runtime > time)
					{
						time = currentExecution->runtime;
						maxExecution = newExecution(currentExecution->operationID, currentExecution->machineID, currentExecution->runtime);
					}
				}

				currentExecution = currentExecution->next;
			}

			*maxExecutions = insertExecution_AtStart_AtList(*maxExecutions, maxExecution);

			// repor lista percorrida (currentExecution), para que se for necessário voltar a percorrer o while da execução de operações de novo
			currentExecution = NULL;
			currentExecution = executions;
			counter += time; // acumular o tempo de utilização de cada execução de operação
			time = 0; // resetar tempo de utilização para a próxima iteração
		}

		currentOperation = currentOperation->next;
	}

	return counter;
}


/**
* @brief	Obter a média de tempo necessário para completar uma operação, considerando todas as alternativas possíveis
* @param	head			Lista de execuções
* @param	operationID		Identificador da operação
* @return	Valor da média de tempo
*/
float getAverageTime_ToCompleteOperation(Execution* head, int operationID)
{
	if (head == NULL) // se a lista estiver vazia
	{
		return -1.0f;
	}

	int sum = 0;
	float average = 0;
	float numberOfExecutions = 0;

	Execution* current = head;

	while (current != NULL)
	{
		if (current->operationID == operationID) // se encontrar a execução de operação relativa à operação
		{
			sum += current->runtime;
			numberOfExecutions++;
		}
		current = current->next;
	}

	if (numberOfExecutions > 0) // para não permitir divisão por 0
	{
		average = sum / numberOfExecutions;
	}

	return average;
}