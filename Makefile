include ./Make.defines

PROGS =	test_logger
OBJS = main.o logger.o

all:	${PROGS}

${OBJS}: logger.h

test_logger:	${OBJS}
		${CC} ${CFLAGS} -o $@ ${OBJS} ${LIBS}

clean:
		rm -f ${PROGS} core core.* *.o temp.* *.out typescript*
