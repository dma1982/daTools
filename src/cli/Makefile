include ../../make.misc
CFLAGS=${OGL_CFLAGS}
CC=g++

OBJS=Submit.o Connection.o JobProxy.o TaskProxy.o

all: ogl

ogl: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@ -L../utils/ -L../common  -lACE -lpthread -lcommon  -lutils -lconfig++

${OBJS}: %.o:%.cpp
	${CC} -c ${CFLAGS} $< -o $@ -I../includes/


clean:
	rm -f ogl *.o
