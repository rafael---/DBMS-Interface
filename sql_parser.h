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


struct tupla	{
	char campo[40];
	char valor[40];
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

bool le_insert(char * linha);
/*
Esta função é responsável por ler uma linha e verificar a sintaxe e inserir o(s) valor(es) na tabela
*/

char * copia_string(char * buffer, char * valor);	
/*
É a função responsável por copiar os valores dos atributos formatados do buffer.
*/
