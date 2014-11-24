# Makefile for Pacnix project

COMPILER_TYPE= gnu

CC=gcc

PROG=	pacnix
SRCS=	main.c pacnix.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-llm -ldriver -lsys 

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>