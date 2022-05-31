# Flexible Job Shop Problem
Proposta de escalonamento para a produção de um produto que envolve várias operações e a utilização de várias máquinas, para minimizar o tempo necessário na sua produção.



## Instalação
Para colocar o projeto a funcionar localmente em uma máquina basta:
1. Clonar o repositório ```git clone https://github.com/luispereira1999/fjssp.git```.
2. Utilizar o Visual Studio (2019 ou superior).


## Desenvolvimento
### Estrutura
As pastas e ficheiros que são utilizados no desenvolvimento do projeto estão estruturados da seguinte maneira:

fjssp.sln\
docs/\
├── html\
├── Doxyfile\
├── er-model.drawio\
├── er-model.png\
fjssp/\
├── data.c\
├── header.h\
├── jobs.c\
├── machines.c\
├── main.c\
├── machines.c\
├── executions.c\
└── operations.c\


### Diretivas
Orientações e normas para o desenvolvimento do projeto:

**Linguagem C**
- todo o código deve ser sintaticamente válido e estar corretamente indentado e comentado.
- todo o código deve ser escrito em inglês, apenas comentários podem ser escritos em português.
- os nomes das variáveis devem utilizar camel case.
- todas as funcionalidades introduzidas devem ser testadas.

**Ficheiros**
- devem conter uma descrição no início do ficheiro.


## Documentação
Para criar a documentação do código é utilizado o [Doxygen](https://www.doxygen.nl/index.html).\
Para atualizar, coloque os ficheiros na pasta ```docs```.


## Contribuições
Qualquer contribuição é sempre bem-vinda e agradecida (desde reportar erros até adicionar novas funcionalidades).\
Para contribuir em desenvolvimento, ler por favor as diretivas estabelecidas para o desenvolvimento do mesmo descritas acima para que outros desenvolvedores possam seguir o mesmo padrão.


## Criadores
- Luís Pereira


## Licença
Ao contribuir para este projeto, concorda com as políticas da licença MIT.
