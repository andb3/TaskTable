var table_size = 8;

// Initializing a class definition


var taskBuffer = ["", 0];

var table = [];
var preclasses = ["PE", "Science", "Computer Science", "History", "English", "Spanish", "Engineering", "Math"];


function setTable(){
  for (i = 0; i<= table_size; i++){

    var taskarray = [];

    var row = [
      preclasses[i],
      taskarray
    ];


    table.push(row);
  }
}

function sendTable(){
  console.log('sending table of ', preclasses.length);
  sendNextTableItem(preclasses, 0);
}

var keys = require('message_keys');
function sendNextTableItem(items, index) {

  console.log('sending row ', index, 'of ', items.length);


  // Build message
  var key = keys.TableUpdate + index;
  //console.log('key');

  var dict = {};
  //console.log('dict');

  dict[key] = items[index];
  //console.log('set');


  console.log('sending message');


  // Send the message
  Pebble.sendAppMessage(dict, function(e) {

    console.log('success');


    // Use success callback to increment index
    index++;

    console.log('incremented');


    if(index < items.length) {
      // Send next item
      console.log('sending next item');
      sendNextTableItem(items, index);
    } else {
      console.log('Last item sent!');
    }
  }, function(e) {
    console.log('Item transmission failed at index: ' + index);
  });

  console.log('after send');

}


Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS ready.');

  setTable();
  // Update s_js_ready on watch
  Pebble.sendAppMessage({'JSReady': 1});

  sendTable();

  console.log('Sent Ready Message');

});


// Get AppMessage events
Pebble.addEventListener('appmessage', function(e) {
  // Get the dictionary from the message
  var dict = e.payload;

  var index = 0;

  console.log('Got message: ' + JSON.stringify(dict));

  if("TaskAddIndex" in dict){
    index = dict['TaskAddIndex'];
    console.log('got index: ', index);
    console.log('into ', table[index][0])
  }
  if ("TaskAddIndex" in dict) {
    taskBuffer[0] = dict['TaskAddDescription'];
    console.log('got description');
  }
  if ("TaskAddIndex" in dict) {
    taskBuffer[1] = dict['TaskAddTime'];
    console.log('got time');

    table[index][1].push(taskBuffer);
    console.log(table[index][1]);

    taskBuffer = ["", 0];
  }

});


/********************************************************/


/*// Assemble data object
var dict = {
  'Temperature': 29,
  'LocationName': 'London, UK'
};

// Send the object
Pebble.sendAppMessage(dict, function() {
  console.log('Message sent successfully: ' + JSON.stringify(dict));
}, function(e) {
  console.log('Message failed: ' + JSON.stringify(e));
});*/




/********************************************************/
