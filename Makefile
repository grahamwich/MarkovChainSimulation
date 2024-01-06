CC = g++
CFLAGS = -g --std=c++14 -Wall -Werror -pedantic
LIB = -lboost_unit_test_framework -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
DEPS = RandWriter.hpp

all: TextWriter test lint

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $< $(LIB)

TextWriter: main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

test: test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

lint: 
	cpplint --filter=-runtime/references,-build/c++11,-build/include_subdir, --root=. *.cpp *.hpp

clean:
	rm *.o TextWriter test
