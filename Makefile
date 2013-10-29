LEX=flex
LDLIBS=-ll

workout: workout.o
workout.o: workout.l

clean:
	 $(RM) workout *.o
