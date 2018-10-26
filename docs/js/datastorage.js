var preclasses;
var times;

function setPreclasses(){
  preclasses = new Array();
  for(var i = 0; i<divArray.length; i++){
    var rowtext = divArray[i].getElementsByClassName("textinput")[0].value;
    preclasses.push(rowtext);
  }
  console.log(preclasses);
}

function getPreclasses(){
  return JSON.stringify(preclasses);
}


function setTimes(){

}

function getTimes(){
  return JSON.stringify(times);
}
