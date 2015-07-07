#include "func.h"


void db_table ( char * dbname ){
    FILE * dic;
    char * tupla;
    int i;
    
    if((dic = fopen("fs_object.dat","a+b")) == NULL)
        return;
    
    tupla = malloc(TAMANHO_NOME_TABELA);

    while(!feof(dic)){
	fread(tupla, 1, TAMANHO_NOME_TABELA, dic);
	if(feof(dic))break;
	i = 0;
	while(dbname[i] != '\0' && *tupla != '/'){
	     if(dbname[i]!=*tupla)
	          break;
	     tupla++;
	     i++;
	}
	if(dbname[i]=='\0' && *tupla++ =='/')
	     puts(tupla);
	fseek(dic, 28, 1);
    }
}

/*
void tb_schema (char * tbname, char * dbname ){
	strcat(dbname, "/");
	strcat(dbname, tbname);
	tbname = dbname;	
	
	if(!verificaNomeTabela(tbname)){
		printf("\nTabela não existe\n");
		return;
	}
	while(*tbname++!='/');
	
}*/
