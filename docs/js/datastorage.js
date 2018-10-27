var preclasses;
var times;

function setPreclasses(){
  preclasses = new Array();
  for(var i = 0; i<divArray.length; i++){
    var rowtext = divArray[i].getElementsByClassName("textinput")[0].value;
    preclasses.push(rowtext);
  }
  console.log(preclasses);
  setRangeDropdowns();
}

function getPreclasses(){
  return JSON.stringify(preclasses);
}

function setupTimes(){

  times = new Array();

  var mon = new Array();
  var tue = new Array();
  var wed = new Array();
  var thu = new Array();
  var fri = new Array();
  var sat = new Array();
  var sun = new Array();


  times.push(mon);
  times.push(tue);
  times.push(wed);
  times.push(thu);
  times.push(fri);
  times.push(sat);
  times.push(sun);
}

function setTimes(){
  var daysToSet = x[0].getElementsByTagName("select")[0].selectedIndex-3;

  if(day>-1){
    times[day] = new Array();
  }

  for (var i = 0; i<rangePickers.length; i++){
    var selElmnt = rangePickers[i].getElementsByTagName("select")[0];
    var startTimeElmnt = rangePickers[i].getElementsByTagName("starttime")[0];
    var endTimeElmnt = rangePickers[i].getElementsByTagName("endtime")[0];



    if(day>-1){
      times[day].push()
    }

  }
}

function getTimesByDay(day){
  if(day>-1){
    return times[day];
  }else if (day==-3||day==-2) {
    return times[0];
  }else{
    return times[5];
  }
}

function getTimes(){
  return JSON.stringify(times);
}

setupTimes();
setPreclasses();
