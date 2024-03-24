args = -Wall -g
test_notes = notes_test
test_list_notes = notes_list_test
test_files_notes = notes_files_test
lib = notes
lib_path = ../notes

all: get_all clean_residue

execute: get_all
	./$(test_notes)
	./$(test_list_notes)
	./$(test_files_notes)

get_all: $(test_notes).o $(lib).o
	gcc -o $(test_notes) $(test_notes).o $(lib).o $(args)
	gcc -o $(test_list_notes) $(test_list_notes).c $(lib).o $(args)
	gcc -o $(test_files_notes) $(test_files_notes).c $(lib).o $(args)

$(test_notes).o: $(test_notes).c
	gcc -o $(test_notes).o -c $(test_notes).c $(args)

$(test_list_notes).o: $(test_list_notes).c
	gcc -o $(test_list_notes).o -c $(test_list_notes).c $(args)

$(test_files_notes).o: $(test_files_notes).c
	gcc -o $(test_files_notes).o -c $(test_files_notes).c $(args)

$(lib).o: $(lib_path).c
	gcc -o $(lib).o -c $(lib_path).c $(args)

clean_residue:
	rm -f *.o vgcore.*

clean: clean_residue
	rm -f $(test_notes) $(test_list_notes) $(test_files_notes)