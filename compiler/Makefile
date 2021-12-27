TARGET = zero
CC=gcc
OBJS = \
  lex.yy.o\
  y.tab.o\
  main.o\
  interface.o\
  create.o\
  fix_tree.o\
  generate.o\
  string.o\
  wchar.o\
  util.o\
  error.o\
  error_message.o\
  ./builtin/builtin.o\
  ../zvm/zvm.o\
  ../share/share.o\
  ../memory/mem.o\
  ../debug/dbg.o

CFLAGS = -c -g -Wall -ansi -Wswitch-enum -DDEBUG

INCLUDES = \
  -I/usr/local/include\
  -I../include

$(TARGET):$(OBJS)
	cd ./builtin; $(MAKE);
	cd ../zvm; $(MAKE);
	cd ../share; $(MAKE);
	cd ../memory; $(MAKE);
	cd ../debug; $(MAKE);
	$(CC) $(OBJS) -o $@ -lm -lmsvcp60

clean:
	rm -f *.o lex.yy.c y.tab.c y.tab.h *~
y.tab.h : zero.y
	bison --yacc -dv zero.y
y.tab.c : zero.y
	bison --yacc -dv zero.y
lex.yy.c : zero.l zero.y y.tab.h
	flex zero.l
y.tab.o: y.tab.c zeroc.h ../include/MEM.h
	$(CC) -c -g $*.c $(INCLUDES)
lex.yy.o: lex.yy.c zeroc.h ../include/MEM.h
	$(CC) -c -g $*.c $(INCLUDES)
.c.o:
	$(CC) $(CFLAGS) $*.c $(INCLUDES)
../memory/mem.o:
	cd ../memory; $(MAKE);
../debug/dbg.o:
	cd ../debug; $(MAKE);
../share/share.o:
	cd ../share; $(MAKE);
../zvm/zvm.o:
	cd ../zvm; $(MAKE);
############################################################
create.o: create.c ../include/MEM.h ../include/DBG.h zeroc.h \
  ../include/ZEC.h ../include/ZVM_code.h ../include/ZVM.h \
  ../include/ZVM_code.h ../include/share.h
zero.tab.o: zero.tab.c zeroc.h ../include/MEM.h ../include/ZEC.h \
  ../include/ZVM_code.h ../include/ZVM.h ../include/ZVM_code.h \
  ../include/share.h
error.o: error.c ../include/MEM.h ../include/DBG.h zeroc.h \
  ../include/ZEC.h ../include/ZVM_code.h ../include/ZVM.h \
  ../include/ZVM_code.h ../include/share.h
error_message.o: error_message.c zeroc.h ../include/MEM.h \
  ../include/ZEC.h ../include/ZVM_code.h ../include/ZVM.h \
  ../include/ZVM_code.h ../include/share.h
fix_tree.o: fix_tree.c ../include/MEM.h ../include/DBG.h zeroc.h \
  ../include/ZEC.h ../include/ZVM_code.h ../include/ZVM.h \
  ../include/ZVM_code.h ../include/share.h
generate.o: generate.c ../include/MEM.h ../include/DBG.h zeroc.h \
  ../include/ZEC.h ../include/ZVM_code.h ../include/ZVM.h \
  ../include/ZVM_code.h ../include/share.h
interface.o: interface.c ../include/MEM.h ../include/DBG.h zeroc.h \
  ../include/ZEC.h ../include/ZVM_code.h ../include/ZVM.h \
  ../include/ZVM_code.h ../include/share.h
main.o: main.c ../include/ZEC.h ../include/ZVM_code.h ../include/ZVM.h \
  ../include/ZVM.h ../include/MEM.h
string.o: string.c ../include/MEM.h zeroc.h ../include/ZEC.h \
  ../include/ZVM_code.h ../include/ZVM.h ../include/ZVM_code.h \
  ../include/share.h
util.o: util.c ../include/MEM.h ../include/DBG.h zeroc.h \
  ../include/ZEC.h ../include/ZVM_code.h ../include/ZVM.h \
  ../include/ZVM_code.h ../include/share.h
wchar.o: wchar.c ../include/DBG.h zeroc.h ../include/MEM.h \
  ../include/ZEC.h ../include/ZVM_code.h ../include/ZVM.h \
  ../include/ZVM_code.h ../include/share.h
