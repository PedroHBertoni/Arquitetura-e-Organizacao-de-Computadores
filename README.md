# Arquitetura-e-Organizacao-de-Computadores
Trabalho Avaliativo: Simulador de COMPUTADOR IAS

Alunos: Pedro Henrique Bertoni de Souza - RA143345
        Gustavo Barbosa Bento           - RA145106


# Especificações do Computador:
Como armazenamento na memória, temos 1000 endereçamentos possíveis de 40 bits, definidos de forma global.
Para os 100 primeiros espacamentos é recluso o armazenamento de dados brutos, com números em formato binário (na linguagem c, o
tratamento dos números já são considerados por bits).
Para os espaçamentos de 100 até 999, temos apenas um armazenamento de instruções, no formato 2 instruções por palavra, ou seja
pode ser escrito um programa de até 1800 instruções dentre as citadas posteriormente.

Para nossa CPU, definida por uma estrutura principal e inicializada com todos os registradores zerados, exceto PC que inicializa para
leitura a partir da memória 100. Além disso, possui todos seus registradores acessados pela estrutura principal, os quais são demonstrados via
terminal com seus estados após cada execução.


# Instruções de execução:
Para progamação e execução de códigos a partir do arquivo "ComputadorIAS.c", é necessário apresentar um arquivo com nome "programa.txt"
(esse nome pode ser alterado em "ComputadorIAS.c" na linha 159), o qual apresenta um código executável com apenas, intruções presentes no
IAS. Segue abaixo todas as funções disponibilizadas para escrita do programa.

-----------------------------------------------------------------------------------------------------------------------------------------

número          : *Armazena diretamente o _número_ como dado, em seu primeiro espaço disponível*

LOAD MQ         : *Transfere o dado de MQ para AC*
LOAD MQ.M(X)    : *Transfere o dado na memória no local X, para MQ*
STOR M(X)       : *Armazena o dado de AC para memória no local X*
LOAD M(X)       : *Transfere o dado da memória no local X, para AC*
LOAD-M(X)       : *Transfere o dado da memória, com inversão de sinal, no local X, para AC*
LOAD |M(X)|     : *Transfere o dado da memória, seu valor absoluto, no local X, para AC*
LOAD-|M(X)|     : *Transfere o dado da memória, seu valor negativo, no local X, para AC*

JUMP M(X,0:19)  : *Traz a próxima leitura da posição X na memória, apartir da instrução esquerda*
JUMP M(X,20:39) : *Traz a próxima leitura da posição X na memória, apartir da instrução direita*
JUMP+M(X,0:19)  : *Caso AC seja não negativo, traz a próxima leitura da posição X na memória, apartir da instrução esquerda*
JUMP+M(X,20:39) : *Caso AC seja não negativo, traz a próxima leitura da posição X na memória, apartir da instrução direita*

ADD M(X)        : *Adiciona em AC o valor da memória no local X*
ADD |M(X)|      : *Adiciona em AC o valor da memória, seu valor absoluto, no local X*
SUB M(X)        : *Diminui em AC o valor da memória no local X*
SUB |M(X)|      : *Diminui em AC o valor da memória, seu valor absoluto, no local X*
MUL M(X)        : *Multiplica em MQ pelo valor da memória no local X, com resultado em AC|MQ conjuntos*
DIV M(X)        : *Divide em AC pelo valor da memória no local X*
LSH             : *Multiplica AC por 2, deslocando 1 bit à esquerda*
RSH             : *Divide AC por 2,     descolando 1 bit à direita*

STOR M(X,8:19)  : *Substitui o endereço à esquerda guardado na memória no local X*
STOR M(X,28:39) : *Substitui o endereço à direita  guardado na memória no local X*

-----------------------------------------------------------------------------------------------------------------------------------------

Dessa forma, segue um exemplo de execução:
120        -> Armazena 120 em M(0)
0          -> Armazena 0   em M(1)
15         -> Armazena 15  em M(2)
LOAD M(0)  -> Carrega M(0) em AC    AC = 120
ADD M(2)   -> Adiciona M(2) em AC   AC = 120 + 15 = 135
STOR M(1)  -> Armazena AC em M(1)


Esperamos uma boa experiência para você, programador.