default: cellsearch 

cellsearch.o: src/main.c
	gcc -c src/main.c -o cellsearch.o -DLV_HAVE_NEON

cellsearch: cellsearch.o
	gcc cellsearch.o -o cellsearch -DLV_HAVE_NEON

clean:
	-rm -f cellsearch.o
	-rm -f cellsearch