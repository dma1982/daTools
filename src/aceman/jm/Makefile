include ../make.misc

CFLAGS=${SW_CFLAGS}
CC=g++


all: JobManagerDaemon

OBJS=JobManagerDaemon

${OBJS}: %:%.cpp$
	${CC} ${CFLAGS} $< -o $@ -lpthread -L${SW_LIB} -lutils -I${SW_INCLUDE} -lACE

clean:
	rm -f JobManagerDaemon
