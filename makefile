options = -Wall -g
app = notes_app
lib = notes_lib
notes = ./$(lib)/notes.c

all: $(app).o $(lib).o
	gcc -o $(app) ./$(app).o ./$(lib).o $(options) -lcurses
	rm -f $(app).o $(lib).o

$(app).o: $(app).c
	gcc -o $(app).o $(app).c -c $(options) 

$(lib).o: 
	gcc -o $(lib).o $(notes) -c $(options)

clean:
	rm -f *.o $(app)