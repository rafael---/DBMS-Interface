all:
	mkdir bin/
	gcc src/buffend.c src/sql_parser.c src/dbms.c src/func.c -o bin/sql_parser -Wall -pedantic -std=gnu99 -O2

debug:
	mkdir debug/
	gcc src/buffend.c src/sql_parser.c src/dbms.c src/func.c -o debug/sql_parser -Wall -pedantic -std=gnu99 -g

clean_debug:
	rm debug/sql_parser && rm -Rf debug/

clean:
	rm bin/sql_parser && rm -Rf bin/
