TARGET = builtin.o
CC=gcc
CFLAGS = -g -DDEBUG -Wall -ansi -Wswitch-enum

$(TARGET):builtin.c
	$(CC) -c $(CFLAGS) -I../../include $*.c

BUILTIN_SCRIPT = \
  ../../require/zero/lang.zeh\
  ../../require/zero/lang.ze

builtin.c:builtin_conv head.txt tail.txt $(BUILTIN_SCRIPT)
	./builtin_conv

CONV_OBJS =\
  ../../memory/memory.o\
  ../../debug/debug.o

builtin_conv:builtin_conv.c ../../include/ZVM.h ../zeroc.h
	$(CC) $(CFLAGS) -o$@ -I../../include builtin_conv.c $(CONV_OBJS)
############################################################
builtin.o: builtin.c ../zeroc.h ../../include/MEM.h ../../include/ZEC.h \
  ../../include/ZVM_code.h ../../include/ZVM.h ../../include/ZVM_code.h \
  ../../include/share.h ../../include/ZEC.h
builtin_conv.o: builtin_conv.c ../../include/ZVM.h ../../include/DBG.h
