include ../../make.misc
CFLAGS=${OGL_CFLAGS}
CC=g++

LIBOBJS=JobManager.o \
    Job.o \
    JobManagerServer.o \
    CreateJobCommand.o \
    CreateTaskCommand.o \
    CommandBuilder.o \
    RunnerManager.o \
    RunnerManagerServer.o

OBJS=JobManagerDaemon.o ${LIBOBJS}

all: jmd libjm.a 

jmd: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@ -L../utils/ -L../common -lACE -lpthread -lcommon -lconfig++ -lutils

libjm.a: ${LIBOBJS}
	ar cr libjm.a ${LIBOBJS}
	ranlib libjm.a

${OBJS}: %.o:%.cpp
	${CC} -c ${CFLAGS} $< -o $@ -I../includes/


clean:
	rm -f jmd *.o libjm.a
