#!/bin/bash

cd `dirname ${BASH_SOURCE[0]}`

rm -f data/lift.sqlite3
cat schema.sql import.sql | sqlite3 data/lift.sqlite3

# for FILE in `ls data/*.workout`; do
# 	cat $FILE | ./workout;
# done

cat data/*.workout | ./workout
