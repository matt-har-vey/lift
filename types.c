#include <time.h>
#include <stdlib.h>

#include "types.h"

#define EXERCISES_BUFFER_SIZE 30
#define SETS_BUFFER_SIZE 20

size_t wkIsoDateTime(char* s, const struct tm* t) {
	return strftime(s, ISO_DATE_BUF_SIZE, "%Y-%m-%dT%H:%M:%S", t);
}

wkSet* wkSetAllocInit() {
	return calloc(1, sizeof(wkSet));
}

void wkSetFree(wkSet* s) {
	if (s) free(s);
}

wkExercise* wkExerciseAllocInit() {
	wkExercise* ret = calloc(1, sizeof(wkExercise));
	ret->sets = calloc(SETS_BUFFER_SIZE, sizeof(wkSet));
	return ret;
}

void wkExerciseFreeDeep(wkExercise* e) {
	if (e) {
		if (e->name) free(e->name);
		for (int i = 0; i < e->num_sets; i++)
			wkSetFree(e->sets[i]);
		free(e->sets);
		free(e);
	}
}

wkWorkout* wkWorkoutAllocInit() {
	wkWorkout* ret = calloc(1, sizeof(wkWorkout));
	ret->exercises = calloc(EXERCISES_BUFFER_SIZE, sizeof(wkExercise));
	return ret;
}

void wkWorkoutFreeDeep(wkWorkout* w) {
	if (w) {
		if (w->mood) free(w->mood);
		if (w->cardio) free(w->cardio);
		if (w->comments) free(w->comments);
		for (int i = 0; i < w->num_exercises; i++)
			wkExerciseFreeDeep(w->exercises[i]);
		free(w->exercises);
		free(w);
	}
}

