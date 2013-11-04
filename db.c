#include <stdio.h>

#include "db.h"

#define DB_FILE "/Users/matt/lift/data/lift.sqlite3"

sqlite3* sqlite;

void sPrintSqliteError() {
	fprintf(stderr, "%s\n", sqlite3_errmsg(sqlite));
}

int wkDbConnect() {
	if (sqlite3_open(DB_FILE, &sqlite) == SQLITE_OK)
		return DB_OK;
	else
		return DB_API;
}

int wkDbDisconnect() {
	if (sqlite3_close(sqlite) == SQLITE_OK)
		return DB_OK;
	else
		return DB_API;
}

static int sWkSimpleExecute(char* sql) {
	int ret;
	char* err = NULL;
	
	ret = sqlite3_exec(sqlite, sql, NULL, NULL, &err);

	if (ret != 0)
		fprintf(stderr, "%s\n", err);

	sqlite3_free(err);

	return ret;
}

int wkDbBegin() {
	return sWkSimpleExecute("BEGIN");
}

int wkDbCommit() {
	return sWkSimpleExecute("COMMIT");
}

int wkDbRollback() {
	return sWkSimpleExecute("ROLLBACK");
}

int wkDbInsertWorkout(wkWorkout* w) {
	int res;

	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(sqlite, "INSERT INTO workouts (begin_time, end_time, mood, cardio, comments) VALUES (?,?,?,?,?)", -1, &stmt, NULL) != SQLITE_OK) {
		sPrintSqliteError();
		return DB_PREPARE;
	}

	sqlite3_bind_int(stmt, 1, timelocal(&(w->start)));
	sqlite3_bind_int(stmt, 2, timelocal(&(w->end)));
	sqlite3_bind_text(stmt, 3, w->mood, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 4, w->cardio, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 5, w->comments, -1, SQLITE_STATIC);

	res = sqlite3_step(stmt);
	if (res != SQLITE_DONE) {
		sPrintSqliteError();
		res = DB_EXECUTION;
	} else {
		res = DB_OK;
	}

	sqlite3_finalize(stmt);
	return res;
}

int wkDbGetExerciseId(const char* exercise_name) {
	sqlite3_stmt* stmt;

	sqlite3_prepare_v2(sqlite, "SELECT id FROM exercises WHERE name=?", -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, exercise_name, -1, SQLITE_STATIC);

	int res_step = sqlite3_step(stmt);
	int exercise_id;

	if (SQLITE_ROW == res_step) {
		exercise_id = sqlite3_column_int(stmt, 0);
	} else if (SQLITE_DONE == res_step) {
		exercise_id = -1;
	} else {
		sPrintSqliteError();
		exercise_id = -1;
	}

	sqlite3_finalize(stmt);
	return exercise_id;
}

int wkDbInsertSet(wkSet* set) {
	int res;

	sqlite3_stmt* stmt;

	struct tm* workout_begin = &(set->exercise->workout->start);

	if (sqlite3_prepare_v2(sqlite, "INSERT INTO sets (workout_begin, exercise_id, position, sequence, reps, weight, rp_reps, comments) VALUES (?,?,?,?,?,?,?,?)", -1, &stmt, NULL) != SQLITE_OK) {
		sPrintSqliteError();
		return DB_PREPARE;
	}

	sqlite3_bind_int64(stmt, 1, timelocal(workout_begin));
	sqlite3_bind_int(stmt, 2, set->exercise->id);
	sqlite3_bind_int(stmt, 3, set->exercise->position);
	sqlite3_bind_int(stmt, 4, set->sequence);
	sqlite3_bind_int(stmt, 5, set->reps);
	sqlite3_bind_double(stmt, 6, set->weight);

	int rp_reps = set->rp_reps;
	if (rp_reps > 0)
		sqlite3_bind_int(stmt, 7, rp_reps);
	else
		sqlite3_bind_null(stmt, 7);

	sqlite3_bind_text(stmt, 8, set->comment, -1, SQLITE_STATIC);

	res = sqlite3_step(stmt);
	if (res != SQLITE_DONE) {
		sPrintSqliteError();
		res = DB_EXECUTION;
	} else {
		res = DB_OK;
	}

	sqlite3_finalize(stmt);
	return res;
}
