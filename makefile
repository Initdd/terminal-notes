options = -Wall -g
app = notes_app
notes_manager = notes_manager
window_manager = window_manager
notes = ./$(notes_manager)/notes_manager.c
windows = ./$(window_manager)/window_manager.c
dev_mode = -DDEVMODE

#all: $(app) 

all: $(app).o $(notes_manager).o $(window_manager).o
	gcc -o $(app) ./$(app).o ./$(notes_manager).o ./$(window_manager).o $(options) -lcurses

dev: $(app)_dev.o $(notes_manager).o $(window_manager).o
	gcc -o $(app) ./$(app).o ./$(notes_manager).o ./$(window_manager).o $(options) -lcurses

$(app).o: $(app).c
	gcc -o $(app).o $(app).c -c $(options) 

$(app)_dev.o: $(app).c
	gcc $(dev_mode) -o $(app).o $(app).c -c $(options)

$(notes_manager).o: 
	gcc -o $(notes_manager).o $(notes) -c $(options)

$(window_manager).o:
	gcc -o $(window_manager).o $(windows) -c $(options)

clean: clean_residue
	rm -f $(app)

clean_residue:
	rm -f *.o
