# This project is to manipulate binary data in C.

PROJECT = binManipulate
GOAL = binManipulate
CFLAGS = -Wall -O3
DATAFILE = testData
INPUT = 
OUTPUT = 

# interactive
run-i: $(GOAL) $(DATAFILE)
	./$(GOAL) $(DATAFILE)

clean:
	rm -f $(GOAL) $(OUTPUT)

submit: $(GOAL).c Makefile README
	tar -czf $(PROJECT).tgz $(GOAL).c Makefile README

