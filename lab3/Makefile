# Makefile for the lab3 driver (based on lab2).

COMPILER_TYPE= gnu

CC=gcc

PROG=	lab3
SRCS=	lab3.c test3.c timer.c asmhandler.S

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-lvt -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>