/*
	Descrição: interface SQL simples para o BD da disciplina BD II
	Autores:
		Rafael Hengen Ribeiro <rafaelhr.ribeiro@gmail.com>
		Ricardo Parizotto
	
	*Obs: Não compilar com -ansi ou -std=<padrao_qualquer>, 
		pois o getline() não faz parte do ANSI C  
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define	DEBUG

struct tupla	{
	char campo[40];
	char valor[40];
};

char * copia_token(char * buffer, char * comando)	{
	uint32_t c = 0;
	while(*buffer == ' ' || *buffer == ',')
		buffer++;
	while(*buffer != ' ' && *buffer != ',' && *buffer != ')')
		comando[c++] = *buffer++;
	comando[c] = 0;
	return buffer;	
}

void to_upper(char * str)	{
	while(*str)	{
		if(*str >= 'a' && *str <= 'z')
			*str -= 32;
		str++;
	}
}

bool le_insert(char * linha)	{
	char minibuffer[255], tblname[40];
	struct tupla attrs[20];
	uint32_t  tcampos= 0, tvalores=0;
	linha = copia_token(linha, minibuffer);
	to_upper(minibuffer);
	if(strcmp(minibuffer,"INTO"))
		return false;
	linha = copia_token(linha,tblname);
	/* Aqui precisa verificar se a tabela existe */
	while(*linha++ == ' ');
	/* verificar se existe ')' */	

	while(*linha != ')')	{
		linha = copia_token(linha,minibuffer);
		strcpy(attrs[tcampos++].campo,minibuffer);
	}	
	linha++; /* Pula o ')'  (verificar)*/
	linha = copia_token(linha,minibuffer);
	to_upper(minibuffer);
	if(strcmp(minibuffer,"VALUES"))
		return false;
	while(*linha++ == ' ');
	
	while(*linha != ')')	{
		linha = copia_token(linha,minibuffer);
		strcpy(attrs[tvalores++].valor, minibuffer);
	}
			
#ifdef DEBUG
	uint32_t c =0 ;
	if(tvalores != tcampos)	{
		puts("Não confere");
		return false;
	}
	printf("Table: %s\n",tblname);
	for(c = 0; c < tcampos; c++)
		printf("%s: %s\n",attrs[c].campo,attrs[c].valor); 
#endif
	
	return true;
}

int main()
{
	char *buffer_linha, *xpto, comando[255];
	size_t tam = 0;
	uint32_t c;

	printf("\nSGBD da UFFS\n\n> ");

	while(getline(&buffer_linha, &tam, stdin) != EOF)	{
		c = 0;
		xpto = buffer_linha;
		while(*xpto != ' ' && *xpto != '\n')	
				comando[c++] = *xpto++;
		comando[c] = 0;
		/* Reconhecimento de comandos */
		to_upper(comando);
		if(!strcmp(comando,"INSERT"))	{
			if(!le_insert(xpto))
				puts("Erro de sintaxe no INSERT");
			/* Temos que implementar os códigos de erro para especificar bem isso*/
		}
		else if(!strcmp(comando,"CONNECT"))	{
			/* Conecta a um BD. 
				Temos que adicionar esta funcionalidade ao SGBD */
		}
		else if(!strcmp(comando,"EXIT"))	
			break;
		else	
			puts("Comando não reconhecido\n");
		printf("> ");	
	}

	free(buffer_linha);
	return 0;
}
