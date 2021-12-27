yacc -dv calculator.y
lex calculator.l
cc -o calculator y.tab.c lex.yy.c
