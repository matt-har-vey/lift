#include <time.h>

#pragma once

#define WK_PH_OK         0
#define WK_PH_ERR        -1

int wkOnStartTime(const struct tm* t);

int wkOnEndTime(const struct tm* t);

int wkOnMood(const char* s);

int wkOnCardio(const char* s);

int wkOnComments(const char* s);

int wkOnExerciseName(const char* s);

int wkOnSet(int reps, int rp_reps, double weight, const char* comment);

int wkOnEnd();
