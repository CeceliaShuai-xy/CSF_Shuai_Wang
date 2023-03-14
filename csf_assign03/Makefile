CC = g++
CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)


csim: main.o csim_functions.o
	$(CC) -o csim main.o csim_functions.o

main.o: main.cpp csim_functions.h 
	$(CC) -c main.cpp $(CFLAGS)

csim_functions.o: csim_functions.cpp csim_functions.h 
	$(CC) -c csim_functions.cpp $(CFLAGS)

.PHONY: clean all
clean:
	rm -f *.o csim