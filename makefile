options = -Wall -g
app = notes_app
notes_manager = notes_manager
window_manager = window_manager
notes = ./$(notes_manager)/notes_manager.c
windows = ./$(window_manager)/window_manager.c
dev_mode = -DDEVMODE
build_dir = build

all: dev # The default target is the development mode

prod: $(build_dir)/$(app).o $(build_dir)/$(notes_manager).o $(build_dir)/$(window_manager).o
	gcc -o $(build_dir)/$(app) $(build_dir)/$(app).o $(build_dir)/$(notes_manager).o $(build_dir)/$(window_manager).o $(options) -lcurses

dev: $(build_dir)/$(app)_dev.o $(build_dir)/$(notes_manager).o $(build_dir)/$(window_manager).o
	gcc -o $(build_dir)/$(app) $(build_dir)/$(app).o $(build_dir)/$(notes_manager).o $(build_dir)/$(window_manager).o $(options) -lcurses

$(build_dir)/$(app).o: $(app).c
	mkdir -p $(build_dir)
	gcc -o $(build_dir)/$(app).o $(app).c -c $(options) 

$(build_dir)/$(app)_dev.o: $(app).c
	mkdir -p $(build_dir)
	gcc $(dev_mode) -o $(build_dir)/$(app).o $(app).c -c $(options)

$(build_dir)/$(notes_manager).o: 
	mkdir -p $(build_dir)
	gcc -o $(build_dir)/$(notes_manager).o $(notes) -c $(options)

$(build_dir)/$(window_manager).o:
	mkdir -p $(build_dir)
	gcc -o $(build_dir)/$(window_manager).o $(windows) -c $(options)

clean: clean_residue
	rm -f $(build_dir)/$(app)

clean_residue:
	rm -f $(build_dir)/*.o
	rm -rf $(build_dir)