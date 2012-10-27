include ../../make.misc
CFLAGS=${OGL_CFLAGS}
CC=g++

LIBOBJS=JobManager.o \
     Job.o \
     Commands.o

OBJS=JobManagerDaemon.o ${LIBOBJS}

all: JobManagerDaemon libjm.a

JobManagerDaemon: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@ -L../utils/ -lutils -lACE -lpthread 

libjm.a: ${LIBOBJS}
	ar cr libjm.a ${LIBOBJS}
	ranlib libjm.a

${OBJS}: %.o:%.cpp
	${CC} -c ${CFLAGS} $< -o $@ -I../includes/


clean:
	rm -f JobManagerDaemon *.o libjm.a
