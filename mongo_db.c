#include "db.h"

#include <mongoc.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

static mongoc_client_t* client;
static mongoc_collection_t* workouts;

int wk_db_open() {
    client = 0;
    workouts = 0;

    client = mongoc_client_new("mongodb://localhost");
    workouts = mongoc_client_get_collection(client, "lift", "workouts");

	if (client != 0 && workouts != 0) {
		return DB_OK;
	} else {
		return DB_EXECUTION;
	}
}

int wk_db_close() {
    mongoc_client_destroy(client);
	return DB_OK;
}

int wk_db_insert_workout(wkWorkout* workout) {
    int res;
    bson_error_t error;
	char index_string[100];
    bson_t *b = bson_new();
    bson_t *q = bson_new();
    bson_t *exers = bson_new();
    bson_t *exer = bson_new();
    bson_t *sets = bson_new();
    bson_t *bset = bson_new();
    bson_t *bdrop = bson_new();

	time_t tt_start = timelocal(&workout->start);
    bson_uint8_t oid_data[12];
	memset(oid_data, 0, 12 * sizeof(bson_uint8_t));
	memcpy(oid_data, &tt_start, sizeof(time_t));

	bson_oid_t oid;
    bson_oid_init_from_data(&oid, oid_data);

	bson_append_oid(q, "_id", 3, &oid);
	bson_append_oid(b, "_id", 3, &oid);

	bson_append_time_t(b, "start", 5, timelocal(&workout->start));
	bson_append_time_t(b, "end", 3, timelocal(&workout->end));
	bson_append_utf8(b, "mood", 4, workout->mood, strlen(workout->mood));
	if (workout->cardio)
		bson_append_utf8(b, "cardio", 6, workout->cardio, strlen(workout->cardio));
	bson_append_utf8(b, "comments", 8, workout->comments, strlen(workout->comments));

	bson_append_array_begin(b, "exercises", 9, exers);
	for (int i = 0; i < workout->num_exercises; i++) {
		snprintf(index_string, 100, "%d", i);
		bson_append_document_begin(exers, index_string, strlen(index_string), exer);

		wkExercise* exercise = workout->exercises[i];

		bson_append_utf8(exer, "name", 4, exercise->name, strlen(exercise->name));

		bson_append_array_begin(exer, "sets", 4, sets);
		for (int j = 0; j < exercise->num_sets; j++) {
			wkSet* set = exercise->sets[j];
			snprintf(index_string, 100, "%d", j);

            bson_append_document_begin(sets, index_string, strlen(index_string), bset);

			bson_append_int32(bset, "reps", 4, (bson_int32_t)set->reps);
			bson_append_double(bset, "weight", 6, set->weight);
			if (set->extend_reps > 0) {
				bson_append_document_begin(bset, "set", 3, bdrop);
				bson_append_int32(bdrop, "reps", 4, (bson_int32_t)set->extend_reps);
				bson_append_double(bdrop, "weight", 6, set->extend_weight);
				bson_append_document_end(bset, bdrop);
                bson_reinit(bdrop);
			}
			if (set->comment)
				bson_append_utf8(bset, "comment", 7, set->comment, strlen(set->comment));

            bson_append_document_end(sets, bset);
            bson_reinit(bset);
		}
		bson_append_array_end(exer, sets);

		bson_append_document_end(exers, exer);
        bson_reinit(exer);
	}
	bson_append_array_end(b, exers);

    res = mongoc_collection_update(workouts, MONGOC_UPDATE_UPSERT, q, b, NULL, &error);

    bson_destroy(bdrop);
    bson_destroy(bset);
    bson_destroy(sets);
    bson_destroy(exer);
    bson_destroy(exers);
	bson_destroy(q);
	bson_destroy(b);

	return res == 1 ? DB_OK : DB_EXECUTION;
}
