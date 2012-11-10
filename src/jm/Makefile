include ../../make.misc
CFLAGS=${OGL_CFLAGS}
CC=g++

LIBOBJS=JobManager.o \
     Job.o \
     JobManagerServer.o \
     CreateJobCommand.o \
     CommandBuilder.o

OBJS=JobManagerDaemon.o ${LIBOBJS}

all: JobManagerDaemon libjm.a 

JobManagerDaemon: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@ -L../utils/ -L../common -lACE -lpthread -lcommon -lconfig++ -lutils

libjm.a: ${LIBOBJS}
	ar cr libjm.a ${LIBOBJS}
	ranlib libjm.a

${OBJS}: %.o:%.cpp
	${CC} -c ${CFLAGS} $< -o $@ -I../includes/


clean:
	rm -f JobManagerDaemon *.o libjm.a
