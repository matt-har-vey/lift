#include <sqlite3.h>

#include "types.h"

#define DB_OK           0
#define DB_API         -1
#define DB_EXECUTION   -2
#define DB_PREPARE     -3
#define DB_FIXTURE     -4

int wk_db_open();

int wk_db_insert_workout(wkWorkout* workout);

int wk_db_close(int had_errors);
