/*
	Descrição: interface SQL simples para o SGBD da disciplina BD II
	Autores:
		Rafael Hengen Ribeiro <rafaelhr.ribeiro@gmail.com>
		Ricardo Parizotto <ricardo.dparizotto@gmail.com>
	
	Compilar com: 
		{gcc,clang} sql_parser.c -o sql_parser -Wall -pedantic -std=gnu99 -O2
	*Obs: Não compilar com -ansi, pois o GNU getline() não faz parte do ANSI C  

	O getline() é uma função segura para a leitura de strings. 
	Além disso, a string é alocada dinamicamente, o que previne o truncamento da string ou até mesmo um buffer overflow no caso de uma função não segura como o gets();
		
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

struct tupla	{
	char campo[40];
	char valor[40];
};

struct tipotupla	{
	char campo[40];
	char tipo[20];
	char attr; /* 0 - Nothing, 1 - Primary key, 2 - Foreign Key */
	char fk_db[40];
	char fk_field[40];
};

char * copia_token(char * buffer, char * comando);
/* 
Copia os tokens da linha para o buffer da string 
O parâmetro buffer é a linha que foi passado pelo usuário
O comando é um mini-buffer para onde é copiado o primeiro token da linha do buffer para avalição futura da sintaxe
*/

void to_upper(char * str);
/*
Converte a string str para letras maiúsculas para garantir que os comandos não sejam case-sensitive
*/

void to_lower(char * str);
/*
Converte a string str para letras minúsculas
*/

bool le_insert(char * linha, char * dbname);
/*
Esta função é responsável por ler uma linha e verificar a sintaxe e inserir o(s) valor(es) na tabela
*/

char * copia_string(char * buffer, char * valor);	
/*
É a função responsável por copiar os valores dos atributos formatados do buffer.
*/

char * copia_nome(char * buffer, char * valor);
/*
Copia os "nomes" de tuplas, funções, comandos mais simples, etc. 
Aceita apenas letras e números, apaga todos os outros caracteres da string
*/

bool create_table(char * linha, char * dbname);
/*
Esta função é responsável por interpretar o comando CREATE TABLE e chamar as funções de criação de tabela anteriormente criadas
*/
