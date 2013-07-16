include ../../make.misc
CFLAGS=${OGL_CFLAGS}
CC=g++

LIBOBJS=JobManager.o \
    Job.o \
    Task.o \
    ClientObjectManager.o \
    JobRunnerObjectManager.o \
    JobScheduler.o \
    JobSchedulerExecutor.o \
    P_FCFS.o 

OBJS=JobManagerDaemon.o ${LIBOBJS}

all: jmd libjm.a 

jmd: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@ -L../utils/ -L../common ${OGL_EXT_LIBS} -lcommon -lutils 

libjm.a: ${LIBOBJS}
	ar cr libjm.a ${LIBOBJS}
	ranlib libjm.a

${OBJS}: %.o:%.cpp
	${CC} -c ${CFLAGS} $< -o $@ -I../includes/


clean:
	rm -f jmd *.o libjm.a
