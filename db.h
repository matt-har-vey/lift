#include <sqlite3.h>

#include <time.h>
#include "types.h"

#define DB_OK           0
#define DB_API         -1
#define DB_EXECUTION   -2
#define DB_PREPARE     -3

int wkDbConnect();
int wkDbDisconnect();

int wkDbBegin();
int wkDbCommit();
int wkDbRollback();

int wkDbInsertWorkout(wkWorkout* workout);
int wkDbGetExerciseId(const char* exercise_name);
int wkDbInsertSet(wkSet* set);
