static int table_size = 8;


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

  if(dict['TaskAdd']){
    table.
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
