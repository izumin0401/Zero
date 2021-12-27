bison --yacc -dv calculator.y
flex calculator.l
gcc -o calculator y.tab.c lex.yy.c
