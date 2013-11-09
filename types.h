#pragma once

#include <time.h>

struct wkExercise;
struct wkSet;

typedef struct wkWorkout{
	struct tm start;
	struct tm end;
	char* mood;
	char* cardio;
	char* comments;
	int num_exercises;
	struct wkExercise** exercises;
} wkWorkout;

typedef struct wkExercise {
	wkWorkout* workout;
	char* name;
	int position;
	int num_sets;
	struct wkSet** sets;
} wkExercise;

typedef struct wkSet {
	wkExercise* exercise;
	int sequence;
	int reps;
	int rp_reps;
	double weight;
	char* comment;
} wkSet;


wkWorkout* wkWorkoutAllocInit();
void wkWorkoutFreeDeep(wkWorkout* w);

wkExercise* wkExerciseAllocInit();
wkSet* wkSetAllocInit();
