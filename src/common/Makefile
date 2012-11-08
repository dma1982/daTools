include ../../make.misc
CFLAGS=${OGL_CFLAGS}
CC=g++

LIBOBJS=Server.o \
        Commands.o \
        Object.o \
        Exception.o

OBJS=${LIBOBJS}

all: libcommon.a

libcommon.a: ${LIBOBJS}
	ar cr libcommon.a ${LIBOBJS}
	ranlib libcommon.a

${OBJS}: %.o:%.cpp
	${CC} -c ${CFLAGS} $< -o $@ -I../includes/


clean:
	rm -f *.o libcommon.a
