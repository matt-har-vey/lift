LEX=flex
YACC=yacc
LDLIBS=-ll
CC=cc -DWKDEBUG

workout: scan.o workout.o parse_handler.o
scan.o: scan.l y.tab.c
y.tab.c: parse.ym
	$(YACC) parse.ym
workout.o: workout.c

clean:
	$(RM) workout *.o scan.c y.tab.c
