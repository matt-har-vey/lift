#include "parse_handler.h"

int wkOnStartParse() { return WK_PH_OK; }
int wkOnStartTime(const struct tm* t) { return WK_PH_OK; }
int wkOnEndTime(const struct tm* t) { return WK_PH_OK; }
int wkOnMood(const char* s) { return WK_PH_OK; }
int wkOnCardio(const char* s) { return WK_PH_OK; }
int wkOnComments(const char* s) { return WK_PH_OK; }
int wkOnExerciseName(const char* s) { return WK_PH_OK; }
int wkOnSet(int reps, double weight, int extend_reps, double extend_weight, const char* comment) { return WK_PH_OK; }
int wkOnEndWorkout() { return WK_PH_OK; }
int wkOnEndParse() { return WK_PH_OK; }

