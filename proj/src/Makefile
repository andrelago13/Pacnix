# Makefile for Pacnix project

COMPILER_TYPE= gnu

CC=gcc

PROG=	pacnix
SRCS=	main.c pacnix.c vmem_txt.c timer.c kbd_funct.c asmhandlerkbd.S mouse.c read_xpm.c video_gr.c videobe.c asprite.c sprite.c maze.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-llm -ldriver -lsys -lvt

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>