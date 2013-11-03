#pragma once

struct wkExerciseStruct;
struct wkSetStruct;

typedef struct wkWorkoutStruct {
	struct tm start;
	struct tm end;
	char* mood;
	char* cardio;
	char* comments;
	int num_exercises;
	struct wkExerciseStruct** exercises;
} wkWorkout;

typedef struct wkExerciseStruct {
	wkWorkout* workout;
	int position;
	int id;
	char* name;
	int num_sets;
	struct wkSetStruct** sets;
} wkExercise;

typedef struct wkSetStruct {
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

#define ISO_DATE_BUF_SIZE 20
size_t wkIsoDateTime(char* s, const struct tm* t);
