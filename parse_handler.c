#include <stdio.h>
#include <string.h>

#include "parse_handler.h"
#include "types.h"
#include "db.h"

#ifdef PHDEBUG
static void sWkPrintTime(const char* prefix, const struct tm* t) {
	printf("%s: %s\n", prefix, asctime(t));
}
#endif

static wkWorkout* workout;
static wkExercise* exercise;
static int exercise_position, set_sequence;
static int all_good;

int wkOnStartParse() {
	all_good = 1;
	if (all_good) all_good = (wk_db_open() == DB_OK);
	int ret = all_good ? WK_PH_OK : WK_PH_ERR;
#ifdef PHDEBUG
	printf("start parse returning %d\n", ret);
#endif
	return ret;
}

int wkOnStartTime(const struct tm* t) {
#ifdef PHDEBUG
	sWkPrintTime("Started at", t);
#endif

	workout = wkWorkoutAllocInit();
	workout->start = *t;

	exercise_position = 0;
	set_sequence = 0;

	return WK_PH_OK;
}

int wkOnEndTime(const struct tm* t) {
#ifdef PHDEBUG
	sWkPrintTime("Finished at", t);
#endif

	workout->end = *t;
	return WK_PH_OK;
}

int wkOnMood(const char* s) {
#ifdef PHDEBUG
	printf("Mood: %s\n", s);
#endif

	workout->mood = strdup(s);
	return WK_PH_OK;
}

int wkOnCardio(const char* s) {
#ifdef PHDEBUG
	if (s != 0)
		printf("Cardio: %s\n", s);
	else
		printf("(No cardio)\n");
#endif

	if (s != 0)
		workout->cardio = strdup(s);
	else
		workout->cardio = 0;

	return WK_PH_OK;
}

int wkOnComments(const char* s) {
#ifdef PHDEBUG
	printf("Comments: %s\n", s);
#endif

	workout->comments = strdup(s);

	return WK_PH_OK;
}

int wkOnExerciseName(const char* s) {
#ifdef PHDEBUG
	printf("\nExercise: %s\n", s);
#endif

	exercise_position += 1;
	set_sequence = 0;

	exercise = wkExerciseAllocInit();
	exercise->name = strdup(s);
	exercise->position = exercise_position;
	exercise->workout = workout;

	workout->exercises[workout->num_exercises] = exercise;
	workout->num_exercises += 1;

	return WK_PH_OK;
}

int wkOnSet(int reps, int rp_reps, double weight, const char* comment) {
#ifdef PHDEBUG
	if (comment)
		printf("%d+%dx%.1f (%s) ", reps, rp_reps, weight, comment);
	else if (rp_reps != 0)
		printf("%d+%dx%.1f ", reps, rp_reps, weight);
	else
		printf("%dx%.1f ", reps, weight);
#endif

	set_sequence += 1;

	if (exercise != NULL) {
		wkSet* set = wkSetAllocInit();
		set->exercise = exercise;
		set->sequence = set_sequence;
		set->reps = reps;
		set->weight = weight;
		set->rp_reps = rp_reps;
		if (comment != NULL)
			set->comment = strdup(comment);

		exercise->sets[exercise->num_sets] = set;
		exercise->num_sets += 1;

		return WK_PH_OK;
	} else {
		return WK_PH_ERR;
	}
}

int wkOnEndWorkout() {
	if (workout) {
		if (all_good) all_good = (wk_db_insert_workout(workout) == DB_OK);
	 	wkWorkoutFreeDeep(workout);
	}

	return all_good ? WK_PH_OK : WK_PH_ERR;
}

int wkOnEndParse() {
	wk_db_close(all_good == 1);
	return WK_PH_OK;
}

