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

char * copia_nome(char * buffer, char * valor)	{
	uint32_t c = 0;
	while(!isalnum(*buffer)) 
		buffer++;
	while(isalnum(*buffer))
		valor[c++] = *buffer++;
	valor[c] = 0;
	return buffer;
}

void to_upper(char * str)	{
	while(*str)	{
		if(islower(*str))
			*str = toupper(*str);
		str++;
	}
}

void to_lower(char * str)	{
	while(*str)	{
		if(isupper(*str))
			*str = tolower(*str);
		str++;
	}
}

bool le_insert(char * linha, char * dbname)	{
	char str_buffer[255], tblname[40];
	struct tupla attrs[20];
	uint32_t  tcampos= 0, tvalores=0, ttuples = 0;
	
	table * tab = malloc(sizeof(table));	

	linha = copia_token(linha, str_buffer);
	to_upper(str_buffer);
	if(strcmp(str_buffer,"INTO"))
		return false;
	linha = copia_token(linha,tblname);

	if(!*dbname)	{
		char *p = strchr(tblname,'.');
		if(!p)	{
			printf("Não conectado a um banco de dados\n");
			return false;
		}
		*p = '/';
	}
	else	{
		char temp[255];
		strcpy(temp,dbname);
		strcat(temp,"/");
		strcat(temp,tblname);
		strcpy(tblname,temp);
	}

	
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

bool create_table(char * linha, char * dbname)	{
	char str_buffer[255], att_temp[100], tblname[40], arquivo[255];
	struct tipotupla tipo_tuplas[20];
	int c = 0, i, k;
	if(dbname[0] == 0)	{
		printf("Você não está conectado a um banco de dados\n");
		return false;
	}
	while(*linha == ' ') linha++;
	while(isalnum(*linha))
		tblname[c++] = *linha++;
	tblname[c] = 0;
	strcpy(arquivo,dbname);
	strcat(arquivo, "/");
	strcat(arquivo,tblname);
	strcat(arquivo,".dat");
	if(existeArquivo(arquivo))	{
		printf("A tabela %s já existe\n",tblname);
		return false;
	}
	c =  0;
	do	{
		i = 0;
		while(!isalnum(*linha)) linha++;
		while(*linha != ',' && *linha != ')' && *linha != ';' && *linha != '\n')	
			str_buffer[i++] = *linha++;
		str_buffer[i] = 0;
		i = k = 0;
		tipo_tuplas[c].attr = 0;
		while(!isalnum(str_buffer[i])) i++;
		while(isalnum(str_buffer[i]))
			tipo_tuplas[c].campo[k++] = str_buffer[i++];
		tipo_tuplas[c].campo[k] = 0;
		k = 0;
		while(!isalnum(str_buffer[i])) i++;
		while(isalnum(str_buffer[i]))
			tipo_tuplas[c].tipo[k++] = str_buffer[i++];
		tipo_tuplas[c].tipo[k] = 0;
		while(str_buffer[i] == ' ') i++;
		if(isalnum(str_buffer[i]))	{
			k = 0;
			while(isalnum(str_buffer[i]))
				att_temp[k++] = str_buffer[i++];
			att_temp[k] = 0;
			to_upper(att_temp);
			if(!strcmp(att_temp,"PRIMARY"))
				tipo_tuplas[c].attr = 1;
			else if(!strcmp(att_temp,"REFERENCES"))	{
				tipo_tuplas[c].attr = 2;
				while(str_buffer[i] == ' ') i++;
				k = 0;
				while(isalnum(str_buffer[i]))
					tipo_tuplas[c].fk_db[k++] = str_buffer[i++];
				tipo_tuplas[c].fk_db[k] = 0;
				while(!isalnum(str_buffer[i])) i++;
				k = 0;
				while(isalnum(str_buffer[i]))
					tipo_tuplas[c].fk_field[k++] = str_buffer[i++];
				tipo_tuplas[c].fk_field[k] = 0;
			}
			else	{
				printf("Erro de sintaxe\n");
				return false;
			}
			
		} 
		
		c++;
	}while(*linha != ')' && *linha != ';' && *linha != '\n');

	arquivo[strlen(arquivo)-4] = 0;

	table * tab = iniciaTabela(arquivo);

	for(i=0; i < c; i++)	{
		int tamanho; char tipo;

		to_upper(tipo_tuplas[i].tipo);

		if(!strcmp(tipo_tuplas[i].tipo,"STRING") || !strcmp(tipo_tuplas[i].tipo,"VARCHAR"))	{
			tipo = 'S';
			tamanho = 20;
		} else if(!strcmp(tipo_tuplas[i].tipo,"INTEGER") || !strcmp(tipo_tuplas[i].tipo,"INT"))	{
			tipo = 'I';
			tamanho = sizeof(int);
		}
		else if(!strcmp(tipo_tuplas[i].tipo,"DOUBLE") || !strcmp(tipo_tuplas[i].tipo,"FLOAT"))	{
			tipo = 'D';
			tamanho = sizeof(double);
		}
		else	{
			printf("%s\n", tipo_tuplas[i].tipo);
			puts("Tipo invalido!");
			return false;
		}
		if(!tipo_tuplas[i].attr)	
			tab = adicionaCampo(tab, tipo_tuplas[i].campo, tipo, tamanho,NPK,"","");     
		else if(tipo_tuplas[c].attr == 1)	
			tab = adicionaCampo(tab, tipo_tuplas[i].campo, tipo, tamanho,PK,"",""); 
		else
			tab = adicionaCampo(tab, tipo_tuplas[i].campo, tipo, tamanho,FK,tipo_tuplas[c].fk_db,tipo_tuplas[c].fk_field); 
	}
	
	finalizaTabela(tab);
	
	puts("CREATE TABLE");

	return true;
}


