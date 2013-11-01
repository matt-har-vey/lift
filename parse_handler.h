#include <time.h>

int wkOnStartTime(const struct tm* t);

int wkOnEndTime(const struct tm* t);

int wkOnMood(const char* s);

int wkOnCardio(const char* s);

int wkOnComments(const char* s);

int wkOnExerciseName(const char* s);

int wkOnSet(int reps, int rpReps, double weight, const char* comment);
