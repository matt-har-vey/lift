Save My Workouts
==========================

When I am in the gym the right thing to do is to write down how much weight
I am lifting and for how many reps in each set so that I can make sure my
future workouts represent progress rather than staying in the same spot.

Over years I've been in the habit of doing this on 3x5 cards in a certain
format. It's easier to retrieve the information from a database than a 3x5
card. So far this repository has code that can move typed versions of my
3x5 cards into a database (SQLite or mongo, my choice. Life is too short to
get humongous, but the JSON is cool anyway.)

This also helps keep my C muscles trained. The main make target is
"workout", which parses the 3x5 card grammar described in parse.ym
(Freaking Mac has to have funky file extensions like .ym and .dylib.) and
then inserts it into the database. There's no check yet to ensure existing
data is not duplicated.
