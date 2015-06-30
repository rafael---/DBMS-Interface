#include "sql_parser.h"
#include "buffend.h"


int main(int argc, char ** argv)
{
	char *buffer_linha, *xpto, comando[255], dbname[255] = {0};
	size_t tam = 0;
	uint32_t c;
	table * tab;


	tab = iniciaTabela("Aluno");                                              
        tab = adicionaCampo(tab, "CPF"     , 'I', (sizeof(int))   ,PK,"","");    
        tab = adicionaCampo(tab, "Nome"    , 'S', 20              ,NPK,"","");        
        tab = adicionaCampo(tab, "Endereco", 'S', 20              ,NPK,"","");
       	tab = adicionaCampo(tab, "Peso"    , 'D', (sizeof(double)),NPK,"","");
        finalizaTabela(tab);
	
	if(argc == 2)	{
		strcpy(dbname,argv[1]);
		printf("Conectado ao banco de dados: %s\n",dbname);
	}

	printf("\nSGBD da UFFS\n\n%s> ",dbname);

	while(getline(&buffer_linha, &tam, stdin) != EOF)	{
		c = 0;
		xpto = buffer_linha;
		while(*xpto != ' ' && *xpto != '\n' && *xpto != ';')	
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
			c = 0;
			while(*xpto == ' ')
				xpto++;
			while(isalnum(*xpto))
				dbname[c++] = *xpto++;
			dbname[c] = 0;
				
			DIR* dir = opendir(dbname);

			if (!dir) {
				printf("O banco de dados %s não existe\n",dbname);
				memset(dbname,0,5);
			}
			else	{
				closedir(dir);
				printf("Conectado ao banco de dados: %s\n",dbname);
			}
		}
		else if(!strcmp(comando,"CREATE"))	{
			xpto = copia_token(xpto,comando);
			to_upper(comando);
			if(!strcmp(comando,"DATABASE"))	{
				while(*xpto == ' ')
					xpto++;
				c = 0;
				while(isalnum(*xpto))
					dbname[c++] = *xpto++;
				to_lower(dbname);
				if(!mkdir(dbname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))	{
        			printf("Banco de dados criado com sucesso\n");
					printf("Conectado ao banco de dados: %s\n",dbname);
				}
				else	{
					printf("Erro ao criar banco de dados %s\n",dbname);
					dbname[0] = 0;
				}
			}
			else if(!strcmp(comando,"TABLE"))	
				create_table(xpto,dbname);
		}
		else if(!strcmp(comando,"EXIT"))	
			break;
		else if(!strcmp(comando, "SELECT_ALL"))
			imprime("Aluno"); 	
	
		else	
			puts("Comando não reconhecido\n");
		printf("%s> ",dbname);	
	}

	free(buffer_linha);
	return 0;
}
