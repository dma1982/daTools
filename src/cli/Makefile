include ../../make.misc
CFLAGS=${OGL_CFLAGS}
CC=g++

OGL_SUB=OglSubmit.o
OGL_CTRL=OglControl.o
COMM_OBJS=JobManagerProxy.o JobProxy.o TaskProxy.o

OBJS=${OGL_SUB} ${OGL_CTRL} ${COMM_OBJS}

all: oglsub oglctrl

oglsub: ${OBJS}
	${CC} ${CFLAGS} OglSubmit.o ${COMM_OBJS} -o $@ -L../utils/ -L../common -lcommon -lutils ${OGL_EXT_LIBS}

oglctrl: ${OBJS}
	${CC} ${CFLAGS} OglControl.o ${COMM_OBJS} -o $@ -L../utils/ -L../common -lcommon -lutils ${OGL_EXT_LIBS}

${OBJS}: %.o:%.cpp
	${CC} -c ${CFLAGS} $< -o $@ -I../includes/


clean:
	rm -f oglsub oglctrl *.o
