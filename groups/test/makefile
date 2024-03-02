
TEST_FILE = groups_test
GROUPS_FILE = groups
GROUPS_FILE_FULL_PATH = ../$(GROUPS_FILE)

ARGS = -Wall -g

all: $(GROUPS_FILE).o $(TEST_FILE).o
	gcc -o $(TEST_FILE) $(GROUPS_FILE).o $(TEST_FILE).o $(ARGS)
	rm -f $(GROUPS_FILE).o $(TEST_FILE).o

$(GROUPS_FILE).o: $(GROUPS_FILE_FULL_PATH).c
	gcc -c $(GROUPS_FILE_FULL_PATH).c $(ARGS)

$(TEST_FILE).o: $(TEST_FILE).c
	gcc -c $(TEST_FILE).c $(ARGS)

clean:
	rm -f $(GROUPS_FILE).o $(TEST_FILE).o $(TEST_FILE) vgcore.*

