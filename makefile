all: stack.out

stack_with_protection.out: stack_main.cpp stack_functions.cpp
	g++ -DSTACK_PROTECTION stack_main.cpp stack_functions.cpp $(cat dedflags.txt) -o stack_with_protection.out

stack.out: stack_main.o stack_functions.o
	g++ stack_main.o stack_functions.o $(cat dedflags.txt) -o stack.out

stack_main.o: stack_main.cpp
	g++ -c stack_main.cpp $(cat dedflags.txt)

stack_functions.o: stack_functions.cpp
	g++ -c stack_functions.cpp $(cat dedflags.txt)
