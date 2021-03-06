/**
Descri??o:          Ficheiro principal da aplica??o com menu e fun??es necess?rias
					FJSSP - parte 2
Desenvolvedor(es):   Ademar Valente (23155)
e-mail: a23155@alunos.ipca.pt
github: ademar1k82
?ltima atualiza??o:  31/05/2022
Cria??o:             24/05/2022
*/

#include <stdio.h>
#include <locale.h>
#include "header.h"


/**
* @brief	Fun??o principal do programa
* @return	Resultado do programa na linha de comandos
*/
int main()
{
	setlocale(LC_ALL, "Portuguese"); // permitir caracteres especiais (portugueses)

	// listas
	Job* jobs = NULL;
	Operation* operations = NULL;
	Machine* machines = NULL;

	// tabela hash das execu??es
	ExecutionNode* executionsTable[HASH_TABLE_SIZE];

	// matriz do plano de produ??o
	Cell plan[NUMBER_MACHINES][MAX_TIME];

	int menuOption = 0;

	do
	{
		// menu 
		printf("-------------------------------------------------------------------------\n\n");
		printf("                       Flexible Job Shop Problem                         \n\n");
		printf("                             1 -> Fase 2                                 \n");
		printf("                             2 -> Sobre                                  \n\n");
		printf("                          IPCA - EDJD - EDA                              \n");
		printf("                        Ademar Valente | 2022                            \n\n");
		printf("-------------------------------------------------------------------------\n");
		printf("Escolha uma das op??es acima: ");

		if (!scanf("%d", &menuOption)) // se n?o introduzir um n?mero
		{
			fseek(stdin, 0, SEEK_END); // repor buffer de entrada para evitar ciclo infinito
			printf("\n");
			printf("O car?cter introduzido n?o ? v?lido. Tente outro.\n");
		}
		else
		{
			printf("\n");

			switch (menuOption)
			{
			case 1: // fase 2 da aplica??o

#pragma region funcionalidade 1: definir estruturas de dados din?micas
				printf("-  1. Definir estruturas de dados din?micas\n");

				// carregar listas em mem?ria
				jobs = loadJobs(JOBS_FILENAME_TEXT);
				machines = loadMachines(MACHINES_FILENAME_TEXT);
				operations = loadOperations(OPERATIONS_FILENAME_TEXT);

				// carregar tabela hash em mem?ria
				*executionsTable = loadExecutionsTable(EXECUTIONS_FILENAME_TEXT, executionsTable);

				printf("Dados carregados em mem?ria com sucesso!\n");
#pragma endregion

#pragma region funcionalidade 2: armazenar e ler as estruturas em ficheiros
				printf("\n\n-  2. Armazenar e ler as estruturas em ficheiros\n");

				// guardar os dados em ficheiros
				writeJobs(JOBS_FILENAME_BINARY, jobs);
				writeMachines(MACHINES_FILENAME_BINARY, machines);
				writeOperations(OPERATIONS_FILENAME_BINARY, operations);
				writeExecutions_AtTable(EXECUTIONS_FILENAME_BINARY, executionsTable);

				printf("Dados exportados com sucesso!\n");

				// definir listas como NULL para ficarem vazias para ler os dados de ficheiros
				jobs = NULL;
				machines = NULL;
				operations = NULL;
				*executionsTable = free_Execution_Table(executionsTable);

				// ler dados de ficheiros
				jobs = readJobs(JOBS_FILENAME_BINARY);
				machines = readMachines(MACHINES_FILENAME_BINARY);
				operations = readOperations(OPERATIONS_FILENAME_BINARY);
				*executionsTable = createExecutionsTable(executionsTable);
				*executionsTable = readExecutions_AtTable(EXECUTIONS_FILENAME_BINARY, executionsTable);

				printf("Dados importados com sucesso!\n");
#pragma endregion

#pragma region funcionalidade 3: remover um trabalho
				printf("\n\n-  3. remover um trabalho\n");

				// remover trabalho
				deleteJob(&jobs, 5);
				printf("trabalho removido com sucesso!\n");

				int operationDeleted = 0;

				do {
					// remover as opera??es associadas ao jobs
					operationDeleted = deleteOperation_ByJob(&operations, 3);
					printf("Opera??es associadas ao trabalho removidas com sucesso!\n");

					// remover as execu??es associadas a cada opera??o
					deleteExecutions_ByOperation_AtTable(&executionsTable, operationDeleted);
					printf("Execu??es associadas ? opera??o removidas com sucesso!\n");
				} while (operationDeleted != -1);
#pragma endregion

#pragma region funcionalidade 4: inserir um trabalho
				printf("\n\n-  4. Inserir um trabalho\n");

				// inserir novo trabalho
				Job* job = NULL;
				job = newJob(9);
				jobs = insertJob_AtStart(jobs, job);

				// guardar a nova inser??o em ficheiro
				writeJobs(JOBS_FILENAME_BINARY, jobs);
				printf("Novos dados exportados com sucesso!\n");
#pragma endregion

#pragma region funcionalidade 5: remover uma opera??o
				printf("\n\n-  5. Remover uma opera??o\n");

				// remover opera??o
				deleteOperation(&operations, 24);
				printf("Opera??o removida com sucesso!\n");

				// remover execu??es associadas ? opera??o
				deleteExecutions_ByOperation_AtTable(&executionsTable, 24);
				printf("Execu??es associadas ? opera??o removidas com sucesso!\n");
#pragma endregion

#pragma region funcionalidade 6: atualizar uma opera??o
				printf("\n\n-  6. Atualizar uma opera??o\n");

				// atualizar a posi??o de uma opera??o X pela posi??o de uma opera??o Y, e vice-versa
				updatePosition(&operations, 5, 6);
				printf("As posi??es das opera??es foram trocadas com sucesso!\n");

				// atualizar o tempo de uma execu??o de opera??o
				updateRuntime_ByOperation_AtTable(executionsTable, 4, 4, 10);
#pragma endregion

#pragma region funcionalidade 7: inserir uma opera??o
				printf("\n\n-  7. Inserir uma opera??o\n");

				// inserir nova opera??o
				Operation* operation = NULL;
				operation = newOperation(39, 2, 8);
				operations = insertOperation_AtStart(operations, operation);

				// inserir nova execu??o de uma opera??o
				Execution* execution = NULL;
				execution = newExecution(39, 5, 17);
				*executionsTable = insertExecution_AtTable(executionsTable, execution);

				// guardar as novas inser??es em ficheiros
				writeOperations(OPERATIONS_FILENAME_BINARY, operations);
				//writeExecutions_AtTable(EXECUTIONS_FILENAME_BINARY, executionsTable);
				printf("Novos dados exportados com sucesso!\n");
#pragma endregion

#pragma region funcionalidade 8: proposta de escalonamento
				printf("\n\n-  8. Plano de ac??o para a utiliza??o das m?quinas (de 1 a 8)\n");
				printf("      na Opera??o 1 de todos os Jobs\n");

				// iniciar um plano de produ??o vazio
				startPlan(plan, -1, -1);

				// preencher um c?lula no plano
				fillCells(plan, 1, 1, 1, 0, 4);
				fillCells(plan, 1, 4, 1, 4, 8);
				fillCells(plan, 1, 6, 1, 8, 11);
				fillCells(plan, 1, 5, 1, 11, 14);
				fillCells(plan, 1, 8, 1, 14, 17);
				fillCells(plan, 1, 2, 1, 17, 18);
				fillCells(plan, 2, 3, 1, 0, 7);
				fillCells(plan, 2, 6, 1, 11, 16);
				fillCells(plan, 2, 8, 1, 17, 21);
				fillCells(plan, 3, 7, 1, 0, 4);
				fillCells(plan, 3, 1, 1, 4, 9);
				fillCells(plan, 3, 4, 1, 9, 12);
				fillCells(plan, 3, 6, 1, 16, 22);
				fillCells(plan, 3, 3, 1, 22, 29);
				fillCells(plan, 3, 2, 1, 29, 34);
				fillCells(plan, 5, 7, 1, 4, 9);
				fillCells(plan, 5, 4, 1, 12, 19);
				fillCells(plan, 5, 2, 1, 34, 40);
				fillCells(plan, 6, 7, 1, 9, 13);
				fillCells(plan, 6, 8, 1, 21, 25);
				fillCells(plan, 8, 3, 1, 29, 37);

				fillCells(plan, 1, 2, 5, 0, 3); // n?o ir? preencher, porque j? existe c?lulas ativas neste intervalo de tempo

				displayPlan(plan);
#pragma endregion

#pragma region mostrar dados
				printf("\n\n-  Mostrar dados\n");

				// mostrar dados na consola
				printf("Trabalhos:\n");
				displayJobs(jobs);

				printf("M?quinas:\n");
				displayMachines(machines);

				printf("Opera??es:\n");
				displayOperations(operations);

				printf("Execu??es de Opera??es:\n");
				displayExecutions_AtTable(executionsTable);

				printf("Dados mostrados com sucesso!\n");
#pragma endregion

				break;

			case 2: // sobre aplica??o

				printf("Flexible Job Shop Problem - Proposta de escalonamento para a produ??o\n");
				printf("de um produto que envolve v?rias opera??es e a utiliza??o de v?rias m?quinas,\n");
				printf("para minimizar o tempo necess?rio na sua produ??o.\n");
				printf("Projeto desenvolvido na unidade curricular Estruturas de Dados Avan?adas,\n");
				printf("no ?mbito do curso Licenciatura em Engenharia em Desenvolvimento de Jogos Digitais.\n");
				printf("Realizado no Instituto Polit?cnico do C?vado e do Ave, a 22 at? 31 de Maio de 2022,\n");
				printf("durante o 2? semestre do 1? ano de curso.\n");
				break;

			default:
				printf("Op??o inv?lida. Tente novamente.\n");
				break;
			}
		}

		printf("\n");
	} while (true);

	return true;
}