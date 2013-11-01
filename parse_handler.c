#include <time.h>
#include <stdio.h>

#define ISO_DATE_BUF_SIZE 20

typedef struct wkSetStruct {
	int reps;
	int rpReps;
	double weight;
	char* comment;
} wkSet;

typedef struct wkSetsStruct {
	char* name;
	int length;
	wkSet* sets;
} wkSets;

typedef struct wkWorkoutStruct {
	struct tm* start;
	struct tm* end;
	char* mood;
	char* cardio;
	char* comments;
} wkWorkout;

static size_t sWkIsoDateTime(char* s, const struct tm* t) {
	return strftime(s, ISO_DATE_BUF_SIZE, "%Y-%m-%dT%H:%M:%S", t);
}

static void sWkPrintTime(const char* prefix, const struct tm* t) {
  char buf[ISO_DATE_BUF_SIZE];
	sWkIsoDateTime(buf, t);
	printf("%s: %s\n", prefix, buf);
}

int wkOnStartTime(const struct tm* t) {
#ifdef WKDEBUG
	sWkPrintTime("Started at", t);
#endif
	return 0;
}

int wkOnEndTime(const struct tm* t) {
#ifdef WKDEBUG
	sWkPrintTime("Finished at", t);
#endif
	return 0;
}

int wkOnMood(const char* s) {
#ifdef WKDEBUG
	printf("Mood: %s\n", s);
#endif
	return 0;
}

int wkOnCardio(const char* s) {
#ifdef WKDEBUG
	if (s != 0)
		printf("Cardio: %s\n", s);
	else
		printf("(No cardio)\n");
#endif
	return 0;
}

int wkOnComments(const char* s) {
#ifdef WKDEBUG
	printf("Comments: %s\n", s);
#endif
	return 0;
}

int wkOnExerciseName(const char* s) {
#ifdef WKDEBUG
	printf("\nExercise: %s\n", s);
#endif
	return 0;
}

int wkOnSet(int reps, int rpReps, double weight, const char* comment) {
#ifdef WKDEBUG
	if (comment)
		printf("%d+%dx%.1f (%s) ", reps, rpReps, weight, comment);
	else if (rpReps != 0)
		printf("%d+%dx%.1f ", reps, rpReps, weight);
	else
		printf("%dx%.1f ", reps, weight);
#endif
	return 0;
}
