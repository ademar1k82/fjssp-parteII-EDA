/**
Descrição:          Ficheiro com todas as funções relativas às execuções de operações
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


#pragma region trabalhar com listas

/**
* @brief	Criar nova execução
* @param	operationID		Identificador da operação
* @param	machineID		Identificador da máquina
* @param	runtime			Unidades de tempo necessárias para a execução da operação
* @return	Nova execução
*/
Execution* newExecution(int operationID, int machineID, int runtime)
{
	Execution* new = (Execution*)malloc(sizeof(Execution));
	if (new == NULL) // se não houver memória para alocar
	{
		return NULL;
	}

	new->operationID = operationID;
	new->machineID = machineID;
	new->runtime = runtime;
	new->next = NULL;

	return new;
}


/**
* @brief	Inserir nova execução no início da lista de execuções
* @param	head	Lista de execuções
* @param	new		Nova execução
* @return	Lista de execuções atualizada
*/
Execution* insertExecution_AtStart_AtList(Execution* head, Execution* new)
{
	if (searchExecution_AtList(head, new->operationID, new->machineID)) // não permitir inserir uma nova com o mesmo ID de operação e ID de máquina
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
* @brief	Inserir execução ordenada pelo ID da operação na lista de execuções
* @param	head	Lista de execuções
* @param	new		Nova execução
* @return	Lista de execuções atualizada
*/
Execution* insertExecution_ByOperation_AtList(Execution* head, Execution* new)
{
	if (searchExecution_AtList(head, new->operationID, new->machineID)) // não permitir inserir uma nova com o mesmo ID de operação e ID de máquina
	{
		return NULL;
	}

	if (head == NULL) // se a lista estiver vazia
	{
		head = new; // inserir no início
	}
	else
	{
		Execution* current = head;
		Execution* previous = NULL;

		// enquanto que atual tem o ID menor que a nova
		while (current != NULL && current->operationID < new->operationID)
		{
			previous = current;
			current = current->next;
		}

		if (previous == NULL)
		{
			new->next = head;
			head = new; // inserir no meio
		}
		else
		{
			previous->next = new;
			new->next = current; // inserir no fim
		}
	}

	return head;
}


/**
* @brief	Atualizar as unidades de tempo necessárias para a execução de uma operação
* @param	head			Apontador para a lista de execuções
* @param	operationID		Identificador da operação
* @param	machineID		Identificador da máquina
* @param	runtime			Unidades de tempo
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool updateRuntime_AtList(Execution** head, int operationID, int machineID, int runtime)
{
	if (*head == NULL) // se lista está vazia
	{
		return false;
	}

	if (!searchExecution_AtList(*head, operationID, machineID)) // se não existir a execução para remover
	{
		return false;
	}

	Execution* current = *head;

	while (current != NULL)
	{
		if (current->operationID == operationID && current->machineID == machineID)
		{
			current->runtime = runtime;
			return true;
		}
		current = current->next;
	}

	return false;
}


/**
* @brief	Remover execução pelo identificador da operação
* @param	head			Apontador para a lista de execuções
* @param	operationID		Identificador da operação
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool deleteExecution_ByOperation_AtList(Execution** head, int operationID)
{
	if (*head == NULL) // se a lista estiver vazia
	{
		return false;
	}

	if (!searchExecution_ByOperation_AtList(*head, operationID)) // se não existir a execução para remover
	{
		return false;
	}

	Execution* current = *head;
	Execution* previous = NULL;

	if (current != NULL && current->operationID == operationID) { // se o elemento que será apagado é o primeiro da lista
		*head = current->next;
		free(current);
		return true;
	}

	while (current != NULL && current->operationID != operationID) // procurar o elemento a ser apagado
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
* @brief	Armazenar lista de execuções em ficheiro binário
* @param	fileName	Nome do ficheiro para armazenar a lista
* @param	head		Lista de execuções
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool writeExecutions_AtList(char fileName[], Execution* head)
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

	Execution* current = head;
	FileExecution currentInFile; // é a mesma estrutura mas sem o campo *next, uma vez que esse campo não é armazenado no ficheiro

	while (current != NULL)
	{
		currentInFile.operationID = current->operationID;
		currentInFile.machineID = current->machineID;
		currentInFile.runtime = current->runtime;
		fwrite(&currentInFile, sizeof(FileExecution), 1, file); // guarda cada registo da lista no ficheiro

		current = current->next;
	}

	fclose(file);

	return true;
}


/**
* @brief	Ler lista de execuções de ficheiro binário
* @param	fileName	Nome do ficheiro para ler a lista
* @return	Lista de execuçõess
*/
Execution* readExecutions_AtList(char fileName[])
{
	FILE* file = NULL;

	if ((file = fopen(fileName, "rb")) == NULL) // erro ao abrir o ficheiro
	{
		return NULL;
	}

	Execution* head = NULL;
	Execution* current = NULL;
	FileExecution currentInFile; // é a mesma estrutura mas sem o campo *next, uma vez que esse campo não é armazenado no ficheiro

	while (fread(&currentInFile, sizeof(FileExecution), 1, file)) // lê todos os registos do ficheiro e guarda na lista
	{
		current = newExecution(currentInFile.operationID, currentInFile.machineID, currentInFile.runtime);
		head = insertExecution_AtStart_AtList(head, current);
	}

	fclose(file);

	return head;
}


/**
* @brief	Mostrar a lista de execuções na consola
* @param	head	Lista de execuções
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool displayExecutions_AtList(Execution* head)
{
	if (head == NULL) // se a lista estiver vazia
	{
		return false;
	}

	Execution* current = head;

	while (current != NULL)
	{
		printf("ID Operação: %d, ID Máquina: %d, Tempo de Execução: %d;\n", current->operationID, current->machineID, current->runtime);
		current = current->next;
	}

	return true;
}


/**
* @brief	Procurar por uma execução na lista de execuções
* @param	head			Lista de execuções
* @param	operationID		Identificador da operação
* @param	machineID		Identificador da máquina
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
Execution* searchExecution_AtList(Execution* head, int operationID, int machineID)
{
	if (head == NULL) // se a lista estiver vazia
	{
		return NULL;
	}

	Execution* current = head;

	while (current != NULL)
	{
		if (current->operationID == operationID && current->machineID == machineID)
		{
			return current;
		}
		current = current->next;
	}

	return NULL;
}


/**
* @brief	Procurar por uma execução através do identificador da operação, na lista de execuções
* @param	head			Lista de execuções
* @param	operationID		Identificador da operação
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
Execution* searchExecution_ByOperation_AtList(Execution* head, int operationID)
{
	if (head == NULL) // se a lista estiver vazia
	{
		return false;
	}

	Execution* current = head;

	while (current != NULL)
	{
		if (current->operationID == operationID)
		{
			return true;
		}
		current = current->next;
	}

	return false;
}


/**
* @brief	Ordenar lista de execuções por ordem crescente do identificador da operação
* @param	head			Lista de execuções
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
Execution* sortExecutions_ByOperation_AtList(Execution* head)
{
	if (head == NULL) // se a lista estiver vazia
	{
		return NULL;
	}

	Execution* current = head;
	Execution* sorted = NULL;
	Execution* new = NULL;

	while (current != NULL)
	{
		new = newExecution(current->operationID, current->machineID, current->runtime);
		sorted = insertExecution_ByOperation_AtList(sorted, new);
		current = current->next;
	}

	return sorted;
}


/**
* @brief	Obter a última execução de uma lista de execuções
* @param	head			Lista de execuções
* @return	Última execução da lista
*/
Execution* getLastExecution_AtList(Execution* head)
{
	if (head == NULL) // se a lista estiver vazia
	{
		return NULL;
	}

	Execution* current = head;

	while (current->next != NULL) // verifica se o próximo é nulo: currentElement = next else return currentElement
	{
		current = current->next;
	}

	return current;
}


/**
* @brief	Libertar a lista de execuções da memória
* @param	head	Lista de execuções
* @return	Lista libertada da memória
*/
Execution* free_Execution_List(Execution* head)
{
	if (head)
	{
		return NULL;
	}

	Execution* current = head;

	while (head)
	{
		current = head;
		head = head->next;
		free(current);
	}

	return current;
}

#pragma endregion


#pragma region trabalhar com tabela hash

/**
 * @brief Carrega dados das execuções de um ficheiro CSV para uma tabela hash em memória
 * @param fileName		Nome do ficheiro
 * @return A tabela hash de execuções do ficheiro CSV
 */
ExecutionNode** loadExecutionsTable(char* fileName, ExecutionNode* table[])
{
	char line[FILE_LINE_SIZE];
	int a = 0, b = 0, c = 0;

	Execution* execution = NULL;

	FILE* file = fopen(fileName, "r");
	if (file == NULL)
	{
		return NULL;
	}

	// iniciar tabela hash vazia
	*table = createExecutionsTable(table);

	while (!feof(file))
	{
		if (fgets(line, FILE_LINE_SIZE, file) != NULL)
		{
			// exemplo: 1;1;1
			sscanf(line, "%d;%d;%d", &a, &b, &c);

			execution = newExecution(a, b, c);
			*table = insertExecution_AtTable(table, execution);
		}
	}
	fclose(file);

	return *table;
}

/**
* @brief	Criar tabela hash das execuções vazia
* @param	table	Tabela hash das execuções
* @return	Tabela hash das execuções criada
*/
ExecutionNode** createExecutionsTable(ExecutionNode* table[])
{
	for (int i = 0; i < HASH_TABLE_SIZE; i++)
	{
		table[i] = createExecutionNode();
	}

	return *table;
}


/**
* @brief	Criar nó vazio para uma posição da tabela hash das execuções
* @return	Nó vazio da tabela hash das execuções criado
*/
ExecutionNode* createExecutionNode()
{
	ExecutionNode* node = malloc(sizeof(ExecutionNode));

	node->start = NULL;
	node->numberOfExecutions = 0;

	return node;
}


/**
* @brief	Gerar hash através de uma função.
*			A função é representada pelo resto da divisão do identificador da operação pelo tamanho da tabela
* @param	operationID		Identificador da operação
* @return	Valor calculado pela função hash
*/
int generateHash(int operationID)
{
	return operationID % HASH_TABLE_SIZE;
}


/**
* @brief	Inserir nova execução na tabela hash das execuções, numa determinada posição
* @param	table	Tabela hash das execuções
* @return	Tabela hash das execuções atualizada
*/
ExecutionNode** insertExecution_AtTable(ExecutionNode* table[], Execution* new)
{
	int index = generateHash(new->operationID);

	table[index]->start = insertExecution_AtStart_AtList(table[index]->start, new);
	table[index]->numberOfExecutions++;

	return *table;
}


/**
* @brief	Atualizar as unidades de tempo necessárias para a execução de uma operação na tabela hash
* @param	table			Tabela hash das execuções
* @param	operationID		Identificador da operação
* @param	machineID		Identificador da máquina
* @param	runtime			Unidades de tempo
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool updateRuntime_ByOperation_AtTable(ExecutionNode** table[], int operationID, int machineID, int runtime)
{
	ExecutionNode** current = table;

	int index = generateHash(operationID);
	bool updated = updateRuntime_AtList(&current[index]->start, operationID, machineID, runtime);

	return updated;
}


/**
* @brief	Remover todas as execuções pelo identificador da operação na tabela
* @param	table			Tabela hash das execuções
* @param	operationID		Identificador da operação
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool deleteExecutions_ByOperation_AtTable(ExecutionNode** table[], int operationID)
{
	ExecutionNode** current = table;

	int index = generateHash(operationID);
	if (index <= -1)
	{
		return false;
	}

	bool deleted = false;

	do
	{
		// enquanto que remover, significa que ainda existe operações e portanto continuará a remover, até remover todas
		deleted = deleteExecution_ByOperation_AtList(&current[index]->start, operationID);

		if (deleted)
		{
			current[index]->numberOfExecutions--;
		}
	} while (deleted == true);

	return deleted;
}


/**
* @brief	Armazenar os registos de todas as listas de execuções na tabela hash, em ficheiro binário
* @param	fileName	Nome do ficheiro para armazenar os registos
* @param	head		Lista de execuções
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool writeExecutions_AtTable(char fileName[], ExecutionNode* table[])
{
	if (table == NULL) // se tabela está vazia
	{
		return false;
	}

	Execution* allList = NULL;
	Execution* last = NULL;

	bool written = false;

	for (int i = 0; i < HASH_TABLE_SIZE; i++)
	{
		if (i == 0)
		{
			allList = table[0]->start;
		}
		else
		{
			last = getLastExecution_AtList(allList);
			last->next = table[i]->start;
		}
	}

	written = writeExecutions_AtList(fileName, allList);

	return written;
}


/**
* @brief	Armazenar os registos de todas as listas de execuções na tabela hash, em ficheiro binário
* @brief	Ler de ficheiro binário, os registos de todas as execuções para a tabela hash
* @param	fileName	Nome do ficheiro para ler a lista
* @return	Tabela hash de execuçõess
*/
ExecutionNode** readExecutions_AtTable(char fileName[], ExecutionNode* table[])
{
	FILE* file = NULL;

	if ((file = fopen(fileName, "rb")) == NULL) // erro ao abrir o ficheiro
	{
		return NULL;
	}

	Execution* execution = NULL;
	Execution* list = NULL;
	list = readExecutions_AtList(fileName);

	if (list == NULL) // erro ao ler dados do ficheiro
	{
		return NULL;
	}

	while (list != NULL) // enquanto que houver dados na lista, guarda-os na tabela
	{
		execution = newExecution(list->operationID, list->machineID, list->runtime);
		*table = insertExecution_AtTable(table, execution);

		list = list->next;
	}

	return *table;
}


/**
* @brief	Mostrar a tabela hash com as listas de execuções na consola
* @param	table	Tabela hash das execuções
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool displayExecutions_AtTable(ExecutionNode* table[])
{
	if (table == NULL)
	{
		return false;
	}

	for (int i = 0; i < HASH_TABLE_SIZE; i++)
	{
		printf("Lista %d - Número de elementos: %d\n", i + 1, table[i]->numberOfExecutions);
		displayExecutions_AtList(table[i]->start);
	}

	return true;
}


/**
* @brief	Procurar por uma execução na tabela hash
* @param	table			Tabela hash das execuções
* @param	operationID		Identificador da operação
* @param	machineID		Identificador da máquina
* @return	Execução encontrada ou retorna nulo se não encontrar
*/
Execution* searchExecution_AtTable(ExecutionNode* table[], int operationID, int machineID)
{
	if (table == NULL) // se a lista estiver vazia
	{
		return NULL;
	}

	int index = generateHash(operationID);
	Execution* search = searchExecution_AtList(table[index]->start, operationID, machineID);

	if (search == NULL) // se a lista estiver vazia
	{
		return NULL;
	}

	return search;
}

/**
* @brief	Libertar a tabela hash de execuções da memória
* @param	head	Lista de execuções
* @return	Tabela hash libertada da memória
*/
ExecutionNode** free_Execution_Table(ExecutionNode* table[])
{
	ExecutionNode* current = NULL;

	for (int i = 0; i < HASH_TABLE_SIZE; i++)
	{
		if (table[i] != NULL)
		{
			current = table[i];
			free_Execution_List(current->start);
			table[i] = NULL;
		}
	}

	return *table;
}

#pragma endregion