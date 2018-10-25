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
      //console.log('Last item sent!');
      console.log("table: ", table);
      sendTableTasks(0,0);
    }
  }, function(e) {
    console.log('Item transmission failed at index: ' + index);
  });

  console.log('after send');

}

/****************/

function sendTableTasks(row, index){

  var rowKey = keys.RowGetTasksRow;
  var indexKey = keys.RowGetTasksIndex;
  var textKey = keys.RowGetTasksString;
  var timeKey = keys.RowGetTasksTime;

  var dict = {};

  dict[rowKey] = row;
  dict[indexKey] = index;
  dict[textKey] = table[row][1][index][0];
  dict[timeKey] = table[row][1][index][1];

  /*console.log(table);
  console.log("table row: " + table[row]);
  console.log(table[row]);
  console.log("table row taskarray: " + table[row][1]);
  console.log("table row taskarray index: " + table[row][1][index]);*/
  console.log("textKey: " + table[row][1][index][0]);
  console.log("timeKey: " + table[row][1][index][1]);

  Pebble.sendAppMessage(dict, function(e) {

    console.log('success');


    // Use success callback to increment index
    if(index<table[row][1].length-1){
      console.log("length ", table[row][1].length);
      index++;
      console.log("index changed (", row, ",", index,")");
      sendTableTasks(row, index);
    }else if (row<table.length-1) {
      index = 0;
      var runNext = true;
      row++;
      while (table[row][1].length<1) {
        if(row<table.length-1){
          row++;
        }else {
          runNext = false;
          break;
        }
      }
      console.log("row changed (", row, ",", index,")");
      if(runNext){
        sendTableTasks(row, index);
      }else{
        console.log('Last item sent!');
      }
    }else{
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
    console.log(localStorage.getItem('table_storage'));
    try{
      table = JSON.parse(localStorage.getItem('table_storage'));
      console.log("table read");
      console.log(table);

    }catch(e){
      console.log("table read failed");
      setTable();
    }
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

Pebble.addEventListener('webviewclosed', function(e) {
  // Decode the user's preferences
  var configData = JSON.parse(decodeURIComponent(e.response));
});

Pebble.addEventListener('showConfiguration', function() {
  var url = 'https://andb3.github.io/TaskTable/config.html';

  console.log("opening ", url);

  Pebble.openURL(url);
});
