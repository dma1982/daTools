include ../../make.misc
CFLAGS=${OGL_CFLAGS}
CC=g++

OBJS=OglSubmit.o JobManagerProxy.o JobProxy.o TaskProxy.o

all: oglsub

oglsub: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@ -L../utils/ -L../common -lcommon -lutils ${OGL_EXT_LIBS}

${OBJS}: %.o:%.cpp
	${CC} -c ${CFLAGS} $< -o $@ -I../includes/


clean:
	rm -f oglsub *.o
