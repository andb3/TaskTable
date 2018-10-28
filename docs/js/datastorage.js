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

  if(daysToSet>-1){
    times[daysToSet] = new Array();

    for (var i = 0; i<rangePickers.length; i++){
      var selElmnt = rangePickers[i].getElementsByTagName("select")[0];
      var startTimeElmnt = rangePickers[i].getElementsByClassName("starttime")[0];
      var endTimeElmnt = rangePickers[i].getElementsByClassName("endtime")[0];




      times[daysToSet].push()


    }
  }else if (daysToSet==-3){
    //return times[0];
    for (var a = 0; a < 7/*all days*/; a++) {
      times[a] = new Array();

      for (var i = 0; i<rangePickers.length; i++){
        var selElmnt = rangePickers[i].getElementsByTagName("select")[0];
        var startTimeElmnt = rangePickers[i].getElementsByClassName("starttime")[0];
        var endTimeElmnt = rangePickers[i].getElementsByClassName("endtime")[0];




        times[a].push()

      }
    }



  }else if (daysToSet==-2) {
    for (var a = 0; a < 5 /*only weekdays*/; a++) {
      times[a] = new Array();

      for (var i = 0; i<rangePickers.length; i++){
        var selElmnt = rangePickers[i].getElementsByTagName("select")[0];
        var startTimeElmnt = rangePickers[i].getElementsByClassName("starttime")[0];
        var endTimeElmnt = rangePickers[i].getElementsByClassName("endtime")[0];




        times[a].push()

      }
    }
  }else{
    for (var a = 5; a < 7 /*only weekends*/; a++) {
      times[a] = new Array();

      for (var i = 0; i<rangePickers.length; i++){
        var selElmnt = rangePickers[i].getElementsByTagName("select")[0];
        var startTimeElmnt = rangePickers[i].getElementsByClassName("starttime")[0];
        var endTimeElmnt = rangePickers[i].getElementsByClassName("endtime")[0];




        times[a].push()

      }
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
