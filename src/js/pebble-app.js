var table_size = 8;

// Initializing a class definition


var taskBuffer = ["", 0];

var table = [];
var preclasses = ["PE", "Science", "Computer Science", "History", "English", "Spanish", "Engineering", "Math"];

var writeTable;


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
        taskCount.push(table[i][1].length);
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

  writeTable = true;
  if(localStorage.getItem('writeTable') == null){
    console.log("writeTable null");
    localStorage.setItem('writeTable', JSON.stringify(false));
  }else{
    try {
      console.log("writeTable reading");
      writeTable = JSON.parse(localStorage.getItem('writeTable'));
    } catch (e) {
      console.log("writeTable error");
      localStorage.setItem('writeTable', JSON.stringify(false));
    }

  }

  console.log("writeTable: ", writeTable);

  if(!writeTable){
    console.log("reading table");
    table = JSON.parse(localStorage.getItem('table_storage'));
  }else{
    console.log("writing table");
    setTable();
    localStorage.setItem('table_storage', JSON.stringify(table));

  }
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
  if ("TaskAddDescription" in dict) {
    taskBuffer[0] = dict['TaskAddDescription'];
    console.log('got description');
  }
  if ("TaskAddTime" in dict) {
    taskBuffer[1] = dict['TaskAddTime'];
    console.log('got time');

    table[index][1].push(taskBuffer);
    console.log(table[index][1]);

    taskBuffer = ["", 0];

    var taskCount = [];
    for(i = 0; i< table_size; i++){
      taskCount.push(table[i][1].length);
    }

    sendNextTableItemInt(taskCount, 0);

  }
  if("TableRemoveRow" in dict && "TableRemoveIndex" in dict){
    var removerow = dict['TableRemoveRow'];
    var removeindex = dict['TableRemoveIndex'];

    console.log("removing ", removeindex, " of ", removerow);

    table[removerow][1].splice(removeindex, 1);

    var taskCount = [];
    for(i = 0; i< table_size; i++){
      taskCount.push(table[i][1].length);
    }


    sendNextTableItemInt(taskCount, 0);
  }



  var taskCount = [];
  for(i = 0; i< table_size; i++){
    taskCount.push(table[i][1].length);
  }

  console.log(taskCount);

  localStorage.setItem('table_storage', JSON.stringify(table));

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
