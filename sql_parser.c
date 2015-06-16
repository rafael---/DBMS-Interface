#include "buffend.h"
#include "sql_parser.h"




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
		valor[c] = 0;
	}
	else
		buffer = copia_token(buffer,valor);
	
	while(buffer && *buffer != ',' && *buffer != '\n' && *buffer != ')') buffer++;	

	return buffer;
}

void to_upper(char * str)	{
	while(*str)	{
		if(islower(*str))
			*str = toupper(*str);
		str++;
	}
}

bool le_insert(char * linha)	{
	char str_buffer[255], tblname[40];
	struct tupla attrs[20];
	uint32_t  tcampos= 0, tvalores=0, ttuples = 0;
	table * tab = malloc(sizeof(table));	

	linha = copia_token(linha, str_buffer);
	to_upper(str_buffer);
	if(strcmp(str_buffer,"INTO"))
		return false;
	linha = copia_token(linha,tblname);
	
	if(!linha) return false;
	
	if(!verificaNomeTabela(tblname)){
		printf("\nTabela \"%s\" não existe\n", tblname);
		return false;
	}

	while(*linha++ == ' ');

	while(*linha != ')')	{
		linha = copia_token(linha,str_buffer);
		if(!linha) return false;
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
			if(!linha) return false;
			strcpy(attrs[tvalores++].valor, str_buffer);
		}
		if(!linha) return false;
		linha++; /* Pula o ')'  */
		ttuples++;
	} while(*linha++ == ',');

	tab->esquema = leSchema(leObjeto(tblname));
	strcpy(tab->nome, tblname);
			
	uint32_t c =0 ;
	/*
		tvalores deve ser um múltiplo de tcampos
			por ex.: attr1,attr2 tem valores (v1,v2) (v3,v4) (v5,v6), etc. 
				e eu decidi não ficar replicando os nomes dos campos
	*/
	if(tvalores != tcampos*ttuples)	{
		return false;
	}	
	
	column * cl = NULL;
	for(c = 0; c < tvalores; c++)
		cl = insereValor(tab, cl, attrs[c%tcampos].campo, attrs[c].valor);
	finalizaInsert(tblname, cl);	
	
	printf("\n%d new rows\n", ttuples);
	
	return true;
}


