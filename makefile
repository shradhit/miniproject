project: calculator.o stack.o cstack.o 
	cc   calculator.o stack.o cstack.o -o project
calculator.o: calculator.c cstack.h cstack.h  
	cc -c  calculator.c 
cstack.o: cstack.c stack.h
	cc -c  cstack.c
stack.o : cstack.c stack.h
	cc -c  stack.c
