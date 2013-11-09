LEX=flex
# YACC=yacc --debug --verbose
LDLIBS=-ll -lsqlite3 -lmongoc
# LDLIBS=-ll -lsqlite3
# CC=CC -DPHDEBUG -DSCANDEBUG
MONGO_PATH=/Users/matt/mongo-c-driver
CFLAGS=-I$(MONGO_PATH)/src
LDFLAGS=-L$(MONGO_PATH)

workout: workout.o mongo_db.o types.o scan.o parse_handler.o

dolex: scan.o parse_only.o

mongo_db.o: mongo_db.c

scan.o: scan.l y.tab.c
y.tab.c: parse.ym
	$(YACC) parse.ym

clean:
	$(RM) workout dolex *.o scan.c y.tab.c y.output
