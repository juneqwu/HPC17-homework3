CC=mpicc
FLAGS=-O3
EXECS=sr sr2 sr2_nonblocking pingpong pingpong_array pingpong_nonblocking

all: ${EXECS}

sr: sr.c
	${CC} ${FLAGS} $^ -o sr

sr2: sr2.c
	${CC} ${FLAGS} $^ -o sr2

sr2_nonblocking: sr2_nonblocking.c
	${CC} ${FLAGS} $^ -o sr2_nonblocking

pingpong: pingpong.c
	${CC} ${FLAGS} $^ -o pingpong

pingpong_array: pingpong_array.c
	${CC} ${FLAGS} $^ -o pingpong_array


clean:
	rm -f ${EXECS}
