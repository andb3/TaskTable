var preclasses;
var times;

function setPreclasses(){
  preclasses = new Array();
  for(var i = 0; i<divArray.length; i++){
    var rowtext = divArray[i].getElementsByClassName("textinput")[0].value;
    preclasses.push(rowtext);
  }
  //console.log(preclasses);
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

  console.log("saving times");
  var daysToSet = x[0].getElementsByTagName("select")[0].selectedIndex-3;

  console.log("setting day ", daysToSet);

  rangePickers = document.getElementsByClassName("range");


  if(daysToSet>-1){

    console.log("setting day ", daysToSet);

    times[daysToSet] = new Array();

    for (var i = 0; i<rangePickers.length; i++){
      var selElmnt = rangePickers[i].getElementsByTagName("select")[0]
      var sel = rangePickers[i].getElementsByClassName("select-selected")[0].innerHTML;
      var startTime = rangePickers[i].getElementsByClassName("starttime")[0].value;
      var endTime = rangePickers[i].getElementsByClassName("endtime")[0].value;

      console.log("sel: " + sel + ", startTime: " + startTime + ", endtime: " + endTime);

      var timeArray = new Array();
      timeArray.push(startTime);
      timeArray.push(endTime);

      timeArray.push(selElmnt.selectedIndex);



      times[daysToSet].push(timeArray)


    }
  }else if (daysToSet==-3){
    //return times[0];
    for (var a = 0; a < 7/*all days*/; a++) {
      times[a] = new Array();

      for (var i = 0; i<rangePickers.length; i++){
        var selElmnt = rangePickers[i].getElementsByTagName("select")[0];
        var sel = rangePickers[i].getElementsByClassName("select-selected")[0].innerHTML;
        var startTime = rangePickers[i].getElementsByClassName("starttime")[0].value;
        var endTime = rangePickers[i].getElementsByClassName("endtime")[0].value;

//        console.log("sel: " + sel + ", startTime: " + startTime + ", endtime: " + endTime);
        console.log(selElmnt.selectedIndex);

        var timeArray = new Array();
        timeArray.push(startTime);
        timeArray.push(endTime);

        timeArray.push(selElmnt.selectedIndex);



        times[a].push(timeArray)

      }
    }



  }else if (daysToSet==-2) {
    for (var a = 0; a < 5 /*only weekdays*/; a++) {
      times[a] = new Array();

      for (var i = 0; i<rangePickers.length; i++){
        var selElmnt = rangePickers[i].getElementsByTagName("select")[0]
        var sel = rangePickers[i].getElementsByClassName("select-selected")[0].innerHTML;
        var startTime = rangePickers[i].getElementsByClassName("starttime")[0].value;
        var endTime = rangePickers[i].getElementsByClassName("endtime")[0].value;

        console.log("sel: " + sel + ", startTime: " + startTime + ", endtime: " + endTime);

        var timeArray = new Array();
        timeArray.push(startTime);
        timeArray.push(endTime);

        timeArray.push(selElmnt.selectedIndex);



        times[a].push(timeArray)

      }
    }
  }else{
    for (var a = 5; a < 7 /*only weekends*/; a++) {
      times[a] = new Array();

      for (var i = 0; i<rangePickers.length; i++){
        var selElmnt = rangePickers[i].getElementsByTagName("select")[0]
        var sel = rangePickers[i].getElementsByClassName("select-selected")[0].innerHTML;
        var startTime = rangePickers[i].getElementsByClassName("starttime")[0].value;
        var endTime = rangePickers[i].getElementsByClassName("endtime")[0].value;

        console.log("sel: " + sel + ", startTime: " + startTime + ", endtime: " + endTime);

        var timeArray = new Array();
        timeArray.push(sel);
        timeArray.push(startTime);
        timeArray.push(endTime);



        times[a].push(timeArray)

      }
    }
  }

  console.log('Times:');
  console.log(times);
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
  var time_check = document.getElementById("time_check");
  if(time_check.checked){
    return JSON.stringify(times);
  }else {
    return "";
  }
}

setupTimes();
