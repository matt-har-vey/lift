#include "db.h"
#include "mongo.h"

#include <time.h>
#include <stdlib.h>

#include <stdio.h>
#define SD(x) fprintf(stderr, "%s\n", x);

#define COLL_WORKOUTS "lift.workouts"

mongo conn[1];

int wkDbConnect() {
	mongo_init(conn);

	int status = mongo_client(conn, "127.0.0.1", 27017);

	if (status == MONGO_OK) {
		return DB_OK;
	} else {
		return DB_EXECUTION;
	}
}

int wkDbDisconnect() {
	mongo_destroy(conn);
	return DB_OK;
}

int wkDbBegin() {
	return DB_OK;
}

int wkDbCommit() {
	return DB_OK;
}

int wkDbRollback() {
	return DB_OK;
}

int wkDbInsertWorkout(wkWorkout* workout) {
	bson b[1];
	bson_init(b);

	time_t tt_start = timelocal(&workout->start);
	bson_oid_t oid;
	memset(&oid.bytes, 0, 12 * sizeof(char));
	memcpy(&oid.bytes, &tt_start, sizeof(time_t));

	bson_append_oid(b, "_id", &oid);
	bson_append_time_t(b, "start", timelocal(&workout->start));
	bson_append_time_t(b, "end", timelocal(&workout->end));
	bson_append_string(b, "mood", workout->mood);
	if (workout->cardio)
		bson_append_string(b, "cardio", workout->cardio);
	bson_append_string(b, "comments", workout->comments);

	bson_finish(b);

	int res = mongo_insert(conn, COLL_WORKOUTS, b, NULL);

	bson_destroy(b);
	return res == MONGO_OK ? DB_OK : DB_EXECUTION;
}

int wkDbGetExerciseId(const char* exercise_name) {
	return 1;
}

int wkDbInsertSet(wkSet* set) {
	return DB_OK;
}
