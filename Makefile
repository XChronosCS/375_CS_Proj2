M = program2

CFLAGS= -g -Wall -Werror -std=c++14

all: $(M)

$(M): $(M).o
	g++ $(CFLAGS) $(M).o -o $(M)

$(M).o: $(M).cpp
	g++ $(CFLAGS) -c $(M).cpp -o $(M).o

.PHONY: clean run all memcheck

run: all
	./$(M)

clean:
	rm -rf *.o $(M).o $(M)

memcheck: program2
	valgrind --leak-check=full ./$(M)
