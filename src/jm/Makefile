include ../../make.misc
CFLAGS=${OGL_CFLAGS}
CC=g++


all: JobManagerDaemon

OBJS=JobManagerDaemon

${OBJS}: %:%.cpp$
	${CC} ${CFLAGS} $< -o $@ -lpthread -L../utils/ -lutils -I../includes/ -lACE

clean:
	rm -f JobManagerDaemon
