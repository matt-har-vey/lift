#!/bin/bash

cd `dirname ${BASH_SOURCE[0]}`

rm -f data/lift.sqlite3
cat schema.sql import.sql | sqlite3 data/lift.sqlite3
cat data/*.workout | ./workout
