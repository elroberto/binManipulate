# Makefile for Project 1 of CS485, Spring 2015.
# This project is to manipulate binary data in C.

PROJECT = proj1-srande1
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

