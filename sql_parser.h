/*
	Descrição: interface SQL simples para o SGBD da disciplina BD II
	Autores:
		Rafael Hengen Ribeiro <rafaelhr.ribeiro@gmail.com>
		Ricardo Parizotto <ricardo.dparizotto@gmail.com>
	
	Compilar com: 
		{gcc,clang} sql_parser.c -o sql_parser -Wall -pedantic -std=gnu99 -O2
	*Obs: Não compilar com -ansi, pois o GNU getline() não faz parte do ANSI C  
		
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

void to_upper(char * str);

bool le_insert(char * linha);

char * copia_string(char * buffer, char * valor);	
