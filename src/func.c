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


void tb_schema (char * tbname, char * dbname ){
	struct fs_objects objeto;	
	int i;	
	char temp[200];
	tp_table * schema;

	strcpy(temp, dbname);
	strcat(temp, "/");
	
	strcat(temp, tbname);
	tbname = temp;	
	
	if(!verificaNomeTabela(tbname)){
		printf("\nTabela não existe\n");
		return;
	}	

	objeto = leObjeto(tbname);
	schema = leSchema(objeto);
	
	puts("Atribute | Type | Modificer");

	for ( i = 0; i < objeto.qtdCampos; i++){
		printf("%s ", schema[i].nome);
		switch(schema[i].tipo){
			case 'I': printf("INTEGER ");break;
			case 'D': printf("DOUBLE ");break;
			case 'S': printf("STRING ");
		}
		switch(schema[i].chave){
			case 0: puts(" - ");break;
			case 1: puts("PRIMARY KEY");break;
			case 2: puts("FOREIGN KEY");
		}
	}		

}
