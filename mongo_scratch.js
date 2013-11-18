var setsByExercise = function (exname) {
  return db.workouts.aggregate([
      {$unwind:'$exercises'},
      {$match:{'exercises.name':exname}},
      {$unwind:'$exercises.sets'},
      {$project:{
        _id:0,
        start: 1,
        reps: '$exercises.sets.reps',
        weight: '$exercises.sets.weight',
        extendReps: '$exercises.sets.set.reps',
        extendWeight: '$exercises.sets.set.weight',
        comment: '$exercises.sets.comment'
      }},
      {$sort:{name:1,start:1}}
  ]);
}
