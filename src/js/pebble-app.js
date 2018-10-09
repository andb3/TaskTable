static int table_size = 8;

let taskBuffer = {
  index: 0
  task_text: "";
  task_time: 0;
}

let table = [];
let preclasses = ["PE", "Science", "Computer Science", "History", "English", "Spanish", "Engineering", "Math"]

function setTable(){
  for (int i = 0; i<= table_size; i++){

    let taskarray = [];

    let row = {
      name: preclasses[i];
      tasks: taskarray;
    }


    table.push(row);
  }
}


Pebble.addEventListener('ready', function() {
  // PebbleKit JS is ready!
  console.log('PebbleKit JS ready!');
});


// Get AppMessage events
Pebble.addEventListener('appmessage', function(e) {
  // Get the dictionary from the message
  var dict = e.payload;

  console.log('Got message: ' + JSON.stringify(dict));

  if(dict['TaskAddIndex']){
    taskBuffer.index = dict['TaskAddIndex'];
    console.log('got index');
  }else if (dict['TaskAddDescription]') {
    taskBuffer.task_text = dict['TaskAddDescription'];
    console.log('got description');
  }else if (dict['TaskAddTime']) {
    taskBuffer.task_time = dict['TaskAddTime'];
    console.log('got time');
  }

});


/********************************************************/


// Assemble data object
var dict = {
  'Temperature': 29,
  'LocationName': 'London, UK'
};

// Send the object
Pebble.sendAppMessage(dict, function() {
  console.log('Message sent successfully: ' + JSON.stringify(dict));
}, function(e) {
  console.log('Message failed: ' + JSON.stringify(e));
});




/********************************************************/
