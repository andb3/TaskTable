var table_size = 8;

// Initializing a class definition


var taskBuffer = ["", 0];

var table = [];
var preclasses = ["PE", "Science", "Computer Science", "History", "English", "Spanish", "Engineering", "Math"];


function setTable(){
  for (i = 0; i< table_size; i++){

    var taskarray = [];

    var row = [
      preclasses[i],
      taskarray
    ];


    table.push(row);
  }
}

/********************************************************/


function sendTable(){
  console.log('sending table of ', preclasses.length);




  sendNextTableItemString(preclasses, 0);

  console.log("strings sent");






}

/****************/

var keys = require('message_keys');
function sendNextTableItemString(strings, index) {

  console.log('sending row(string) ', index, 'of ', strings.length);


  // Build message
  var key = keys.TableUpdateString + index;
  //console.log('key');

  var dict = {};
  //console.log('dict');

  dict[key] = strings[index];
  //console.log('set');


  console.log('sending message');


  // Send the message
  Pebble.sendAppMessage(dict, function(e) {

    console.log('success');


    // Use success callback to increment index
    index++;

    console.log('incremented');


    if(index < strings.length) {
      // Send next item
      console.log('sending next item');
      sendNextTableItemString(strings, index);
    } else {
      console.log('Last item sent!');

      var taskCount = [];
      for(i = 0; i< table_size; i++){
        taskCount.push(table[i][1].size);
      }

      console.log(taskCount);


      sendNextTableItemInt(taskCount, 0);

      console.log("count sent");
    }
  }, function(e) {
    console.log('Item transmission failed at index: ' + index);
  });

  console.log('after send');

}

/****************/

function sendNextTableItemInt(ints, index) {

  console.log('sending row(int) ', index, 'of ', ints.length);


  // Build message
  var key = keys.TableInt + index;
  console.log(key);

  var dict = {};
  //console.log('dict');

  dict[key] = ints[index];
  //console.log('set');


  console.log('sending message');


  // Send the message
  Pebble.sendAppMessage(dict, function(e) {

    console.log('success');


    // Use success callback to increment index
    index++;

    console.log('incremented');


    if(index < ints.length) {
      // Send next item
      console.log('sending next item');
      sendNextTableItemInt(ints, index);
    } else {
      console.log('Last item sent!');
    }
  }, function(e) {
    console.log('Item transmission failed at index: ' + index);
  });

  console.log('after send');

}

/********************************************************/



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
