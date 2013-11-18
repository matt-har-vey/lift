var http = require('http');
var MongoClient = require('mongodb').MongoClient;

var okJson = function(res) {
  res.writeHead(200, {'Content-Type': 'application/json'});
};

var jsonArray = function(cursor, res) {
  okJson(res);
  res.write('[');
  var first = true;
  cursor.each(function(err, item) {
    if (item) {
      if (!first)
        res.write(',');
      res.write(JSON.stringify(item));
      first = false;
    } else {
      res.write(']');
      res.end();
    }
  });
};

var exercisesFull = function(workouts, cb) {
  workouts.aggregate([
    {$unwind: '$exercises'},
    {$unwind: '$exercises.sets'},
    {$group: {_id:{name:'$exercises.name',start:'$start'},
               sets:{$push:'$exercises.sets'},
               volume:{
                $sum:{
                    $add:[{$multiply:['$exercises.sets.weight','$exercises.sets.reps']},
                          {$ifNull:[{$multiply:['$exercises.sets.set.weight','$exercises.sets.set.reps']},0]}]
                  }
               }
             }},
    {$sort: {'_id.start':-1}},
    {$group: {_id:'$_id.name', workouts:{$push:{start:'$_id.start',volume:'$volume',sets:'$sets'}}}},
    {$project: {_id:0, name:'$_id', workouts:'$workouts'}},
    {$sort:{name:1}}
    ], cb);
};

MongoClient.connect('mongodb://127.0.0.1:27017/lift', function(err, db) {
  var workouts = db.collection('workouts');

  var server = http.createServer(function(request, response) {
    if (request.url == '/api/workouts/full') {
      jsonArray(workouts.find(), response);
    } else if (request.url == '/api/workouts') {
      jsonArray(workouts.find({},{"_id":1,start:1}), response);
    } else if (request.url == '/api/exercises/full') {
      exercisesFull(workouts, function(err, result) {
        okJson(response);
        // TODO Stream?
        response.write(JSON.stringify(result));
        response.end();
      });
    } else {
      response.writeHead(404);
      response.end();
    }
  });

  server.on('close', function() {
    db.close();
  });

  server.listen(8282, 'localhost');
});
