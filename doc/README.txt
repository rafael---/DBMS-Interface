Este parser reconhece o comando INSERT, CREATE DATABASE e CREATE TABLE utilizando a 
sintaxe padrão ANSI-SQL 92, incluindo inserção de múltiplas tuplas.

<INSERT>
       INSERT INTO <table_name> (attr1, ..., attrn) VALUES (value11,...,value1n),...,(valuem1,...valuemn);
	
	Insere na tabela <table_name> os valores <value> nos correspondentes atributos <attr>.

<CREATE DATABASE>
	CREATE DATABASE <database_name>;

	Cria e acessa o banco de dados <database_name>

<CREATE TABLE>
	CREATE TABLE <table_name> (atrn_name DATA_TYPE [PRYMARY | REFERENCES tp_name(atr_name)], ...);
	
	DATA TYPE = [INTEGER, DOUBLE, VARCHAR]
	
	Cria a tabela <table_name> com os atributos [atr0_name, atrn_name]. 
	

<DROP TABLE>
	DROPTABLE <table_name>;

	Remove a tabela <table_name> do banco de dados

<CONNECT>
	CONNECT <database_name>;

	Conecta ao banco de dados <database_name>

<SHOW>
	SHOW <table_name>;

	Mostra a tabela <table_name>

<LIST>
	LIST <database_name>;
	
	Lista todas as tabelas que fazem parte do banco de dados <database_name>

<EXIT>
	EXIT;

	Sai do banco de dados




