LOPTS = -Wall -Wextra -g -std=c99
OOPTS = $(LOPTS) -c

# cd /mnt/c/CS201-master/CS201FALL2018/assign0/

all: test-da test-cda test-stack test-queue

test: test-da test-cda test-stack test-queue
	./test-da
	./test-cda
	./test-stack
	./test-queue

run: test clean

# Dynamic Array
test-da: integer.o da.o test-da.o
	gcc $(LOPTS) integer.o da.o test-da.o -o test-da

test-da.o: test-da.c da.h integer.h
	gcc $(OOPTS) test-da.c

da.o: da.c da.h
	gcc $(OOPTS) da.c

# Circular Dynamic Array
test-cda: integer.o cda.o test-cda.o
	gcc $(LOPTS) integer.o cda.o test-cda.o -o test-cda

test-cda.o: test-cda.c cda.h integer.h
	gcc $(OOPTS) test-cda.c

cda.o: cda.c cda.h
	gcc $(OOPTS) cda.c

# Stack
test-stack: integer.o da.o stack.o test-stack.o
	gcc $(LOPTS) integer.o da.o stack.o test-stack.o -o test-stack

test-stack.o: test-stack.c stack.h integer.h da.h
	gcc $(OOPTS) test-stack.c

stack.o: stack.c stack.h
	gcc $(OOPTS) stack.c

# Queue
test-queue: integer.o cda.o queue.o test-queue.o
	gcc $(LOPTS) integer.o cda.o queue.o test-queue.o -o test-queue

test-queue.o: test-queue.c queue.h integer.h cda.h
	gcc $(OOPTS) test-queue.c

queue.o: queue.c queue.h
	gcc $(OOPTS) queue.c

#Integer
integer.o: integer.c integer.h
	gcc $(OOPTS) integer.c

#Valgrind
valgrind  : all
	echo testing Dynamic Array
	valgrind ./test-da
	echo
	echo testing Circular Dynamic Array
	valgrind ./test-cda
	echo
	echo testing stack
	valgrind ./test-stack
	echo
	echo testing queue
	valgrind ./test-queue
	echo

clean:
	rm -f integer.o
	rm -f test-da.o da.o test-da
	rm -f test-cda.o cda.o test-cda
	rm -f test-stack.o stack.o test-stack
	rm -f test-queue.o queue.o test-queue
