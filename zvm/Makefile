TARGET = zvm.o
CC=gcc
CFLAGS = -c -g -DDEBUG -Wall -ansi -Wswitch-enum
OBJS = execute.o \
  load.o \
  heap.o \
  native.o \
  nativeif.o \
  wchar.o \
  util.o \
  error.o \
  error_message.o

$(TARGET):$(OBJS)
	ld -r -o $@ $(OBJS)
.c.o:
	$(CC) $(CFLAGS) -I../include $*.c
error.o: error.c ../include/MEM.h ../include/DBG.h zvm_pri.h \
  ../include/ZVM_code.h ../include/ZVM.h ../include/ZVM_dev.h \
  ../include/share.h ../include/ZVM_code.h
error_message.o: error_message.c zvm_pri.h ../include/ZVM_code.h \
  ../include/ZVM.h ../include/ZVM_dev.h ../include/share.h \
  ../include/ZVM_code.h
execute.o: execute.c ../include/MEM.h ../include/DBG.h zvm_pri.h \
  ../include/ZVM_code.h ../include/ZVM.h ../include/ZVM_dev.h \
  ../include/share.h ../include/ZVM_code.h
heap.o: heap.c ../include/MEM.h ../include/DBG.h zvm_pri.h \
  ../include/ZVM_code.h ../include/ZVM.h ../include/ZVM_dev.h \
  ../include/share.h ../include/ZVM_code.h
load.o: load.c ../include/ZEC.h ../include/ZVM_code.h ../include/ZVM.h \
  ../include/MEM.h ../include/DBG.h zvm_pri.h ../include/ZVM_code.h \
  ../include/ZVM_dev.h ../include/share.h
native.o: native.c ../include/MEM.h ../include/DBG.h zvm_pri.h \
  ../include/ZVM_code.h ../include/ZVM.h ../include/ZVM_dev.h \
  ../include/share.h ../include/ZVM_code.h
nativeif.o: nativeif.c ../include/MEM.h ../include/DBG.h zvm_pri.h \
  ../include/ZVM_code.h ../include/ZVM.h ../include/ZVM_dev.h \
  ../include/share.h ../include/ZVM_code.h
util.o: util.c ../include/MEM.h ../include/DBG.h zvm_pri.h \
  ../include/ZVM_code.h ../include/ZVM.h ../include/ZVM_dev.h \
  ../include/share.h ../include/ZVM_code.h
wchar.o: wchar.c ../include/DBG.h ../include/MEM.h zvm_pri.h \
  ../include/ZVM_code.h ../include/ZVM.h ../include/ZVM_dev.h \
  ../include/share.h ../include/ZVM_code.h
