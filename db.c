#include "db.h"

#include <time.h>
#include <stdio.h>

#define DB_FILE "/Users/matt/lift/data/lift.sqlite3"

#ifdef SQLDEBUG
static void* log_callback_sqlite3(void* ctx, int rcode, char* message) {
	fprintf(stderr, "%d %s\n", rcode, message);
	return NULL;
}
#endif

sqlite3* sqlite;

static int s_wk_simple_execute(char* sql) {
	int ret;
	char* err = NULL;
	
	ret = sqlite3_exec(sqlite, sql, NULL, NULL, &err);

	if (ret != 0)
		fprintf(stderr, "%s\n", err);

	sqlite3_free(err);

	return ret;
}

void s_print_sqlite_error() {
	fprintf(stderr, "%s\n", sqlite3_errmsg(sqlite));
}

static int s_wk_db_get_exercise_id(const char* exercise_name) {
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
		s_print_sqlite_error();
		exercise_id = -1;
	}

	sqlite3_finalize(stmt);
	return exercise_id;
}

static int s_wk_db_insert_set(time_t workout_begin, int exercise_id, wkSet* set) {
	int res;

	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(sqlite, "INSERT INTO sets (workout_begin, exercise_id, position, sequence, reps, weight, rp_reps, comments) VALUES (?,?,?,?,?,?,?,?)", -1, &stmt, NULL) != SQLITE_OK) {
		s_print_sqlite_error();
		return DB_PREPARE;
	}

	sqlite3_bind_int64(stmt, 1, workout_begin);
	sqlite3_bind_int(stmt, 2, exercise_id);
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
		s_print_sqlite_error();
		res = DB_EXECUTION;
	} else {
		res = DB_OK;
	}

	sqlite3_finalize(stmt);
	return res;
}

int wk_db_open() {
#ifdef SQLDEBUG
	sqlite3_config(SQLITE_CONFIG_LOG, &log_callback_sqlite3);
#endif
	if (sqlite3_open(DB_FILE, &sqlite) == SQLITE_OK) {
		if (s_wk_simple_execute("BEGIN") == SQLITE_OK) {
			return DB_OK;
		} else {
			return DB_EXECUTION;
		}
	} else {
		return DB_API;
	}
}

int wk_db_close(int had_errors) {
	int ret = DB_OK;

	if (had_errors) {
		if (s_wk_simple_execute("COMMIT") != SQLITE_OK)
			ret = DB_EXECUTION;
	} else {
		if (s_wk_simple_execute("ROLLBACK") != SQLITE_OK)
			ret = DB_EXECUTION;
	}

	if (sqlite3_close(sqlite) != SQLITE_OK && ret == DB_OK)
		ret = DB_API;

	return ret;
}

int wk_db_insert_workout(wkWorkout* w) {
	int res;

	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(sqlite, "INSERT INTO workouts (begin_time, end_time, mood, cardio, comments) VALUES (?,?,?,?,?)", -1, &stmt, NULL) != SQLITE_OK) {
		s_print_sqlite_error();
		return DB_PREPARE;
	}

	time_t workout_begin = timelocal(&(w->start));

	sqlite3_bind_int(stmt, 1, workout_begin);
	sqlite3_bind_int(stmt, 2, timelocal(&(w->end)));
	sqlite3_bind_text(stmt, 3, w->mood, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 4, w->cardio, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 5, w->comments, -1, SQLITE_STATIC);

	res = sqlite3_step(stmt);
	if (res != SQLITE_DONE) {
		s_print_sqlite_error();
		res = DB_EXECUTION;
	} else {
		res = DB_OK;
	}

	sqlite3_finalize(stmt);

	if (res != DB_OK)
		return res;

	for (int i = 0; i < w->num_exercises; i++) {
		wkExercise* exercise = w->exercises[i];
		const char* exercise_name = exercise->name;

		int exercise_id = s_wk_db_get_exercise_id(exercise_name);
		if (exercise_id <= 0) {
			fprintf(stderr, "Exercise [%s] was not found.\n", exercise_name);
			return DB_FIXTURE;
		} else {
			for (int j = 0; j < exercise->num_sets; j++)
				s_wk_db_insert_set(workout_begin, exercise_id, exercise->sets[j]);
		}
	}

	return DB_OK;
}
