include ../../make.misc
CFLAGS=${OGL_CFLAGS}
CC=g++

LIBOBJS=JobRunnerManager.o 

OBJS=JobRunnerDaemon.o ${LIBOBJS}

all: jrd libjr.a 

jrd: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@ -L../utils/ -L../common -lACE -lpthread -lcommon -lconfig++ -lutils

libjr.a: ${LIBOBJS}
	ar cr libjr.a ${LIBOBJS}
	ranlib libjr.a

${OBJS}: %.o:%.cpp
	${CC} -c ${CFLAGS} $< -o $@ -I../includes/


clean:
	rm -f jrd *.o libjr.a
