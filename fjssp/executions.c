/**
Descri??o:          Ficheiro com todas as fun??es relativas ?s execu??es de opera??es
					FJSSP - parte 2
Desenvolvedor(es):   Ademar Valente (23155)
e-mail: a23155@alunos.ipca.pt
github: ademar1k82
?ltima atualiza??o:  31/05/2022
Cria??o:             22/05/2022
*/

#include <stdio.h>
#include <stdlib.h>
#include "header.h"


#pragma region trabalhar com listas

/**
* @brief	Criar nova execu??o
* @param	operationID		Identificador da opera??o
* @param	machineID		Identificador da m?quina
* @param	runtime			Unidades de tempo necess?rias para a execu??o da opera??o
* @return	Nova execu??o
*/
Execution* newExecution(int operationID, int machineID, int runtime)
{
	Execution* new = (Execution*)malloc(sizeof(Execution));
	if (new == NULL) // se n?o houver mem?ria para alocar
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
* @brief	Inserir nova execu??o no in?cio da lista de execu??es
* @param	head	Lista de execu??es
* @param	new		Nova execu??o
* @return	Lista de execu??es atualizada
*/
Execution* insertExecution_AtStart_AtList(Execution* head, Execution* new)
{
	if (searchExecution_AtList(head, new->operationID, new->machineID)) // n?o permitir inserir uma nova com o mesmo ID de opera??o e ID de m?quina
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
* @brief	Inserir execu??o ordenada pelo ID da opera??o na lista de execu??es
* @param	head	Lista de execu??es
* @param	new		Nova execu??o
* @return	Lista de execu??es atualizada
*/
Execution* insertExecution_ByOperation_AtList(Execution* head, Execution* new)
{
	if (searchExecution_AtList(head, new->operationID, new->machineID)) // n?o permitir inserir uma nova com o mesmo ID de opera??o e ID de m?quina
	{
		return NULL;
	}

	if (head == NULL) // se a lista estiver vazia
	{
		head = new; // inserir no in?cio
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
* @brief	Atualizar as unidades de tempo necess?rias para a execu??o de uma opera??o
* @param	head			Apontador para a lista de execu??es
* @param	operationID		Identificador da opera??o
* @param	machineID		Identificador da m?quina
* @param	runtime			Unidades de tempo
* @return	Booleano para o resultado da fun??o (se funcionou ou n?o)
*/
bool updateRuntime_AtList(Execution** head, int operationID, int machineID, int runtime)
{
	if (*head == NULL) // se lista est? vazia
	{
		return false;
	}

	if (!searchExecution_AtList(*head, operationID, machineID)) // se n?o existir a execu??o para remover
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
* @brief	Remover execu??o pelo identificador da opera??o
* @param	head			Apontador para a lista de execu??es
* @param	operationID		Identificador da opera??o
* @return	Booleano para o resultado da fun??o (se funcionou ou n?o)
*/
bool deleteExecution_ByOperation_AtList(Execution** head, int operationID)
{
	if (*head == NULL) // se a lista estiver vazia
	{
		return false;
	}

	if (!searchExecution_ByOperation_AtList(*head, operationID)) // se n?o existir a execu??o para remover
	{
		return false;
	}

	Execution* current = *head;
	Execution* previous = NULL;

	if (current != NULL && current->operationID == operationID) { // se o elemento que ser? apagado ? o primeiro da lista
		*head = current->next;
		free(current);
		return true;
	}

	while (current != NULL && current->operationID != operationID) // procurar o elemento a ser apagado
	{
		previous = current;
		current = current->next;
	}

	if (current == NULL) // se o elemento n?o foi encontrado
	{
		return false;
	}

	previous->next = current->next; // desassociar o elemento da lista
	free(current);

	return true;
}


/**
* @brief	Armazenar lista de execu??es em ficheiro bin?rio
* @param	fileName	Nome do ficheiro para armazenar a lista
* @param	head		Lista de execu??es
* @return	Booleano para o resultado da fun??o (se funcionou ou n?o)
*/
bool writeExecutions_AtList(char fileName[], Execution* head)
{
	if (head == NULL) // se lista est? vazia
	{
		return false;
	}

	FILE* file = NULL;

	if ((file = fopen(fileName, "wb")) == NULL) // erro ao abrir o ficheiro
	{
		return false;
	}

	Execution* current = head;
	FileExecution currentInFile; // ? a mesma estrutura mas sem o campo *next, uma vez que esse campo n?o ? armazenado no ficheiro

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
* @brief	Ler lista de execu??es de ficheiro bin?rio
* @param	fileName	Nome do ficheiro para ler a lista
* @return	Lista de execu??ess
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
	FileExecution currentInFile; // ? a mesma estrutura mas sem o campo *next, uma vez que esse campo n?o ? armazenado no ficheiro

	while (fread(&currentInFile, sizeof(FileExecution), 1, file)) // l? todos os registos do ficheiro e guarda na lista
	{
		current = newExecution(currentInFile.operationID, currentInFile.machineID, currentInFile.runtime);
		head = insertExecution_AtStart_AtList(head, current);
	}

	fclose(file);

	return head;
}


/**
* @brief	Mostrar a lista de execu??es na consola
* @param	head	Lista de execu??es
* @return	Booleano para o resultado da fun??o (se funcionou ou n?o)
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
		printf("ID Opera??o: %d, ID M?quina: %d, Tempo de Execu??o: %d;\n", current->operationID, current->machineID, current->runtime);
		current = current->next;
	}

	return true;
}


/**
* @brief	Procurar por uma execu??o na lista de execu??es
* @param	head			Lista de execu??es
* @param	operationID		Identificador da opera??o
* @param	machineID		Identificador da m?quina
* @return	Booleano para o resultado da fun??o (se funcionou ou n?o)
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
* @brief	Procurar por uma execu??o atrav?s do identificador da opera??o, na lista de execu??es
* @param	head			Lista de execu??es
* @param	operationID		Identificador da opera??o
* @return	Booleano para o resultado da fun??o (se funcionou ou n?o)
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
* @brief	Ordenar lista de execu??es por ordem crescente do identificador da opera??o
* @param	head			Lista de execu??es
* @return	Booleano para o resultado da fun??o (se funcionou ou n?o)
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
* @brief	Obter a ?ltima execu??o de uma lista de execu??es
* @param	head			Lista de execu??es
* @return	?ltima execu??o da lista
*/
Execution* getLastExecution_AtList(Execution* head)
{
	if (head == NULL) // se a lista estiver vazia
	{
		return NULL;
	}

	Execution* current = head;

	while (current->next != NULL) // verifica se o pr?ximo ? nulo: currentElement = next else return currentElement
	{
		current = current->next;
	}

	return current;
}


/**
* @brief	Libertar a lista de execu??es da mem?ria
* @param	head	Lista de execu??es
* @return	Lista libertada da mem?ria
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
 * @brief Carrega dados das execu??es de um ficheiro CSV para uma tabela hash em mem?ria
 * @param fileName		Nome do ficheiro
 * @return A tabela hash de execu??es do ficheiro CSV
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
* @brief	Criar tabela hash das execu??es vazia
* @param	table	Tabela hash das execu??es
* @return	Tabela hash das execu??es criada
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
* @brief	Criar n? vazio para uma posi??o da tabela hash das execu??es
* @return	N? vazio da tabela hash das execu??es criado
*/
ExecutionNode* createExecutionNode()
{
	ExecutionNode* node = malloc(sizeof(ExecutionNode));

	node->start = NULL;
	node->numberOfExecutions = 0;

	return node;
}


/**
* @brief	Gerar hash atrav?s de uma fun??o.
*			A fun??o ? representada pelo resto da divis?o do identificador da opera??o pelo tamanho da tabela
* @param	operationID		Identificador da opera??o
* @return	Valor calculado pela fun??o hash
*/
int generateHash(int operationID)
{
	return operationID % HASH_TABLE_SIZE;
}


/**
* @brief	Inserir nova execu??o na tabela hash das execu??es, numa determinada posi??o
* @param	table	Tabela hash das execu??es
* @return	Tabela hash das execu??es atualizada
*/
ExecutionNode** insertExecution_AtTable(ExecutionNode* table[], Execution* new)
{
	int index = generateHash(new->operationID);

	table[index]->start = insertExecution_AtStart_AtList(table[index]->start, new);
	table[index]->numberOfExecutions++;

	return *table;
}


/**
* @brief	Atualizar as unidades de tempo necess?rias para a execu??o de uma opera??o na tabela hash
* @param	table			Tabela hash das execu??es
* @param	operationID		Identificador da opera??o
* @param	machineID		Identificador da m?quina
* @param	runtime			Unidades de tempo
* @return	Booleano para o resultado da fun??o (se funcionou ou n?o)
*/
bool updateRuntime_ByOperation_AtTable(ExecutionNode** table[], int operationID, int machineID, int runtime)
{
	ExecutionNode** current = table;

	int index = generateHash(operationID);
	bool updated = updateRuntime_AtList(&current[index]->start, operationID, machineID, runtime);

	return updated;
}


/**
* @brief	Remover todas as execu??es pelo identificador da opera??o na tabela
* @param	table			Tabela hash das execu??es
* @param	operationID		Identificador da opera??o
* @return	Booleano para o resultado da fun??o (se funcionou ou n?o)
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
		// enquanto que remover, significa que ainda existe opera??es e portanto continuar? a remover, at? remover todas
		deleted = deleteExecution_ByOperation_AtList(&current[index]->start, operationID);

		if (deleted)
		{
			current[index]->numberOfExecutions--;
		}
	} while (deleted == true);

	return deleted;
}


/**
* @brief	Armazenar os registos de todas as listas de execu??es na tabela hash, em ficheiro bin?rio
* @param	fileName	Nome do ficheiro para armazenar os registos
* @param	head		Lista de execu??es
* @return	Booleano para o resultado da fun??o (se funcionou ou n?o)
*/
bool writeExecutions_AtTable(char fileName[], ExecutionNode* table[])
{
	if (table == NULL) // se tabela est? vazia
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
* @brief	Armazenar os registos de todas as listas de execu??es na tabela hash, em ficheiro bin?rio
* @brief	Ler de ficheiro bin?rio, os registos de todas as execu??es para a tabela hash
* @param	fileName	Nome do ficheiro para ler a lista
* @return	Tabela hash de execu??ess
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
* @brief	Mostrar a tabela hash com as listas de execu??es na consola
* @param	table	Tabela hash das execu??es
* @return	Booleano para o resultado da fun??o (se funcionou ou n?o)
*/
bool displayExecutions_AtTable(ExecutionNode* table[])
{
	if (table == NULL)
	{
		return false;
	}

	for (int i = 0; i < HASH_TABLE_SIZE; i++)
	{
		printf("Lista %d - N?mero de elementos: %d\n", i + 1, table[i]->numberOfExecutions);
		displayExecutions_AtList(table[i]->start);
	}

	return true;
}


/**
* @brief	Procurar por uma execu??o na tabela hash
* @param	table			Tabela hash das execu??es
* @param	operationID		Identificador da opera??o
* @param	machineID		Identificador da m?quina
* @return	Execu??o encontrada ou retorna nulo se n?o encontrar
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
* @brief	Libertar a tabela hash de execu??es da mem?ria
* @param	head	Lista de execu??es
* @return	Tabela hash libertada da mem?ria
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