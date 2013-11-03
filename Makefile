LEX=flex
YACC=yacc
LDLIBS=-ll -lsqlite3
CC=CC -DSCANDEBUG

workout: types.o db.o scan.o parse_handler.o workout.o
scan.o: scan.l y.tab.c
y.tab.c: parse.ym
	$(YACC) parse.ym

clean:
	$(RM) workout *.o scan.c y.tab.c
