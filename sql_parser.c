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
	printf("%s\n",arquivo);
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
	c--;
	for(; c >= 0; c--)	{
		/*
			Aqui vc primeiramente verifica se os tipos são válidos (String e varchar = 'S', double e float = 'D', int e integer = 'I')
			daí conforme a estrutura
				struct tipotupla	{
				char campo[40];
				char tipo[20];
				char attr; // 0 - Nothing, 1 - Primary key, 2 - Foreign Key 
				char fk_db[40];
				char fk_field[40];
			};
			Precisa converter o tipo
			vc passa como argumento. Por exemplo
				tab = iniciaTabela(tblname);
            			tab = adicionaCampo(tab, tipo_tuplas[c].campo, tipo_tuplas[c].tipo[0], sizeof(tipo) ou n,FK,fk_db,fk_field);     
				...
				tab = finalizaTabela(tab);   

				*/
		if(!tipo_tuplas[c].attr)
			printf("%s - %s\n",tipo_tuplas[c].campo,tipo_tuplas[c].tipo);
		else if(tipo_tuplas[c].attr == 1)
			printf("PK - %s - %s\n",tipo_tuplas[c].campo,tipo_tuplas[c].tipo);
		else
			printf("FK - %s - %s REF (%s.%s)\n",tipo_tuplas[c].campo,tipo_tuplas[c].tipo,tipo_tuplas[c].fk_db,tipo_tuplas[c].fk_field);
	}
	putchar('\n');

	return true;
}

