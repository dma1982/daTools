include ../../make.misc
CFLAGS=${OGL_CFLAGS}
CC=g++

LIBOBJS=JobRunner.o JobRunnerManager.o 

OBJS=JobRunnerDaemon.o ${LIBOBJS}

all: jrd libjr.a 

jrd: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@ -L../utils/ -L../common -lcommon -lutils ${OGL_EXT_LIBS}

libjr.a: ${LIBOBJS}
	ar cr libjr.a ${LIBOBJS}
	ranlib libjr.a

${OBJS}: %.o:%.cpp
	${CC} -c ${CFLAGS} $< -o $@ -I../includes/


clean:
	rm -f jrd *.o libjr.a
