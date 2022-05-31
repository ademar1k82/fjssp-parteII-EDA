/**
Descrição:           Declarações necessárias para aplicação
					 FJSSP - parte 2
Desenvolvedor(es):   Ademar Valente (23155)
e-mail: a23155@alunos.ipca.pt
github: ademar1k82
Última atualização:  31/05/2022
Criação:             22/05/2022
*/


#ifndef HEADER_H

#pragma region constantes

#define HEADER_H

/**
 * @brief	Tamanho relativos às estruturas de dados
 */
#define HASH_TABLE_SIZE 13 //numero primo - evitar colisões
#define NUMBER_MACHINES 8
#define MAX_TIME 40

/**
 * @brief	Tamanhos e nomes relativos a ficheiros de texto
 */
#define FILE_LINE_SIZE 50
#define JOBS_FILENAME_TEXT "text/jobs.csv"
#define MACHINES_FILENAME_TEXT "text/machines.csv"
#define OPERATIONS_FILENAME_TEXT "text/operations.csv"
#define EXECUTIONS_FILENAME_TEXT "text/executions.csv"

/**
 * @brief	Nomes para os ficheiros binários onde os dados são armazenados durante a execução do programa
 */
#define JOBS_FILENAME_BINARY "binary/jobs.bin"
#define MACHINES_FILENAME_BINARY "binary/machines.bin"
#define OPERATIONS_FILENAME_BINARY "binary/operations.bin"
#define EXECUTIONS_FILENAME_BINARY "binary/executions.bin"

#pragma endregion


#pragma region representações

  /**
   * @brief	Criar tipo de dados booleano
   */
typedef enum bool
{
   false = 0,
   true = 1
} bool;

#pragma endregion


#pragma region estruturas de dados em memória

/**
 * @brief	Estrutura de dados para representar a lista de trabalhos (em memória)
 */
typedef struct Job
{
	int id;
	struct Job* next;
} Job;

// lista de trabalhos
extern Job* jobs; // extern: informa o compilador que esta variável está definida algures no código

/**
 * @brief	Estrutura de dados para representar a lista de máquinas (em memória)
 */
typedef struct Machine
{
	int id;
	bool isBusy; // se a máquina está ou não em utilização
	struct Machine* next;
} Machine;

// lista de máquinas
extern Machine* machines;

/**
 * @brief	Estrutura de dados para representar a lista de operações (em memória)
 */
typedef struct Operation
{
	int id;
	int jobID;
	int position; // posição da operação (se é a 1º, 2º, 3º... a ser executada)
	struct Operation* next;
} Operation;

// lista de operações
extern Operation* operations;

/**
 * @brief	Estrutura de dados para representar a lista das execuções das operações em máquinas (em memória)
 */
typedef struct Execution
{
	int operationID;
	int machineID;
	int runtime; // unidades de tempo necessárias para a execução da operação
	struct Execution* next;
} Execution;

// lista de execuções
extern Execution* executions;

/**
 * @brief	Estrutura de dados para representar cada nó (posição) da tabela hash das execuções (em memória)
 */
typedef struct {
	Execution* start; // apontador para o primeiro elemento de cada posição da tabela
	int numberOfExecutions; // quantidade de execuções de cada posição da tabela
} ExecutionNode;

// tabela hash para armazenar as execuções e fazer buscas de forma mais eficiente,
// é um array de apontadores para listas de execuções
extern ExecutionNode* executionsTable[HASH_TABLE_SIZE];

/**
 * @brief	Estrutura de dados para representar um plano de produção
 */
typedef struct Cell
{
	int jobID;
	int operationID;
	int initialTime;
	int finalTime;
} Cell;

extern Cell plan[NUMBER_MACHINES][MAX_TIME];

#pragma endregion


#pragma region estruturas de dados em ficheiros

/**
 * @brief	Estrutura de dados para armazenar em ficheiro a lista de trabalhos
 */
typedef struct FileJob
{
	int id;
} FileJob;

/**
 * @brief	Estrutura de dados para armazenar em ficheiro a lista de máquinas
 */
typedef struct FileMachine
{
	int id;
	bool isBusy; // se a máquina está ou não em utilização
} FileMachine;

/**
 * @brief	Estrutura de dados para armazenar em ficheiro a lista de operações
 */
typedef struct FileOperation
{
	int id;
	int jobID;
	int position; // posição da operação (se é a 1º, 2º, 3º... a ser executada)
} FileOperation;

/**
 * @brief	Estrutura de dados para armazenar em ficheiro a lista de execuções das operações em máquinas
 */
typedef struct FileExecution
{
	int operationID;
	int machineID;
	int runtime; // unidades de tempo necessárias para a execução da operação
} FileExecution;

#pragma endregion


#pragma region funções

/**
 * @brief	Sobre trabalhos
 */
Job* loadJobs(char* fileName);
Job* newJob(int id);
Job* insertJob_AtStart(Job* head, Job* new);
bool deleteJob(Job** head, int id);
bool writeJobs(char fileName[], Job* head);
Job* readJobs(char fileName[]);
bool displayJobs(Job* head);
bool searchJob(Job* head, int id);

/**
 * @brief	Sobre máquinas
 */
Machine* loadMachines(char* fileName);
Machine* newMachine(int id, bool isBusy);
Machine* insertMachine_AtStart(Machine* head, Machine* new);
bool writeMachines(char fileName[], Machine* head);
Machine* readMachines(char fileName[]);
bool displayMachines(Machine* head);
bool searchMachine(Machine* head, int id);

/**
 * @brief	Sobre operações
 */
Operation* loadOperations(char* fileName);
Operation* newOperation(int id, int jobID, int position);
Operation* insertOperation_AtStart(Operation* head, Operation* new);
bool updatePosition(Operation** head, int xOperationID, int yOperationID);
bool deleteOperation(Operation** head, int id);
int deleteOperation_ByJob(Operation** head, int jobID);
bool writeOperations(char fileName[], Operation* head);
Operation* readOperations(char fileName[]);
bool displayOperations(Operation* head);
bool searchOperation(Operation* head, int id);
bool searchOperation_ByJob(Operation* head, int jobID);
Operation* getOperation(Operation* head, int id);
int getMinTime_ToCompleteJob(Operation* operations, Execution* executions, int jobID, Execution** minExecutions);
int getMaxTime_ToCompleteJob(Operation* operations, Execution* executions, int jobID, Execution** maxExecutions);
float getAverageTime_ToCompleteOperation(Execution* head, int operationID);

/**
 * @brief	Sobre execuções com listas
 */
Execution* newExecution(int operationID, int machineID, int runtime);
Execution* insertExecution_AtStart_AtList(Execution* head, Execution* new);
Execution* insertExecution_ByOperation_AtList(Execution* head, Execution* new);
bool updateRuntime_AtList(Execution** head, int operationID, int machineID, int runtime);
bool deleteExecution_ByOperation_AtList(Execution** head, int operationID);
bool writeExecutions_AtList(char fileName[], Execution* head);
Execution* readExecutions_AtList(char fileName[]);
bool displayExecutions_AtList(Execution* head);
Execution* searchExecution_AtList(Execution* head, int operationID, int machineID);
Execution* searchExecution_ByOperation_AtList(Execution* head, int operationID);
Execution* sortExecutions_ByOperation_AtList(Execution* head);
Execution* getLastExecution_AtList(Execution* head);
Execution* free_Execution_List(Execution* head);

/**
 * @brief	Sobre execuções com tabela hash
 */
ExecutionNode** loadExecutionsTable(char* fileName, ExecutionNode* table[]);
ExecutionNode** createExecutionsTable(ExecutionNode* table[]);
ExecutionNode* createExecutionNode();
int generateHash(int id);
ExecutionNode** insertExecution_AtTable(ExecutionNode* table[], Execution* new);
bool updateRuntime_ByOperation_AtTable(ExecutionNode** table[], int operationID, int machineID, int runtime);
bool deleteExecutions_ByOperation_AtTable(ExecutionNode** table[], int operationID);
bool writeExecutions_AtTable(char fileName[], ExecutionNode* table[]);
ExecutionNode** readExecutions_AtTable(char fileName[], ExecutionNode* tableee[]);
bool displayExecutions_AtTable(ExecutionNode* table[]);
Execution* searchExecution_AtTable(ExecutionNode* table[], int operationID, int machineID);
ExecutionNode** free_Execution_Table(ExecutionNode* table[]);

/**
 * @brief	Sobre os planos de produção
 */
bool startPlan(Cell plan[][MAX_TIME], int jobID, int executionID);
bool fillCells(Cell p[][MAX_TIME], int mid, int jid, int oid, int it, int ft);
bool searchActiveCells(Cell p[][MAX_TIME], int mid, int it, int ft);
bool displayPlan(Cell p[][MAX_TIME]);

#pragma endregion

#endif