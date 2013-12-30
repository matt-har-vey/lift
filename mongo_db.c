#include "db.h"

#include <mongoc.h>
#include <time.h>
#include <stdlib.h>

#define COLL_WORKOUTS "lift.workouts"

mongo conn[1];

int wk_db_open() {
	mongo_init(conn);

	int status = mongo_client(conn, "127.0.0.1", 27017);

	if (status == MONGO_OK) {
		return DB_OK;
	} else {
		return DB_EXECUTION;
	}
}

int wk_db_close() {
	mongo_destroy(conn);
	return DB_OK;
}

int wk_db_insert_workout(wkWorkout* workout) {
	char index_string[100];
	bson b[1], q[1];
	bson_init(b);
	bson_init(q);

	time_t tt_start = timelocal(&workout->start);
	bson_oid_t oid;
	memset(&oid.bytes, 0, 12 * sizeof(char));
	memcpy(&oid.bytes, &tt_start, sizeof(time_t));

	bson_append_oid(q, "_id", &oid);

	bson_append_oid(b, "_id", &oid);
	bson_append_time_t(b, "start", timelocal(&workout->start));
	bson_append_time_t(b, "end", timelocal(&workout->end));
	bson_append_string(b, "mood", workout->mood);
	if (workout->cardio)
		bson_append_string(b, "cardio", workout->cardio);
	bson_append_string(b, "comments", workout->comments);

	bson_append_start_array(b, "exercises");
	for (int i = 0; i < workout->num_exercises; i++) {
		snprintf(index_string, 100, "%d", i);
		bson_append_start_object(b, index_string);

		wkExercise* exercise = workout->exercises[i];

		bson_append_string(b, "name", exercise->name);

		bson_append_start_array(b, "sets");
		for (int j = 0; j < exercise->num_sets; j++) {
			wkSet* set = exercise->sets[j];
			snprintf(index_string, 100, "%d", j);
			bson_append_start_object(b, index_string);

			bson_append_int(b, "reps", set->reps);
			bson_append_double(b, "weight", set->weight);
			if (set->extend_reps > 0) {
				bson_append_start_object(b, "set");
				bson_append_int(b, "reps", set->extend_reps);
				bson_append_double(b, "weight", set->extend_weight);
				bson_append_finish_object(b);
			}
			if (set->comment)
				bson_append_string(b, "comment", set->comment);

			bson_append_finish_object(b);
		}
		bson_append_finish_array(b);

		bson_append_finish_object(b);
	}
	bson_append_finish_array(b);

	bson_finish(b);
	bson_finish(q);

	int res = mongo_update(conn, COLL_WORKOUTS, q, b, MONGO_UPDATE_UPSERT, NULL);

	bson_destroy(b);
	bson_destroy(q);

	return res == MONGO_OK ? DB_OK : DB_EXECUTION;
}
