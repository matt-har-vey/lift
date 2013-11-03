int yylex(void);

#include "parse_handler.h"

int wkOnStartTime(const struct tm* t) { return WK_PH_OK; }
int wkOnEndTime(const struct tm* t) { return WK_PH_OK; }
int wkOnMood(const char* s) { return WK_PH_OK; }
int wkOnCardio(const char* s) { return WK_PH_OK; }
int wkOnComments(const char* s) { return WK_PH_OK; }
int wkOnExerciseName(const char* s) { return WK_PH_OK; }
int wkOnSet(int reps, int rp_reps, double weight, const char* comment) { return WK_PH_OK; }
int wkOnEnd() { return WK_PH_OK; }

int main(int argc, const char** argv) {
	while (yylex() != 0) {}
}
