# Target library

lib := libuthread.a

all: $(lib)

## TODO: Phase 1

#ar rcs my_library.a file1.o file2.o
#how to use this command???

mythreads: context.o preempt.o queue.o sem.o uthread.o
	gcc -Wall -Wextra -Werror -o mythreads context.o preempt.o queue.o sem.o uthread.o

context.o: context.c private.h uthread.h
	gcc -Wall -Wextra -Werror -c -o context.o context.c 

preempt.o: preempt.c priv.h uthread.h
	gcc -Wall -Wextra -Werror -c -o preempt.o preempt.c

queue.o: queue.c queue.h
	gcc -Wall -Wextra -Werror -c -o queue.o queue.c

sem.o: sem.c queue.h sem.h
	gcc -Wall -Wextra -Werror -c -o sem.o sem.c

uthread.o: uthread.c priv.h uthread.h queue.h
	gcc -Wall -Wextra -Werror -c -o uthread.o uthread.c

README.html: README.md
	pandoc -o README.html README.md