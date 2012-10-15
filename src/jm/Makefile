include ../../make.misc
CFLAGS=${OGL_CFLAGS}
CC=g++

OBJS=JobManagerDaemon.o \
     JobManager.o \
     Job.o \
     Commands.o

JobManagerDaemon: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@ -L../utils/ -lutils -lACE -lpthread 

${OBJS}: %.o:%.cpp
	${CC} -c ${CFLAGS} $< -o $@ -I../includes/


clean:
	rm -f JobManagerDaemon *.o
