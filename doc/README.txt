Este parser reconhece o comando INSERT utilizando a sintaxe padrão ANSI-SQL 92, incluindo inserção de múltiplas tuplas.
Só é aceito um INSERT com os nomes das colunas explícitos.


Sintaxe

INSERT INTO <nome_table> (attr1,..., attrn) VALUES (value_11, ..., value_1n), ... ,(value_m1, ..., value_mn); 

Exemplos de uso:

insert into Aluno (CPF, Nome, Endereco, Peso) values (83223,'Rafael','Avenida Fernando Machado, S/N',75.0), (9833,'Ricardo','Avenida General Osorio, 666N', 50.0);
insert into Aluno (CPF, Nome, Endereco, Peso) values (83225,'Harold','Rua das Borboletas, S/N',25.0), (98338,'Maria Pinto','Rua de Jesus, 7N', 120.0);
insert into Aluno (CPF, Nome, Endereco, Peso) values (83229,'Jesus Cristo','Rua Jerusalem, S/N',60.0);
INSERT INTO Aluno (CPF, Nome, Endereco, Peso) VALUES (39091,'Baphomet','Avenida Bom Jesus, 666',128.0);
