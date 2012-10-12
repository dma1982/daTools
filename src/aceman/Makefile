include make.misc

CFLAGS=${SW_CFLAGS}
CC=g++


all: test_ace_task

OBJS=test_ace_task

${OBJS}: %:%.cpp$
	${CC} ${CFLAGS} $< -o $@ -lpthread -L${SW_LIB} -lutils -I${SW_INCLUDE} -lACE

clean:
	rm -f test_mem test_ace_task
