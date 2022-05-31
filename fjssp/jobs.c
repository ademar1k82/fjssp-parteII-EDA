/**
Descrição:          Ficheiro com todas as funções relativas aos trabalhos
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
 * @brief Carrega dados dos trabalhos de um ficheiro CSV para uma lista em memória
 * @param fileName		Nome do ficheiro
 * @return A lista de trabalhos do ficheiro CSV
 */
Job* loadJobs(char* fileName)
{
	char line[FILE_LINE_SIZE];
	int a = 0;

	Job* job = NULL;
	Job* jobs = NULL;

	FILE* file = fopen(fileName, "r");
	if (file == NULL)
	{
		return NULL;
	}

	while (!feof(file))
	{
		if (fgets(line, FILE_LINE_SIZE, file) != NULL)
		{
			// exemplo: 1
			sscanf(line, "%d", &a);
			job = newJob(a);
			jobs = insertJob_AtStart(jobs, job);
		}
	}

	fclose(file);

	return jobs;
}


/**
* @brief	Criar novo trabalho
* @param	id	Identificador do trabalho
* @return	Novo trabalho
*/
Job* newJob(int id)
{
	Job* new = (Job*)malloc(sizeof(Job));
	if (new == NULL) // se não houver memória para alocar
	{
		return NULL;
	}

	new->id = id;
	new->next = NULL;

	return new;
}


/**
* @brief	Inserir novo trabalho no início da lista de trabalhos
* @param	head	Lista de trabalhos
* @param	new		Novo trabalho
* @return	Lista de trabalhos atualizada
*/
Job* insertJob_AtStart(Job* head, Job* new)
{
	if (searchJob(head, new->id)) // não permitir inserir um novo com o mesmo ID
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
* @brief	Remover um trabalho da lista de trabalhos
* @param	head	Apontador para a lista de trabalhos
* @param	id		Identificador do trabalho
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool deleteJob(Job** head, int id)
{
	if (*head == NULL)
	{
		return false;
	}

	Job* current = *head;
	Job* previous = NULL;

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
* @brief	Armazenar lista de trabalhos em ficheiro binário
* @param	fileName	Nome do ficheiro para armazenar a lista
* @param	head		Lista de trabalhos
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool writeJobs(char fileName[], Job* head)
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

	Job* current = head;
	FileJob currentInFile; // é a mesma estrutura mas sem o campo *next, uma vez que esse campo não é armazenado no ficheiro

	while (current != NULL)
	{
		currentInFile.id = current->id;
		fwrite(&currentInFile, sizeof(FileJob), 1, file); // guarda cada registo da lista no ficheiro

		current = current->next;
	}

	fclose(file);

	return true;
}


/**
* @brief	Ler lista de trabalhos de ficheiro binário
* @param	fileName	Nome do ficheiro para ler a lista
* @return	Lista de trabalhos
*/
Job* readJobs(char fileName[])
{
	FILE* file = NULL;

	if ((file = fopen(fileName, "rb")) == NULL) // erro ao abrir o ficheiro
	{
		return NULL;
	}

	Job* head = NULL;
	Job* current = NULL;
	FileJob currentInFile; // é a mesma estrutura mas sem o campo *next, uma vez que esse campo não é armazenado no ficheiro

	while (fread(&currentInFile, sizeof(FileJob), 1, file)) // lê todos os registos do ficheiro e guarda na lista
	{
		current = newJob(currentInFile.id);
		head = insertJob_AtStart(head, current);
	}

	fclose(file);

	return head;
}


/**
* @brief	Mostrar a lista de trabalhos na consola
* @param	head	Lista de trabalhos
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool displayJobs(Job* head)
{
	if (head == NULL) // se lista está vazia
	{
		return false;
	}

	Job* current = head;

	while (current != NULL)
	{
		printf("ID: %d;\n", current->id);
		current = current->next;
	}

	return true;
}


/**
* @brief	Procurar por um trabalho na lista de trabalhos
* @param	head	Lista de trabalhos
* @param	id		Identificador do trabalho
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool searchJob(Job* head, int id)
{
	if (head == NULL) // se lista está vazia
	{
		return false;
	}

	Job* current = head;

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