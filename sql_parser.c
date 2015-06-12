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


#define	DEBUG

struct tupla	{
	char campo[40];
	char valor[40];
};

char * copia_token(char * buffer, char * comando)	{
	uint32_t c = 0;
	while(*buffer == ' ' || *buffer == ',')
		buffer++;
	while(*buffer != ' ' && *buffer != ',' && *buffer != '(' && *buffer != ')')	{
		if(*buffer == '\n')
				return NULL;
		comando[c++] = *buffer++;
	}
	comando[c] = 0;
	return buffer;	
}

char * copia_string(char * buffer, char * valor)	{
	uint32_t c = 0;
	while(*buffer == ' ' || *buffer == ',')
		buffer++;
	if(*buffer == '\'')	{
		buffer++;
		while(*buffer != '\'')	{
			if(*buffer == '\n')
				return NULL;
			valor[c++] = *buffer++;
		}
	}
	else
		copia_token(buffer,valor);
	valor[c] = 0;
	if(*buffer++ != '\'')
		return NULL;
	return buffer;
}

void to_upper(char * str)	{
	while(*str)	{
		if(islower(*str))
			*str = toupper(*str);
		str++;
	}
}

int32_t le_insert(char * linha)	{
	char str_buffer[255], tblname[40];
	struct tupla attrs[20];
	uint32_t  tcampos= 0, tvalores=0, ttuples = 0;
	linha = copia_token(linha, str_buffer);
	to_upper(str_buffer);
	if(strcmp(str_buffer,"INTO"))
		return false;
	linha = copia_token(linha,tblname);
	if(!linha)
		return false;
	/* Aqui precisa verificar se a tabela `tblname`  existe
		-- Faz isso Ricardo
	 */
	while(*linha++ == ' ');

	while(*linha != ')')	{
		linha = copia_token(linha,str_buffer);
		if(!linha)
			return false;
		strcpy(attrs[tcampos++].campo,str_buffer);
	}	

	linha++; /* Pula o ')'  */
	linha = copia_token(linha,str_buffer);
	to_upper(str_buffer);
	if(strcmp(str_buffer,"VALUES"))
		return false;
	do	{
		while(*linha++ == ' ');
		
		while(*linha != ')')	{
			linha = copia_string(linha,str_buffer);
			if(!linha)
				return false;
			strcpy(attrs[tvalores++].valor, str_buffer);
		}
		linha++; /* Pula o ')'  */
		ttuples++;
	} while(*linha++ == ',');
			
#ifdef DEBUG
	uint32_t c =0 ;
	/*
		tvalores deve ser um múltiplo de tcampos
			por ex.: attr1,attr2 tem valores (v1,v2) (v3,v4) (v5,v6), etc. 
				e eu decidi não ficar replicando os nomes dos campos
	*/
	if(tvalores != tcampos*ttuples)	{
		puts("Não confere");
		return false;
	}
	printf("Table: %s\n",tblname);
	for(c = 0; c < tvalores; c++)
		printf("%s: %s\n",attrs[c%tcampos].campo,attrs[c].valor); 
#endif
	
	return true;
}

int main(int argc, char ** argv)
{
	char *buffer_linha, *xpto, comando[255], dbname[255] = {0};
	size_t tam = 0;
	uint32_t c;
	
	if(argc == 2)	{
		strcpy(dbname,argv[1]);
		/*
			Mil conexao aqui
		*/
		printf("Conectado ao banco de dados: %s\n",dbname);
	}

	printf("\nSGBD da UFFS\n\n%s> ",dbname);

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
		else if(!strcmp(comando,"CONNECT")) 	{
			/* Conecta a um BD. 
				Temos que adicionar esta funcionalidade ao SGBD 
				
			*/
			c= 0;
			while(*xpto == ' ')
				xpto++;
			while(isalnum(*xpto))
				dbname[c++] = *xpto++;
			dbname[c] = 0;
			/*
				Mil conexao aqui
			*/
			printf("Conectado ao banco de dados: %s\n",dbname);
		}
		else if(!strcmp(comando,"EXIT"))	
			break;
		else	
			puts("Comando não reconhecido\n");
		printf("%s> ",dbname);	
	}

	free(buffer_linha);
	return 0;
}
