FLAGS = -Wall -L./ -m32

all: ut.a binsem.a ph

ph: ph.c 
	gcc $(FLAGS) ph.c -lbinsem -lut -o ph


binsem.a: binsem.c binsem.h atomic.h 
	gcc $(FLAGS) -c binsem.c
	ar rcu libbinsem.a binsem.o
	ranlib libbinsem.a

ut.a: ut.c ut.h
	gcc $(FLAGS) -c ut.c
	ar rcu libut.a ut.o
	ranlib libut.a

# clean up
clean:
	rm -f *~
	rm -f a.out
	rm -f *.o
	rm -f *.a
	rm -f ph