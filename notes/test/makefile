args = -Wall -g
test = notes_test
lib = notes
lib_path = ../notes

all: get_all clean_residue

get_all: $(test).o $(lib).o
	gcc -o $(test) $(test).o $(lib).o

$(test).o: $(test).c
	gcc -o $(test).o -c $(test).c $(args)

$(lib).o: $(lib_path).c
	gcc -o $(lib).o -c $(lib_path).c $(args)

clean_residue:
	rm -f *.o vgcore.*

clean: clean_residue
	rm -f $(test)