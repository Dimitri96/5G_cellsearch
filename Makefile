default: cellsearch 

cellsearch.o: src/main.c
	gcc -c src/main.c -o cellsearch.o -lm

cellsearch: cellsearch.o
	gcc cellsearch.o -o cellsearch -lm

clean:
	-rm -f cellsearch.o
	-rm -f cellsearch